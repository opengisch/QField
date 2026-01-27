

# File enum.qml

[**File List**](files.md) **>** [**cogoparameterwidgets**](dir_a00d444b399c5bcccd8697d65fbda43e.md) **>** [**enum.qml**](cogoparameterwidgets_2enum_8qml.md)

[Go to the documentation of this file](cogoparameterwidgets_2enum_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme
import org.qgis
import org.qfield

CogoParameterWidgetBase {
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
        color: Theme.secondaryTextColor
        font: Theme.tipFont
        text: parameterLabel
      }
    }

    QfComboBox {
      id: enumComboBox
      Layout.fillWidth: true
      model: parameterConfiguration["options"]

      onCurrentValueChanged: {
        processValue();
      }
    }
  }

  function processValue() {
    valueChangeRequested(enumComboBox.currentValue);
  }
}
```


