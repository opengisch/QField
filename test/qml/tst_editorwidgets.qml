import QtQuick
import QtTest
import org.qfield
import Theme
import "../../src/qml/editorwidgets" as EditorWidgets

TestCase {
  name: "EditorWidgets"

  Item {
    // mainWindow for widgets
    id: mainWindowItem
  }

  EditorWidgets.TextEdit {
    id: textEdit
    property var mainWindow: mainWindowItem
    property string value: "DEFAULT_VALUE"
    property var config: undefined
    property bool isEnabled: true
  }

  EditorWidgets.Range {
    id: range
    property var mainWindow: mainWindowItem
    property real value: default_value
    property var config: undefined
    property bool isEnabled: true

    readonly property real default_value: 999
  }

  EditorWidgets.DateTime {
    id: dateTime
    property var mainWindow: mainWindowItem
    fieldIsDate: false // to simulate LayerUtils.fieldType( field ) != 'QDate'
    property string value: "2022-01-01"
    property var config: undefined
    property var field: undefined
    property bool isEnabled: true
  }

  EditorWidgets.CheckBox {
    id: checkBox
    property var mainWindow: mainWindowItem
    property bool value: true
    property var config: undefined
    property var field: undefined
  }

  EditorWidgets.ValueMap {
    id: valueMap
    property var mainWindow: mainWindowItem
    property var value: undefined
    property var config: undefined
    property var field: undefined
    property var currentLayer: undefined

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
    property var config: undefined
    property bool isAdding: false
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
    const textField = textEdit.children[1];
    const textArea = textEdit.children[2];
    compare(textReadonlyValue.text, ""); // NOTE: If the config is undefined, the label will be an empty string.
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
    compare(range.precision, 1);
    compare(range.min, -Infinity);
    compare(range.max, Infinity);
    compare(range.step, 1);
    compare(range.suffix, "");

    // Row
    // compare(sliderRow.visible, true) // ERROR ? should work but not working!
    compare(textField.text, "3");
    range.config = {
      "Style": "Slider",
      "Precision": 2,
      "Min": -10,
      "Max": 10,
      "Step": 10,
      "Suffix": "DEFAULT_SUFFIX"
    };
    range.value = 4;
    compare(range.widgetStyle, "Slider");
    compare(range.precision, 2);
    compare(range.min, -10);
    compare(range.max, 10);
    compare(range.step, 10);
    compare(range.suffix, "DEFAULT_SUFFIX");
    compare(sliderRow.visible, false);
    compare(valueLabel.text, range.min + ".00DEFAULT_SUFFIX"); // NOTE: using `range.min` because of `rangeItem.parent.value`
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
    const label = dateTime.children[1].children[0];
    compare(label.text, ""); // NOTE: setting value without setting `config` and `field` objects won't work
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
          "field_format": "UNKNOWN!",
          "allow_null": false
        };
        dateTime.field = {
          "isDateOrTime": true
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
    compare(checkBox.checkedLabel, ""); // NOTE: `checkedLabel` initialized with "" when config is undefined
    compare(checkBox.uncheckedLabel, ""); // NOTE: `uncheckedLabel` initialized with "" when config is undefined
    compare(labelItem.text, "");
    compare(checkBoxItem.checked, false); // NOTE: even if `value` be true, without config `checked` will be false
    checkBox.config = {
      "TextDisplayMethod": 1,
      "CheckedState": "DEFAULT_CHECKED_STATE",
      "UncheckedState": "DEFAULT_UNCHECKED_STATE"
    };
    checkBox.field = {
      "type": 1
    };
    checkBox.value = true;
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
    checkBox.field = {
      "type": 1
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
    checkBox.field = {
      "type": 0
    };
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
}
