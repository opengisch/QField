import QtQuick 2.6

import Qt.labs.settings 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.3

import "js/style.js" as Style

import "."

Page {
  signal finished

  property alias showScaleBar: showScaleBarCheckBox.checked
  property alias fullScreenIdentifyView: fullScreenIdentifyViewCheckBox.checked
  property alias incrementalRendering: incrementalRenderingCheckBox.checked
  property alias numericalDigitizingInformation: numericalDigitizingInformationCheckBox.checked
  property alias useNativeCamera: useNativeCameraCheckBox.checked

  Settings {
    property alias showScaleBar: showScaleBarCheckBox.checked
    property alias fullScreenIdentifyView: fullScreenIdentifyViewCheckBox.checked
    property alias incrementalRendering: incrementalRenderingCheckBox.checked
    property alias numericalDigitizingInformation: numericalDigitizingInformationCheckBox.checked
    property alias useNativeCamera: useNativeCameraCheckBox.checked
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
        spacing: 2 * dp

        Switch{
          id: showScaleBarCheckBox
          anchors { left: parent.left; right: parent.right }
          text: qsTr( "Show scalebar" )
        }

        Switch {
          id: fullScreenIdentifyViewCheckBox
          anchors { left: parent.left; right: parent.right }
          text: qsTr( "Show attribute form in full screen" )
        }

        Switch {
          id: incrementalRenderingCheckBox
          anchors { left: parent.left; right: parent.right }
          text: qsTr( "Redraw map every 250 ms while rendering" )
        }

        Switch {
          id: numericalDigitizingInformationCheckBox
          anchors { left: parent.left; right: parent.right }
          text: qsTr( "Show numerical information while digitizing" )
          checked: true
        }

        Switch {
          id: useNativeCameraCheckBox
          anchors { left: parent.left; right: parent.right }
          text: qsTr( "Use native camera function (unstable on recent Android versions)" )
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
    }
  }
}
