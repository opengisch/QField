import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import Theme 1.0

Page {
  property alias model: table.model
  signal showOpenProjectDialog

  Rectangle {
    id: welcomeBackground
    anchors.fill: parent
    gradient: Gradient  {
      GradientStop  {
        position: 0.0
        color: "#ffdedede"
      }
      GradientStop  {
        position: 0.33
        color: "#00dedede"
      }
    }
  }

  ScrollView {
    padding: 0
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AsNeeded
    contentItem: welcomeGrid
    contentWidth: welcomeGrid.width
    contentHeight: welcomeGrid.height
    anchors.fill: parent
    clip: true

  GridLayout {
    id: welcomeGrid
    columns: 1
    rowSpacing: 4

    width: mainWindow.width

    Image {
      Layout.margins: 6
      Layout.topMargin: 14
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      Layout.preferredWidth: Math.min( 138, mainWindow.height / 4 )
      Layout.preferredHeight: Math.min( 138, mainWindow.height / 4 )
      fillMode: Image.PreserveAspectFit
      smooth: true
      source: "qrc:/images/qfield_logo.svg"
      sourceSize.width: 200
      sourceSize.height: 200
    }

    Text {
      Layout.leftMargin: 6
      Layout.rightMargin: 6
      Layout.topMargin: 2
      Layout.bottomMargin: 2
      id: welcomeText
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      text: ""
      font: Theme.defaultFont
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.WordWrap
      Layout.fillWidth: true
    }

    Rectangle {
      Layout.leftMargin: 6
      Layout.rightMargin: 6
      Layout.topMargin: 2
      Layout.bottomMargin: 2
      Layout.fillWidth: true
      Layout.maximumWidth: 410
      Layout.minimumHeight: welcomeActions.height
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      color: "transparent"

      ColumnLayout {
        id: welcomeActions
        width: parent.width
        spacing: 12

        QfButton {
          id: localProjectButton
          Layout.fillWidth: true
          text: qsTr( "Open local project" )
          onClicked: {
            showOpenProjectDialog()
          }
        }
        QfButton {
          id: cloudProjectButton
          Layout.fillWidth: true
          text: qsTr( "QField Cloud projects, coming soon" )
          enabled: false
        }

        Text {
          id: recentText
          text: qsTr( "Recent Projects" )
          font.pointSize: Theme.tipFont.pointSize
          font.bold: true
          horizontalAlignment: Text.AlignHCenter
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        Rectangle {
            Layout.fillWidth: true
            height: table.height
            color: "transparent"
            border.color: "transparent"
            border.width: 1

        ListView {
          id: table
          ScrollBar.vertical: ScrollBar {}
          flickableDirection: Flickable.VerticalFlick
          boundsBehavior: Flickable.StopAtBounds
          clip: true
          width: parent.width
          height: table.model.rowCount() * ( 80)

          delegate: Rectangle {
            id: rectangle
            property string path: ProjectPath
            width: parent ? parent.width : undefined
            height: line.height
            color: "transparent"

            Row {
              id: line
              Layout.fillWidth: true
              leftPadding: 6
              rightPadding: 10
              topPadding: 9
              bottomPadding: 3
              spacing: 0
              Image {
                id: type
                anchors.verticalCenter: inner.verticalCenter
                source: ProjectType == 0 ? Theme.getThemeIcon('ic_map_green_48dp') : ''
                sourceSize.width: 80
                sourceSize.height: 80
                width: 40
                height: 40
              }
              ColumnLayout {
                id: inner
                width: rectangle.width - type.width - 10
                Text {
                  id: projectTitle
                  topPadding: 5
                  leftPadding: 3
                  text: ProjectTitle
                  font.pointSize: Theme.tipFont.pointSize
                  font.underline: true
                  color: Theme.mainColor
                  wrapMode: Text.WordWrap
                  Layout.fillWidth: true
                }
                Text {
                  id: projectNote
                  leftPadding: 3
                  text: {
                    if (index == 0) {
                        var firstRun = !settings.value( "/QField/FirstRunFlag", false )
                        return !firstRun && qgisProject.fileName == '' ? qsTr( "Last session" ) : ""
                    }
                    else
                    {
                        return ""
                    }
                  }
                  visible: text != ""
                  font.pointSize: Theme.tipFont.pointSize - 2
                  font.italic: true
                  wrapMode: Text.WordWrap
                  Layout.fillWidth: true
                }
              }
            }
          }
          MouseArea {
            property Item pressedItem
            anchors.fill: parent
            onClicked: {
              var item = table.itemAt(mouse.x, mouse.y)
              if (item)
                iface.loadProject(item.path)
            }
            onPressed: {
              var item = table.itemAt(mouse.x, mouse.y)
              if (item) {
                pressedItem = item.children[0].children[1].children[0];
                pressedItem.color = "#5a8725"
              }
            }
            onCanceled: {
              if (pressedItem) {
                pressedItem.color = Theme.mainColor
                pressedItem = null
              }
            }
            onReleased: {
              if (pressedItem) {
                pressedItem.color = Theme.mainColor
                pressedItem = null
              }
            }
            onPressAndHold: {
                var item = table.itemAt(mouse.x, mouse.y)
                if (item) {
                    recentProjectActions.recentProjectPath = item.path;
                    recentProjectActions.popup(mouse.x, mouse.y)
                }
            }
          }

          Menu {
            id: recentProjectActions

            property string recentProjectPath: ''

            title: 'Recent Project Actions'
            width: Math.max(320, mainWindow.width/2)

            MenuItem {
              id: removeProject

              font: Theme.defaultFont
              width: parent.width
              height: visible ? 48: 0
              leftPadding: 10

              text: qsTr( "Remove Recent Project" )
              onTriggered: {
                  iface.removeRecentProject( recentProjectActions.recentProjectPath );
                  model.reloadModel();
              }
            }
          }
        }
        }
      }
    }
  }
  }

  QfToolButton {
    id: currentProjectButton
    visible: false
    anchors {
      top: parent.top
      left: parent.left
    }
    iconSource: Theme.getThemeIcon( 'ic_chevron_left_black_24dp' )
    bgcolor: "transparent"

    onClicked: {
      welcomeScreen.visible = false;
      welcomeScreen.focus = false;
    }
  }

  function adjustWelcomeScreen() {
    if (visible) {
      if (qgisProject.fileName != '') {
        welcomeText.text = " ";
        currentProjectButton.visible = true
      } else {
        var firstRun = !settings.value( "/QField/FirstRunFlag", false )
        if ( firstRun ) {
          welcomeText.text = qsTr( "Welcome to QField. First time using this application? Try out a few demos listed in the recent projects below." )
        } else {
          welcomeText.text = qsTr( "Welcome back to QField." )
        }
        currentProjectButton.visible = false
      }
    }
  }

  Component.onCompleted: {
    adjustWelcomeScreen()
  }

  onVisibleChanged: {
    adjustWelcomeScreen()
  }
}
