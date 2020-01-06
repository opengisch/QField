import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.2
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
    padding: 0 * dp
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
    rowSpacing: 4 * dp

    width: mainWindow.width

    Image {
      Layout.margins: 6 * dp
      Layout.topMargin: 14 * dp
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      Layout.preferredWidth: Math.min( 138 * dp, mainWindow.height / 4 )
      Layout.preferredHeight: Math.min( 138 * dp, mainWindow.height / 4 )
      fillMode: Image.PreserveAspectFit
      smooth: true
      source: "qrc:/images/qfield-logo.svg"
      sourceSize.width: 200 * dp
      sourceSize.height: 200 * dp
    }

    Text {
      Layout.leftMargin: 6 * dp
      Layout.rightMargin: 6 * dp
      Layout.topMargin: 2 * dp
      Layout.bottomMargin: 2 * dp
      id: welcomeText
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      text: ""
      font: Theme.defaultFont
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.WordWrap
      Layout.fillWidth: true
    }

    Rectangle {
      Layout.leftMargin: 6 * dp
      Layout.rightMargin: 6 * dp
      Layout.topMargin: 2 * dp
      Layout.bottomMargin: 2 * dp
      Layout.fillWidth: true
      Layout.maximumWidth: 410 * dp
      Layout.minimumHeight: welcomeActions.height
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      color: "transparent"

      ColumnLayout {
        id: welcomeActions
        width: parent.width
        spacing: 12 * dp

        Button {
          id: localProjectButton
          padding: 8 * dp
          topInset: 2 * dp
          bottomInset: 2 * dp
          Layout.fillWidth: true
          text: qsTr( "Open local project" )
          background: Rectangle {
              anchors.fill: parent
              color: !parent.enabled ? Theme.lightGray : parent.down ? "#5a8725" : Theme.mainColor
              radius: 12 * dp
              Behavior on color {
                PropertyAnimation {
                  duration: 25
                  easing.type: Easing.InQuart
                }
              }
          }
          contentItem: Text {
              text: parent.text
              font: Theme.tipFont
              color: "white"
              horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
              elide: Text.ElideRight
          }
          onClicked: {
            showOpenProjectDialog()
          }
        }
        Button {
          id: cloudProjectButton
          padding: 8 * dp
          topInset: 2 * dp
          bottomInset: 2 * dp
          Layout.fillWidth: true
          text: qsTr( "QField Cloud projects, coming soon" )
          enabled: false
          background: Rectangle {
              anchors.fill: parent
              color: !parent.enabled ? Theme.lightGray : parent.down ? "#5a8725" : Theme.mainColor
              radius: 12 * dp
              Behavior on color {
                PropertyAnimation {
                  duration: 25
                  easing.type: Easing.InQuart
                }
              }
          }
          contentItem: Text {
              text: parent.text
              font: Theme.tipFont
              color: "white"
              horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
              elide: Text.ElideRight
          }
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
          height: table.model.rowCount() * ( 80 * dp )

          delegate: Rectangle {
            id: rectangle
            property string path: ProjectPath
            width: parent.width
            height: line.height
            color: "transparent"

            Row {
              id: line
              Layout.fillWidth: true
              leftPadding: 6 * dp
              rightPadding: 10 * dp
              topPadding: 9 * dp
              bottomPadding: 3 * dp
              spacing: 0
              Image {
                id: type
                anchors.verticalCenter: inner.verticalCenter
                source: ProjectType == 0 ? Theme.getThemeIcon('ic_map_green_48dp') : ''
                sourceSize.width: 80 * dp
                sourceSize.height: 80 * dp
                width: 40 * dp
                height: 40 * dp
              }
              ColumnLayout {
                id: inner
                width: rectangle.width - type.width - 10 * dp
                Text {
                  id: projectTitle
                  topPadding: 5 * dp
                  leftPadding: 3 * dp
                  text: ProjectTitle
                  font.pointSize: Theme.tipFont.pointSize
                  font.underline: true
                  color: Theme.mainColor
                  wrapMode: Text.WordWrap
                  Layout.fillWidth: true
                }
                Text {
                  id: projectNote
                  leftPadding: 3 * dp
                  text: {
                    if (index == 0) {
                        var firstRun = !settings.value( "/QField/FirstRunFlag", false )
                        return !firstRun && qgisProject.fileName == '' ? qsTr( "Last session" ) : ""
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
          }
        }
        }
      }
    }
  }
  }

  ToolButton {
    id: currentProjectButton
    height: 56 * dp
    width: 56 * dp
    visible: false
    anchors {
      top: parent.top
      left: parent.left
    }
    contentItem: Rectangle {
      anchors.fill: parent
      color: "transparent"
      Image {
        anchors.fill: parent
        fillMode: Image.Pad
        horizontalAlignment: Image.AlignHCenter
        verticalAlignment: Image.AlignVCenter
        source: Theme.getThemeIcon( 'ic_chevron_left_black_24dp' )
      }
    }

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
