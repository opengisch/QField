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

  function test_reshapeWithValidLineProducesExpectedGeometry() {
    const model = initReshapeOnFields();
    // a reshape line that crosses the polygon boundary twice, cutting a new edge
    rubberband.addVertexFromPoint(GeometryUtils.point(1030845.75, 5911397.39));
    rubberband.addVertexFromPoint(GeometryUtils.point(1030771.49, 5911511.09));
    rubberband.addVertexFromPoint(GeometryUtils.point(1030857.23, 5911624.79));

    // drive the operation directly and roll back instead of committing, so the
    // shared layer data is untouched
    if (!fieldsLayer.editBuffer())
      fieldsLayer.startEditing();
    const result = GeometryUtils.reshapeFromRubberband(fieldsLayer, model.feature.id, rubberband);
    // the reshape succeeds and produces this exact polygon
    compare(Number(result), Number(GeometryUtils.Success));
    const expected = "Polygon ((1031040.99 5911336.9, 1030978.97 5911394.33, 1030845.75 5911397.39, 1030845.75 5911397.4, 1030857.23 5911624.79, 1031057.07 5911646.23, 1031082.33 5911535.21, 1031119.08 5911493.1, 1031093.82 5911453.28, 1031072.67 5911421.57, 1031063.19 5911407.34, 1031044.82 5911362.94, 1031041.44 5911340.01, 1031040.99 5911336.9))";
    compare(fieldsLayer.getFeature(model.feature.id).geometry.asWkt(2), expected);
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
    compare(rubberband.vertexCount, 3);

    reshapeTool.cancel();

    // cancel resets the rubberband to empty
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
