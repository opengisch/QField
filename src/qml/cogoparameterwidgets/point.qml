import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme
import org.qgis
import org.qfield

CogoParameterWidgetBase {
  id: pointParameter

  width: parent.width
  height: childrenRect.height

  GridLayout {
    anchors {
      left: parent.left
      right: parent.right
    }
    columns: 2

    Label {
      Layout.fillWidth: true
      color: Theme.secondaryTextColor
      font: Theme.tipFont
      text: (label ? label + ": " : "") + qsTr("X")
    }

    QfToolButton {
      Layout.rowSpan: 4

      iconSource: Theme.getThemeVectorIcon("ic_location_valid_white_24dp")
      iconColor: Theme.positionColor
      bgcolor: "transparent"
      round: true

      onClicked: {
        requestPosition(pointParameter);
      }
    }

    QfTextField {
      id: xField
      Layout.fillWidth: true
      font: Theme.tipFont

      onTextEdited: {
        processValue();
      }
    }

    Label {
      Layout.fillWidth: false
      color: Theme.secondaryTextColor
      font: Theme.tipFont
      text: (label ? label + ": " : "") + qsTr("Y")
    }

    QfTextField {
      id: yField
      Layout.fillWidth: true
      font: Theme.tipFont

      onTextEdited: {
        processValue();
      }
    }
  }

  function processValue() {
    const x = parseFloat(xField.text);
    const y = parseFloat(yField.text);
    if (!isNaN(x) && !isNaN(y)) {
      valueChangeRequested(GeometryUtils.point(x, y));
    } else {
      valueChangeRequested(undefined);
    }
  }

  function requestedPositionReceived(position) {
    if (position.isValid) {
      xField.text = position.x;
      yField.text = position.y;
      processValue();
    } else {
      xField.text = "";
      yField.text = "";
      processValue();
    }
  }
}
