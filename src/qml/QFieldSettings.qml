import QtQuick 2.0

import Qt.labs.settings 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.3

import "js/style.js" as Style

Page {
  signal finished

  property alias showScaleBar: showScaleBarCheckBox.checked
  property alias fullScreenIdentifyView: fullScreenIdentifyViewCheckBox.checked
  property alias incrementalRendering: incrementalRenderingCheckBox.checked
  property alias numericalDigitizingInformation: numericalDigitizingInformationCheckBox.checked

  Settings {
    property alias showScaleBar: showScaleBarCheckBox.checked
    property alias fullScreenIdentifyView: fullScreenIdentifyViewCheckBox.checked
    property alias incrementalRendering: incrementalRenderingCheckBox.checked
    property alias numericalDigitizingInformation: numericalDigitizingInformationCheckBox.checked
  }

  Rectangle {
    color: "white"
    anchors.fill: parent
  }

  ColumnLayout {
    anchors {
      top: toolbar.bottom
      left: parent.left
      right: parent.right
      bottom: parent.bottom
    }

    TabBar {
      id: bar
      anchors { right: parent.right; left: parent.left }
      Layout.preferredHeight: 36*dp

      TabButton {
        height: 36*dp
        text: qsTr("Layout")
        font.pointSize: 14
      }
      TabButton {
        height: 36*dp
        text: qsTr("Global Variables")
        font.pointSize: 14
      }
    }

    StackLayout {
      width: parent.width
      currentIndex: bar.currentIndex

      ColumnLayout {
        Controls.CheckBox {
          id: showScaleBarCheckBox
          anchors { left: parent.left; right: parent.right }
          text: qsTr( "Show scalebar" )
        }

        Controls.CheckBox {
          id: fullScreenIdentifyViewCheckBox
          anchors { left: parent.left; right: parent.right }
          text: qsTr( "Show attribute form in full screen" )
        }

        Controls.CheckBox {
          id: incrementalRenderingCheckBox
          anchors { left: parent.left; right: parent.right }
          text: qsTr( "Redraw map every 250 ms while rendering" )
        }

        Controls.CheckBox {
          id: numericalDigitizingInformationCheckBox
          anchors { left: parent.left; right: parent.right }
          text: qsTr( "Show numerical information while digitizing" )
          checked: true
        }

/*
  // To be used in combination with code in main.cpp
        Switch {
          text: qsTr( "High Dpi support. !EXPERIMENTAL! (requires restart)" )
          font.pointSize: 14

          checked: settings.value( "/HighDpiScaling", false )

          onCheckedChanged: {
            settings.setValue( "/HighDpiScaling", checked )
          }
        }
*/
      }

      Item {
        VariableEditor {
          id: variableEditor
        }
      }
    }
  }

  /** The title toolbar **/
  ToolBar {
    id: toolbar
    anchors {
      left: parent.left
      right: parent.right
    }

    RowLayout {
      spacing: 20
      anchors.fill: parent

      ToolButton {
        id: saveButton

        contentItem: Image {
          fillMode: Image.Pad
          horizontalAlignment: Image.AlignHCenter
          verticalAlignment: Image.AlignVCenter
          source:Style.getThemeIcon( "ic_save_white_24dp" )
        }

        onClicked: {
          variableEditor.apply()
          finished()
        }
      }

      Label {
        id: titleLabel
        text: qsTr( 'QField Settings' )
        font.bold: true
        font.pointSize: 16
        elide: Label.ElideRight
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        Layout.fillWidth: true
      }

      ToolButton {
        id: closeButton

        contentItem: Image {
          fillMode: Image.Pad
          horizontalAlignment: Image.AlignHCenter
          verticalAlignment: Image.AlignVCenter
          source: Style.getThemeIcon( "ic_close_white_24dp" )
        }

        onClicked: {
          variableEditor.reset()
          finished()
        }
      }
    }
  }
}
