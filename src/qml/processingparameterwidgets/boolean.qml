import QtQuick
import QtQuick.Controls
import Theme
import org.qfield
import org.qgis

ProcessingParameterWidgetBase {
  id: booleanItem

  height: childrenRect.height

  Row {
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top
    spacing: 5

    Label {
      id: labelValue

      anchors.verticalCenter: parent.verticalCenter
      width: parent.width - checkBox.width
      font.pointSize: Theme.defaultFont.pointSize
      font.bold: Theme.defaultFont.bold

      text: value ? qsTr("True") : qsTr("False")
    }

    QfSwitch {
      id: checkBox

      anchors.verticalCenter: parent.verticalCenter
      width: implicitContentWidth
      small: true

      checked: value
    }
  }

  MouseArea {
    id: checkArea
    anchors.fill: parent

    onClicked: {
      valueChangeRequested(!value);
    }
  }
}
