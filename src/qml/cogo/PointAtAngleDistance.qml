import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme
import org.qgis
import org.qfield
import "."

CogoBase {
  title: qsTr("Point at Angle/Distance")

  GridLayout {
    anchors {
      left: parent.left
      leftMargin: 10
      right: parent.right
      rightMargin: 10
    }
    columns: 3

    Label {
      Layout.fillWidth: false
      color: Theme.secondaryTextColor
      font: Theme.tipFont
      text: qsTr("X")
    }

    QfTextField {
      id: xField
      Layout.fillWidth: true
      font: Theme.tipFont
    }

    QfToolButton {
      Layout.rowSpan: 2

      iconSource: Theme.getThemeVectorIcon("ic_location_valid_white_24dp")
      iconColor: Theme.positionColor
      bgcolor: "transparent"
      round: true
    }

    Label {
      Layout.fillWidth: false
      color: Theme.secondaryTextColor
      font: Theme.tipFont
      text: qsTr("Y")
    }

    QfTextField {
      id: yField
      Layout.fillWidth: true
      font: Theme.tipFont
    }

    Label {
      Layout.fillWidth: false
      color: Theme.secondaryTextColor
      font: Theme.tipFont
      text: qsTr("Angle")
    }

    QfTextField {
      id: angleField
      Layout.fillWidth: true
      Layout.columnSpan: 2
      font: Theme.tipFont
    }

    Label {
      Layout.fillWidth: false
      color: Theme.secondaryTextColor
      font: Theme.tipFont
      text: qsTr("Distance")
    }

    QfTextField {
      id: distanceField
      Layout.fillWidth: true
      Layout.columnSpan: 2
      font: Theme.tipFont
    }
  }
}
