import QtQuick
import QtTest
import org.qfield
import Theme
import "../../src/qml/editorwidgets" as EditorWidgets
import "Utils.js" as Utils

TestCase {
  name: "EditorWidgets"

  Item {
    // mainWindow for widgets
    id: mainWindowItem
    width: 300
    height: 400
    property int sceneTopMargin: 10
    property int sceneBottomMargin: 10
    property int sceneLeftMargin: 10
    property int sceneRightMargin: 10
  }

  AppExpressionContextScopesGenerator {
    id: appScopesGenerator
  }

  EditorWidgets.TextEdit {
    id: textEdit
    property var mainWindow: mainWindowItem
    property string value: "DEFAULT_VALUE"
    property var config: ({})
    property var field: FeatureUtils.createField("dummy", FeatureUtils.String)
    property bool isEnabled: true
  }

  EditorWidgets.Range {
    id: range
    property var mainWindow: mainWindowItem
    property real value: default_value
    property var config: ({})
    property var field: FeatureUtils.createField("dummy", FeatureUtils.Double, "", 20, 10)
    property bool isEnabled: true

    readonly property real default_value: 999
  }

  EditorWidgets.DateTime {
    id: dateTime
    property var mainWindow: mainWindowItem
    fieldIsDateTime: false
    fieldIsDate: false
    fieldIsString: false
    property string value: "2022-01-01"
    property var config: ({})
    property var field: FeatureUtils.createField("dummy", FeatureUtils.String)
    property bool isEnabled: true
  }

  EditorWidgets.CheckBox {
    id: checkBox
    property var mainWindow: mainWindowItem
    property bool value: true
    property var config: ({})
    property var field: FeatureUtils.createField("dummy", FeatureUtils.String)
    property bool isEnabled: true
  }

  EditorWidgets.ValueMap {
    id: valueMap
    property var mainWindow: mainWindowItem
    property var value: undefined
    property var config: ({})
    property var field: FeatureUtils.createField("dummy", FeatureUtils.String)
    property var currentLayer: undefined
    property bool isEnabled: true
    property string fieldLabel: "dummy"

    // to simulate customProperty('QFieldSync/value_map_button_interface_threshold') -> toggleButtons view
    Item {
      id: currentLayerTrue
      function customProperty(value) {
        return 100;
      }
    }

    // to to simulate customProperty('QFieldSync/value_map_button_interface_threshold') -> comboBox veiw
    Item {
      id: currentLayerFalse
      function customProperty(value) {
        return 0;
      }
    }
  }

  EditorWidgets.UuidGenerator {
    id: uuidGenerator
    property var mainWindow: mainWindowItem
    property var value: undefined
    property var config: ({})
    property bool isAdding: false
  }

  EditorWidgets.ValueRelation {
    id: valueRelation
    width: parent.width
    height: parent.height
    property var mainWindow: mainWindowItem
    property var value: undefined
    property var config: ({})
    property var field: FeatureUtils.createField("dummy", FeatureUtils.String)
    property var currentLayer: undefined
    property var currentFeature: FeatureUtils.createBlankFeature()
    property bool isEnabled: false
    property string fieldLabel: "dummy"

    Connections {
      target: valueRelation

      function onValueChangeRequested(value, isNull) {
        valueRelation.value = value;
      }
    }
  }

  EditorWidgets.ValueRelation {
    id: valueRelation2
    property var mainWindow: mainWindowItem
    property var value: undefined
    property var config: ({})
    property var field: FeatureUtils.createField("dummy", FeatureUtils.String)
    property var currentLayer: undefined
    property var currentFeature: FeatureUtils.createBlankFeature()
    property bool isEnabled: false
    property string fieldLabel: "dummy"

    Connections {
      target: valueRelation

      function onValueChangeRequested(value, isNull) {
        valueRelation.value = value;
      }
    }
  }

  /**
   * Test case for textEdit widget
   *
   * This function tests the textEdit widget's functionality by setting its config to allow multiline and use html,
   * setting its value to "SECOND_VALUE", verifying that it contains "SECOND_VALUE", resetting its config to disable multiline and html,
   * setting its value to "THIRD_VALUE", and comparing its children's texts with "THIRD_VALUE".
   */
  function test_01_textEdit() {
    const textReadonlyValue = textEdit.children[0];
    const textField = textEdit.children[2];
    const textArea = textEdit.children[3];
    compare(textReadonlyValue.text, "DEFAULT_VALUE");
    compare(textField.text, "DEFAULT_VALUE");
    compare(textArea.text, "DEFAULT_VALUE");
    textEdit.config = {
      "IsMultiline": true,
      "UseHtml": true
    };
    textEdit.value = "SECOND_VALUE";
    verify(textReadonlyValue.text.search("SECOND_VALUE") !== -1);
    compare(textField.text, "SECOND_VALUE");
    verify(textArea.text.search("SECOND_VALUE") !== -1);
    textEdit.config = {
      "IsMultiline": false,
      "UseHtml": false
    };
    textEdit.value = "THIRD_VALUE";
    compare(textReadonlyValue.text, "THIRD_VALUE");
    compare(textField.text, "THIRD_VALUE");
    compare(textArea.text, "THIRD_VALUE");
  }

  /**
   * Test case for range widget
   *
   * This function tests the range widget's functionality by:
   *
   * - Verifying that the initial value of the range is correct
   * - Setting the config to undefined for all properties and verifying that the default values are used
   * - Verifying that the widget style changes to "TextField" when no style is specified in the config
   * - Verifying that the precision, min, max, step, and suffix values are set to default values when not specified in the config
   *
   * Then, setting the config with specific values and verifying that they are used:
   *
   * - Setting the widget style to "Slider" and verifying that it changes
   * - Setting precision, min, max, step, and suffix values and verifying that they are used
   *
   * Finally, setting the value of the range and verifying that it is displayed correctly in both text and slider formats.
   */
  function test_01_range() {
    const sliderRow = range.children[0];
    const textField = sliderRow.children[0];
    const valueLabel = range.children[1].children[0];
    const slider = range.children[1].children[1];
    compare(textField.text, range.default_value + "");
    range.config = {
      "Style": undefined,
      "Precision": undefined,
      "Min": undefined,
      "Max": undefined,
      "Step": undefined,
      "Suffix": undefined
    };
    range.value = 3;
    compare(range.widgetStyle, "TextField");
    compare(range.precision, 2);
    compare(range.step, 1);
    compare(range.suffix, "");
    range.isDouble = false;
    compare(range.precision, 0);
    compare(range.min, -Infinity);
    compare(range.max, Infinity);
    range.isDouble = true;
    compare(range.precision, 2);
    compare(range.min, -Infinity);
    compare(range.max, Infinity);

    // Row
    // compare(sliderRow.visible, true) // ERROR ? should work but not working!
    compare(textField.text, "3");
    range.config = {
      "Style": "Slider",
      "Precision": 4,
      "Min": -10,
      "Max": 10,
      "Step": 10,
      "Suffix": "DEFAULT_SUFFIX"
    };
    range.value = 4;
    compare(range.widgetStyle, "Slider");
    compare(range.precision, 4);
    compare(range.min, -10);
    compare(range.max, 10);
    compare(range.step, 10);
    compare(range.suffix, "DEFAULT_SUFFIX");
    compare(sliderRow.visible, false);
    compare(valueLabel.text, range.min + ".0000DEFAULT_SUFFIX"); // NOTE: using `range.min` because of `rangeItem.parent.value`
    compare(slider.value, range.min); // NOTE: using `range.min` because of `rangeItem.parent.value`
  }

  /**
   * Test case for datetime widget
   *
   * This function tests the datetime widget's functionality by:
   *
   * - Setting an initial value without setting `config` and `field` objects and verifying that value is empty
   *
   * Then, it sets various display formats and verifies that the date and time are displayed correctly in different formats:
   *
   * - For each of the test times, it iterates over the display formats and sets the config with the format, calendar popup enabled, unknown field format, and disallows null values
   * - It sets the field to a date or time type
   * - It sets the value of the datetime widget to the test time and verifies that the text label is displayed correctly according to the format
   *
   * TODO: Test `fieldIsDate = true` too, if the field is a date only -> revert the time zone offset.
   */
  function test_01_dateTime() {
    const label = dateTime.children[0].children[0];
    compare(label.text, "2022-01-01");
    const testTimes = ["2023-01-01", "2023-01-01 23:33:56"];
    const displayFormats = ["yyyy-MM-dd", "yyyy-MM.dd", "yyyy-MM-dd HH:mm:ss", "HH:mm:ss", "HH:mm"];
    const results = ["2023-01-01", "2023-01.01", "2023-01-01 00:00:00", "00:00:00", "00:00", "2023-01-01", "2023-01.01", "2023-01-01 23:33:56", "23:33:56", "23:33"];
    let resultIdx = 0;
    for (let time of testTimes) {
      for (let format of displayFormats) {
        dateTime.fieldIsDate = false;
        dateTime.config = {
          "display_format": format,
          "calendar_popup": true,
          "field_format": "yyyy-MM-dd",
          "allow_null": false
        };
        dateTime.value = time;
        compare(label.text, results[resultIdx++]);
      }
    }
  }

  /**
   * Test case for checkBox widget
   *
   * This function tests the checkBox widget's functionality by:
   *
   * - Verifying the initial state of the widget (value, isBool, and null) and its labels
   *
   * Then, it sets a configuration with a custom checked and unchecked state and verifies that the widget reflects these states correctly:
   *
   * - Verifies the value, isBool, and null properties of the widget
   * - Verifies the text displayed in the label
   * - Verifies the checked state of the QfSwitch widget
   *
   * It also tests two additional scenarios:
   *
   * - Sets `TextDisplayMethod` to 0 and verifies that the display is different
   * - Sets `type` of the field to 0 and verifies that the display is different
   */
  function test_01_checkBox() {
    const labelItem = checkBox.children[0];
    const checkBoxItem = checkBox.children[1];
    compare(checkBox.value, true);
    compare(checkBox.isBool, false);
    compare(checkBox.isNull, false);
    compare(checkBox.checkedLabel, "True");
    compare(checkBox.uncheckedLabel, "False");
    compare(labelItem.text, "False");
    compare(checkBoxItem.checked, false); // NOTE: even if `value` be true, without config `checked` will be false
    checkBox.config = {
      "TextDisplayMethod": 1,
      "CheckedState": "DEFAULT_CHECKED_STATE",
      "UncheckedState": "DEFAULT_UNCHECKED_STATE"
    };
    checkBox.value = true;
    checkBox.field = FeatureUtils.createField("dummy", FeatureUtils.Bool);
    compare(checkBox.value, true);
    compare(checkBox.isBool, true);
    compare(checkBox.isNull, false);
    compare(checkBox.checkedLabel, "DEFAULT_CHECKED_STATE");
    compare(checkBox.uncheckedLabel, "DEFAULT_UNCHECKED_STATE");
    compare(labelItem.text, "DEFAULT_CHECKED_STATE");
    compare(checkBoxItem.checked, true);
    checkBox.value = false;
    compare(labelItem.text, "DEFAULT_UNCHECKED_STATE");
    compare(checkBoxItem.checked, false);

    // test TextDisplayMethod = 0
    checkBox.config = {
      "TextDisplayMethod": 0,
      "CheckedState": "DEFAULT_CHECKED_STATE",
      "UncheckedState": "DEFAULT_UNCHECKED_STATE"
    };
    checkBox.value = true;
    compare(checkBox.checkedLabel, "True");
    compare(checkBox.uncheckedLabel, "False");

    // test field type = 0
    checkBox.config = {
      "TextDisplayMethod": 1,
      "CheckedState": "DEFAULT_CHECKED_STATE",
      "UncheckedState": "DEFAULT_UNCHECKED_STATE"
    };
    checkBox.field = FeatureUtils.createField("dummy", FeatureUtils.String);
    checkBox.value = true;
    compare(labelItem.text, "DEFAULT_UNCHECKED_STATE"); // NOTE: value is true but it in unchecked state
    compare(checkBoxItem.checked, false); // NOTE: value is true but its not checked
    checkBox.value = false;
    compare(labelItem.text, "DEFAULT_UNCHECKED_STATE");
    compare(checkBoxItem.checked, false);
  }

  /**
   * Tests the valueMap object and its properties
   *
   * This function tests the valueMap object and its properties, including:
   * - The initial state and current item count
   * - The comboBoxItem model and current index
   * - The toggleButtonsItem selected index
   * - The current layer and config
   * - The value and current key value
   *
   * TODO: needs more checks on search and changing selected item in combobox or toggleButtons
   */
  function test_01_valueMap() {
    const toggleButtonsItem = valueMap.children[0].children[0];
    const comboBoxItem = valueMap.children[0].children[1];
    compare(valueMap.toggleButtonsThreshold, 0);
    compare(valueMap.state, "comboBoxItemView");
    compare(valueMap.currentItemCount, 0);
    compare(comboBoxItem.model.length, undefined);
    compare(comboBoxItem.currentIndex, toggleButtonsItem.selectedIndex);
    valueMap.config = {
      "map": [{
          "Buckfast bee": "Apis Mellifera"
        }, {
          "Carniolan honey bee": "Apis Mellifera Carnica"
        }, {
          "European honey bee": "Apis Mellifera Mellifera"
        }]
    };
    valueMap.currentLayer = currentLayerTrue;
    valueMap.value = "Apis Mellifera";
    compare(valueMap.state, "toggleButtonsView");
    compare(comboBoxItem.currentIndex, toggleButtonsItem.selectedIndex);
    compare(comboBoxItem.currentIndex, toggleButtonsItem.selectedIndex);
    compare(valueMap.currentKeyValue, "Apis Mellifera");
    valueMap.currentLayer = currentLayerFalse;
    valueMap.value = "Apis Mellifera Carnica";
    compare(valueMap.state, "comboBoxItemView");
    compare(comboBoxItem.currentIndex, toggleButtonsItem.selectedIndex);
    compare(comboBoxItem.currentIndex, toggleButtonsItem.selectedIndex);
    compare(valueMap.currentKeyValue, "Apis Mellifera Carnica");
  }

  /**
   * Tests the UUIDGenerator object and its properties
   *
   * This function tests the UUIDGenerator object and its properties, including:
   * - The initial state and value
   * - The isLoaded and isAdding properties
   * - The label text
   * - The reset function
   *
   * TODO:
   * - The generateUUID function
   */
  function test_01_UuidGenerator() {
    const label = uuidGenerator.children[0];
    compare(label.text, "");
    compare(uuidGenerator.isLoaded, false);
    uuidGenerator.value = "ANY_VALUE";
    compare(label.text, "ANY_VALUE");
    uuidGenerator.isAdding = true;
    uuidGenerator.isLoaded = true;
    uuidGenerator.value = "";
    verify(label.text !== "ANY_VALUE");
  }

  /**
   * Helper function: Initializes the ValueRelation component in read-only mode.
   *
   * This function:
   * - Sets the current layer and feature
   * - Disables the widget (read-only state)
   * - Resets valueRelation state
   * - Verifies that the layer allows adding value relation features
   */
  function setupValueRelationInReadonlyMode() {
    const selectedLayer = qgisProject.mapLayersByName('Fields')[0];
    valueRelation.currentLayer = selectedLayer;
    const selectedFeature = qgisProject.mapLayersByName('Fields')[0].getFeature("39");
    valueRelation.currentFeature = selectedFeature;
    valueRelation.isEnabled = false;
    valueRelation.value = undefined;
    waitForRendering(valueRelation);
    wait(500);
    verify(selectedLayer.customProperty('QFieldSync/allow_value_relation_feature_addition') !== undefined);
    compare(selectedLayer.customProperty('QFieldSync/allow_value_relation_feature_addition'), true);
  }

  /**
   * Tests ValueRelation behavior in read-only vs editable mode.
   *
   * This test:
   * - Verifies that the combobox and add feature button are disabled in read-only mode
   * - Enables editing and checks that UI elements become interactive
   * - Confirms that the combobox is populated and the list view is empty
   */
  function test_01_ValueRelation() {
    valueRelation.config = {
      "AllowMulti": false,
      "AllowNull": false,
      "CompleterMatchFlags": 2,
      "DisplayGroupName": false,
      "Key": "id",
      "LayerName": "TestRelationValues",
      "LayerProviderName": "ogr",
      "NofColumns": 1,
      "OrderByDescending": false,
      "OrderByField": false,
      "OrderByFieldName": "id",
      "OrderByKey": true,
      "OrderByValue": false,
      "UseCompleter": false,
      "Value": "name"
    };
    setupValueRelationInReadonlyMode();
    const relationComboBoxParent = valueRelation.children[0];
    const comboBoxItem = Utils.findChildren(relationComboBoxParent, "RelationComboBox");
    const addFeatureButton = Utils.findChildren(relationComboBoxParent, "AddFeatureButton");
    const valueRelationListComponentParent = valueRelation.children[1];
    const valueRelationRepeater = Utils.findChildren(valueRelationListComponentParent, "ValueRelationRepeater");

    // check ui in readonly mode
    compare(relationComboBoxParent.enabled, false);
    compare(addFeatureButton.enabled, false);

    // turn on editable mode
    valueRelation.isEnabled = true;
    waitForRendering(valueRelation);

    // check ui in editable mode
    compare(relationComboBoxParent.enabled, true);
    compare(relationComboBoxParent.allowAddFeature, true);
    compare(addFeatureButton.enabled, true);

    // be sure that comboBox is loaded and listview is empty
    compare(valueRelationRepeater.count, 0);
    verify(comboBoxItem.count > 0);
  }

  /**
   * Tests ValueRelation ordering by feature key (OrderByKey = true).
   *
   * This test:
   * - Verifies that items in the combobox should be sorted by feature key
   * - Compares actual order with the expected order
   */
  function test_02_ValueRelation() {
    valueRelation.config = {
      "AllowMulti": false,
      "AllowNull": false,
      "CompleterMatchFlags": 2,
      "DisplayGroupName": false,
      "Key": "id",
      "LayerName": "TestRelationValues",
      "LayerProviderName": "ogr",
      "NofColumns": 1,
      "OrderByDescending": false,
      "OrderByField": false,
      "OrderByFieldName": "id",
      "OrderByKey": true,
      "OrderByValue": false,
      "UseCompleter": false,
      "Value": "name"
    };
    setupValueRelationInReadonlyMode();
    const relationComboBoxParent = valueRelation.children[0];
    const comboBoxItem = Utils.findChildren(relationComboBoxParent, "RelationComboBox");
    const featureListModel = comboBoxItem.model;
    const expectedOrderedData = {
      "name": ["Ethan", "Liam", "Olivia", "Sophia", "Noah", "Ava", "Mathieu", "Mason"]
    };
    const namesInList = expectedOrderedData["name"];
    wait(1000);
    compare(comboBoxItem.count, namesInList.length);

    // check every element inside combobox model is correctly setted
    for (let i = 0; i < comboBoxItem.count; ++i) {
      const value = featureListModel.dataFromRowIndex(i, FeatureListModel.DisplayStringRole);
      compare(value, namesInList[i]);
    }
  }

  /**
   * Tests ValueRelation ordering by display value (OrderByValue = true).
   *
   * This test:
   * - Ensures combobox items are sorted alphabetically by display value
   * - Confirms the order matches the expected sequence
   */
  function test_03_ValueRelation() {
    valueRelation.config = {
      "AllowMulti": false,
      "AllowNull": false,
      "CompleterMatchFlags": 2,
      "DisplayGroupName": false,
      "Key": "id",
      "LayerName": "TestRelationValues",
      "LayerProviderName": "ogr",
      "NofColumns": 1,
      "OrderByDescending": false,
      "OrderByField": false,
      "OrderByFieldName": "id",
      "OrderByKey": false,
      "OrderByValue": true,
      "UseCompleter": false,
      "Value": "name"
    };
    setupValueRelationInReadonlyMode();
    const relationComboBoxParent = valueRelation.children[0];
    const comboBoxItem = Utils.findChildren(relationComboBoxParent, "RelationComboBox");
    const featureListModel = comboBoxItem.model;
    const expectedOrderedData = {
      "name": ["Ava", "Ethan", "Liam", "Mason", "Mathieu", "Noah", "Olivia", "Sophia"]
    };
    const namesInList = expectedOrderedData["name"];
    wait(500);

    // check every element inside combobox model is correctly setted
    for (let i = 0; i < comboBoxItem.count; ++i) {
      const value = featureListModel.dataFromRowIndex(i, FeatureListModel.DisplayStringRole);
      compare(value, namesInList[i]);
    }
  }

  /**
   * Tests embedded feature form integration in ValueRelation.
   *
   * This test:
   * - Checks that embeddedFeatureForm is null initially
   * - After clicking the Add feature button, verifies that the embedded form appears
   *   and its state is set to "Add"
   */
  function test_04_ValueRelation() {
    valueRelation.config = {
      "AllowMulti": false,
      "AllowNull": false,
      "CompleterMatchFlags": 2,
      "DisplayGroupName": false,
      "Key": "id",
      "LayerName": "TestRelationValues",
      "LayerProviderName": "ogr",
      "NofColumns": 1,
      "OrderByDescending": false,
      "OrderByField": false,
      "OrderByFieldName": "id",
      "OrderByKey": true,
      "OrderByValue": false,
      "UseCompleter": false,
      "Value": "name"
    };
    setupValueRelationInReadonlyMode();
    const relationComboBoxParent = valueRelation.children[0];
    const addFeatureButton = Utils.findChildren(relationComboBoxParent, "AddFeatureButton");
    valueRelation.isEnabled = true;
    waitForRendering(valueRelation);
    verify(relationComboBoxParent.embeddedFeatureForm === null);

    // After click embeddedFeatureForm should be not null and in `Add` state
    addFeatureButton.click();
    wait(500);
    compare(relationComboBoxParent.embeddedFeatureForm.state, "Add");
    relationComboBoxParent.embeddedFeatureForm.close();
    compare(relationComboBoxParent.embeddedFeatureForm.opened, false);
  }

  /**
   * Tests ValueRelation when AllowNull is enabled.
   *
   * This test:
   * - Verifies that a NULL option is displayed as <i>NULL</i>
   * - Checks that other items are displayed and ordered correctly
   */
  function test_05_ValueRelation() {
    valueRelation.config = {
      "AllowMulti": false,
      "AllowNull": true,
      "CompleterMatchFlags": 2,
      "DisplayGroupName": false,
      "Key": "id",
      "LayerName": "TestRelationValues",
      "LayerProviderName": "ogr",
      "NofColumns": 1,
      "OrderByDescending": false,
      "OrderByField": false,
      "OrderByFieldName": "id",
      "OrderByKey": false,
      "OrderByValue": true,
      "UseCompleter": false,
      "Value": "name"
    };
    setupValueRelationInReadonlyMode();
    const relationComboBoxParent = valueRelation.children[0];
    const comboBoxItem = Utils.findChildren(relationComboBoxParent, "RelationComboBox");
    const featureListModel = comboBoxItem.model;
    const expectedOrderedData = {
      "name": ["Ava", "Ethan", "Liam", "Mason", "Mathieu", "Noah", "Olivia", "Sophia"]
    };
    const namesInList = expectedOrderedData["name"];
    wait(500);
    compare(comboBoxItem.count, namesInList.length + 1);
    compare(comboBoxItem.displayText, "<i>NULL</i>");

    // check every element inside combobox model is correctly setted
    for (let i = 1; i < comboBoxItem.count - 1; ++i) {
      const value = featureListModel.dataFromRowIndex(i, FeatureListModel.DisplayStringRole);
      compare(value, namesInList[i - 1]);
    }
  }

  /**
   * Tests ValueRelation in multi-select mode with grouping.
   *
   * This test:
   * - Verifies UI state in read-only and editable mode
   * - Checks that grid view layout is applied
   * - Confirms that the repeater contains grouped items after enabling editing
   * - Uses a second ValueRelation instance (valueRelation2) because the first one
   *   had a populated combobox; we need a fresh instance to ensure the combobox model
   *   remains empty in list mode
   */
  function test_06_ValueRelation() {
    valueRelation2.config = {
      "AllowMulti": true,
      "AllowNull": true,
      "CompleterMatchFlags": 2,
      "DisplayGroupName": true,
      "Group": "location",
      "Key": "id",
      "LayerName": "TestRelationValues",
      "LayerProviderName": "ogr",
      "NofColumns": 2,
      "OrderByDescending": false,
      "OrderByField": false,
      "OrderByFieldName": "id",
      "OrderByKey": true,
      "OrderByValue": false,
      "UseCompleter": true,
      "Value": "name"
    };
    setupValueRelationInReadonlyMode();
    const relationComboBoxParent = valueRelation2.children[0];
    const comboBoxItem = Utils.findChildren(relationComboBoxParent, "RelationComboBox");
    const addFeatureButton = Utils.findChildren(relationComboBoxParent, "AddFeatureButton");
    const valueRelationListComponentParent = valueRelation2.children[1];
    const valueRelationGridView = Utils.findChildren(valueRelationListComponentParent, "ValueRelationGridView");
    const valueRelationRepeater = Utils.findChildren(valueRelationListComponentParent, "ValueRelationRepeater");
    const valueRelationSearchBar = Utils.findChildren(valueRelationListComponentParent, "ValueRelationSearchBar");
    const featureListModel = valueRelationRepeater.model;

    // check ui in readonly mode
    compare(relationComboBoxParent.enabled, false);
    compare(addFeatureButton.enabled, false);
    compare(valueRelationSearchBar.enabled, false);

    // turn on editable mode
    valueRelation2.isEnabled = true;
    waitForRendering(valueRelation2);

    // check ui in editable mode
    compare(relationComboBoxParent.enabled, true);
    compare(addFeatureButton.enabled, true);
    compare(valueRelationSearchBar.enabled, true);
    compare(comboBoxItem.count, 0);
    verify(valueRelationGridView.columns > 0);
    verify(valueRelationRepeater.count > 0);
  }

  /**
   * Tests grouped display in ValueRelation when grouping by 'team' with multi-select mode.
   *
   * This test:
   * - Compares actual item order against expected grouped order
   */
  function test_07_ValueRelation() {
    valueRelation.config = {
      "AllowMulti": true,
      "AllowNull": true,
      "CompleterMatchFlags": 2,
      "DisplayGroupName": true,
      "Group": "team",
      "Key": "id",
      "LayerName": "TestRelationValues",
      "LayerProviderName": "ogr",
      "NofColumns": 2,
      "OrderByDescending": false,
      "OrderByField": false,
      "OrderByFieldName": "name",
      "OrderByKey": true,
      "OrderByValue": false,
      "UseCompleter": false,
      "Value": "name"
    };
    setupValueRelationInReadonlyMode();
    const valueRelationListComponentParent = valueRelation.children[1];
    const valueRelationRepeater = Utils.findChildren(valueRelationListComponentParent, "ValueRelationRepeater");
    const featureListModel = valueRelationRepeater.model;
    const expectedOrderedData = {
      "name": ["<i>NULL</i>", "Ethan", "Olivia", "Mason", "Liam", "Mathieu", "Sophia", "Noah", "Ava"]
    };
    compare(valueRelationRepeater.count, expectedOrderedData["name"].length);
    for (let i = 0; i < valueRelationRepeater.count; ++i) {
      const value = featureListModel.dataFromRowIndex(i, FeatureListModel.DisplayStringRole);
      compare(value, expectedOrderedData["name"][i]);
    }
  }

  /**
   * Tests ValueRelation ordering by value when grouping is enabled (group by 'team').
   *
   * This test:
   * - Ensures all items are displayed in the expected sequence
   * - Verifies correct handling of grouping and sorting by value
   */
  function test_08_ValueRelation() {
    valueRelation.config = {
      "AllowMulti": true,
      "AllowNull": false,
      "CompleterMatchFlags": 2,
      "DisplayGroupName": true,
      "Group": "team",
      "Key": "id",
      "LayerName": "TestRelationValues",
      "LayerProviderName": "ogr",
      "NofColumns": 1,
      "OrderByDescending": false,
      "OrderByField": false,
      "OrderByFieldName": "name",
      "OrderByKey": false,
      "OrderByValue": true,
      "UseCompleter": false,
      "Value": "name"
    };
    setupValueRelationInReadonlyMode();
    const valueRelationListComponentParent = valueRelation.children[1];
    const valueRelationRepeater = Utils.findChildren(valueRelationListComponentParent, "ValueRelationRepeater");
    const featureListModel = valueRelationRepeater.model;
    const expectedOrderedData = {
      "name": ["Ethan", "Mason", "Olivia", "Liam", "Mathieu", "Ava", "Noah", "Sophia"]
    };
    compare(valueRelationRepeater.count, expectedOrderedData["name"].length);
    for (let i = 0; i < valueRelationRepeater.count; ++i) {
      const value = featureListModel.dataFromRowIndex(i, FeatureListModel.DisplayStringRole);
      compare(value, expectedOrderedData["name"][i]);
    }
  }

  /**
   * Tests ValueRelation search filtering
   *
   * This test:
   * - Verifies that the search bar filters items correctly based on input text
   * - Ensures correct item count after applying search terms
   */
  function test_09_ValueRelation() {
    valueRelation.config = {
      "AllowMulti": true,
      "AllowNull": false,
      "CompleterMatchFlags": 2,
      "DisplayGroupName": false,
      "Group": "location",
      "Key": "id",
      "LayerName": "TestRelationValues",
      "LayerProviderName": "ogr",
      "NofColumns": 2,
      "OrderByDescending": false,
      "OrderByField": false,
      "OrderByFieldName": "id",
      "OrderByKey": true,
      "OrderByValue": false,
      "UseCompleter": true,
      "Value": "name"
    };
    setupValueRelationInReadonlyMode();
    const valueRelationListComponentParent = valueRelation.children[1];
    const valueRelationRepeater = Utils.findChildren(valueRelationListComponentParent, "ValueRelationRepeater");
    const valueRelationSearchBar = Utils.findChildren(valueRelationListComponentParent, "ValueRelationSearchBar");
    const searchTextField = valueRelationSearchBar.children[0].children[2];

    // turn on editable mode
    valueRelation.isEnabled = true;
    waitForRendering(valueRelation);

    // write in search bar
    searchTextField.text = "o";

    // only Olivia, Noah, Sophia, Mason
    compare(valueRelationRepeater.count, 4);

    // write in search bar
    searchTextField.text = "ia";

    // only Olivia, Sophia, Liam
    compare(valueRelationRepeater.count, 3);
  }

  /**
   * Tests ValueRelation component behavior
   *
   * Verifies:
   * - Initial state and display text with undefined value
   * - Value update in editable mode reflects in the combobox
   * - Search popup filtering, result ordering, and highlighting
   * - Selecting an item updates the ValueRelation correctly
   *
   * TODO: In this test, value is manually set after search selection to continue the test,
   *       but in real usage, value should be set automatically upon item selection.
   */
  function test_10_ValueRelation() {
    valueRelation.config = {
      "AllowMulti": false,
      "AllowNull": false,
      "CompleterMatchFlags": 2,
      "DisplayGroupName": false,
      "Group": "team",
      "Key": "id",
      "LayerName": "TestRelationValues",
      "LayerProviderName": "ogr",
      "NofColumns": 1,
      "OrderByDescending": false,
      "OrderByField": false,
      "OrderByFieldName": "id",
      "OrderByKey": true,
      "OrderByValue": false,
      "UseCompleter": true,
      "Value": "name"
    };
    setupValueRelationInReadonlyMode();
    const relationComboBoxParent = valueRelation.children[0];
    const comboBoxItem = Utils.findChildren(relationComboBoxParent, "RelationComboBox");
    const addFeatureButton = Utils.findChildren(relationComboBoxParent, "AddFeatureButton");
    const openSearchFeaturePopupButton = Utils.findChildren(relationComboBoxParent, "OpenSearchFeaturePopupButton");

    // Initially, the combobox should not select any item because the value is undefined
    compare(comboBoxItem.displayText, "");

    // Enable editable mode
    valueRelation.isEnabled = true;

    // Setting the value to 3 should update the combobox display text to `Olivia`
    valueRelation.value = 3;
    compare(comboBoxItem.count, 8);
    wait(500);
    compare(comboBoxItem.displayText, "Olivia");
    compare(relationComboBoxParent.searchPopup.opened, false);

    // Open the search popup and search for 'ia'.
    // Only three items should be visible. After selecting one, the combobox display text should update.
    openSearchFeaturePopupButton.click();
    wait(500);
    compare(relationComboBoxParent.searchPopup.opened, true);
    const searchFeaturePopup = relationComboBoxParent.searchPopup.contentItem;
    const searchBarTextField = searchFeaturePopup.children[0].children[1].children[0].children[0].children[2];
    const searchFeatureResultsList = searchFeaturePopup.children[0].children[1].children[1];
    const featureListModel = searchFeatureResultsList.model;
    compare(searchFeatureResultsList.count, 8);
    const expectedOrderedData = ["Ethan", "Olivia", "Mason", "Liam", "Mathieu", "Sophia", "Noah", "Ava"];
    for (let i = 0; i < searchFeatureResultsList.count; ++i) {
      const value = featureListModel.dataFromRowIndex(i, FeatureListModel.DisplayStringRole);
      compare(expectedOrderedData[i], value);
    }
    searchBarTextField.text = "ai";
    compare(searchFeatureResultsList.count, 0);
    searchBarTextField.text = "ia";
    const expectedOrderedData2 = ["Olivia", "Liam", "Sophia"];
    compare(searchFeatureResultsList.count, expectedOrderedData2.length);
    for (let j = 0; j < searchFeatureResultsList.count; ++j) {
      const value = featureListModel.dataFromRowIndex(j, FeatureListModel.DisplayStringRole);
      compare(value, expectedOrderedData2[j]);
    }
    wait(500);
    const itemToClick = searchFeatureResultsList.itemAtIndex(2);
    compare(itemToClick.children[0].children[2].text, "Soph<span style=\"text-decoration:underline;color:#000000\">ia</span>"); // `Sophia` highlighted!
    const clickX = itemToClick.x + itemToClick.width / 2;
    const clickY = itemToClick.y + itemToClick.height / 2;
    mouseClick(searchFeatureResultsList, clickX, clickY);
    wait(500);
    compare(relationComboBoxParent.searchPopup.opened, false);
    compare(comboBoxItem.displayText, "Sophia");
  }
}
