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
            anchors.fill: parent.parent

            Label {
                padding: 8 * dp
                leftPadding: 24 * dp
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

            SwitchDelegate {
                id: showScaleBarCheckBox
                Layout.alignment: Qt.AlignTop
                width: 48 * dp
                padding: 10 * dp
                background: Rectangle {
                    width: 68 * dp
                    color: transparent
                }
                indicator: Rectangle {
                    implicitWidth: 48 * dp
                    implicitHeight: 26 * dp
                    x: parent.leftPadding
                    y: ( parent.height + parent.topPadding - 6 ) / 2 - height / 2
                    radius: 13 * dp
                    color: parent.checked ? Theme.mainColor : Theme.lightGray
                    border.color: parent.checked ? Theme.mainColor : Theme.lightGray

                    Rectangle {
                        x: parent.parent.checked ? parent.width - width : 0
                        width: 26 * dp
                        height: 26 * dp
                        radius: 13 * dp
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
                padding: 8 * dp
                leftPadding: 24 * dp
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

            SwitchDelegate {
                id: fullScreenIdentifyViewCheckBox
                Layout.alignment: Qt.AlignTop
                width: 48 * dp
                padding: 10 * dp
                background: Rectangle {
                    width: 68 * dp
                    color: transparent
                }
                indicator: Rectangle {
                    implicitWidth: 48 * dp
                    implicitHeight: 26 * dp
                    x: parent.leftPadding
                    y: ( parent.height + parent.topPadding - 6 ) / 2 - height / 2
                    radius: 13 * dp
                    color: parent.checked ? Theme.mainColor : Theme.lightGray
                    border.color: parent.checked ? Theme.mainColor : Theme.lightGray

                    Rectangle {
                        x: parent.parent.checked ? parent.width - width : 0
                        width: 26 * dp
                        height: 26 * dp
                        radius: 13 * dp
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
                padding: 8 * dp
                leftPadding: 24 * dp
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

            SwitchDelegate {
                id: locatorKeepScaleCheckBox
                Layout.alignment: Qt.AlignTop
                width: 48 * dp
                padding: 10 * dp
                background: Rectangle {
                    width: 68 * dp
                    color: transparent
                }
                indicator: Rectangle {
                    implicitWidth: 48 * dp
                    implicitHeight: 26 * dp
                    x: parent.leftPadding
                    y: ( parent.height + parent.topPadding - 6 ) / 2 - height / 2
                    radius: 13 * dp
                    color: parent.checked ? Theme.mainColor : Theme.lightGray
                    border.color: parent.checked ? Theme.mainColor : Theme.lightGray

                    Rectangle {
                        x: parent.parent.checked ? parent.width - width : 0
                        width: 26 * dp
                        height: 26 * dp
                        radius: 13 * dp
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

            ColumnLayout {

                Label {
                    padding: 8 * dp
                    leftPadding: 24 * dp
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
                    leftPadding: 24 * dp
                    text: qsTr( "When progressive rendering is enabled, the map will be drawn every 250 milliseconds while rendering" )
                    font: Theme.tipFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                }

            }

            SwitchDelegate {
                id: incrementalRenderingCheckBox
                Layout.alignment: Qt.AlignTop
                width: 48 * dp
                padding: 10 * dp
                background: Rectangle {
                    width: 68 * dp
                    color: transparent
                }
                indicator: Rectangle {
                    implicitWidth: 48 * dp
                    implicitHeight: 26 * dp
                    x: parent.leftPadding
                    y: ( parent.height + parent.topPadding - 6 ) / 2 - height / 2
                    radius: 13 * dp
                    color: parent.checked ? Theme.mainColor : Theme.lightGray
                    border.color: parent.checked ? Theme.mainColor : Theme.lightGray

                    Rectangle {
                        x: parent.parent.checked ? parent.width - width : 0
                        width: 26 * dp
                        height: 26 * dp
                        radius: 13 * dp
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

            ColumnLayout {

                Label {
                    padding: 8 * dp
                    leftPadding: 24 * dp
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
                    leftPadding: 24 * dp
                    text: qsTr( "When switched on, coordinate information, such as latitude and longitude, is overlayed onto the canvas while digitizing new features or using the measure tool." )
                    font: Theme.tipFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                }

            }

            SwitchDelegate {
                id: numericalDigitizingInformationCheckBox
                Layout.alignment: Qt.AlignTop
                width: 48 * dp
                padding: 10 * dp
                background: Rectangle {
                    width: 68 * dp
                    color: transparent
                }
                indicator: Rectangle {
                    implicitWidth: 48 * dp
                    implicitHeight: 26 * dp
                    x: parent.leftPadding
                    y: ( parent.height + parent.topPadding - 6 ) / 2 - height / 2
                    radius: 13 * dp
                    color: parent.checked ? Theme.mainColor : Theme.lightGray
                    border.color: parent.checked ? Theme.mainColor : Theme.lightGray

                    Rectangle {
                        x: parent.parent.checked ? parent.width - width : 0
                        width: 26 * dp
                        height: 26 * dp
                        radius: 13 * dp
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

            ColumnLayout {

                Label {
                    padding: 8 * dp
                    leftPadding: 24 * dp
                    text: qsTr( "Use native camera" )
                    font: Theme.defaultFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                    MouseArea {
                        anchors.fill: parent
                        onClicked: useNativeCameraCheckBox.toggle()
                    }
                }

                Label {
                    padding: 8 * dp
                    topPadding: 0
                    leftPadding: 24 * dp
                    text: qsTr( "Warning: native camera function is unstable on recent Android versions." )
                    font: Theme.tipFont

                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Layout.minimumHeight: contentHeight
                }

            }

            SwitchDelegate {
                id: useNativeCameraCheckBox
                Layout.alignment: Qt.AlignTop
                width: 48 * dp
                padding: 10 * dp
                background: Rectangle {
                    width: 68 * dp
                    color: transparent
                }
                indicator: Rectangle {
                    implicitWidth: 48 * dp
                    implicitHeight: 26 * dp
                    x: parent.leftPadding
                    y: ( parent.height + parent.topPadding - 6 ) / 2 - height / 2
                    radius: 13 * dp
                    color: parent.checked ? Theme.mainColor : Theme.lightGray
                    border.color: parent.checked ? Theme.mainColor : Theme.lightGray

                    Rectangle {
                        x: parent.parent.checked ? parent.width - width : 0
                        width: 26 * dp
                        height: 26 * dp
                        radius: 13 * dp
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
        rightPadding: 48*dp
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
