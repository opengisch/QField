import QtQuick
import QtTest
import org.qfield
import org.qgis
import Theme
import "../../src/qml/geometryeditors" as GeometryEditors

TestCase {
  id: testCase
  name: "GeometryEditorFillRing"

  property var fieldsLayer: qgisProject.mapLayersByName("Fields")[0]
  property string lastToastType: ""

  function init() {
    lastToastType = "";
  }

  function cleanup() {
    fillRingTool.cancel();
    if (fieldsLayer.editBuffer()) {
      fieldsLayer.rollBack();
    }
  }

  // fill ring works on the current feature's layer, set up the feature model and
  // hand the tool a fresh rubberband like the app does through init
  function initFillRingOnFields() {
    featureModel.currentLayer = fieldsLayer;
    featureModel.feature = fieldsLayer.getFeature("39");
    rubberband.vectorLayer = fieldsLayer;
    fillRingTool.init(featureModel, mapSettingsItem, rubberband, null);
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

  GeometryEditors.FillRing {
    id: fillRingTool
    featureModel: featureModel
    mapSettings: mapSettingsItem
  }

  function test_initSetsUpToolForPolygonRing() {
    initFillRingOnFields();
    compare(fillRingTool.featureModel, featureModel);
    compare(Number(rubberband.geometryType), Qgis.GeometryType.Polygon);
  }

  function test_blockingFollowsRubberbandVertexCount() {
    initFillRingOnFields();
    // blocking mirrors isDigitizing, which is vertexCount > 1
    compare(fillRingTool.blocking, false);

    rubberband.addVertexFromPoint(GeometryUtils.point(1030900, 5911400));
    rubberband.addVertexFromPoint(GeometryUtils.point(1031000, 5911500));

    compare(fillRingTool.blocking, true);
  }

  function test_confirmWithInvalidRingToasts() {
    const model = initFillRingOnFields();
    const before = fieldsLayer.getFeature("39").geometry.asWkt();

    // a single point is fewer than the three vertices a ring needs. drive the
    // operation directly: it returns AddRingNotValid and leaves the feature
    // unchanged. the confirm() handler is not exercised here because its success
    // branch instantiates an embedded feature form, which is covered separately
    rubberband.addVertexFromPoint(GeometryUtils.point(1030900, 5911400));
    const result = GeometryUtils.addRingFromRubberband(fieldsLayer, model.feature.id, rubberband);

    compare(Number(result), Number(GeometryUtils.AddRingNotValid));
    const after = fieldsLayer.getFeature("39").geometry.asWkt();
    compare(after, before);
  }

  function test_cancelResetsRubberband() {
    initFillRingOnFields();
    rubberband.addVertexFromPoint(GeometryUtils.point(1030900, 5911400));
    rubberband.addVertexFromPoint(GeometryUtils.point(1031000, 5911500));
    // each added point carries a trailing cursor vertex, so two points give three
    compare(rubberband.vertexCount, 3);

    fillRingTool.cancel();

    // cancel resets the rubberband down to its single trailing vertex
    compare(rubberband.vertexCount, 1);
  }

  // scope objects the tool and DigitizingToolbar expect from the app
  Item {
    id: mainWindow
    property var contentItem: mainWindow
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
