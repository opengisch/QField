import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import Qt.labs.settings 1.0

import Theme 1.0
import "."

Page {
  signal finished

  property alias showScaleBar: registry.showScaleBar
  property alias fullScreenIdentifyView: registry.fullScreenIdentifyView
  property alias locatorKeepScale: registry.locatorKeepScale
  property alias incrementalRendering: registry.incrementalRendering
  property alias numericalDigitizingInformation: registry.numericalDigitizingInformation
  property alias nativeCamera: registry.nativeCamera

  Settings {
    id: registry
    property bool showScaleBar
    property bool fullScreenIdentifyView
    property bool locatorKeepScale
    property bool incrementalRendering
    property bool numericalDigitizingInformation
    property bool nativeCamera
  }

  ListModel {
      id: settingsModel
      ListElement {
          title: QT_TR_NOOP( "Show scale bar" )
          settingAlias: "showScaleBar"
      }
      ListElement {
          title: QT_TR_NOOP( "Maximized attribute form" )
          settingAlias: "fullScreenIdentifyView"
      }
      ListElement {
          title: QT_TR_NOOP( "Fixed scale navigation" )
          description: QT_TR_NOOP( "When fixed scale navigation is active, focusing on a search result will pan to the feature. With fixed scale navigation disabled it will pan and zoom to the feature." )
          settingAlias: "locatorKeepScale"
      }
      ListElement {
          title: QT_TR_NOOP( "Progressive rendering" )
          description: QT_TR_NOOP( "When progressive rendering is enabled, the map will be drawn every 250 milliseconds while rendering." )
          settingAlias: "incrementalRendering"
      }
      ListElement {
          title: QT_TR_NOOP( "Show digitizing information" )
          description: QT_TR_NOOP( "When switched on, coordinate information, such as latitude and longitude, is overlayed onto the canvas while digitizing new features or using the measure tool." )
          settingAlias: "numericalDigitizingInformation"
      }
      ListElement {
          title: QT_TR_NOOP( "Use native camera" )
          description: QT_TR_NOOP( "If disabled, QField will use a minimalist internal camera instead of the camera app on the device.<br>Tip: Enable this option and install the open camera app to create geo tagged photos." )
          settingAlias: "nativeCamera"
      }
  }

  Rectangle {
    color: "white"
    anchors.fill: parent
  }

  ColumnLayout {
    anchors {
      top: parent.top
      left: parent.left
      right: parent.right
      bottom: parent.bottom
    }

    TabBar {
      id: bar
      Layout.fillWidth: true
      Layout.preferredHeight: 48*dp

      TabButton {
        height: 48*dp
        text: qsTr("General")
        font: Theme.defaultFont
        anchors.verticalCenter : parent.verticalCenter
      }
      TabButton {
        height: 48*dp
        text: qsTr("Variables")
        font: Theme.defaultFont
        anchors.verticalCenter : parent.verticalCenter
      }
    }

    StackLayout {
      width: mainWindow.width
      currentIndex: bar.currentIndex


      ListView {
//        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
//        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        flickableDirection: Flickable.VerticalFlick
        boundsBehavior: Flickable.StopAtBounds
        width: mainWindow.width
        clip: true
        //rowSpacing: 10 * dp
        ScrollBar.vertical: ScrollBar {}

        model: settingsModel

        delegate: Row {
          width: parent.width

          Column {
            width: parent.width - toggle.width
            Label {
              width: parent.width
              padding: 8 * dp
              leftPadding: 22 * dp
              text: title
              font: Theme.defaultFont
              wrapMode: Text.WordWrap
              MouseArea {
                  anchors.fill: parent
                  onClicked: toggle.toggle()
              }
            }
            Label {
              width: parent.width
              visible: !!description
              padding: !!description ? 8 * dp : 0
              topPadding: 0
              leftPadding: 22 * dp
              text: description
              font: Theme.tipFont
              color: Theme.gray
              wrapMode: Text.WordWrap
            }
          }

          QfSwitch {
            id: toggle
            width: implicitContentWidth
            checked: registry[settingAlias]
            Layout.alignment: Qt.AlignTop | Qt.AlignRight
            onCheckedChanged: registry[settingAlias] = checked
          }
        }
      }

      Item {
        VariableEditor {
          id: variableEditor
          anchors.fill: parent
          anchors.margins: 4 * dp
        }
      }
    }
  }
  header: PageHeader {
      title: qsTr("QField Settings")

      showApplyButton: true
      showCancelButton: false

      onFinished: {
          parent.finished()
          variableEditor.apply()
      }
    }

  Keys.onReleased: {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true
      variableEditor.apply()
    }
  }
}
