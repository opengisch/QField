import QtQuick
import QtTest
import org.qfield
import Theme
import "../../src/qml/" as QFieldControls
import "Utils.js" as Utils

TestCase {
  name: "FeatureForm"

  QFieldControls.FeatureForm {
    id: featureForm
    property var mainWindow: mainWindowItem
    property var mSelectedLayer: qgisProject.mapLayersByName('Apiary')[0]
    property var mSelectedFeature: qgisProject.mapLayersByName('Apiary')[0].getFeature("64")

    model: AttributeFormModel {
      featureModel: FeatureModel {
        project: qgisProject
        currentLayer: featureForm.mSelectedLayer
        feature: featureForm.mSelectedFeature
      }
    }
  }

  /**
   * Test function to verify that the qgisProject has been loaded and attributes are accessible.
   *
   * This function checks the initial values of features in three different layers:
   * - Apiary layer with id 64
   * - Tracks layer with id 1
   * - Field layer with id 39
   *
   * It verifies the attribute values for each feature in these layers, including
   * their FID, name, region, editor name, proprietor, photo, date of review, reviewer, and plants.
   */
  function test_00_featureForm() {
    // check initial value of an `Apiary` layer with id = 64
    const apiaryId64 = qgisProject.mapLayersByName('Apiary')[0].getFeature("64");
    compare(apiaryId64.attribute("fid"), 64);
    compare(apiaryId64.attribute("photo"), "DCIM/3.jpg");
    compare(apiaryId64.attribute("beekeeper"), "Stephen Hawking");
    compare(apiaryId64.attribute("Amount of Bees"), "1000");
    compare(apiaryId64.attribute("number of boxes"), 7);
    compare(apiaryId64.attribute("Species of Bees"), "Apis Mellifera Carnica");

    // check initial value of an `Tracks` layer with id 1
    const trackId1 = qgisProject.mapLayersByName('Tracks')[0].getFeature("1");
    compare(trackId1.attribute("fid"), 1);
    compare(trackId1.attribute("Track Name"), "Munt Sura");
    compare(trackId1.attribute("Region"), "");
    compare(trackId1.attribute("Editor Name"), "Linda Camathiias");

    // check initial value of an `Field` layer with id 39
    // const reviewDate = new Date("2019-05-23T04:30:00.000");
    const fieldId1 = qgisProject.mapLayersByName('Fields')[0].getFeature("39");
    compare(fieldId1.attribute("fid"), 39);
    compare(fieldId1.attribute("Proprietor"), "national");
    compare(fieldId1.attribute("photo"), "DCIM/taraxacum.jpg");
    // compare(fieldId1.attribute("Date of Review"), reviewDate); // test depends on workstation local time
    compare(fieldId1.attribute("Reviewer"), "David Signer");
    compare(fieldId1.attribute("Plants"), "taraxacum"); // description: Dandelions
  }

  /**
   * Test case for the featureForm (Apiary type).
   *
   * This test verifies that the feature form is properly initialized and displays the expected data for a selected feature.
   *
   * Preconditions:
   * - The featureForm object is initialized and has a reference to a layer.
   *
   * Steps:
   * 1. Verify that the toolbar is not visible.
   * 2. Verify that the title label text is "View feature on Apiary".
   * 3. Verify that the tab row has 6 rows and that each row has a valid tab name (e.g., "General", "Picture", "Issues", "Review", "Consumption", "GNSS").
   * 4. For each tab:
   *   - Verify that the tab delegate has a valid text value matching the expected tab name.
   *   - Verify that the tab delegate is currently selected (i.e., isCurrentIndex is true).
   * 5. For each field repeater item in the "General" tab:
   *   - Verify that the item loader has a valid container name matching the expected value.
   *   - Verify that the field container has a child element with a text value matching the expected container name.
   *   - Verify that the attribute editor loader has a valid widget type and source matching the expected values.
   *   - Verify that the attribute editor loader's current feature attribute value matches the expected value.
   *
   * Expected Results:
   * 1. The toolbar is not visible.
   * 2. The title label text is "View feature on Apiary".
   * 3. The tab row has 6 rows with valid tab names.
   * 4. Each tab delegate has a valid text value and is currently selected.
   * 5. Each field repeater item in the "General" tab has valid container names, widget types, and source values, and matches the expected attribute editor loader values.
   */
  function test_01_featureForm() {
    const toolbar = Utils.findChildren(featureForm, "toolbar");
    compare(toolbar.visible, false);
    const titleLabel = Utils.findChildren(featureForm, "titleLabel");
    compare(titleLabel.text, "View feature on Apiary");
    const tabRow = Utils.findChildren(featureForm, "tabRow");
    compare(tabRow.model.hasTabs, true);
    compare(tabRow.model.rowCount(), 6);
    const expectedTabs = ["General", "Picture", "Issues", "Review", "Consumption", "GNSS"];
    compareFeatureFormTabNamesWithExpectedResults(expectedTabs);
    const expectedModel = [
      {
        "containerName": "Number of Boxes",
        "widgetType": "Range",
        "source": "editorwidgets/Range.qml",
        "value": 7
      },
      {
        "containerName": "Species of Bees",
        "widgetType": "ValueMap",
        "source": "editorwidgets/ValueMap.qml",
        "value": "Apis Mellifera Carnica"
      },
      {
        "containerName": "Amount of Bees",
        "widgetType": "ValueMap",
        "source": "editorwidgets/ValueMap.qml",
        "value": "1000"
      },
      {
        "containerName": "Beekeeper",
        "widgetType": "TextEdit",
        "source": "editorwidgets/TextEdit.qml",
        "value": "Stephen Hawking"
      },
      {
        "containerName": "Yearly Harvest (kg)",
        "widgetType": "Range",
        "source": "editorwidgets/Range.qml",
        "value": 10
      },
      {
        "containerName": "Owner",
        "widgetType": "ValueRelation",
        "source": "editorwidgets/ValueRelation.qml",
        "value": ""
      }
    ];
    compareFeatureFormWithExpectedResults(expectedModel);
  }

  /**
   * Test case for the featureForm (Field type).
   *
   * This test verifies that the feature form is properly initialized and displays the expected data for a selected feature.
   *
   * Preconditions:
   * - The qgisProject object is initialized and has a layer named "Fields" with at least one feature.
   * - The featureForm object is initialized and has a reference to the "Fields" layer.
   *
   * Steps:
   * 1. Set the mSelectedLayer property of featureForm to the "Fields" layer.
   * 2. Set the mSelectedFeature property of featureForm to the first feature in the "Fields" layer.
   * 3. Verify that the toolbar is not visible.
   * 4. Verify that the title label text is "View feature on Fields".
   * 5. Verify that the tab row has 4 rows and that each row has a valid tab name (e.g., "General", "Picture", "Review", "Consuming Apiaries").
   * 6. For each tab:
   *   - Verify that the tab delegate has a valid text value matching the expected tab name.
   *   - Verify that the tab delegate is currently selected (i.e., isCurrentIndex is true).
   * 7. For each field repeater item in the "General" tab:
   *   - Verify that the item loader has a valid container name matching the expected value.
   *   - Verify that the field container has a child element with a text value matching the expected container name.
   *   - Verify that the attribute editor loader has a valid widget type and source matching the expected values.
   *   - Verify that the attribute editor loader's current feature attribute value matches the expected value.
   *
   * Expected Results:
   * 1. The toolbar is not visible.
   * 2. The title label text is "View feature on Fields".
   * 3. The tab row has 4 rows with valid tab names.
   * 4. Each tab delegate has a valid text value and is currently selected.
   * 5. Each field repeater item in the "General" tab has valid container names, widget types, and source values, and matches the expected attribute editor loader values.
   */
  function test_02_featureForm() {
    featureForm.mSelectedLayer = qgisProject.mapLayersByName('Fields')[0];
    featureForm.mSelectedFeature = featureForm.mSelectedLayer.getFeature("39");
    const toolbar = Utils.findChildren(featureForm, "toolbar");
    compare(toolbar.visible, false);
    const titleLabel = Utils.findChildren(featureForm, "titleLabel");
    compare(titleLabel.text, "View feature on Fields");
    const expectedTabs = ["General", "Picture", "Review", "Consuming Apiaries"];
    compareFeatureFormTabNamesWithExpectedResults(expectedTabs);
    const expectedModel = [
      {
        "containerName": "Proprietor",
        "widgetType": "ValueMap",
        "source": "editorwidgets/ValueMap.qml",
        "value": "national"
      },
      {
        "containerName": "Plants",
        "widgetType": "ValueMap",
        "source": "editorwidgets/ValueMap.qml",
        "value": "taraxacum"
      },
      {
        "containerName": "Owner",
        "widgetType": "ValueRelation",
        "source": "editorwidgets/ValueRelation.qml",
        "value": "2"
      }
    ];
    compareFeatureFormWithExpectedResults(expectedModel);
  }

  /**
   * Test case for the featureForm (Tracks type).
   *
   * This test verifies that the feature form is properly initialized and displays the expected data for a selected feature from the "Tracks" layer.
   *
   * Preconditions:
   * - The qgisProject object is initialized and has a layer named "Tracks".
   *
   * Steps:
   * 1. Set the mSelectedLayer property of featureForm to the "Tracks" layer.
   * 2. Set the mSelectedFeature property of featureForm to the first feature in the "Tracks" layer.
   * 3. Verify that the toolbar is not visible.
   * 4. Verify that the title label text is "View feature on Tracks".
   * 5. Verify that the tab row does not have any tabs (i.e., model.hasTabs is false).
   * 6. For each field repeater item:
   *   - Verify that the item loader has a valid container name matching the expected value.
   *   - Verify that the field container has a child element with a text value matching the expected container name.
   *   - Verify that the attribute editor loader has a valid widget type and source matching the expected values.
   *   - Verify that the attribute editor loader's current feature attribute value matches the expected value.
   *
   * Expected Results:
   * 1. The toolbar is not visible.
   * 2. The title label text is "View feature on Tracks".
   * 3. The tab row does not have any tabs.
   * 4. Each field repeater item has valid container names, widget types, and source values, and matches the expected attribute editor loader values.
   */
  function test_03_featureForm() {
    featureForm.mSelectedLayer = qgisProject.mapLayersByName('Tracks')[0];
    featureForm.mSelectedFeature = featureForm.mSelectedLayer.getFeature("1");
    const toolbar = Utils.findChildren(featureForm, "toolbar");
    compare(toolbar.visible, false);
    const titleLabel = Utils.findChildren(featureForm, "titleLabel");
    compare(titleLabel.text, "View feature on Tracks");
    const tabRow = Utils.findChildren(featureForm, "tabRow");
    compare(tabRow.model.hasTabs, undefined); // Notice: its better to be false not undefined
    const expectedModel = [
      {
        "containerName": "Track Name",
        "widgetType": "TextEdit",
        "source": "editorwidgets/TextEdit.qml",
        "value": "Munt Sura"
      },
      {
        "containerName": "Region",
        "widgetType": "TextEdit",
        "source": "editorwidgets/TextEdit.qml",
        "value": ""
      },
      {
        "containerName": "Editor Name",
        "widgetType": "TextEdit",
        "source": "editorwidgets/TextEdit.qml",
        "value": "Linda Camathiias"
      }
    ];
    compareFeatureFormWithExpectedResults(expectedModel);
  }

  /**
   * Test case for featureForm
   *
   * This test case tests the functionality of the featureForm component.
   *
   * It sets the selected layer and feature, and then checks the initial state of the attribute editor.
   * It then changes the value of the attribute editor, toggles the edit state, and checks that the value has been updated correctly.
   */
  function test_04_featureForm() {
    featureForm.mSelectedLayer = qgisProject.mapLayersByName('Apiary')[0];
    featureForm.mSelectedFeature = featureForm.mSelectedLayer.getFeature("64");
    featureForm.state = 'ReadOnly';
    wait(500);
    const toolbar = Utils.findChildren(featureForm, "toolbar");
    const fieldItem = Utils.findChildren(featureForm, "fieldRepeater");
    const itemLoader = fieldItem.itemAt(0).children[2].children[0];
    const attributeEditorLoader = Utils.findChildren(featureForm, "attributeEditorLoader" + itemLoader.containerName);
    compare(itemLoader.containerName, "Number of Boxes");
    compare(attributeEditorLoader.isEditable, true);
    compare(attributeEditorLoader.isEnabled, false);
    compare(attributeEditorLoader.isEditing, false);
    compare(attributeEditorLoader.value, 7);
    featureForm.state = 'Edit';
    compare(attributeEditorLoader.isEditable, true);
    compare(attributeEditorLoader.isEnabled, true);
    compare(attributeEditorLoader.isEditing, true);
    attributeEditorLoader.value = 99;
    compare(attributeEditorLoader.value, 99);
    attributeEditorLoader.value = 7;
    wait(50);
    featureForm.state = 'ReadOnly';
    wait(50);
  }

  /**
   * Test that Range widget properly handles null and empty values
   * This prevents crashes when empty values are intended for server-side fills
   */
  function test_05_rangeWidgetNullHandling() {
    featureForm.mSelectedLayer = qgisProject.mapLayersByName('Apiary')[0];
    featureForm.mSelectedFeature = featureForm.mSelectedLayer.getFeature("64");

    // Reset to ReadOnly state first to reload the feature with original values
    featureForm.state = 'ReadOnly';
    wait(100);

    // now switch to Edit mode
    featureForm.state = 'Edit';
    wait(100);

    const fieldItem = Utils.findChildren(featureForm, "fieldRepeater");
    const rangeItemLoader = fieldItem.itemAt(0).children[2].children[0];
    const attributeEditorLoader = Utils.findChildren(featureForm, "attributeEditorLoader" + rangeItemLoader.containerName);

    compare(rangeItemLoader.containerName, "Number of Boxes");
    compare(attributeEditorLoader.value, 7, "Initial value should be 7");

    // Test setting a valid value
    attributeEditorLoader.value = 15;
    wait(100);
    compare(attributeEditorLoader.value, 15, "Value should update to 15");

    // Test setting null-> should not crash
    attributeEditorLoader.value = null;
    wait(100);
    verify(true, "Setting null value should not crash");

    // Test setting back to valid value after null
    attributeEditorLoader.value = 20;
    wait(100);
    compare(attributeEditorLoader.value, 20, "Value should be set correctly after null");

    // edge case for numeric fields
    attributeEditorLoader.value = 0;
    wait(100);
    compare(attributeEditorLoader.value, 0, "Zero should be handled correctly");
    attributeEditorLoader.value = 7;
    wait(50);
    featureForm.state = "ReadOnly";
    wait(50);
  }

  /**
   * Test TextEdit widgets handling of empty vs null values
   * Ensures server-side value fills are not corrupted
   */
  function test_06_textEditEmptyStringPreservation() {
    featureForm.mSelectedLayer = qgisProject.mapLayersByName('Tracks')[0];
    featureForm.mSelectedFeature = featureForm.mSelectedLayer.getFeature("1");
    featureForm.state = 'Edit';
    wait(100);

    const fieldItem = Utils.findChildren(featureForm, "fieldRepeater");
    const regionItemLoader = fieldItem.itemAt(1).children[2].children[0];
    const attributeEditorLoader = Utils.findChildren(featureForm, "attributeEditorLoader" + regionItemLoader.containerName);

    compare(regionItemLoader.containerName, "Region");

    // Verify initial empty string (server-side fill scenario)
    compare(attributeEditorLoader.value, '', "Region should start as empty string");
    verify(attributeEditorLoader.value !== null, "Initial value should be empty string, not null");
    verify(attributeEditorLoader.value !== undefined, "Initial value should be empty string, not undefined");
    attributeEditorLoader.value = "test region";
    wait(100);
    compare(attributeEditorLoader.value, "test region", "Value should update to 'test region'");

    // set back to empty string-> simulating user clearing the field
    attributeEditorLoader.value = "";
    wait(100);

    // empty string should remain empty string, not become null
    compare(attributeEditorLoader.value, "", "Value should be empty string after clearing");
    verify(attributeEditorLoader.value !== null, "Cleared value should NOT be null (would break server-side fills)");
    verify(attributeEditorLoader.value !== undefined, "Cleared value should NOT be undefined");
    verify(typeof attributeEditorLoader.value === 'string', "Cleared value should be string type");

    // Test null handling separately
    attributeEditorLoader.value = null;
    wait(100);
    verify(true, "Setting null should not crash");
    attributeEditorLoader.value = "";
    wait(50);
    featureForm.state = "ReadOnly";
    wait(50);
  }

  /**
   * Test that different widget types handle null and empty values correctly
   * This is a regression test for the bug fixed in the PR
   */
  function test_07_widgetValueHandlingConsistency() {
    featureForm.mSelectedLayer = qgisProject.mapLayersByName('Apiary')[0];
    featureForm.mSelectedFeature = featureForm.mSelectedLayer.getFeature("64");
    featureForm.state = 'Edit';
    wait(100);

    const fieldItem = Utils.findChildren(featureForm, "fieldRepeater");

    // Test Range widget
    const rangeLoader = Utils.findChildren(featureForm, "attributeEditorLoader" + fieldItem.itemAt(0).children[2].children[0].containerName);
    compare(rangeLoader.widget, "Range");

    const initialRangeValue = rangeLoader.value;
    rangeLoader.value = null;
    wait(50);
    verify(true, "Range widget should handle null without crashing");

    // Restore value
    rangeLoader.value = initialRangeValue;
    wait(50);

    // Test TextEdit widget
    const textEditLoader = Utils.findChildren(featureForm, "attributeEditorLoader" + fieldItem.itemAt(3).children[2].children[0].containerName);
    compare(textEditLoader.widget, "TextEdit");

    const initialTextValue = textEditLoader.value;

    textEditLoader.value = "Test Name";
    wait(50);
    compare(textEditLoader.value, "Test Name");

    // Test empty string preservation
    textEditLoader.value = "";
    wait(50);
    compare(textEditLoader.value, "");
    verify(textEditLoader.value !== null, "TextEdit empty string should not become null");

    textEditLoader.value = null;
    wait(50);
    verify(true, "TextEdit should handle null without crashing");

    // Restore
    textEditLoader.value = initialTextValue;
    wait(50);

    // Test ValueMap widget
    const valueMapLoader = Utils.findChildren(featureForm, "attributeEditorLoader" + fieldItem.itemAt(1).children[2].children[0].containerName);
    compare(valueMapLoader.widget, "ValueMap");

    const initialValueMapValue = valueMapLoader.value;

    valueMapLoader.value = "";
    wait(50);
    verify(true, "ValueMap should handle empty string without crashing");

    // Restore
    valueMapLoader.value = initialValueMapValue;
    wait(50);
  }

  /**
   * Test state transitions with null or empty values
   * Ensures that switching between ReadOnly and Edit modes preserves values correctly
   */
  function test_08_stateTransitionWithNullValues() {
    featureForm.mSelectedLayer = qgisProject.mapLayersByName('Tracks')[0];
    featureForm.mSelectedFeature = featureForm.mSelectedLayer.getFeature("1");

    // Start in ReadOnly mode
    featureForm.state = 'ReadOnly';
    wait(100);

    const fieldItem = Utils.findChildren(featureForm, "fieldRepeater");
    const regionLoader = Utils.findChildren(featureForm, "attributeEditorLoader" + fieldItem.itemAt(1).children[2].children[0].containerName);

    // Region starts as empty string
    compare(regionLoader.value, "");

    // Switch to Edit mode
    featureForm.state = 'Edit';
    wait(100);

    // Empty string should be preserved across state change
    compare(regionLoader.value, "");
    verify(regionLoader.value !== null, "Empty string should not become null on state change");

    // set a value
    regionLoader.value = "Test Region";
    wait(50);

    // Switch back to ReadOnly
    featureForm.state = 'ReadOnly';
    wait(100);

    // Value should be preserved
    compare(regionLoader.value, "Test Region");

    // Switch back to Edit and clear
    featureForm.state = 'Edit';
    wait(100);
    regionLoader.value = "";
    wait(50);

    // Empty should still be empty, not null
    compare(regionLoader.value, "");
    verify(regionLoader.value !== null, "Cleared value should be empty string, not null");
  }

  /**
   * Compares the feature form tab names with expected results by checking if the tabs exist and
   * then iterating through each tab to compare its name with the expected name.
   *
   * @param expectedTabs - The expected tab names to compare with the feature form tabs.
   */
  function compareFeatureFormTabNamesWithExpectedResults(expectedTabs) {
    const tabRow = Utils.findChildren(featureForm, "tabRow");
    compare(tabRow.model.hasTabs, true);
    compare(tabRow.model.rowCount(), expectedTabs.length);
    for (var i = 0; i < tabRow.model.rowCount(); ++i) {
      tabRow.currentIndex = i;
      const delegate = Utils.findChildren(featureForm, "tabRowdDelegate_" + i);
      compare(delegate.text, expectedTabs[i]);
      compare(delegate.isCurrentIndex, true);
      compare(tabRow.currentIndex, i);
    }
  }

  /**
   * Compares the feature form with expected results by iterating through each field in the form and
   * comparing its properties with the expected values.
   *
   * @param expectedModel - The expected model to compare with the feature form.
   */
  function compareFeatureFormWithExpectedResults(expectedModel) {
    const fieldItem = Utils.findChildren(featureForm, "fieldRepeater");
    for (var j = 0; j < fieldItem.count; ++j) {
      const itemLoader = fieldItem.itemAt(j).children[2].children[0];
      const fieldContainer = fieldItem.itemAt(j).children[2].children[1];
      const fieldLabel = fieldContainer.children[0].text;
      const attributeEditorLoader = Utils.findChildren(featureForm, "attributeEditorLoader" + itemLoader.containerName);
      const attributeConfig = attributeEditorLoader.config;
      const initialValue = attributeEditorLoader.currentFeature.attribute(itemLoader.containerName);
      compare(itemLoader.containerName, expectedModel[j].containerName);
      compare(fieldLabel, expectedModel[j].containerName);
      compare(attributeEditorLoader.widget, expectedModel[j].widgetType);
      compare(attributeEditorLoader.source, expectedModel[j].source);
      compare(initialValue, expectedModel[j].value);
    }
  }

  // dummy useless item to fix static id problems and avoid warnings
  Item {
    id: mainWindowItem
  }

  Item {
    id: qfieldSettings

    property bool autoSave: false
  }

  Item {
    id: coordinateLocator
    property string positionInformation: ""
    property string overrideLocation: ""
    property string topSnappingResult: ""
  }

  Item {
    id: projectInfo
    property string cloudUserInformation: ""
  }

  Item {
    id: sketcher

    signal finished
    signal cancelled
  }
}
