import QtQuick 2.0

import Qt.labs.settings 1.0
import QtQuick.Controls 2.0 as Controls
import QtQuick.Layouts 1.3

import "js/style.js" as Style

Item {
  signal finished

  property alias showScaleBar: showScaleBarCheckBox.checked

  Settings {
    property alias showScaleBar: showScaleBarCheckBox.checked
  }

  Rectangle {
    color: "white"
    anchors.fill: parent
  }

  ColumnLayout {
    anchors { left: parent.left; right: parent.right; bottom: parent.bottom; top: toolbar.bottom; leftMargin: 4 * dp }

    Controls.TabBar {
      id: bar
      anchors { right: parent.right; left: parent.left }

      Controls.TabButton {
        text: qsTr("Layout")
      }
      Controls.TabButton {
        text: qsTr("Global Variables")
      }
    }

    StackLayout {
      width: parent.width
      currentIndex: bar.currentIndex

      Item {
        Controls.CheckBox {
          id: showScaleBarCheckBox
          anchors { left: parent.left; right: parent.right }
          text: qsTr( "Show Scalebar" )
          checked: true

          onCheckedChanged: {
            QFieldSettingsManager.scaleBarVisible = checked
          }
        }
      }

      Item {
        VariableEditor {
          id: variableEditor

          anchors { left: parent.left; right: parent.right; bottom: parent.bottom }
        }
      }
    }
  }

  /** The title toolbar **/
  Rectangle {
    id: toolbar

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top

    height: visible ? 48 * dp : 0

    Button {
      id: saveButton
      anchors.right: parent.right

      width: 48 * dp
      height: 48 * dp

      iconSource: Style.getThemeIcon( "ic_save_white_24dp" )

      onClicked: {
        variableEditor.apply()
        finished()
      }
    }

    Rectangle {
      color: "#FFEB3B"

      anchors.fill: titleLabel
    }

    Controls.Label {
      id: titleLabel
      anchors { right: saveButton.left; left: closeButton.right }
      height: closeButton.height
      text: qsTr( 'QField Settings' )
      font.bold: true
      horizontalAlignment: Text.AlignHCenter
    }

    Button {
      id: closeButton
      anchors.left: parent.left

      width: 48 * dp
      height: 48 * dp

      iconSource: Style.getThemeIcon( "ic_close_white_24dp" )

      onClicked: {
        variableEditor.reset()
        finished()
      }
    }
  }
}
