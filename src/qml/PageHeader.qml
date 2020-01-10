import QtQuick 2.12
import QtQuick.Controls 2.11
import QtQuick.Layouts 1.12
import "."
import Theme 1.0

ToolBar {
  property alias title: titleLabel.text
  property alias showApplyButton: applyButton.visible
  property alias showCancelButton: cancelButton.visible

  height: 48 * dp

  signal cancel
  signal apply
  signal finished

  anchors {
    top: parent.top
    left: parent.left
    right: parent.right
  }

  background: Rectangle {
    color: Theme.mainColor
  }

  RowLayout {
    anchors.fill: parent

    Layout.margins: 0

    Button {
      id: applyButton

      Layout.alignment: Qt.AlignTop | Qt.AlignLeft

      width: 48*dp
      height: 48*dp
      clip: true
      bgcolor: Theme.darkGray

      iconSource: Theme.getThemeIcon( 'ic_check_white_48dp' )

      onClicked:
      {
        apply()
        finished()
      }
    }

    Label {
      id: titleLabel
      leftPadding: !showApplyButton && showCancelButton ? 48 * dp : 0
      rightPadding: showApplyButton && !showCancelButton ? 48 * dp : 0
      font: Theme.strongFont
      color: Theme.light
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
      bgcolor: Theme.darkGray

      iconSource: Theme.getThemeIcon( 'ic_close_white_24dp' )

      onClicked: {
        cancel()
        finished()
      }
    }
  }
}
