

# File CheckBox.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**CheckBox.qml**](CheckBox_8qml.md)

[Go to the documentation of this file](CheckBox_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material.impl
import org.qfield
import Theme

EditorWidgetBase {
  id: checkBoxEditorWidgetBase
  height: childrenRect.height

  // if the field type is boolean, ignore the configured 'CheckedState' and 'UncheckedState' values and work with true/false always
  readonly property bool isBool: field.type == 1  // needs type coercion
  property string checkedLabel: config['TextDisplayMethod'] === 1 && config['CheckedState'] !== null && config['CheckedState'] !== '' ? config['CheckedState'] : qsTr('True')
  property string uncheckedLabel: config['TextDisplayMethod'] === 1 && config['UncheckedState'] !== null && config['UncheckedState'] !== '' ? config['UncheckedState'] : qsTr('False')

  anchors {
    right: parent.right
    left: parent.left
  }

  Label {
    id: checkValue
    height: Math.max(48, fontMetrics.height + 20)
    anchors {
      left: parent.left
      right: checkBox.left
    }

    font.pointSize: Theme.defaultFont.pointSize
    font.bold: Theme.defaultFont.bold
    verticalAlignment: Text.AlignVCenter
    color: (!isEditable && isEditing) || isNull || isEmpty ? Theme.mainTextDisabledColor : Theme.mainTextColor

    text: isEmpty ? qsTr("Empty") : isNull ? qsTr('NULL') : checkBox.checked ? checkedLabel : uncheckedLabel
  }

  QfSwitch {
    id: checkBox
    enabled: isEnabled
    width: implicitContentWidth

    anchors {
      right: parent.right
      verticalCenter: checkValue.verticalCenter
    }

    checked: {
      if (isBool) {
        let actualValue = value;
        // Some datasets - such as Geopackage - can send the value as a 'True' or 'False' string, we have to work around that
        if (!isNull) {
          if (typeof value == 'string') {
            actualValue = value.toLowerCase() === 'true';
          }
        }
        return !isNull ? actualValue : false;
      } else {
        return !isNull ? String(value) === config['CheckedState'] : false;
      }
    }
  }

  MouseArea {
    id: checkArea
    enabled: isEnabled
    anchors.fill: parent

    onClicked: {
      let editedValue = true;
      if (isBool) {
        let actualValue = value;
        // Some datasets - such as Geopackage - can send the value as a 'True' or 'False' string, we have to work around that
        if (!isNull && typeof value == 'string') {
          actualValue = value.toLowerCase() === 'true';
        }
        editedValue = !isNull ? !actualValue : true;
      } else {
        if (!isNull) {
          // Type coercion is desired here as custom unchecked/checked states are stored as strings yet value could be integers
          editedValue = value == config['CheckedState'] ? config['UncheckedState'] : config['CheckedState'];
        } else {
          editedValue = config['CheckedState'];
        }
      }
      valueChangeRequested(editedValue, false);
    }
  }

  FontMetrics {
    id: fontMetrics
    font: checkValue.font
  }
}
```


