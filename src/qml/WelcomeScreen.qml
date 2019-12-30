import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.12
import Theme 1.0

Page {
  signal showOpenProjectDialog

  padding: 6 * dp

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

  GridLayout {
    id: welcomeGrid
    columns: 1
    rowSpacing: 10 * dp
    width: mainWindow.width
    anchors.fill: parent

    Image {
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      Layout.preferredWidth: 138 * dp
      Layout.preferredHeight: 138 * dp
      fillMode: Image.PreserveAspectFit
      smooth: true
      source: "qrc:/images/qfield-logo.svg"
      sourceSize.width: 200 * dp
      sourceSize.height: 200 * dp
    }

    Text {
      id: welcomeText
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      text: ""
      font: Theme.defaultFont
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.WordWrap
      Layout.fillWidth: true
    }

    Rectangle {
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      Layout.maximumWidth: 380 * dp
      Layout.maximumHeight: 460 * dp
      color: "transparent"

      ScrollView {
        padding: 5 * dp
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        contentWidth: welcomeActions.width
        contentHeight: welcomeActions.height
        anchors.fill: parent
        clip: true

      ColumnLayout {
        id: welcomeActions
        width: parent.parent.width
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
          text: qsTr( "Open cloud-stored project" )
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
          Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
          text: qsTr( "Recent Projects" )
          font: Theme.strongFont
          horizontalAlignment: Text.AlignHCenter
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        Text {
          property string path: ""
          id: recent0
          Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
          text: ""
          font.pointSize: Theme.tipFont.pointSize
          font.underline: true
          color: Theme.mainColor
          horizontalAlignment: Text.AlignHCenter
          wrapMode: Text.WordWrap
          Layout.fillWidth: true

          MouseArea {
            anchors.fill: parent
            onClicked: { if (parent.path != '') iface.loadProject(parent.path) }
          }
        }

        Text {
          property string path: ""
          id: recent1
          Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
          text: ""
          font.pointSize: Theme.tipFont.pointSize
          font.underline: true
          color: Theme.mainColor
          horizontalAlignment: Text.AlignHCenter
          wrapMode: Text.WordWrap
          Layout.fillWidth: true

          MouseArea {
            anchors.fill: parent
            onClicked: { if (parent.path != '') iface.loadProject(parent.path) }
          }
        }

        Text {
          property string path: ""
          id: recent2
          Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
          text: ""
          font.pointSize: Theme.tipFont.pointSize
          font.underline: true
          color: Theme.mainColor
          horizontalAlignment: Text.AlignHCenter
          wrapMode: Text.WordWrap
          Layout.fillWidth: true

          MouseArea {
            anchors.fill: parent
            onClicked: { if (parent.path != '') iface.loadProject(parent.path) }
          }
        }

        Text {
          property string path: ""
          id: recent3
          Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
          text: ""
          font.pointSize: Theme.tipFont.pointSize
          font.underline: true
          color: Theme.mainColor
          horizontalAlignment: Text.AlignHCenter
          wrapMode: Text.WordWrap
          Layout.fillWidth: true

          MouseArea {
            anchors.fill: parent
            onClicked: { if (parent.path != '') iface.loadProject(parent.path) }
          }
        }

        Text {
          property string path: ""
          id: recent4
          Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
          text: ""
          font.pointSize: Theme.tipFont.pointSize
          font.underline: true
          color: Theme.mainColor
          horizontalAlignment: Text.AlignHCenter
          wrapMode: Text.WordWrap
          Layout.fillWidth: true

          MouseArea {
            anchors.fill: parent
            onClicked: { if (parent.path != '') iface.loadProject(parent.path) }
          }
        }

        Item {
            Layout.fillHeight: true
        }
      }
      }
    }
  }

  function adjustWelcomeScreen() {
    var recentProjects = iface.recentProjects()
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

    if (recentProjects.length > 0) {
      recent0.text = recentProjects[0].split('}|{')[0]
      recent0.path = recentProjects[0].split('}|{')[1]
      recent0.visible = true
    } else {
      recent0.visible = false
    }
    if (recentProjects.length > 1) {
      recent1.text = recentProjects[1].split('}|{')[0]
      recent1.path = recentProjects[1].split('}|{')[1]
      recent1.visible = true
    } else {
      recent1.visible = false
    }
    if (recentProjects.length > 2) {
      recent2.text = recentProjects[2].split('}|{')[0]
      recent2.path = recentProjects[2].split('}|{')[1]
      recent2.visible = true
    } else {
      recent2.visible = false
    }
    if (recentProjects.length > 3) {
      recent3.text = recentProjects[3].split('}|{')[0]
      recent3.path = recentProjects[3].split('}|{')[1]
      recent3.visible = true
    } else {
      recent3.visible = false
    }
    if (recentProjects.length > 4) {
      recent4.text = recentProjects[4].split('}|{')[0]
      recent4.path = recentProjects[4].split('}|{')[1]
      recent4.visible = true
    } else {
      recent4.visible = false
    }
  }

  Component.onCompleted: {
    adjustWelcomeScreen()
  }

  onVisibleChanged: {
    adjustWelcomeScreen()
  }
}
