import QtQuick
import QtTest
import org.qgis
import org.qfield

TestCase {
  name: "LayerUtils"

  /**
   * Creates a memory layer with two fields and three features, then exercises
   * LayerUtils.saveVectorLayerAs and LayerUtils.loadVectorLayer end-to-end.
   *
   * Verifies that:
   * Exporting with no filter writes every feature and returns the final path
   * A filter expression restricts the exported subset
   * Attributes survive the export / reload round trip
   * Features can be iterated from a loaded layer and copied into a target
   * layer via LayerUtils.addFeature
   */

  function test_00_exportImportWorkflow() {
    //Create source layer and register it
    let sourceFields = FeatureUtils.createFields([FeatureUtils.createField("id", FeatureUtils.Int), FeatureUtils.createField("label", FeatureUtils.String)]);
    let sourceLayer = LayerUtils.createMemoryLayer("LayerUtilsSource", sourceFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    verify(sourceLayer !== null, "Source layer must be created");
    ProjectUtils.addMapLayer(qgisProject, sourceLayer);

    //Seed three features
    sourceLayer.startEditing();
    for (let i = 0; i < 3; ++i) {
      let feature = FeatureUtils.createBlankFeature(sourceLayer.fields, GeometryUtils.createGeometryFromWkt("POINT(" + i + " " + i + ")"));
      feature.setAttribute(sourceLayer.fields.indexFromName("id"), i + 1);
      feature.setAttribute(sourceLayer.fields.indexFromName("label"), "row-" + (i + 1));
      verify(LayerUtils.addFeature(sourceLayer, feature), "Feature insertion failed");
    }
    sourceLayer.commitChanges();

    //Export every feature
    const allPath = LayerUtils.saveVectorLayerAs(sourceLayer, qgisProject.homePath + "/layerutils-export-all.gpkg", "", "");
    verify(allPath !== "", "Export with empty filter must return a final path");
    let exportedLayer = LayerUtils.loadVectorLayer(allPath, "LayerUtilsExportAllReload", "ogr");
    verify(exportedLayer.isValid, "Reloaded export must be valid");
    let allCount = 0;
    let allIt = LayerUtils.createFeatureIterator(exportedLayer);
    while (allIt.hasNext()) {
      allIt.next();
      ++allCount;
    }
    allIt.close();
    compare(allCount, 3);

    // Export a filtered subset
    const filteredPath = LayerUtils.saveVectorLayerAs(sourceLayer, qgisProject.homePath + "/layerutils-export-filtered.gpkg", "", "\"id\" >= 2");
    verify(filteredPath !== "", "Filtered export must return a final path");
    let exportedFilteredLayer = LayerUtils.loadVectorLayer(filteredPath, "LayerUtilsExportFilteredReload", "ogr");
    verify(exportedFilteredLayer.isValid, "Reloaded filtered export must be valid");
    let filteredCount = 0;
    let filteredIt = LayerUtils.createFeatureIterator(exportedFilteredLayer);
    while (filteredIt.hasNext()) {
      filteredIt.next();
      ++filteredCount;
    }
    filteredIt.close();
    compare(filteredCount, 2);

    // Check a single feature attributes
    let it = LayerUtils.createFeatureIteratorFromExpression(exportedLayer, "\"id\" = 2");
    verify(it.hasNext(), "Reloaded export must contain id=2");
    compare(it.next().attribute("label"), "row-2");
    it.close();

    // Import: iterate the loaded layer and copy features into a target layer
    let targetFields = FeatureUtils.createFields([FeatureUtils.createField("id", FeatureUtils.Int), FeatureUtils.createField("label", FeatureUtils.String)]);
    let targetLayer = LayerUtils.createMemoryLayer("LayerUtilsTarget", targetFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    verify(targetLayer !== null, "Target layer must be created");
    ProjectUtils.addMapLayer(qgisProject, targetLayer);
    targetLayer.startEditing();
    let sourceIt = LayerUtils.createFeatureIterator(exportedLayer);
    while (sourceIt.hasNext()) {
      let sourceFeature = sourceIt.next();
      let copy = FeatureUtils.createBlankFeature(targetLayer.fields, sourceFeature.geometry);
      copy.setAttribute(targetLayer.fields.indexFromName("id"), sourceFeature.attribute("id"));
      copy.setAttribute(targetLayer.fields.indexFromName("label"), sourceFeature.attribute("label"));
      verify(LayerUtils.addFeature(targetLayer, copy), "Imported feature insertion failed");
    }
    sourceIt.close();
    targetLayer.commitChanges();
    let targetCount = 0;
    let targetIt = LayerUtils.createFeatureIterator(targetLayer);
    while (targetIt.hasNext()) {
      targetIt.next();
      ++targetCount;
    }
    targetIt.close();
    compare(targetCount, 3);
  }
}
