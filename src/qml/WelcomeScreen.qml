import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.12
import Theme 1.0

Page {
  signal showOpenProjectDialog
  signal loadLastProject

  padding: 6 * dp

  GridLayout {
    id: welcomeGrid
    columns: 1
    rowSpacing: 10 * dp
    width: mainWindow.width
    anchors.fill: parent

    Image {
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      source: "qrc:/images/qfield-logo.svg"
      fillMode: Image.PreserveAspectFit
    }

    Text {
      id: welcomeText
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      text: qsTr( "Welcome back to QField." )
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
      Layout.maximumHeight: 380 * dp
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
        anchors.fill: parent.parent
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
              color: !parent.enabled ? Theme.lightGray : Theme.mainColor
              radius: 12 * dp
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
            settings.setValue( "/QField/FirstRunFlag", false )
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
              color: !parent.enabled ? Theme.lightGray : Theme.mainColor
              radius: 12 * dp
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
        Button {
          id: lastProjectButton
          padding: 8 * dp
          topInset: 2 * dp
          bottomInset: 2 * dp
          Layout.fillWidth: true
          text: qsTr( "Re-open last project" )
          background: Rectangle {
              anchors.fill: parent
              color: !parent.enabled ? Theme.lightGray : Theme.mainColor
              radius: 12 * dp
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
            if (qgisProject.fileName != '') {
              welcomeScreen.visible = false;
            } else {
              loadLastProject()
            }
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
    if (visible) {
      if (qgisProject.fileName != '') {
        welcomeText.text = " ";
        lastProjectButton.text = qsTr( "Return to current project" )
        lastProjectButton.visible = true
      } else {
        if ( !settings.value( "/QField/FirstRunFlag", false ) ) {
          welcomeText.text = qsTr( "Welcome to QField. First time using this application? Try a demo project by clicking on the <i>Open local project</i> button." )
        } else {
          welcomeText.text = qsTr( "Welcome back to QField" )
        }
        lastProjectButton.text = qsTr( "Re-open last project" )
        lastProjectButton.visible = settings.value("/qgis/project/lastProjectFile", '') != ''
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
