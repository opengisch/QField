/***************************************************************************
                            qgismobileapp.qml
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQml 2.2
import org.qgis 1.0

Rectangle {
  id: mainWindow
  anchors.fill: parent

  Item {
    id: mapCanvas

    property MapSettings mapSettings: MapSettings {
    }

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.bottom: parent.bottom
    anchors.right: featureForm.left

    MapCanvas {
      id: mapCanvasMap
      mapSettings: mapCanvas.mapSettings

      anchors.fill: parent
    }

    Item {
      anchors.fill: parent

      transform: MapTransform {
        mapSettings: mapCanvas.mapSettings
      }

      FeatureListModelHighlight {
        model: featureListModel
        selection: featureForm.selection
        color: "yellow"
        selectionColor: "#ff7777"
        width: 5
      }
    }
  }

  FeatureForm {
    id: featureForm
    mapSettings: mapCanvas.mapSettings

    anchors.right: parent.right
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    width: 0

    border.color: "lightGray"
    border.width: 1

    selection: FeatureListModelSelection { model: featureListModel }
    selectionColor: "#ff7777"
  }

  Column {
    id: mainMenuBar
    x: 10
    y: 10
    spacing: 10

    Item {
      height: dp*48
      width: dp*48

      Rectangle {
        anchors.fill: parent
        color: "#bb555555"
        border.color: "#dddddd"
        border.width: 1
        radius: dp*5
      }

      MouseArea {
        anchors.fill: parent

        onClicked: {
          mainMenu.popup()
        }
      }

      Image {
        anchors.centerIn: parent
        width: 42
        height: 42
        source: "/themes/holodark/settings.png"
      }
    }
/*
    Item {
      id: gpsButton
      height: dp*48
      width: dp*48
      state: "Off"

      property alias icon: icon.source

      states: [
        State {
          name: "Off"
          PropertyChanges {
            target: gpsButton
            icon: "/themes/holodark/location_off.png"
          }
        },

        State {
          name: "On"
          PropertyChanges {
            target: gpsButton
            icon: "/themes/holodark/location.png"
          }
        }
      ]

      Rectangle {
        anchors.fill: parent
        color: "#bb555555"
        border.color: "#dddddd"
        border.width: 1
        radius: dp*5
      }

      MouseArea {
        anchors.fill: parent

        onClicked: {
          parent.toggleGps()
        }
      }

      function toggleGps() {
        switch ( gpsButton.state )
        {
          case "Off":
            gpsButton.state = "On"
            displayToast( "GPS is now on" )
            break;

          case "On":
            gpsButton.state = "Off"
            displayToast( "GPS is now off" )
            break;
        }
      }

      Image {
        id: icon
        anchors.centerIn: parent
        width: 42
        height: 42
      }
    }
  */
  }

  FileDialog {
    id: openProjectDialog
    title: "Please choose a project"
    visible: false
    width: parent.width
    height: parent.height

    onAccepted: {
      console.debug( "Open file " + openProjectDialog.fileUrl.toString().slice(7) )
      iface.loadProject( openProjectDialog.fileUrl.toString().slice(7) )
    }
  }

  Menu {
    id: mainMenu
    title: "Main Menu"

    MenuItem {
      text: "Open Project"
      iconSource: "/themes/holodark/map.png"
      onTriggered: {
        openProjectDialog.visible = true
      }
    }
/*
    MenuItem {
      text: "Layers"
      iconSource: "/themes/holodark/layers.png"
      onTriggered: {
        messageDialog.visible = true
      }
    }
*/
    MenuSeparator {}

    MenuItem {
      text: "Quit"
      iconSource: "/themes/holodark/remove.png"
      onTriggered: {
        Qt.quit()
      }
    }
  }

  MessageDialog {
    id: messageDialog
    title: "Not implemented yet"
    text: "And of course you could only agree"
    onAccepted: {
      messageDialog.visible = false
    }
  }

  function displayToast(message) {
    toastMessage.text = message
    toast.opacity = 1
  }

  Rectangle {
    id: busyMessage
    anchors.fill: parent
    color: "#272727"
    opacity: 0.5
    visible: false

    BusyIndicator {
      id: busyMessageIndicator
      anchors.centerIn: parent
      running: true
    }

    Text {
      id: busyMessageText
      anchors.top: busyMessageIndicator.bottom
      anchors.horizontalCenter: parent.horizontalCenter
      text: qsTr( "Loading Project" )
    }

    Connections {
      target: iface

      onLoadProjectStarted: {
        busyMessageText.text = qsTr( "Loading Project: %1" ).arg( path )
        busyMessage.visible = true
      }

      onLoadProjectEnded: {
        busyMessage.visible = false
      }
    }
  }

  WelcomeScreen {
    id: welcomeScreen
    anchors.fill: parent
    visible: !settings.value( "/QField/FirstRunFlag", false )

    onShowOpenProjectDialog: {
      welcomeScreen.visible = false
      openProjectDialog.visible = true
    }
  }

  // Toast
  Rectangle {
    id: toast
    anchors.horizontalCenter: parent.horizontalCenter
    color: "#272727"
    height: 40*dp;
    width: ( (toastMessage.width + 16*dp) <= 192*dp ) ? 192*dp : toastMessage.width + 16*dp
    opacity: 0
    radius: 20*dp
    y: parent.height - 112*dp
    z: 1
    Behavior on opacity {
      NumberAnimation { duration: 500 }
    }

    // Visible only for 3 seconds
    onOpacityChanged: {
      toast.visible = (toast.opacity > 0)
      if ( toast.opacity == 1 ) {
        toastTimer.start()
      }
    }

    Text {
      id: toastMessage
      anchors.centerIn: parent
      font.pixelSize: 16*dp
      color: "#ffffff"
    }
      // Visible only for 3 seconds
    Timer {
      id: toastTimer
      interval: 3000
      onTriggered: { toast.opacity = 0 }
    }
  }
/*
  ToolBar {
    width: childrenRect.width
    height: parent.height
    Column {
      ToolButton {
        iconSource: "/themes/holodark/settings.png"
      }
    }
  }
*/
}
