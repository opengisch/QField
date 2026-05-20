

# File distance.qml

[**File List**](files.md) **>** [**cogoparameterwidgets**](dir_a00d444b399c5bcccd8697d65fbda43e.md) **>** [**distance.qml**](cogoparameterwidgets_2distance_8qml.md)

[Go to the documentation of this file](cogoparameterwidgets_2distance_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme
import org.qgis
import org.qfield

CogoParameterWidgetBase {
  id: distanceParameter

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
        text: '[' + UnitTypes.toAbbreviatedString(destinationCrs.mapUnits) + ']'
      }
    }

    QfTextField {
      id: distanceField
      Layout.fillWidth: true
      font: Theme.tipFont
      inputMethodHints: Qt.ImhFormattedNumbersOnly

      onTextEdited: {
        processValue();
      }
    }
  }

  function processValue() {
    const distance = parseFloat(distanceField.text);
    if (!isNaN(distance)) {
      valueChangeRequested(distance);
    } else {
      valueChangeRequested(undefined);
    }
  }
}
```


