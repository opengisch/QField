import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import "."
import "js/style.js" as Style

ToolBar {
  property alias title: titleLabel.text
  height: 48 * dp

  signal cancel
  signal apply

  anchors {
    top: parent.top
    left: parent.left
    right: parent.right
  }

  background: Rectangle {
    color: '#80CC28'
  }

  RowLayout {
    anchors.fill: parent
    Layout.margins: 0

    Button {
      id: applayButton

      Layout.alignment: Qt.AlignTop | Qt.AlignLeft

      width: 48*dp
      height: 48*dp
      clip: true
      bgcolor: "#212121"

      iconSource: Style.getThemeIcon( 'ic_check_white_48dp' )

      onClicked: apply()
    }

    Label {
      id: titleLabel

      font.pointSize: 14
      font.bold: true
      color: "#FFFFFF"
      elide: Label.ElideRight
      horizontalAlignment: Qt.AlignHCenter
      verticalAlignment: Qt.AlignVCenter
      Layout.fillWidth: true
    }

    Button {
      id: cancelButton

      Layout.alignment: Qt.AlignTop | Qt.AlignRight

      width: 48*dp
      height: 48*dp
      clip: true
      bgcolor: "#212121"

      iconSource: Style.getThemeIcon( 'ic_close_white_24dp' )

      onClicked: {
        cancel()
      }
    }
  }
}
