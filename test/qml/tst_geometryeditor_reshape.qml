import QtQuick
import QtTest
import org.qfield
import org.qgis
import Theme
import "../../src/qml/geometryeditors" as GeometryEditors

// The reshape maths is covered in test_geometryutils.cpp, so here we just check the tool wiring around it: init,
// the blocking state, the failure path (toast and rollback), and cancel. A valid
// reshape through confirm() needs live digitizing state to be reproduced, so
// that path is left to the C++ tests.

TestCase {
  id: testCase
  name: "GeometryEditorReshape"

  property var fieldsLayer: qgisProject.mapLayersByName("Fields")[0]
  property string lastToastType: ""

  function init() {
    lastToastType = "";
  }

  function cleanup() {
    reshapeTool.cancel();
    if (fieldsLayer.editBuffer()) {
      fieldsLayer.rollBack();
    }
  }

  // reshape works on the current features layer, set up the feature model and
  // hand the tool a fresh rubberband like the app does through init
  function initReshapeOnFields() {
    featureModel.currentLayer = fieldsLayer;
    featureModel.feature = fieldsLayer.getFeature("39");
    rubberband.vectorLayer = fieldsLayer;
    reshapeTool.init(featureModel, mapSettingsItem, rubberband, null);
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

  GeometryEditors.Reshape {
    id: reshapeTool
    featureModel: featureModel
    mapSettings: mapSettingsItem
  }

  function test_initSetsUpToolForPolygonReshape() {
    initReshapeOnFields();

    // init wires the feature model and forces the rubberband to polygon mode,
    // since reshape only operates on polygon rings
    compare(reshapeTool.featureModel, featureModel);
    compare(Number(rubberband.geometryType), Qgis.GeometryType.Polygon);
  }

  function test_blockingFollowsRubberbandVertexCount() {
    initReshapeOnFields();
    // blocking mirrors isDigitizing, which is vertexCount > 1
    compare(reshapeTool.blocking, false);

    rubberband.addVertexFromPoint(GeometryUtils.point(1030900, 5911400));
    rubberband.addVertexFromPoint(GeometryUtils.point(1031000, 5911500));

    compare(reshapeTool.blocking, true);
  }

  function test_confirmWithInvalidLineToastsAndDoesNotChangeGeometry() {
    const model = initReshapeOnFields();
    const before = fieldsLayer.getFeature("39").geometry.asWkt();

    // a single point is not a valid reshape line, so reshapeFromRubberband
    // returns non-Success. the confirm handler should toast an error and roll
    // back, leaving the feature untouched
    rubberband.addVertexFromPoint(GeometryUtils.point(1030900, 5911400));
    reshapeTool.children[0].confirm();

    compare(lastToastType, "error");
    const after = fieldsLayer.getFeature("39").geometry.asWkt();
    compare(after, before);
  }

  function test_cancelResetsRubberband() {
    initReshapeOnFields();
    rubberband.addVertexFromPoint(GeometryUtils.point(1030900, 5911400));
    rubberband.addVertexFromPoint(GeometryUtils.point(1031000, 5911500));
    verify(rubberband.vertexCount > 1);

    reshapeTool.cancel();

    // cancel clears the rubberband back down
    verify(rubberband.vertexCount <= 1);
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
