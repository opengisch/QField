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
    }
  }

  function requestedPositionReceived(position) {
    xField.text = position.x;
    yField.text = position.y;
  }
}
