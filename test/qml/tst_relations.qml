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
   * Creates parent/child memory layers using the API,
   * registers a relation between them, inserts features, and validates
   * relation consistency.
   *
   * Verifies that:
   * - Layers and fields are created correctly
   * - Relation is valid and correctly registered in the project
   * - Parent/child features commit and respect the relation
   */
  function test_00_fullRelationWorkflow() {
    // 1) Create parent layer
    var parentFields = FeatureUtils.createFields([FeatureUtils.createField("district_uuid", FeatureUtils.String), FeatureUtils.createField("district_name", FeatureUtils.String)]);
    var parentLayer = LayerUtils.createMemoryLayer("Districts", parentFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    verify(parentLayer !== null, "Parent layer must be created");
    compare(parentLayer.fields.count, 2);
    verify(parentLayer.fields.indexFromName("district_uuid") >= 0);
    verify(parentLayer.fields.indexFromName("district_name") >= 0);

    // 2) Create child layer
    var childFields = FeatureUtils.createFields([FeatureUtils.createField("village_uuid", FeatureUtils.String), FeatureUtils.createField("village_name", FeatureUtils.String), FeatureUtils.createField("parent_district_uuid", FeatureUtils.String), FeatureUtils.createField("population", FeatureUtils.Int)]);
    var childLayer = LayerUtils.createMemoryLayer("Villages", childFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    verify(childLayer !== null, "Child layer must be created");
    compare(childLayer.fields.count, 4);
    verify(childLayer.fields.indexFromName("village_uuid") >= 0);
    verify(childLayer.fields.indexFromName("parent_district_uuid") >= 0);

    // 3) Register layers in project
    ProjectUtils.addMapLayer(qgisProject, parentLayer);
    ProjectUtils.addMapLayer(qgisProject, childLayer);

    // 4) Create and register relation
    var relation = RelationUtils.createRelation(qgisProject, parentLayer, "district_uuid", childLayer, "parent_district_uuid");
    verify(relation.isValid, "Relation should be valid");
    var relationManager = qgisProject.relationManager;
    relationManager.addRelation(relation);
    var registeredRelation = relationManager.relation(relation.id);
    verify(registeredRelation.isValid, "Relation must be retrievable");
    compare(registeredRelation.referencedLayer.id, parentLayer.id);
    compare(registeredRelation.referencingLayer.id, childLayer.id);

    // 5) Add parent feature
    var parentFeature = FeatureUtils.createFeature(parentLayer);
    parentFeature.setAttribute(0, "uuid-001");
    parentFeature.setAttribute(1, "Central District");
    parentLayer.startEditing();
    verify(LayerUtils.addFeature(parentLayer, parentFeature), "Parent feature insertion failed");
    parentLayer.commitChanges();

    // 6) Add child features referencing the parent
    childLayer.startEditing();
    var child1 = FeatureUtils.createFeature(childLayer);
    child1.setAttribute(0, "village-uuid-001");
    child1.setAttribute(1, "Village Alpha");
    child1.setAttribute(2, "uuid-001");
    child1.setAttribute(3, 2500);
    var child2 = FeatureUtils.createFeature(childLayer);
    child2.setAttribute(0, "village-uuid-002");
    child2.setAttribute(1, "Village Beta");
    child2.setAttribute(2, "uuid-001");
    child2.setAttribute(3, 2555);
    verify(LayerUtils.addFeature(childLayer, child1), "Child1 insertion failed");
    verify(LayerUtils.addFeature(childLayer, child2), "Child2 insertion failed");
    childLayer.commitChanges();

    // 7) Final relation integrity check
    var finalRelation = relationManager.relation(relation.id);
    verify(finalRelation.referencedLayer.id === parentLayer.id);
    verify(finalRelation.referencingLayer.id === childLayer.id);
  }

  function test_02_featureAttributes() {
    const parentFeature = qgisProject.mapLayersByName('Districts')[0].getFeature("1");
    compare(parentFeature.attribute("district_uuid"), "uuid-001");
    compare(parentFeature.attribute("district_name"), "Central District");
    const childFeature1 = qgisProject.mapLayersByName('Villages')[0].getFeature("1");
    compare(childFeature1.attribute("village_uuid"), "village-uuid-001");
    compare(childFeature1.attribute("village_name"), "Village Alpha");
    compare(childFeature1.attribute("parent_district_uuid"), "uuid-001");
    compare(childFeature1.attribute("population"), 2500);
    const childFeature2 = qgisProject.mapLayersByName('Villages')[0].getFeature("2");
    compare(childFeature2.attribute("village_uuid"), "village-uuid-002");
    compare(childFeature2.attribute("village_name"), "Village Beta");
    compare(childFeature2.attribute("parent_district_uuid"), "uuid-001");
    compare(childFeature2.attribute("population"), 2555);
  }
}
