import QtQuick
import QtTest
import org.qfield
import org.qgis
import Theme
import "Utils.js" as Utils
import "../../src/qml/geometryeditors" as GeometryEditors

TestCase {
  id: testCase
  name: "GeometryEditorSplit"

  property var testLayer: null
  property string lastToastType: ""
  readonly property string squareJson: '{"type":"FeatureCollection","features":[{"type":"Feature","id":0,"geometry":{"type":"Polygon","coordinates":[[[0,0],[10,0],[10,10],[0,10],[0,0]]]},"properties":{}}]}'

  function init() {
    lastToastType = "";
    testLayer = LayerUtils.memoryLayerFromJsonString("split_test", squareJson, CoordinateReferenceSystemUtils.fromDescription("EPSG:3857"));
  }

  function cleanup() {
    splitTool.cancel();
    testLayer = null;
  }

  // set up the feature model on the memory layer and
  // hand the tool a fresh rubberband like the app does through init
  function initSplit() {
    featureModel.currentLayer = testLayer;
    featureModel.feature = testLayer.getFeature(1);
    rubberband.vectorLayer = testLayer;
    splitTool.init(featureModel, mapSettingsItem, rubberband, null);
    return featureModel;
  }

  function addToolVertex(toolbar, x, y) {
    rubberband.currentCoordinate = GeometryUtils.point(x, y);
    toolbar.addVertex();
  }

  function toolbar() {
    return Utils.findChildren(splitTool, "splitDigitizingToolbar");
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

  GeometryEditors.SplitFeature {
    id: splitTool
    featureModel: featureModel
    mapSettings: mapSettingsItem
  }

  SignalSpy {
    id: finishedSpy
    target: splitTool
    signalName: "finished"
  }

  function test_initSetsUpToolForLineSplit() {
    initSplit();

    // init wires the feature model and sets the rubberband to line mode, since
    // the split is drawn as a line across the feature
    compare(splitTool.featureModel, featureModel);
    compare(Number(rubberband.geometryType), Qgis.GeometryType.Line);
  }

  function test_blockingFollowsRubberbandVertexCount() {
    initSplit();
    // blocking mirrors isDigitizing, which is vertexCount > 1
    compare(splitTool.blocking, false);

    const tb = toolbar();
    addToolVertex(tb, 5, 5);
    addToolVertex(tb, 6, 6);

    compare(splitTool.blocking, true);
  }

  function test_splitWithValidLineProducesExpectedGeometries() {
    initSplit();
    const tb = toolbar();

    // a vertical line crossing the square, splitting it into two halves
    addToolVertex(tb, 5, -1);
    addToolVertex(tb, 5, 11);

    // drive the tool through confirm so its onConfirmed runs the split
    tb.confirm();

    // the square is cut at x=5 into two rectangles: the original feature keeps
    // the right half and a new feature holds the left half
    compare(testLayer.getFeature(1).geometry.asWkt(2), "Polygon ((5 0, 5 10, 10 10, 10 0, 5 0))");
    compare(testLayer.getFeature(2).geometry.asWkt(2), "Polygon ((5 10, 5 0, 0 0, 0 10, 5 10))");
  }

  function test_confirmWithInvalidLineToasts() {
    initSplit();
    const before = testLayer.getFeature(1).geometry.asWkt();
    const tb = toolbar();

    // a single point is not a valid split line, so splitFeatureFromRubberband
    // returns non-Success. the handler should toast an error and leave the
    // feature unchanged
    addToolVertex(tb, 5, 5);
    tb.confirm();

    compare(lastToastType, "error");
    const after = testLayer.getFeature(1).geometry.asWkt();
    compare(after, before);
  }

  function test_confirmEmitsFinished() {
    initSplit();
    finishedSpy.clear();
    const tb = toolbar();

    // confirm always emits finished so the editor closes, even on a failed split
    addToolVertex(tb, 5, 5);
    tb.confirm();

    compare(finishedSpy.count, 1);
  }

  function test_cancelResetsRubberband() {
    initSplit();
    const tb = toolbar();
    addToolVertex(tb, 5, 5);
    addToolVertex(tb, 6, 6);
    compare(rubberband.vertexCount, 3);

    tb.cancel();

    // cancel resets the rubberband down to its single trailing vertex
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
