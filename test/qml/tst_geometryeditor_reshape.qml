import QtQuick
import QtTest
import org.qfield
import org.qgis
import Theme
import "Utils.js" as Utils
import "../../src/qml/geometryeditors" as GeometryEditors

TestCase {
  id: testCase
  name: "GeometryEditorReshape"

  property var testLayer: null
  property string lastToastType: ""
  readonly property string squareJson: '{"type":"FeatureCollection","features":[{"type":"Feature","id":0,"geometry":{"type":"Polygon","coordinates":[[[0,0],[10,0],[10,10],[0,10],[0,0]]]},"properties":{}}]}'

  function init() {
    lastToastType = "";
    testLayer = LayerUtils.memoryLayerFromJsonString("reshape_test", squareJson, CoordinateReferenceSystemUtils.fromDescription("EPSG:3857"));
  }

  function cleanup() {
    reshapeTool.cancel();
    testLayer = null;
  }

  // set up the feature model on the memory layer and
  // hand the tool a fresh rubberband like the app does through init
  function initReshape() {
    featureModel.currentLayer = testLayer;
    featureModel.feature = testLayer.getFeature(1);
    rubberband.vectorLayer = testLayer;
    reshapeTool.init(featureModel, mapSettingsItem, rubberband, null);
    return featureModel;
  }

  function addToolVertex(toolbar, x, y) {
    rubberband.currentCoordinate = GeometryUtils.point(x, y);
    toolbar.addVertex();
  }

  function toolbar() {
    return Utils.findChildren(reshapeTool, "reshapeDigitizingToolbar");
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

  GeometryEditors.Reshape {
    id: reshapeTool
    featureModel: featureModel
    mapSettings: mapSettingsItem
  }

  function test_initSetsUpToolForPolygonReshape() {
    initReshape();

    // init wires the feature model and forces the rubberband to polygon mode,
    // since reshape only operates on polygon rings
    compare(reshapeTool.featureModel, featureModel);
    compare(Number(rubberband.geometryType), Qgis.GeometryType.Polygon);
  }

  function test_blockingFollowsRubberbandVertexCount() {
    initReshape();
    // blocking mirrors isDigitizing, which is vertexCount > 1
    compare(reshapeTool.blocking, false);

    const tb = toolbar();
    addToolVertex(tb, 5, 5);
    addToolVertex(tb, 6, 6);

    compare(reshapeTool.blocking, true);
  }

  function test_reshapeWithValidLineProducesExpectedGeometry() {
    initReshape();
    const tb = toolbar();

    // a reshape line that cuts across the top right corner of the square
    addToolVertex(tb, 10, 5);
    addToolVertex(tb, 5, 10);

    // drive the tool through confirm so its onConfirmed runs the reshape
    tb.confirm();

    // the corner is cut off, replaced by the reshape line
    const expected = "Polygon ((5 10, 0 10, 0 0, 10 0, 10 5, 5 10))";
    compare(testLayer.getFeature(1).geometry.asWkt(2), expected);
  }

  function test_confirmWithInvalidLineToastsAndDoesNotChangeGeometry() {
    initReshape();
    const before = testLayer.getFeature(1).geometry.asWkt();
    const tb = toolbar();

    // a single point is not a valid reshape line, so reshapeFromRubberband
    // returns non-Success. the confirm handler should toast an error and roll
    // back, leaving the feature untouched
    addToolVertex(tb, 5, 5);
    tb.confirm();

    compare(lastToastType, "error");
    const after = testLayer.getFeature(1).geometry.asWkt();
    compare(after, before);
  }

  function test_confirmWithNonOverlappingLineToastsAndDoesNotChangeGeometry() {
    initReshape();
    const before = testLayer.getFeature(1).geometry.asWkt();
    const tb = toolbar();
    addToolVertex(tb, 0, -5);
    addToolVertex(tb, 10, -5);
    tb.confirm();

    compare(lastToastType, "error");
    const after = testLayer.getFeature(1).geometry.asWkt();
    compare(after, before);
  }

  function test_cancelResetsRubberband() {
    initReshape();
    const tb = toolbar();
    addToolVertex(tb, 5, 5);
    addToolVertex(tb, 6, 6);
    compare(rubberband.vertexCount, 3);
    tb.cancel();
    compare(rubberband.vertexCount, 1);
  }

  // scope objects the tool and DigitizingToolbar expect from the app
  Item {
    id: mainWindow
    property var contentItem: mainWindow
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
