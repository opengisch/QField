import QtQuick
import QtTest
import org.qfield
import org.qgis
import Theme
import "../../src/qml/geometryeditors" as GeometryEditors

TestCase {
  id: testCase
  name: "GeometryEditorSplit"

  property var fieldsLayer: qgisProject.mapLayersByName("Fields")[0]
  property string lastToastType: ""

  function init() {
    lastToastType = "";
  }

  function cleanup() {
    splitTool.cancel();
    if (fieldsLayer.editBuffer()) {
      fieldsLayer.rollBack();
    }
  }

  function initSplitOnFields() {
    featureModel.currentLayer = fieldsLayer;
    featureModel.feature = fieldsLayer.getFeature("39");
    rubberband.vectorLayer = fieldsLayer;
    splitTool.init(featureModel, mapSettingsItem, rubberband, null);
    return featureModel;
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
    initSplitOnFields();

    // init wires the feature model and sets the rubberband to line mode, since
    // the split is drawn as a line across the feature
    compare(splitTool.featureModel, featureModel);
    compare(Number(rubberband.geometryType), Qgis.GeometryType.Line);
  }

  function test_blockingFollowsRubberbandVertexCount() {
    initSplitOnFields();
    // blocking mirrors isDigitizing, which is vertexCount > 1
    compare(splitTool.blocking, false);

    rubberband.addVertexFromPoint(GeometryUtils.point(1030900, 5911400));
    rubberband.addVertexFromPoint(GeometryUtils.point(1031000, 5911500));

    compare(splitTool.blocking, true);
  }

  function test_confirmWithInvalidLineToasts() {
    initSplitOnFields();
    const before = fieldsLayer.getFeature("39").geometry.asWkt();

    // a single point is not a valid split line, so splitFeatureFromRubberband
    // returns non-Success and rolls back. the handler should toast an error and
    // leave the feature unchanged
    rubberband.addVertexFromPoint(GeometryUtils.point(1030900, 5911400));
    splitTool.children[0].confirm();

    compare(lastToastType, "error");
    const after = fieldsLayer.getFeature("39").geometry.asWkt();
    compare(after, before);
  }

  function test_confirmEmitsFinished() {
    initSplitOnFields();
    finishedSpy.clear();

    // confirm always emits finished so the editor closes, even on a failed split
    rubberband.addVertexFromPoint(GeometryUtils.point(1030900, 5911400));
    splitTool.children[0].confirm();

    compare(finishedSpy.count, 1);
  }

  function test_cancelResetsRubberband() {
    initSplitOnFields();
    rubberband.addVertexFromPoint(GeometryUtils.point(1030900, 5911400));
    rubberband.addVertexFromPoint(GeometryUtils.point(1031000, 5911500));
    // each added point carries a trailing cursor vertex, so two points give three
    compare(rubberband.vertexCount, 3);

    splitTool.cancel();

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
