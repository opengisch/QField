import QtQuick 2.14
import QtQuick.Controls 2.14
import Theme 1.0
import org.qfield 1.0
import org.qgis 1.0
import "."

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
