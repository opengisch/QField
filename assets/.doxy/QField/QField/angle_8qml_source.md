

# File angle.qml

[**File List**](files.md) **>** [**cogoparameterwidgets**](dir_a00d444b399c5bcccd8697d65fbda43e.md) **>** [**angle.qml**](angle_8qml.md)

[Go to the documentation of this file](angle_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme
import org.qgis
import org.qfield

CogoParameterWidgetBase {
  id: angleParameter

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
        color: Theme.mainTextColor
        font: Theme.tipFont
        text: parameterLabel
      }

      Label {
        Layout.fillWidth: true
        color: Theme.secondaryTextColor
        font: Theme.tipFont
        text: '[' + qsTr('deg') + ']'
      }
    }

    QfTextField {
      id: angleField
      Layout.fillWidth: true
      font: Theme.tipFont
      inputMethodHints: Qt.ImhFormattedNumbersOnly

      onTextEdited: {
        processValue();
      }
    }
  }

  function processValue() {
    const angle = parseFloat(angleField.text);
    if (!isNaN(angle)) {
      valueChangeRequested(angle);
    } else {
      valueChangeRequested(undefined);
    }
  }
}
```


