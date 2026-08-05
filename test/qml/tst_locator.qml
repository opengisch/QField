import QtQuick
import QtTest
import org.qgis
import org.qfield
import org.qfield.core as QFieldControls

/**
 * test_locator.cpp covers each filter's logic in isolation. These tests instead
 * drive the locator the way the search bar does: performSearch() on a wired-up
 * LocatorModelSuperBridge, reading results back from proxyModel().
 */
TestCase {
  id: testCase
  name: "Locator"

  property var searchLayer: null

  QFieldControls.QfMapCanvas {
    id: mapCanvas
    objectName: "mapCanvas"
  }

  // Stands in for the real geometry highlighter that triggers write to
  QtObject {
    id: geometryHighlighterStub

    property var qgsGeometry
    property var crs
  }

  LocatorModelSuperBridge {
    id: locatorBridge

    mapSettings: mapCanvas.mapSettings
    geometryHighlighter: geometryHighlighterStub
  }

  SignalSpy {
    id: jumpSpy

    target: locatorBridge
    signalName: "requestJumpToPoint"
  }

  LocatorFiltersModel {
    id: locatorFiltersModel

    locatorModelSuperBridge: locatorBridge
  }

  Component {
    id: pluginFilterComponent

    QFieldLocatorFilter {
      name: "echo"
      displayName: "Echo"
      prefix: "echo"
      locatorBridge: locatorBridge
      source: "file://" + dataDir + "/locator_plugin_filter.qml"
    }
  }

  readonly property int resultTypeRole: 258
  readonly property int entryTypeResult: 2

  function initTestCase() {
    let mapSettings = mapCanvas.mapSettings;
    mapSettings.destinationCrs = CoordinateReferenceSystemUtils.wgs84Crs();
    mapSettings.outputSize = Qt.size(1000, 500);
    mapSettings.extent = GeometryUtils.createRectangleFromPoints(GeometryUtils.point(-10, -10), GeometryUtils.point(10, 10));

    let fields = FeatureUtils.createFields([FeatureUtils.createField("name", FeatureUtils.String), FeatureUtils.createField("fid", FeatureUtils.Int)]);
    searchLayer = LayerUtils.createMemoryLayer("LocatorPoints", fields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    ProjectUtils.addMapLayer(qgisProject, searchLayer);

    searchLayer.startEditing();
    addPoint(1, "Alpha", 1, 1);
    addPoint(2, "Beta", 2, 2);
    searchLayer.commitChanges();

    locatorBridge.activeLayer = searchLayer;
  }

  function cleanupTestCase() {
    locatorBridge.activeLayer = null;
    ProjectUtils.removeMapLayer(qgisProject, searchLayer);
  }

  function cleanup() {
    locatorBridge.invalidateResults();
  }

  function addPoint(fid, name, x, y) {
    let feature = FeatureUtils.createBlankFeature(searchLayer.fields, GeometryUtils.createGeometryFromWkt("POINT(" + x + " " + y + ")"));
    feature.setAttribute(searchLayer.fields.indexOf("fid"), fid);
    feature.setAttribute(searchLayer.fields.indexOf("name"), name);
    LayerUtils.addFeature(searchLayer, feature);
  }

  // Searches and waits for the asynchronous fetch to settle before returning
  // the results model.
  function search(string) {
    locatorBridge.performSearch(string);
    tryVerify(function () {
      return !locatorBridge.isRunning;
    }, 5000);
    return locatorBridge.proxyModel();
  }

  // Row of the first result matching text (or any result if text is empty),
  // skipping the filter-name and group header rows. Returns -1 if none.
  function findResultRow(model, text) {
    for (let row = 0; row < model.rowCount(); ++row) {
      let index = model.index(row, 0);
      if (model.data(index, resultTypeRole) !== entryTypeResult) {
        continue;
      }
      let displayText = ("" + model.data(index, Qt.DisplayRole)).trim();
      if (text === "" || displayText === text) {
        return row;
      }
    }
    return -1;
  }

  function resultText(model, row) {
    return ("" + model.data(model.index(row, 0), Qt.DisplayRole)).trim();
  }

  function filterRow(prefix) {
    for (let row = 0; row < locatorFiltersModel.rowCount(); ++row) {
      if (locatorFiltersModel.data(locatorFiltersModel.index(row, 0), LocatorFiltersModel.PrefixRole) === prefix) {
        return row;
      }
    }
    return -1;
  }

  function test_00_registersBuiltinFilters() {
    compare(locatorFiltersModel.rowCount(), 6);
    verify(filterRow("f") !== -1);
    verify(filterRow("af") !== -1);
    verify(filterRow("go") !== -1);
    verify(filterRow("b") !== -1);
    verify(filterRow("=") !== -1);
    verify(filterRow("?") !== -1);
  }

  function test_calculatorEvaluatesThroughPrefix() {
    let model = search("= 1 + 1");
    let row = findResultRow(model, "");
    verify(row !== -1);
    verify(resultText(model, row).indexOf("2") !== -1);
  }

  function test_gotoParsesCoordinateAndOffersNavigation() {
    let model = search("go 1.5 2.5");
    let row = findResultRow(model, "");
    verify(row !== -1);
    let actions = locatorBridge.contextMenuActionsModel(row);
    verify(actions !== null);
    verify(actions.rowCount() > 0);
  }

  // The bridge does not move the map itself; it emits requestJumpToPoint, which
  // the app connects to the canvas. Input is "lat lon", so (x, y) is (lon, lat).
  function test_gotoTriggerRequestsJumpToCoordinate() {
    let model = search("go 3 4");
    let row = findResultRow(model, "");
    verify(row !== -1);

    jumpSpy.clear();
    locatorBridge.triggerResultAtRow(row);
    compare(jumpSpy.count, 1);

    let point = jumpSpy.signalArguments[0][0];
    fuzzyCompare(point.x, 4, 0.001);
    fuzzyCompare(point.y, 3, 0.001);
  }

  function test_activeLayerFeatureSearchMatchesByName() {
    let model = search("f Alpha");
    verify(findResultRow(model, "Alpha") !== -1);
    compare(findResultRow(model, "Beta"), -1);
  }

  function test_noMatchYieldsEmptyResults() {
    let model = search("f zzzzznomatch");
    compare(findResultRow(model, ""), -1);
  }

  function test_defaultToggleControlsUnprefixedUse() {
    let gotoRow = filterRow("go");
    verify(gotoRow !== -1);

    let index = locatorFiltersModel.index(gotoRow, 0);
    let wasDefault = locatorFiltersModel.data(index, LocatorFiltersModel.DefaultRole);
    verify(locatorFiltersModel.setData(index, !wasDefault, LocatorFiltersModel.DefaultRole));
    compare(locatorFiltersModel.data(index, LocatorFiltersModel.DefaultRole), !wasDefault);

    locatorFiltersModel.setData(index, wasDefault, LocatorFiltersModel.DefaultRole);
  }

  function test_pluginFilterContributesResults() {
    let pluginFilter = createTemporaryObject(pluginFilterComponent, testCase);
    verify(pluginFilter !== null);
    locatorBridge.registerQFieldLocatorFilter(pluginFilter);

    let model = search("echo hello");
    verify(findResultRow(model, "echo: hello") !== -1);

    locatorBridge.deregisterQFieldLocatorFilter(pluginFilter);
  }
}
