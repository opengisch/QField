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
  property alias nativeCamera: nativeCameraCheckBox.checked

  Settings {
    property alias showScaleBar: showScaleBarCheckBox.checked
    property alias fullScreenIdentifyView: fullScreenIdentifyViewCheckBox.checked
    property alias locatorKeepScale: locatorKeepScaleCheckBox.checked
    property alias incrementalRendering: incrementalRenderingCheckBox.checked
    property alias numericalDigitizingInformation: numericalDigitizingInformationCheckBox.checked
    property alias nativeCamera: nativeCameraCheckBox.checked
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

      ScrollView {
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        contentWidth: settingsGrid.width
        contentHeight: settingsGrid.height
        clip: true

        GridLayout {
            id: settingsGrid
            columns: 2
            columnSpacing: 2 * dp
            rowSpacing: 10 * dp
            width: mainWindow.width

            Label {
                padding: 8 * dp
                leftPadding: 22 * dp
                text: qsTr( "Show scale bar" )
                font: Theme.defaultFont

                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: contentHeight
                MouseArea {
                    anchors.fill: parent
                    onClicked: showScaleBarCheckBox.toggle()
                }
            }

            QfSwitch {
                id: showScaleBarCheckBox
                Layout.alignment: Qt.AlignTop
            }

            Label {
                padding: 8 * dp
                leftPadding: 22 * dp
                text: qsTr( "Maximized attribute form" )
                font: Theme.defaultFont

                wrapMode: Text.WordWrap
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.minimumHeight: contentHeight
                MouseArea {
                    anchors.fill: parent
                    onClicked: fullScreenIdentifyViewCheckBox.toggle()
                }
            }

            QfSwitch {
                id: fullScreenIdentifyViewCheckBox
                Layout.alignment: Qt.AlignTop
            }

            ColumnLayout {

                Label {
                    padding: 8 * dp
                    leftPadding: 22 * dp
                    text: qsTr( "Fixed scale navigation" )
                    font: Theme.defaultFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    MouseArea {
                        anchors.fill: parent
                        onClicked: locatorKeepScaleCheckBox.toggle()
                    }
                }

                Label {
                    padding: 8 * dp
                    topPadding: 0
                    leftPadding: 22 * dp
                    text: qsTr( "When fixed scale navigation is active, focusing on a search result will pan to the feature. With fixed scale navigation disabled it will pan and zoom to the feature." )
                    font: Theme.tipFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                }

            }

            QfSwitch {
                id: locatorKeepScaleCheckBox
                Layout.alignment: Qt.AlignTop
            }

            ColumnLayout {

                Label {
                    padding: 8 * dp
                    leftPadding: 22 * dp
                    text: qsTr( "Progressive rendering" )
                    font: Theme.defaultFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    MouseArea {
                        anchors.fill: parent
                        onClicked: incrementalRenderingCheckBox.toggle()
                    }
                }

                Label {
                    padding: 8 * dp
                    topPadding: 0
                    leftPadding: 22 * dp
                    text: qsTr( "When progressive rendering is enabled, the map will be drawn every 250 milliseconds while rendering." )
                    font: Theme.tipFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                }

            }

            QfSwitch {
                id: incrementalRenderingCheckBox
                Layout.alignment: Qt.AlignTop
            }

            ColumnLayout {

                Label {
                    padding: 8 * dp
                    leftPadding: 22 * dp
                    text: qsTr( "Show digitizing information" )
                    font: Theme.defaultFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    MouseArea {
                        anchors.fill: parent
                        onClicked: numericalDigitizingInformationCheckBox.toggle()
                    }
                }

                Label {
                    padding: 8 * dp
                    topPadding: 0
                    leftPadding: 22 * dp
                    text: qsTr( "When switched on, coordinate information, such as latitude and longitude, is overlayed onto the canvas while digitizing new features or using the measure tool." )
                    font: Theme.tipFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                }

            }

            QfSwitch {
                id: numericalDigitizingInformationCheckBox
                Layout.alignment: Qt.AlignTop
            }

            ColumnLayout {

                Label {
                    padding: 8 * dp
                    leftPadding: 22 * dp
                    text: qsTr( "Use native camera" )
                    font: Theme.defaultFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    MouseArea {
                        anchors.fill: parent
                        onClicked: nativeCameraCheckBox.toggle()
                    }
                }

                Label {
                    padding: 8 * dp
                    topPadding: 0
                    leftPadding: 22 * dp
                    text: qsTr( "If disabled, QField will use a minimalist internal camera instead of the camera app on the device.<br>Tip: Enable this option and install the open camera app to create geo tagged photos." )
                    font: Theme.tipFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                }

            }

            QfSwitch {
                id: nativeCameraCheckBox
                checked: true
                Layout.alignment: Qt.AlignTop
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
      }

      Item {
        VariableEditor {
          id: variableEditor
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
