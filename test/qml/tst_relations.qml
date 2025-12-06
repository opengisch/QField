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
   * - Correct creation of layers, fields, and feature attributes via the API
   * - Relation creation, registration, retrieval, and validity checks
   * - Parent/child feature insertion and relation integrity across commits
   * - Proper population, sorting, and live updates of the relation editor
   *   (including addition and deletion of child features)
   */
  function test_00_fullRelationWorkflow() {
    // 1) Create referenced layer
    let referencedFields = FeatureUtils.createFields([FeatureUtils.createField("district_uuid", FeatureUtils.String), FeatureUtils.createField("district_name", FeatureUtils.String)]);
    let referencedLayer = LayerUtils.createMemoryLayer("Districts", referencedFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    verify(referencedLayer !== null, "Referenced layer must be created");
    compare(referencedLayer.fields.count, 2);
    verify(referencedLayer.fields.indexFromName("district_uuid") >= 0);
    verify(referencedLayer.fields.indexFromName("district_name") >= 0);

    // 2) Create referencing layer
    let referencingFields = FeatureUtils.createFields([FeatureUtils.createField("village_uuid", FeatureUtils.String), FeatureUtils.createField("village_name", FeatureUtils.String), FeatureUtils.createField("parent_district_uuid", FeatureUtils.String), FeatureUtils.createField("population", FeatureUtils.Int)]);
    let referencingLayer = LayerUtils.createMemoryLayer("Villages", referencingFields, Qgis.WkbType.Point, CoordinateReferenceSystemUtils.wgs84Crs());
    verify(referencingLayer !== null, "Referencing layer must be created");
    compare(referencingLayer.fields.count, 4);
    verify(referencingLayer.fields.indexFromName("village_uuid") >= 0);
    verify(referencingLayer.fields.indexFromName("parent_district_uuid") >= 0);

    // 3) Register layers in project
    ProjectUtils.addMapLayer(qgisProject, referencedLayer);
    ProjectUtils.addMapLayer(qgisProject, referencingLayer);

    // 4) Create and register relation
    let relation = RelationUtils.createRelation(referencedLayer, "district_uuid", referencingLayer, "parent_district_uuid");
    verify(relation.isValid, "Relation should be valid");
    let relationManager = qgisProject.relationManager;
    relationManager.addRelation(relation);
    let registeredRelation = relationManager.relation(relation.id);
    verify(registeredRelation.isValid, "Relation must be retrievable");
    compare(registeredRelation.referencedLayer.id, referencedLayer.id);
    compare(registeredRelation.referencingLayer.id, referencingLayer.id);

    // 5) Add referenced feature
    let referencedFeature = FeatureUtils.createFeature(referencedLayer);
    referencedFeature.setAttribute(0, "uuid-001");
    referencedFeature.setAttribute(1, "Central District");
    referencedLayer.startEditing();
    verify(LayerUtils.addFeature(referencedLayer, referencedFeature), "Referenced feature insertion failed");
    referencedLayer.commitChanges();

    // 6) Add referencing features
    referencingLayer.startEditing();
    let child1 = FeatureUtils.createFeature(referencingLayer);
    child1.setAttribute(0, "village-uuid-001");
    child1.setAttribute(1, "Village Alpha");
    child1.setAttribute(2, "uuid-001");
    child1.setAttribute(3, 2500);
    let child2 = FeatureUtils.createFeature(referencingLayer);
    child2.setAttribute(0, "village-uuid-002");
    child2.setAttribute(1, "Village Beta");
    child2.setAttribute(2, "uuid-001");
    child2.setAttribute(3, 2555);
    verify(LayerUtils.addFeature(referencingLayer, child1), "Child1 insertion failed");
    verify(LayerUtils.addFeature(referencingLayer, child2), "Child2 insertion failed");
    referencingLayer.commitChanges();

    // 7) Relation integrity check
    let finalRelation = relationManager.relation(relation.id);
    verify(finalRelation.referencedLayer.id === referencedLayer.id);
    verify(finalRelation.referencingLayer.id === referencingLayer.id);

    // 8) Test relation editor with referenced feature
    relation_editor.currentLayer = referencedLayer;
    relation_editor.currentFeature = referencedLayer.getFeature("1");
    relation_editor.relationId = relation.id;
    relation_editor.source = '../../src/qml/editorwidgets/relationeditors/relation_editor.qml';
    wait(200);
    verify(relation_editor.item !== null);
    verify(relation_editor.item.relationEditorModel !== null);
    compare(relation_editor.item.relationEditorModel.feature.id, 1);
    compare(relation_editor.item.relationEditorModel.rowCount(), 2);
    let delegate1TextString = relation_editor.item.listView.itemAtIndex(0).children[2].children[0].text;
    let delegate2TextString = relation_editor.item.listView.itemAtIndex(1).children[2].children[0].text;
    compare(delegate1TextString, "Village Alpha");
    compare(delegate2TextString, "Village Beta");

    // 9) Sort action
    relation_editor.item.toggleSortAction();
    delegate1TextString = relation_editor.item.listView.itemAtIndex(0).children[2].children[0].text;
    delegate2TextString = relation_editor.item.listView.itemAtIndex(1).children[2].children[0].text;
    compare(delegate1TextString, "Village Beta");
    compare(delegate2TextString, "Village Alpha");
    relation_editor.item.toggleSortAction();
    delegate1TextString = relation_editor.item.listView.itemAtIndex(0).children[2].children[0].text;
    delegate2TextString = relation_editor.item.listView.itemAtIndex(1).children[2].children[0].text;
    compare(delegate1TextString, "Village Alpha");
    compare(delegate2TextString, "Village Beta");

    // 10) Add a new child feature
    referencingLayer.startEditing();
    let child3 = FeatureUtils.createFeature(referencingLayer);
    child3.setAttribute(0, "village-uuid-003");
    child3.setAttribute(1, "Village Gamma");
    child3.setAttribute(2, "uuid-001");
    child3.setAttribute(3, 3000);
    verify(LayerUtils.addFeature(referencingLayer, child3), "Child3 insertion failed");
    referencingLayer.commitChanges();
    relation_editor.item.relationEditorModel.reload();
    wait(200);
    compare(relation_editor.item.relationEditorModel.rowCount(), 3);
    const delegate3TextString = relation_editor.item.listView.itemAtIndex(2).children[2].children[0].text;
    compare(delegate3TextString, "Village Gamma");

    // 11) Delete a feature
    relation_editor.item.deleteDialog.referencingFeatureId = "3";
    relation_editor.item.deleteDialog.accepted();
    wait(200);
    compare(relation_editor.item.relationEditorModel.rowCount(), 2);
    delegate1TextString = relation_editor.item.listView.itemAtIndex(0).children[2].children[0].text;
    delegate2TextString = relation_editor.item.listView.itemAtIndex(1).children[2].children[0].text;
    compare(delegate1TextString, "Village Alpha");
    compare(delegate2TextString, "Village Beta");
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

  Loader {
    id: relation_editor

    property var mainWindow: Item {
      id: mainWindowItem
      property double sceneTopMargin: SafeArea.margins.top
    }

    property bool isEnabled: false
    property bool isEditing: false
    property bool isEditable: false

    property var value: undefined
    property var config: ({})
    property string widget: "RelationEditor"
    property string relationEditorWidget: "relation_editor"
    property string relationEditorWidgetConfig: `{"buttons":"SaveChildEdits|AddChildFeature|DuplicateChildFeature|DeleteChildFeature|ZoomToChildFeature","show_first_feature":true}`
    property var field: undefined
    property string relationId: ""
    property string nmRelationId: ""
    property var currentFeature: undefined
    property var currentLayer: undefined
  }
}
