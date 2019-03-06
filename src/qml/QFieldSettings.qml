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
  property alias locatorKeepScale: locatorKeepScaleCheckBox.checked
  property alias incrementalRendering: incrementalRenderingCheckBox.checked
  property alias numericalDigitizingInformation: numericalDigitizingInformationCheckBox.checked
  property alias useNativeCamera: useNativeCameraCheckBox.checked

  Settings {
    property alias showScaleBar: showScaleBarCheckBox.checked
    property alias fullScreenIdentifyView: fullScreenIdentifyViewCheckBox.checked
    property alias locatorKeepScale: locatorKeepScaleCheckBox.checked
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
      Layout.fillWidth: true
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
          width: parent.width
          text: qsTr( "Show scalebar" )
        }

        Switch {
          id: fullScreenIdentifyViewCheckBox
          width: parent.width
          text: qsTr( "Show attribute form in full screen" )
        }

        Switch {
          id: locatorKeepScaleCheckBox
          width: parent.width
          text: qsTr( "Keep current scale when triggering search results" )
        }

        Switch {
          id: incrementalRenderingCheckBox
          width: parent.width
          text: qsTr( "Redraw map every 250 ms while rendering" )
        }


        Switch {
          id: numericalDigitizingInformationCheckBox
          width: parent.width
          text: qsTr( "Show numerical information while digitizing" )
          checked: true
        }

        Switch {
          id: useNativeCameraCheckBox
          width: parent.width
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

  Keys.onReleased: {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true
      variableEditor.apply()
    }
  }
}
