import QtQuick
import QtTest
import org.qgis
import org.qfield.core

TestCase {
  name: "LayerUtils"

  /**
   * Creates a memory layer with two fields and three features, then exercises
   * QfLayerUtils.saveVectorLayerAs and QfLayerUtils.loadVectorLayer end-to-end.
   *
   * Verifies that:
   * Exporting with no filter writes every feature and returns the final path
   * A filter expression restricts the exported subset
   * Attributes survive the export / reload round trip
   * Features can be iterated from a loaded layer and copied into a target
   * layer via QfLayerUtils.addFeature
   */

  function test_00_exportImportWorkflow() {
    //Create source layer and register it
    let sourceFields = QfFeatureUtils.createFields([QfFeatureUtils.createField("id", QfFeatureUtils.Int), QfFeatureUtils.createField("label", QfFeatureUtils.String)]);
    let sourceLayer = QfLayerUtils.createMemoryLayer("LayerUtilsSource", sourceFields, Qgis.WkbType.Point, QfCoordinateReferenceSystemUtils.wgs84Crs());
    verify(sourceLayer !== null, "Source layer must be created");
    QfProjectUtils.addMapLayer(qgisProject, sourceLayer);

    //Seed three features
    sourceLayer.startEditing();
    for (let i = 0; i < 3; ++i) {
      let feature = QfFeatureUtils.createBlankFeature(sourceLayer.fields, QfGeometryUtils.createGeometryFromWkt("POINT(" + i + " " + i + ")"));
      feature.setAttribute(sourceLayer.fields.indexFromName("id"), i + 1);
      feature.setAttribute(sourceLayer.fields.indexFromName("label"), "row-" + (i + 1));
      verify(QfLayerUtils.addFeature(sourceLayer, feature), "Feature insertion failed");
    }
    sourceLayer.commitChanges();

    //Export every feature
    const allPath = QfLayerUtils.saveVectorLayerAs(sourceLayer, qgisProject.homePath + "/layerutils-export-all.gpkg", "", "");
    verify(allPath !== "", "Export with empty filter must return a final path");
    let exportedLayer = QfLayerUtils.loadVectorLayer(allPath, "LayerUtilsExportAllReload", "ogr");
    verify(exportedLayer.isValid, "Reloaded export must be valid");
    let allCount = 0;
    let allIt = QfLayerUtils.createFeatureIterator(exportedLayer);
    while (allIt.hasNext()) {
      allIt.next();
      ++allCount;
    }
    allIt.close();
    compare(allCount, 3);

    // Export a filtered subset
    const filteredPath = QfLayerUtils.saveVectorLayerAs(sourceLayer, qgisProject.homePath + "/layerutils-export-filtered.gpkg", "", "\"id\" >= 2");
    verify(filteredPath !== "", "Filtered export must return a final path");
    let exportedFilteredLayer = QfLayerUtils.loadVectorLayer(filteredPath, "LayerUtilsExportFilteredReload", "ogr");
    verify(exportedFilteredLayer.isValid, "Reloaded filtered export must be valid");
    let filteredCount = 0;
    let filteredIt = QfLayerUtils.createFeatureIterator(exportedFilteredLayer);
    while (filteredIt.hasNext()) {
      filteredIt.next();
      ++filteredCount;
    }
    filteredIt.close();
    compare(filteredCount, 2);

    // Check a single feature attributes
    let it = QfLayerUtils.createFeatureIteratorFromExpression(exportedLayer, "\"id\" = 2");
    verify(it.hasNext(), "Reloaded export must contain id=2");
    compare(it.next().attribute("label"), "row-2");
    it.close();

    // Import: iterate the loaded layer and copy features into a target layer
    let targetFields = QfFeatureUtils.createFields([QfFeatureUtils.createField("id", QfFeatureUtils.Int), QfFeatureUtils.createField("label", QfFeatureUtils.String)]);
    let targetLayer = QfLayerUtils.createMemoryLayer("LayerUtilsTarget", targetFields, Qgis.WkbType.Point, QfCoordinateReferenceSystemUtils.wgs84Crs());
    verify(targetLayer !== null, "Target layer must be created");
    QfProjectUtils.addMapLayer(qgisProject, targetLayer);
    targetLayer.startEditing();
    let sourceIt = QfLayerUtils.createFeatureIterator(exportedLayer);
    while (sourceIt.hasNext()) {
      let sourceFeature = sourceIt.next();
      let copy = QfFeatureUtils.createBlankFeature(targetLayer.fields, sourceFeature.geometry);
      copy.setAttribute(targetLayer.fields.indexFromName("id"), sourceFeature.attribute("id"));
      copy.setAttribute(targetLayer.fields.indexFromName("label"), sourceFeature.attribute("label"));
      verify(QfLayerUtils.addFeature(targetLayer, copy), "Imported feature insertion failed");
    }
    sourceIt.close();
    targetLayer.commitChanges();
    let targetCount = 0;
    let targetIt = QfLayerUtils.createFeatureIterator(targetLayer);
    while (targetIt.hasNext()) {
      targetIt.next();
      ++targetCount;
    }
    targetIt.close();
    compare(targetCount, 3);
  }
}
