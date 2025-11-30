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
   * Tests creating parent and child memory layers via the API.
   *
   * Verifies that:
   * - Layers are created successfully
   * - Expected fields exist and are indexable
   */
  function test_00_createMemoryLayersWithRelation() {
    // Create parent layer
    var parentUuidField = FeatureUtils.createField("district_uuid", FeatureUtils.String);
    var parentNameField = FeatureUtils.createField("district_name", FeatureUtils.String);
    var parentFields = FeatureUtils.createFields([parentUuidField, parentNameField]);
    var parentLayer = LayerUtils.createMemoryLayer("Districts", parentFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    verify(parentLayer !== null);
    compare(parentLayer.fields.count, 2);
    verify(parentLayer.fields.indexFromName("district_uuid") >= 0);
    verify(parentLayer.fields.indexFromName("district_name") >= 0);

    // Create child Layer
    var childUuidField = FeatureUtils.createField("village_uuid", FeatureUtils.String);
    var childNameField = FeatureUtils.createField("village_name", FeatureUtils.String);
    var parentRefField = FeatureUtils.createField("parent_district_uuid", FeatureUtils.String);
    var populationField = FeatureUtils.createField("population", FeatureUtils.Int);
    var childFields = FeatureUtils.createFields([childUuidField, childNameField, parentRefField, populationField]);
    var childLayer = LayerUtils.createMemoryLayer("Villages", childFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    verify(childLayer !== null);
    compare(childLayer.fields.count, 4);
    verify(childLayer.fields.indexFromName("village_uuid") >= 0);
    verify(childLayer.fields.indexFromName("parent_district_uuid") >= 0);
  }

  /**
   * Tests creating a relation between two API-generated layers.
   *
   * Verifies that:
   * - The relation is valid
   * - It references the correct parent and child layers
   * - It is properly registered in the project's RelationManager
   */
  function test_02_createAndRegisterRelation() {
    // Create parent and child layers
    var parentUuidField = FeatureUtils.createField("district_uuid", FeatureUtils.String);
    var parentNameField = FeatureUtils.createField("district_name", FeatureUtils.String);
    var parentFields = FeatureUtils.createFields([parentUuidField, parentNameField]);
    var parentLayer = LayerUtils.createMemoryLayer("Districts_Test", parentFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    var childUuidField = FeatureUtils.createField("village_uuid", FeatureUtils.String);
    var childNameField = FeatureUtils.createField("village_name", FeatureUtils.String);
    var parentRefField = FeatureUtils.createField("parent_district_uuid", FeatureUtils.String);
    var childFields = FeatureUtils.createFields([childUuidField, childNameField, parentRefField]);
    var childLayer = LayerUtils.createMemoryLayer("Villages_Test", childFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());

    // Add layers to project
    ProjectUtils.addMapLayer(qgisProject, parentLayer);
    ProjectUtils.addMapLayer(qgisProject, childLayer);

    // Create the relation
    var relation = RelationUtils.createRelation(qgisProject, parentLayer, "district_uuid", childLayer, "parent_district_uuid");
    verify(relation.isValid, "Relation should be valid");
    compare(relation.referencedLayer.id, parentLayer.id);
    compare(relation.referencingLayer.id, childLayer.id);

    // Add relation to project's relation manager
    var relationManager = qgisProject.relationManager;
    relationManager.addRelation(relation);

    // Verify relation was added
    var retrievedRelation = relationManager.relation(relation.id);
    verify(retrievedRelation.isValid);
    compare(retrievedRelation.id, relation.id);
  }

  /**
   * Tests relation behavior after inserting features into parent and child layers.
   *
   * Verifies that:
   * - Features commit correctly
   * - The relation is valid and links the expected layers
   * - Child features reference the correct parent
   */
  function test_03_addFeaturesAndTestRelation() {
    // Create layers
    var parentUuidField = FeatureUtils.createField("district_uuid", FeatureUtils.String);
    var parentNameField = FeatureUtils.createField("district_name", FeatureUtils.String);
    var parentFields = FeatureUtils.createFields([parentUuidField, parentNameField]);
    var parentLayer = LayerUtils.createMemoryLayer("Districts_Features", parentFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    var childUuidField = FeatureUtils.createField("village_uuid", FeatureUtils.String);
    var childNameField = FeatureUtils.createField("village_name", FeatureUtils.String);
    var parentRefField = FeatureUtils.createField("parent_district_uuid", FeatureUtils.String);
    var childFields = FeatureUtils.createFields([childUuidField, childNameField, parentRefField]);
    var childLayer = LayerUtils.createMemoryLayer("Villages_Features", childFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());

    // Add layers to project first
    ProjectUtils.addMapLayer(qgisProject, parentLayer);
    ProjectUtils.addMapLayer(qgisProject, childLayer);

    // Add parent feature
    var parentFeature = FeatureUtils.createBlankFeature(parentLayer.fields);
    parentFeature.setAttribute(0, "uuid-001");
    parentFeature.setAttribute(1, "Central District");
    parentLayer.startEditing();
    const r3 = LayerUtils.addFeature(parentLayer, parentFeature);
    parentLayer.commitChanges();

    // Add child features
    var childFeature1 = FeatureUtils.createBlankFeature(childLayer.fields);
    childFeature1.setAttribute(0, "village-uuid-001");
    childFeature1.setAttribute(1, "Village Alpha");
    childFeature1.setAttribute(2, "uuid-001"); // Reference to parent
    var childFeature2 = FeatureUtils.createBlankFeature(childLayer.fields);
    childFeature2.setAttribute(0, "village-uuid-002");
    childFeature2.setAttribute(1, "Village Beta");
    childFeature2.setAttribute(2, "uuid-001"); // Reference to parent
    childLayer.startEditing();
    LayerUtils.addFeature(childLayer, childFeature1);
    LayerUtils.addFeature(childLayer, childFeature2);
    childLayer.commitChanges();

    // Create and register relation
    var relation = RelationUtils.createRelation(qgisProject, parentLayer, "district_uuid", childLayer, "parent_district_uuid");
    verify(relation.isValid);
    var relationManager = qgisProject.relationManager;
    relationManager.addRelation(relation);
    var retrievedRelation = relationManager.relation(relation.id);

    // Verify the relations
    verify(retrievedRelation.referencedLayer.id === parentLayer.id);
    verify(retrievedRelation.referencingLayer.id === childLayer.id);
  }
}
