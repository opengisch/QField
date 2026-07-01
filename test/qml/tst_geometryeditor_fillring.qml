import QtQuick
import QtQuick.Window
import QtTest
import org.qfield
import org.qgis
import Theme
import "Utils.js" as Utils
import "../../src/qml/geometryeditors" as GeometryEditors

TestCase {
  id: testCase
  name: "GeometryEditorFillRing"

  property var testLayer: null
  property string lastToastType: ""
  readonly property string squareJson: '{"type":"FeatureCollection","features":[{"type":"Feature","id":0,"geometry":{"type":"Polygon","coordinates":[[[0,0],[10,0],[10,10],[0,10],[0,0]]]},"properties":{}}]}'

  function init() {
    lastToastType = "";
    testLayer = LayerUtils.memoryLayerFromJsonString("fillring_test", squareJson, CoordinateReferenceSystemUtils.fromDescription("EPSG:3857"));
  }

  function cleanup() {
    fillRingTool.cancel();
    testLayer = null;
  }

  function initFillRing() {
    featureModel.currentLayer = testLayer;
    featureModel.feature = testLayer.getFeature(1);
    rubberband.vectorLayer = testLayer;
    fillRingTool.init(featureModel, mapSettingsItem, rubberband, null);
    return featureModel;
  }

  function addToolVertex(toolbar, x, y) {
    rubberband.currentCoordinate = GeometryUtils.point(x, y);
    toolbar.addVertex();
  }

  function toolbar() {
    return Utils.findChildren(fillRingTool, "fillRingDigitizingToolbar");
  }

  MapSettings {
    id: mapSettingsItem
    destinationCrs: CoordinateReferenceSystemUtils.fromDescription("EPSG:3857")
  }

  RubberbandModel {
    id: rubberband
    crs: CoordinateReferenceSystemUtils.fromDescription("EPSG:3857")
  }

  FeatureModel {
    id: featureModel
    project: qgisProject

    vertexModel: VertexModel {
      id: geometryEditingVertexModel
    }
  }

  GeometryEditors.FillRing {
    id: fillRingTool
    featureModel: featureModel
    mapSettings: mapSettingsItem
  }

  function test_initSetsUpToolForPolygonRing() {
    initFillRing();
    compare(fillRingTool.featureModel, featureModel);
    compare(Number(rubberband.geometryType), Qgis.GeometryType.Polygon);
  }

  function test_blockingFollowsRubberbandVertexCount() {
    initFillRing();
    compare(fillRingTool.blocking, false);

    const tb = toolbar();
    addToolVertex(tb, 5, 5);
    addToolVertex(tb, 6, 6);

    compare(fillRingTool.blocking, true);
  }

  function test_confirmWithValidRingAddsInteriorRing() {
    initFillRing();
    const tb = toolbar();

    // the feature starts as a plain square with a single ring
    compare(testLayer.getFeature(1).geometry.asWkt(2), "Polygon ((0 0, 10 0, 10 10, 0 10, 0 0))");

    addToolVertex(tb, 2, 2);
    addToolVertex(tb, 8, 2);
    addToolVertex(tb, 2, 8);

    tb.confirm();

    // confirm adds the interior ring, so the polygon now has a hole
    const expected = "Polygon ((0 0, 10 0, 10 10, 0 10, 0 0),(2 2, 2 8, 2 8, 8 2, 2 2))";
    compare(testLayer.getFeature(1).geometry.asWkt(2), expected);
  }

  function test_confirmWithInvalidRingToasts() {
    initFillRing();
    const before = testLayer.getFeature(1).geometry.asWkt();
    const tb = toolbar();

    addToolVertex(tb, 5, 5);
    tb.confirm();

    compare(lastToastType, "error");
    const after = testLayer.getFeature(1).geometry.asWkt();
    compare(after, before);
  }

  function test_confirmWithRingOutsideFeatureToasts() {
    initFillRing();
    const before = testLayer.getFeature(1).geometry.asWkt();
    const tb = toolbar();

    addToolVertex(tb, 20, 20);
    addToolVertex(tb, 30, 20);
    addToolVertex(tb, 20, 30);
    tb.confirm();

    compare(lastToastType, "error");
    const after = testLayer.getFeature(1).geometry.asWkt();
    compare(after, before);
  }

  function test_cancelResetsRubberband() {
    initFillRing();
    const tb = toolbar();
    addToolVertex(tb, 5, 5);
    addToolVertex(tb, 6, 6);
    compare(rubberband.vertexCount, 3);

    tb.cancel();

    compare(rubberband.vertexCount, 1);
  }

  function test_fillRingCreatesNewFeature() {
    initFillRing();
    const tb = toolbar();
    addToolVertex(tb, 2, 2);
    addToolVertex(tb, 8, 2);
    addToolVertex(tb, 2, 8);
    tb.confirm();
    fillRingTool.addPolygonDialog.accept();
    fillRingTool.formPopupLoader.confirmForm();
    compare(testLayer.getFeature(1).geometry.asWkt(2), "Polygon ((0 0, 10 0, 10 10, 0 10, 0 0),(2 2, 2 8, 2 8, 8 2, 2 2))");
    compare(testLayer.getFeature(2).geometry.asWkt(2), "Polygon ((2 2, 8 2, 2 8, 2 8, 2 2))");
  }

  Item {
    id: mainWindow
    property var contentItem: Window.window ? Window.window.contentItem : null
    width: 800
    height: 600
    property int sceneTopMargin: 0
    property int sceneBottomMargin: 0
  }

  Item {
    id: dashBoard
    property bool shouldReturnHome: false
  }

  Item {
    id: stateMachine
    property string state: "digitize"
  }

  function displayToast(message, type, actionText, actionCallback) {
    lastToastType = type !== undefined ? type : "";
  }

  Item {
    id: qfieldSettings
    property bool autoSave: false
  }

  Item {
    id: coordinateLocator
    property var currentCoordinate: GeometryUtils.point(0, 0)
    property string positionInformation: ""
    property string topSnappingResult: ""
    property bool positionLocked: false
    function flash() {
    }
  }

  Item {
    id: projectInfo
    property string cloudUserInformation: ""
  }

  AppExpressionContextScopesGenerator {
    id: appScopesGenerator
    objectName: "appScopesGenerator"
    positionInformation: positionSource.positionInformation
    positionLocked: coordinateLocator.positionLocked
    cloudUserInformation: projectInfo.cloudUserInformation
  }

  Item {
    id: positionSource
    property string positionInformation: ""
    property bool averagedPosition: false
    property int averagedPositionCount: 0
  }

  Item {
    id: positioningSettings
    property bool averagedPositioning: false
    property int averagedPositioningMinimumCount: 0
    property bool averagedPositioningAutomaticStop: false
    property bool accuracyIndicator: false
    property bool accuracyRequirement: false
    property real accuracyBad: 0
  }
}
