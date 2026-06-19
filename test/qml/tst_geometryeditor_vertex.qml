import QtQuick
import QtTest
import org.qfield
import org.qgis
import Theme
import "../../src/qml/geometryeditors" as GeometryEditors

// tests for the vertex editor tool. The vertex model itself is tested in
// test_vertexmodel.cpp, so here we only check the tool around it: init wiring,
// applyChanges and the autoSave branch, cancel, remove, and undo.
//
// canvasClicked is not tested here on purpose. It turns a screen point into a
// map coordinate through mapSettings.screenToCoordinate, which only makes sense
// with a real rendered canvas. without one the screen-to-map math is meaningless,
// so a click test would just be asserting made up numbers, so that path belongs in
// an integration (spix) test, not here.
//
// the tool reads a few things from its parent scope in the app, so we provide
// them here as plain items the same way tst_featureForm.qml does.
TestCase {
  id: testCase
  name: "GeometryEditorVertex"

  property var fieldsLayer: qgisProject.mapLayersByName("Fields")[0]
  property var tracksLayer: qgisProject.mapLayersByName("Tracks")[0]

  // we never commit to the layer, so just make sure any open edit session is
  // rolled back between tests and the layer is left clean
  function cleanup() {
    vertexEditor.cancel();
    if (fieldsLayer.editBuffer()) {
      fieldsLayer.rollBack();
    }
    if (tracksLayer.editBuffer()) {
      tracksLayer.rollBack();
    }
    qfieldSettings.autoSave = false;
  }

  function makeFeatureModel(layer, fid) {
    featureModel.currentLayer = layer;
    featureModel.feature = layer.getFeature(fid);
    featureModel.applyGeometryToVertexModel();
    return featureModel;
  }

  // Fields/39 is a simple polygon. its vertex model interleaves segment
  // candidates with real vertices, so the real (existing) vertices sit at the
  // odd indices and 1 is the first of them.
  function makeFieldsModel() {
    return makeFeatureModel(fieldsLayer, "39");
  }

  // select the first real vertex and move it, so the model goes dirty like it
  // would after a drag on the canvas. the layer is in meters so a meter-scale
  // move is well above the tools ignore-tiny-moves threshold.
  function selectAndMoveFirstVertex() {
    const vertexModel = featureModel.vertexModel;
    vertexModel.editingMode = VertexModel.EditVertex;
    vertexModel.currentVertexIndex = 1;
    const point = vertexModel.currentPoint;
    vertexModel.currentPoint = GeometryUtils.point(point.x + 5, point.y + 5);
  }

  MapSettings {
    id: mapSettingsItem
    destinationCrs: CoordinateReferenceSystemUtils.fromDescription("EPSG:3857")
  }

  FeatureModel {
    id: featureModel
    project: qgisProject

    vertexModel: VertexModel {
      id: geometryEditingVertexModel
    }
  }

  GeometryEditors.VertexEditor {
    id: vertexEditor
    featureModel: featureModel
    mapSettings: mapSettingsItem
  }

  function test_initWiresFeatureModelAndResetsCurrentVertex() {
    const model = makeFieldsModel();
    model.vertexModel.currentVertexIndex = 3;

    vertexEditor.init(model, mapSettingsItem, null, null);

    compare(vertexEditor.featureModel, model);
    // init should always start with no vertex selected
    compare(model.vertexModel.currentVertexIndex, -1);
  }

  function test_blockingFollowsVertexModelDirtyState() {
    const model = makeFieldsModel();
    vertexEditor.init(model, mapSettingsItem, null, null);
    compare(vertexEditor.blocking, false);

    selectAndMoveFirstVertex();

    // blocking is just dirty, so an unsaved edit should block
    compare(model.vertexModel.dirty, true);
    compare(vertexEditor.blocking, true);
  }

  // returns how many points in list a are not present (within tolerance) in
  // list b. Comparing both directions makes this independent of vertex order or
  // ring rotation, which a vertex move can introduce.
  function pointsNotIn(a, b) {
    let missing = 0;
    for (let i = 0; i < a.length; ++i) {
      let found = false;
      for (let j = 0; j < b.length; ++j) {
        if (Math.abs(a[i].x - b[j].x) < 0.001 && Math.abs(a[i].y - b[j].y) < 0.001) {
          found = true;
          break;
        }
      }
      if (!found)
        missing++;
    }
    return missing;
  }

  // reads the vertices out of a geometry's WKT as a plain array of points, so
  // tests can compare geometries vertex by vertex. Uses asWkt only, which is the
  // same accessor the other tests rely on. For a polygon ring the closing vertex
  // repeats the first one, so we drop that duplicate to count each vertex once.
  function geometryPoints(geometry) {
    const points = [];
    const wkt = geometry.asWkt(6);
    // pull the coordinate pairs from inside the parentheses
    const inner = wkt.substring(wkt.indexOf("("), wkt.lastIndexOf(")") + 1);
    const pairs = inner.replace(/[()]/g, " ").trim().split(",");
    for (let i = 0; i < pairs.length; ++i) {
      const nums = pairs[i].trim().split(/\s+/);
      if (nums.length >= 2 && nums[0] !== "") {
        points.push({
          x: parseFloat(nums[0]),
          y: parseFloat(nums[1])
        });
      }
    }
    // drop a repeated closing vertex (polygons repeat the first point at the end)
    if (points.length > 1) {
      const first = points[0];
      const last = points[points.length - 1];
      if (Math.abs(first.x - last.x) < 0.001 && Math.abs(first.y - last.y) < 0.001) {
        points.pop();
      }
    }
    return points;
  }

  function test_applyChangesAppliesMovedVertexToLayerBuffer() {
    const model = makeFieldsModel();
    vertexEditor.init(model, mapSettingsItem, null, null);

    // capture the polygon before the edit
    const before = geometryPoints(model.feature.geometry);

    // move the first real vertex and apply. we never commit, so the layer file
    // is untouched.
    selectAndMoveFirstVertex();
    vertexEditor.applyChanges(true);
    const movedGeom = model.feature.geometry;
    verify(!movedGeom.isNull);
    compare(movedGeom.type, Qgis.GeometryType.Polygon);

    const after = geometryPoints(movedGeom);
    compare(after.length, before.length);
    compare(pointsNotIn(before, after), 1);
    compare(pointsNotIn(after, before), 1);
  }

  function test_applyChangesWithAutoSaveOffDoesNotApply() {
    qfieldSettings.autoSave = false;
    const model = makeFieldsModel();
    vertexEditor.init(model, mapSettingsItem, null, null);

    selectAndMoveFirstVertex();
    const before = model.feature.geometry.asWkt();

    // the next/prev/add buttons call applyChanges(autoSave). with autoSave off
    // the edit stays in the model and is not pushed to the feature yet
    vertexEditor.applyChanges(qfieldSettings.autoSave);

    compare(model.feature.geometry.asWkt(), before);
    compare(model.vertexModel.dirty, true);
  }

  function test_applyChangesNoOpWhenNotDirty() {
    const model = makeFieldsModel();
    vertexEditor.init(model, mapSettingsItem, null, null);
    compare(model.vertexModel.dirty, false);

    const before = model.feature.geometry.asWkt();

    // nothing changed, so apply should do nothing
    vertexEditor.applyChanges(true);

    compare(model.feature.geometry.asWkt(), before);
  }

  function test_cancelResetsEditingModeAndClearsDirty() {
    const model = makeFieldsModel();
    vertexEditor.init(model, mapSettingsItem, null, null);
    selectAndMoveFirstVertex();
    compare(model.vertexModel.dirty, true);

    vertexEditor.cancel();

    // cancel throws away the edit and leaves editing mode
    compare(model.vertexModel.editingMode, VertexModel.NoEditing);
    compare(model.vertexModel.dirty, false);
  }

  function test_removeVertexReducesCountAndSetsDirty() {
    const model = makeFieldsModel();
    vertexEditor.init(model, mapSettingsItem, null, null);
    const vertexModel = model.vertexModel;

    vertexModel.editingMode = VertexModel.EditVertex;
    vertexModel.currentVertexIndex = 1;
    // the polygon has plenty of real vertices, well above the minimum, so
    // removing one is allowed
    verify(vertexModel.canRemoveVertex);
    const countBefore = vertexModel.vertexCount;

    vertexModel.removeCurrentVertex();

    // removing one existing vertex drops it and its paired segment candidate, so
    // the row count goes down by exactly two, and the edit is marked dirty
    compare(vertexModel.vertexCount, countBefore - 2);
    compare(vertexModel.dirty, true);
  }

  function test_canRemoveVertexFalseWithoutEditMode() {
    const model = makeFieldsModel();
    vertexEditor.init(model, mapSettingsItem, null, null);

    // canRemoveVertex only holds while a vertex is being edited, never before
    compare(model.vertexModel.editingMode, VertexModel.NoEditing);
    compare(model.vertexModel.canRemoveVertex, false);
  }

  function test_undoRestoresGeometryAfterMove() {
    const model = makeFieldsModel();
    vertexEditor.init(model, mapSettingsItem, null, null);
    const vertexModel = model.vertexModel;

    const before = vertexModel.currentPoint;
    selectAndMoveFirstVertex();
    verify(vertexModel.canUndo);

    vertexModel.undoHistory();

    // undo should put the moved vertex back where it started
    vertexModel.currentVertexIndex = 1;
    const restored = vertexModel.currentPoint;
    compare(restored.x, before.x);
    compare(restored.y, before.y);
  }

  function test_undoIsNoOpWithEmptyHistory() {
    const model = makeFieldsModel();
    vertexEditor.init(model, mapSettingsItem, null, null);

    // nothing has been edited, so there is nothing to undo
    compare(model.vertexModel.canUndo, false);
    model.vertexModel.undoHistory();
    compare(model.vertexModel.dirty, false);
  }

  function test_applyChangesAppliesMovedVertexOnLineLayer() {
    // same apply path but on a line feature, to cover the line geometry branch
    const model = makeFeatureModel(tracksLayer, "1");
    vertexEditor.init(model, mapSettingsItem, null, null);
    const vertexModel = model.vertexModel;

    const before = geometryPoints(model.feature.geometry);

    // on a line the first real vertex is also at an odd index
    vertexModel.editingMode = VertexModel.EditVertex;
    vertexModel.currentVertexIndex = 1;
    const point = vertexModel.currentPoint;
    vertexModel.currentPoint = GeometryUtils.point(point.x + 5, point.y + 5);
    compare(vertexModel.dirty, true);

    vertexEditor.applyChanges(true);

    // a still-valid line with the same vertex count, where exactly one original
    // point is replaced by exactly one new point: the moved vertex
    const movedGeom = model.feature.geometry;
    verify(!movedGeom.isNull);
    compare(movedGeom.type, Qgis.GeometryType.Line);

    const after = geometryPoints(movedGeom);
    compare(after.length, before.length);
    compare(pointsNotIn(before, after), 1);
    compare(pointsNotIn(after, before), 1);
  }

  // scope objects the tool expects from the app
  Item {
    id: mainWindowItem
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
  }

  Item {
    id: projectInfo
    property string cloudUserInformation: ""
  }

  Item {
    id: positionSource
    property string positionInformation: ""
  }
}
