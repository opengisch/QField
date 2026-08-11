import QtQuick
import QtQuick.Controls
import org.qfield.gui
import org.qfield.core
import org.qgis

QfProcessingParameterWidgetBase {
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
      font.pointSize: QfTheme.defaultFont.pointSize
      font.bold: QfTheme.defaultFont.bold

      text: value ? qsTr("True") : qsTr("False")
    }

    QfSwitch {
      id: checkBox

      anchors.verticalCenter: parent.verticalCenter
      width: implicitContentWidth

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
