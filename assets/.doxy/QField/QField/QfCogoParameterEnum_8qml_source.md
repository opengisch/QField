

# File QfCogoParameterEnum.qml

[**File List**](files.md) **>** [**cogoparameterwidgets**](dir_862d3fe9c1a901e6783c6f66e1e7cc2e.md) **>** [**QfCogoParameterEnum.qml**](QfCogoParameterEnum_8qml.md)

[Go to the documentation of this file](QfCogoParameterEnum_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.gui
import org.qgis
import org.qfield.core

QfCogoParameterWidgetBase {
  id: enumParameter

  width: parent.width
  height: childrenRect.height

  GridLayout {
    anchors {
      left: parent.left
      right: parent.right
    }
    columns: 1

    RowLayout {
      Rectangle {
        visible: parameterColor != "#00000000"
        width: 10
        height: 10
        radius: 5
        color: parameterColor
      }

      Label {
        Layout.fillWidth: true
        color: QfTheme.mainTextColor
        font: QfTheme.tipFont
        text: parameterLabel
      }
    }

    QfToggleButtonGroup {
      id: enumToggleButtonGroup
      Layout.fillWidth: true
      visible: !!parameterConfiguration["toggle"]
      model: parameterConfiguration["options"]
      font: QfTheme.tipFont
      buttonMininumWidth: parent.width / 2 - buttonSpacing
      selectedIndex: 0

      onSelectedIndexChanged: {
        processValue();
      }
    }

    QfComboBox {
      id: enumComboBox
      Layout.fillWidth: true
      visible: !!!parameterConfiguration["toggle"]
      model: parameterConfiguration["options"]

      onCurrentValueChanged: {
        processValue();
      }
    }
  }

  function processValue() {
    valueChangeRequested(enumComboBox.visible ? enumComboBox.currentValue : enumToggleButtonGroup.model[enumToggleButtonGroup.selectedIndex]);
  }
}
```


