import QtQuick 2.6

import Qt.labs.settings 1.0
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.4

import Theme 1.0

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
      Layout.preferredHeight: 48*dp

      TabButton {
        height: 48*dp
        text: qsTr("Layout")
        font: Theme.defaultFont
        anchors.verticalCenter : parent.verticalCenter
      }
      TabButton {
        height: 48*dp
        text: qsTr("Global Variables")
        font: Theme.defaultFont
        anchors.verticalCenter : parent.verticalCenter
      }
    }

    StackLayout {
      width: parent.width
      currentIndex: bar.currentIndex

      ColumnLayout {
        spacing: 2 * dp
        anchors.fill: parent

        SwitchDelegate {
          id: showScaleBarCheckBox
          width: parent.width
          padding: 25 * dp
          topPadding: 35 * dp
          text: qsTr( "Show scale bar" )
          contentItem: Text {
                    text: parent.text
                    font: Theme.defaultFont
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    rightPadding: parent.indicator.width + parent.spacing
                }
          background: Rectangle {
              implicitWidth: parent.width
              color: transparent
          }
          indicator: Rectangle {
              implicitWidth: 50 * dp
              implicitHeight: 28 * dp
              x: parent.width - width - parent.rightPadding
              y: parent.height / 2 - height / 2 + parent.topPadding / 2
              radius: 14 * dp
              color: parent.checked ? Theme.mainColor : Theme.lightGray
              border.color: parent.checked ? Theme.mainColor : Theme.lightGray

              Rectangle {
                  x: parent.parent.checked ? parent.width - width : 0
                  width: 28 * dp
                  height: 28 * dp
                  radius: 14 * dp
                  color: parent.parent.down ? Theme.lightGray : Theme.light
                  border.color: parent.parent.checked ? Theme.mainColor : Theme.lightGray
                  Behavior on x {
                    PropertyAnimation {
                      duration: 150
                      easing.type: Easing.Linear
                    }
                  }
              }
          }
        }

        SwitchDelegate {
          id: fullScreenIdentifyViewCheckBox
          width: parent.width
          padding: 25 * dp
          topPadding: 35 * dp
          text: qsTr( "Maximized attribute form" )
          contentItem: Text {
                    text: parent.text
                    font: Theme.defaultFont
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    rightPadding: parent.indicator.width + parent.spacing
                }
          background: Rectangle {
              implicitWidth: parent.width
              color: transparent
          }
          indicator: Rectangle {
              implicitWidth: 50 * dp
              implicitHeight: 28 * dp
              x: parent.width - width - parent.rightPadding
              y: parent.height / 2 - height / 2 + parent.topPadding / 2
              radius: 14 * dp
              color: parent.checked ? Theme.mainColor : Theme.lightGray
              border.color: parent.checked ? Theme.mainColor : Theme.lightGray

              Rectangle {
                  x: parent.parent.checked ? parent.width - width : 0
                  width: 28 * dp
                  height: 28 * dp
                  radius: 14 * dp
                  color: parent.parent.down ? Theme.lightGray : Theme.light
                  border.color: parent.parent.checked ? Theme.mainColor : Theme.lightGray
                  Behavior on x {
                    PropertyAnimation {
                      duration: 150
                      easing.type: Easing.Linear
                    }
                  }
              }
          }
        }

        SwitchDelegate {
          id: locatorKeepScaleCheckBox
          width: parent.width
          padding: 25 * dp
          topPadding: 35 * dp
          text: qsTr( "Fixed scale navigation" )
          contentItem: Text {
                    text: parent.text
                    font: Theme.defaultFont
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    rightPadding: parent.indicator.width + parent.spacing
                }
          background: Rectangle {
              implicitWidth: parent.width
              color: transparent
          }
          indicator: Rectangle {
              implicitWidth: 50 * dp
              implicitHeight: 28 * dp
              x: parent.width - width - parent.rightPadding
              y: parent.height / 2 - height / 2 + parent.topPadding / 2
              radius: 14 * dp
              color: parent.checked ? Theme.mainColor : Theme.lightGray
              border.color: parent.checked ? Theme.mainColor : Theme.lightGray

              Rectangle {
                  x: parent.parent.checked ? parent.width - width : 0
                  width: 28 * dp
                  height: 28 * dp
                  radius: 14 * dp
                  color: parent.parent.down ? Theme.lightGray : Theme.light
                  border.color: parent.parent.checked ? Theme.mainColor : Theme.lightGray
                  Behavior on x {
                    PropertyAnimation {
                      duration: 150
                      easing.type: Easing.Linear
                    }
                  }
              }
          }
        }

        SwitchDelegate {
          id: incrementalRenderingCheckBox
          width: parent.width
          padding: 25 * dp
          topPadding: 35 * dp
          text: qsTr( "Progressive rendering" )
          contentItem: Text {
                    text: parent.text
                    font: Theme.defaultFont
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    rightPadding: parent.indicator.width + parent.spacing
                }
          background: Rectangle {
              implicitWidth: parent.width
              color: transparent
          }
          indicator: Rectangle {
              implicitWidth: 50 * dp
              implicitHeight: 28 * dp
              x: parent.width - width - parent.rightPadding
              y: parent.height / 2 - height / 2 + parent.topPadding / 2
              radius: 14 * dp
              color: parent.checked ? Theme.mainColor : Theme.lightGray
              border.color: parent.checked ? Theme.mainColor : Theme.lightGray

              Rectangle {
                  x: parent.parent.checked ? parent.width - width : 0
                  width: 28 * dp
                  height: 28 * dp
                  radius: 14 * dp
                  color: parent.parent.down ? Theme.lightGray : Theme.light
                  border.color: parent.parent.checked ? Theme.mainColor : Theme.lightGray
                  Behavior on x {
                    PropertyAnimation {
                      duration: 150
                      easing.type: Easing.Linear
                    }
                  }
              }
          }
        }
        Label {
          leftPadding: 30 * dp
          rightPadding: ( 30 + 54 ) * dp
          bottomPadding: 55 * dp
          text: qsTr( "When progressive rendering is enabled, the map will be drawn every 250 milliseconds while rendering" )
          font.pointSize: 12
          font.italic: true

          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.fillHeight: true
          Layout.minimumHeight: contentHeight
          Layout.maximumHeight: contentHeight
        }


        SwitchDelegate {
          id: numericalDigitizingInformationCheckBox
          width: parent.width
          padding: 25 * dp
          topPadding: 35 * dp
          text: qsTr( "Show digitizing information" )
          contentItem: Text {
                    text: parent.text
                    font: Theme.defaultFont
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    rightPadding: parent.indicator.width + parent.spacing
                }
          background: Rectangle {
              implicitWidth: parent.width
              color: transparent
          }
          checked: true
          indicator: Rectangle {
              implicitWidth: 50 * dp
              implicitHeight: 28 * dp
              x: parent.width - width - parent.rightPadding
              y: parent.height / 2 - height / 2 + parent.topPadding / 2
              radius: 14 * dp
              color: parent.checked ? Theme.mainColor : Theme.lightGray
              border.color: parent.checked ? Theme.mainColor : Theme.lightGray

              Rectangle {
                  x: parent.parent.checked ? parent.width - width : 0
                  width: 28 * dp
                  height: 28 * dp
                  radius: 14 * dp
                  color: parent.parent.down ? Theme.lightGray : Theme.light
                  border.color: parent.parent.checked ? Theme.mainColor : Theme.lightGray
                  Behavior on x {
                    PropertyAnimation {
                      duration: 150
                      easing.type: Easing.Linear
                    }
                  }
              }
          }
        }
        Label {
          leftPadding: 30 * dp
          rightPadding: ( 30 + 54 ) * dp
          bottomPadding: 55 * dp
          text: qsTr( "When switched on, coordinate information, such as latitude and longitude, is overlayed onto the canvas while digitizing new features or using the measure tool." )
          font.pointSize: 12
          font.italic: true

          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.fillHeight: true
          Layout.minimumHeight: contentHeight
          Layout.maximumHeight: contentHeight
        }

        SwitchDelegate {
          id: useNativeCameraCheckBox
          width: parent.width
          padding: 25 * dp
          topPadding: 35 * dp
          text: qsTr( "Use native camera" )
          contentItem: Text {
                    text: parent.text
                    font: Theme.defaultFont
                    verticalAlignment: Text.AlignVCenter
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    rightPadding: parent.indicator.width + parent.spacing
                }
          background: Rectangle {
              implicitWidth: parent.width
              color: transparent
          }
          indicator: Rectangle {
              implicitWidth: 50 * dp
              implicitHeight: 28 * dp
              x: parent.width - width - parent.rightPadding
              y: parent.height / 2 - height / 2 + parent.topPadding / 2
              radius: 14 * dp
              color: parent.checked ? Theme.mainColor : Theme.lightGray
              border.color: parent.checked ? Theme.mainColor : Theme.lightGray

              Rectangle {
                  x: parent.parent.checked ? parent.width - width : 0
                  width: 28 * dp
                  height: 28 * dp
                  radius: 14 * dp
                  color: parent.parent.down ? Theme.lightGray : Theme.light
                  border.color: parent.parent.checked ? Theme.mainColor : Theme.lightGray
                  Behavior on x {
                    PropertyAnimation {
                      duration: 150
                      easing.type: Easing.Linear
                    }
                  }
              }
          }
        }
        Label {
          leftPadding: 30 * dp
          rightPadding: ( 30 + 54 ) * dp
          bottomPadding: 55 * dp
          text: qsTr( "Warning: native camera function is unstable on recent Android versions" )
          font.pointSize: 12
          font.italic: true

          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.fillHeight: true
          Layout.minimumHeight: contentHeight
          Layout.maximumHeight: contentHeight
        }


/*
  // To be used in combination with code in main.cpp
        Switch {
          text: qsTr( "High Dpi support. !EXPERIMENTAL! (requires restart)" )
          font: Theme.defaultFont

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
    height: 48 * dp
    visible: true

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
        id: enterButton

        Layout.alignment: Qt.AlignTop | Qt.AlignLeft

        visible: form.state === 'Add' || form.state === 'Edit'
        width: 48*dp
        height: 48*dp
        clip: true
        bgcolor: Theme.darkGray

        iconSource: Theme.getThemeIcon( 'ic_check_white_48dp' )

        onClicked: finished()
      }

      Label {
        id: titleLabel

        text:  qsTr( 'QField Settings' )
        font: Theme.strongFont
        color: "#FFFFFF"
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
