import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2
import QtQml 2.2
import org.qgis 1.0

Rectangle {
  id: mainWindow
  anchors.fill: parent
  signal openProjectDialog()

  MapCanvas {
    anchors.fill: parent
    id: mapCanvas
    signal openProjectDialog()
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
  }

  /*
  FileDialog {
    id: openProjectDialog
    title: "Please choose a project"
    visible: false

    onAccepted: {
      console.debug( "Open file " + openProjectDialog.fileUrl.toString().slice(7) )
      project.read( openProjectDialog.fileUrl.toString().slice(7) )
    }
  }
  */

  Menu {
    id: mainMenu
    title: "Main Menu"

    MenuItem {
      text: "Open Project"
      iconSource: "/themes/holodark/map.png"
      onTriggered: {
        mainWindow.openProjectDialog()
      }
    }

    MenuItem {
      text: "Layers"
      iconSource: "/themes/holodark/layers.png"
      onTriggered: {
        messageDialog.visible = true
      }
    }

    MenuSeparator {}

    MenuItem {
      text: "Quit"
      iconSource: "/themes/holodark/remove.png"
      onTriggered: {
        Qt.quit()
      }
    }
  }

  Rectangle {
    id: featureForm
    anchors.fill: parent
    visible: false
    color: "#dddddddd"
    focus: true

    function show() {
      visible = true
      featureForm.anchors.centerIn = parent
    }

    ListView {
      model: featureModel
      anchors.fill: parent
      anchors.margins: 10

      Component.onCompleted: {
        mapCanvas.mapCanvasWrapper.identifyFeature.connect( featureForm.show )
      }

      delegate:
        Text {
          id: itemText
          text: model.display
          font.family: "Roboto"
          font.pointSize: 20
      }

      MouseArea {
        anchors.fill: parent
        onClicked: {
          featureForm.visible = false
        }
      }
/*
      Keys.onReleased: {
        console.debug( "rleease" )
        if ( event.key === Qt.Key_Back ||
            event.key === Qt.Key_Left )
        {
          featureForm.visible = false
          event.accepted = true
        }
      }
*/
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
}
