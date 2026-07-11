import QtQuick
import QtTest
import org.qfield
import org.qgis
import Theme
import "Utils.js" as Utils
import "../../src/qml/geometryeditors" as GeometryEditors

TestCase {
  id: testCase
  name: "GeometryEditorVertex"

  property var testLayer: null
  property var lineLayer: null
  readonly property string squareJson: '{"type":"FeatureCollection","features":[{"type":"Feature","id":0,"geometry":{"type":"Polygon","coordinates":[[[0,0],[10,0],[10,10],[0,10],[0,0]]]},"properties":{}}]}'
  readonly property string lineJson: '{"type":"FeatureCollection","features":[{"type":"Feature","id":0,"geometry":{"type":"LineString","coordinates":[[0,0],[5,0],[10,0]]},"properties":{}}]}'

  function init() {
    testLayer = LayerUtils.memoryLayerFromJsonString("vertex_test", squareJson, CoordinateReferenceSystemUtils.fromDescription("EPSG:3857"));
    lineLayer = LayerUtils.memoryLayerFromJsonString("vertex_line_test", lineJson, CoordinateReferenceSystemUtils.fromDescription("EPSG:3857"));
  }

  function cleanup() {
    vertexTool.cancel();
    geometryEditingVertexModel.clear();
    testLayer = null;
    lineLayer = null;
  }

  function initVertex(layer) {
    featureModel.currentLayer = layer;
    featureModel.feature = layer.getFeature(1);
    geometryEditingVertexModel.geometry = featureModel.feature.geometry;
    geometryEditingVertexModel.crs = layer.crs;
    vertexTool.init(featureModel, mapSettingsItem, null, null);
    return featureModel;
  }

  function selectVertexAt(x, y) {
    const screenPoint = mapSettingsItem.coordinateToScreen(GeometryUtils.point(x, y));
    vertexTool.canvasClicked(screenPoint, "");
  }

  function moveCurrentVertexTo(x, y) {
    geometryEditingVertexModel.currentPoint = GeometryUtils.point(x, y);
  }

  function button(name) {
    return Utils.findChildren(vertexTool, name);
  }

  MapSettings {
    id: mapSettingsItem
    destinationCrs: CoordinateReferenceSystemUtils.fromDescription("EPSG:3857")
    outputSize: Qt.size(1000, 1000)
    extent: GeometryUtils.createRectangleFromPoints(GeometryUtils.point(-5, -5), GeometryUtils.point(15, 15))
  }

  FeatureModel {
    id: featureModel
    project: qgisProject

    vertexModel: VertexModel {
      id: geometryEditingVertexModel
      mapSettings: mapSettingsItem
    }
  }

  GeometryEditors.VertexEditor {
    id: vertexTool
    featureModel: featureModel
    mapSettings: mapSettingsItem
  }

  function test_initClearsSelection() {
    initVertex(testLayer);

    compare(vertexTool.featureModel, featureModel);
    compare(geometryEditingVertexModel.currentVertexIndex, -1);
    compare(geometryEditingVertexModel.dirty, false);
  }

  function test_blockingFollowsDirty() {
    initVertex(testLayer);
    compare(vertexTool.blocking, false);

    selectVertexAt(0, 0);
    moveCurrentVertexTo(1, 1);

    compare(vertexTool.blocking, true);
  }

  function test_moveVertexOnPolygonSavesExpectedGeometry() {
    initVertex(testLayer);

    selectVertexAt(0, 0);
    moveCurrentVertexTo(-2, -2);
    button("vertexEditorApplyButton").clicked();

    compare(testLayer.getFeature(1).geometry.asWkt(2), "Polygon ((10 0, 10 10, 0 10, -2 -2, 10 0))");
  }

  function test_moveVertexOnLineSavesExpectedGeometry() {
    initVertex(lineLayer);

    selectVertexAt(5, 0);
    moveCurrentVertexTo(5, 5);
    button("vertexEditorApplyButton").clicked();

    compare(lineLayer.getFeature(1).geometry.asWkt(2), "LineString (0 0, 5 5, 10 0)");
  }

  function test_removeVertexOnLineSavesExpectedGeometry() {
    initVertex(lineLayer);

    selectVertexAt(5, 0);
    verify(geometryEditingVertexModel.canRemoveVertex);
    button("vertexEditorRemoveVertexButton").clicked();
    button("vertexEditorApplyButton").clicked();

    compare(lineLayer.getFeature(1).geometry.asWkt(2), "LineString (0 0, 10 0)");
  }

  function test_moveTwoVerticesSavesExpectedGeometry() {
    initVertex(testLayer);

    selectVertexAt(0, 0);
    moveCurrentVertexTo(-1, -1);
    selectVertexAt(10, 10);
    moveCurrentVertexTo(12, 12);
    button("vertexEditorApplyButton").clicked();

    compare(testLayer.getFeature(1).geometry.asWkt(2), "Polygon ((10 0, 12 12, 0 10, -1 -1, 10 0))");
  }

  function test_cancelDiscardsChanges() {
    initVertex(testLayer);
    const before = testLayer.getFeature(1).geometry.asWkt(2);

    selectVertexAt(0, 0);
    moveCurrentVertexTo(-2, -2);
    compare(vertexTool.blocking, true);

    button("vertexEditorCancelButton").clicked();

    compare(geometryEditingVertexModel.dirty, false);
    compare(testLayer.getFeature(1).geometry.asWkt(2), before);
  }

  function displayToast(message, type, actionText, actionCallback) {
  }

  Item {
    id: qfieldSettings
    property bool autoSave: false
  }

  Item {
    id: coordinateLocator
    property var currentCoordinate: GeometryUtils.point(0, 0)
    property string positionInformation: ""
    property var topSnappingResult
    property bool positionLocked: false
    function flash() {
    }
  }

  Item {
    id: projectInfo
    property var cloudUserInformation
  }

  Item {
    id: positionSource
    property var positionInformation
    property bool averagedPosition: false
    property int averagedPositionCount: 0
  }
}
