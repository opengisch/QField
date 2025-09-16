import QtQuick
import QtQuick.Controls
import Theme

EditorWidgetBase {
  height: childrenRect.height

  // if the field type is boolean, ignore the configured 'CheckedState' and 'UncheckedState' values and work with true/false always
  readonly property bool isBool: field.type == 1  // needs type coercion
  property bool isNull: value == undefined
  property string checkedLabel: config['TextDisplayMethod'] === 1 && config['CheckedState'] !== null && config['CheckedState'] !== '' ? config['CheckedState'] : qsTr('True')
  property string uncheckedLabel: config['TextDisplayMethod'] === 1 && config['UncheckedState'] !== null && config['UncheckedState'] !== '' ? config['UncheckedState'] : qsTr('False')

  anchors {
    right: parent.right
    left: parent.left
  }

  Label {
    id: checkValue
    height: fontMetrics.height + 20
    anchors {
      left: parent.left
      right: checkBox.left
    }

    topPadding: 10
    bottomPadding: 10
    font.pointSize: Theme.defaultFont.pointSize
    font.bold: Theme.defaultFont.bold
    font.italic: isNull
    color: isEnabled && !isNull ? Theme.mainTextColor : Theme.mainTextDisabledColor

    text: !isNull ? checkBox.checked ? checkedLabel : uncheckedLabel : isEnabled ? qsTr('NULL') : ''
  }

  QfSwitch {
    id: checkBox
    enabled: isEnabled
    visible: isEnabled
    width: implicitContentWidth
    small: true

    anchors {
      right: parent.right
      verticalCenter: checkValue.verticalCenter
    }

    checked: {
      if (isBool) {
        var actualValue = value;
        // Some datasets - such as Geopackage - can send the value as a 'True' or 'False' string, we have to work around that
        if (!isNull && typeof value == 'string') {
          actualValue = value.toLowerCase() === 'true';
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
      var editedValue = true;
      if (isBool) {
        var actualValue = value;
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

  Rectangle {
    id: backgroundRect
    anchors.left: parent.left
    anchors.right: parent.right
    y: checkValue.height - height - checkValue.bottomPadding / 2
    implicitWidth: 120
    height: checkBox.activeFocus || checkBox.pressed || checkArea.containsPress ? 2 : 1
    color: checkBox.activeFocus || checkBox.pressed || checkArea.containsPress ? Theme.accentColor : Theme.accentLightColor
  }

  FontMetrics {
    id: fontMetrics
    font: checkValue.font
  }
}
