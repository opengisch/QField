import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme
import org.qgis
import org.qfield

CogoParameterWidgetBase {
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
        color: Theme.mainTextColor
        font: Theme.tipFont
        text: parameterLabel ? parameterLabel : qsTr("Elevation")
      }

      Label {
        Layout.fillWidth: true
        color: Theme.secondaryTextColor
        font: Theme.tipFont
        text: destinationCrs.isGeographic ? '' : '[' + UnitTypes.toAbbreviatedString(destinationCrs.mapUnits) + ']'
      }
    }

    QfTextField {
      id: elevationField
      Layout.fillWidth: true
      font: Theme.tipFont

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
