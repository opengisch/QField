

# File QfCogoParameterElevation.qml

[**File List**](files.md) **>** [**cogoparameterwidgets**](dir_862d3fe9c1a901e6783c6f66e1e7cc2e.md) **>** [**QfCogoParameterElevation.qml**](QfCogoParameterElevation_8qml.md)

[Go to the documentation of this file](QfCogoParameterElevation_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.gui
import org.qgis
import org.qfield.core

QfCogoParameterWidgetBase {
  id: elevationParameter

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
        color: QfTheme.mainTextColor
        font: QfTheme.tipFont
        text: parameterLabel
      }

      Label {
        Layout.fillWidth: true
        color: QfTheme.secondaryTextColor
        font: QfTheme.tipFont
        text: destinationCrs.isGeographic ? '' : '[' + UnitTypes.toAbbreviatedString(destinationCrs.mapUnits) + ']'
      }
    }

    QfTextField {
      id: elevationField
      Layout.fillWidth: true
      font: QfTheme.tipFont
      inputMethodHints: Qt.ImhFormattedNumbersOnly

      onTextEdited: {
        processValue();
      }
    }
  }

  function processValue() {
    const elevation = parseFloat(elevationField.text);
    if (!isNaN(elevation)) {
      valueChangeRequested(elevation);
    } else {
      valueChangeRequested(undefined);
    }
  }
}
```


