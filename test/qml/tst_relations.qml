import QtQuick
import QtTest
import org.qgis
import org.qfield
import Theme
import "../../src/qml/" as QFieldControls
import "Utils.js" as Utils

TestCase {
  name: "Relations"
  /**
   * Creates referenced/referencing memory layers using the API,
   * registers a relation between them, inserts features, and validates
   * relation consistency.
   *
   * Verifies that:
   * - Layers and fields are created correctly
   * - Relation is valid and correctly registered in the project
   * - Referenced/referencing features commit and respect the relation
   */
  function test_00_fullRelationWorkflow() {
    // 1) Create referenced layer
    var referencedFields = FeatureUtils.createFields([FeatureUtils.createField("district_uuid", FeatureUtils.String), FeatureUtils.createField("district_name", FeatureUtils.String)]);
    var referencedLayer = LayerUtils.createMemoryLayer("Districts", referencedFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    verify(referencedLayer !== null, "Referenced layer must be created");
    compare(referencedLayer.fields.count, 2);
    verify(referencedLayer.fields.indexFromName("district_uuid") >= 0);
    verify(referencedLayer.fields.indexFromName("district_name") >= 0);

    // 2) Create referencing layer
    var referencingFields = FeatureUtils.createFields([FeatureUtils.createField("village_uuid", FeatureUtils.String), FeatureUtils.createField("village_name", FeatureUtils.String), FeatureUtils.createField("parent_district_uuid", FeatureUtils.String), FeatureUtils.createField("population", FeatureUtils.Int)]);
    var referencingLayer = LayerUtils.createMemoryLayer("Villages", referencingFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    verify(referencingLayer !== null, "Referencing layer must be created");
    compare(referencingLayer.fields.count, 4);
    verify(referencingLayer.fields.indexFromName("village_uuid") >= 0);
    verify(referencingLayer.fields.indexFromName("parent_district_uuid") >= 0);

    // 3) Register layers in project
    ProjectUtils.addMapLayer(qgisProject, referencedLayer);
    ProjectUtils.addMapLayer(qgisProject, referencingLayer);

    // 4) Create and register relation
    var relation = RelationUtils.createRelation(qgisProject, referencedLayer, "district_uuid", referencingLayer, "parent_district_uuid");
    verify(relation.isValid, "Relation should be valid");
    var relationManager = qgisProject.relationManager;
    relationManager.addRelation(relation);
    var registeredRelation = relationManager.relation(relation.id);
    verify(registeredRelation.isValid, "Relation must be retrievable");
    compare(registeredRelation.referencedLayer.id, referencedLayer.id);
    compare(registeredRelation.referencingLayer.id, referencingLayer.id);

    // 5) Add referenced feature
    var referencedFeature = FeatureUtils.createFeature(referencedLayer);
    referencedFeature.setAttribute(0, "uuid-001");
    referencedFeature.setAttribute(1, "Central District");
    referencedLayer.startEditing();
    verify(LayerUtils.addFeature(referencedLayer, referencedFeature), "Referenced feature insertion failed");
    referencedLayer.commitChanges();

    // 6) Add referencing features
    referencingLayer.startEditing();
    var child1 = FeatureUtils.createFeature(referencingLayer);
    child1.setAttribute(0, "village-uuid-001");
    child1.setAttribute(1, "Village Alpha");
    child1.setAttribute(2, "uuid-001");
    child1.setAttribute(3, 2500);
    var child2 = FeatureUtils.createFeature(referencingLayer);
    child2.setAttribute(0, "village-uuid-002");
    child2.setAttribute(1, "Village Beta");
    child2.setAttribute(2, "uuid-001");
    child2.setAttribute(3, 2555);
    verify(LayerUtils.addFeature(referencingLayer, child1), "Child1 insertion failed");
    verify(LayerUtils.addFeature(referencingLayer, child2), "Child2 insertion failed");
    referencingLayer.commitChanges();

    // 7) Final relation integrity check
    var finalRelation = relationManager.relation(relation.id);
    verify(finalRelation.referencedLayer.id === referencedLayer.id);
    verify(finalRelation.referencingLayer.id === referencingLayer.id);
  }

  function test_02_featureAttributes() {
    const referencedFeature = qgisProject.mapLayersByName('Districts')[0].getFeature("1");
    compare(referencedFeature.attribute("district_uuid"), "uuid-001");
    compare(referencedFeature.attribute("district_name"), "Central District");
    const referencingFeature1 = qgisProject.mapLayersByName('Villages')[0].getFeature("1");
    compare(referencingFeature1.attribute("village_uuid"), "village-uuid-001");
    compare(referencingFeature1.attribute("village_name"), "Village Alpha");
    compare(referencingFeature1.attribute("parent_district_uuid"), "uuid-001");
    compare(referencingFeature1.attribute("population"), 2500);
    const referencingFeature2 = qgisProject.mapLayersByName('Villages')[0].getFeature("2");
    compare(referencingFeature2.attribute("village_uuid"), "village-uuid-002");
    compare(referencingFeature2.attribute("village_name"), "Village Beta");
    compare(referencingFeature2.attribute("parent_district_uuid"), "uuid-001");
    compare(referencingFeature2.attribute("population"), 2555);
  }
}
