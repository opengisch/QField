import QtQuick 2.14
import QtQuick.Controls 2.14

import org.qfield 1.0
import Theme 1.0

Drawer {
  id: controller
  height: defaultHeight
  Overlay.modal: null
  modal: false
  width: parent.width
  edge: Qt.BottomEdge
  dragMargin: 0
  closePolicy: Popup.NoAutoClose

  property Navigation navigation
  property PositioningSettings positioningSettings
  property real defaultHeight: positioningPreciseView.height + details.height + 32
  property real positioningPreciseViewHeight
  property alias positioningPreciseView: positioningPv
  property bool closeRequested: false
  property bool shouldBeOpen: false
  property bool positioningPreciseEnabled: false

  onShouldBeOpenChanged: {
    if(shouldBeOpen){
      closeRequested = false;
      open()
    }else{
      closeRequested = true;
      close()
    }
  }

  onClosed: {
    if (!closeRequested) {
      // view dragged down but we should bring it back
      controller.height = details.height + 24
      controller.open()
    }
  }

  background: Rectangle {
    anchors.fill: parent
    color: "transparent"
  }

  Column {
    spacing: 8
    width: parent.width

    Rectangle {
      id: details
      anchors.horizontalCenter: parent.horizontalCenter
      width: parent.width - 16
      height: navigationInformationView.height
      radius: 8
      color: Theme.mainBackgroundColor

      NavigationInformationView {
        id: navigationInformationView
        navigation: controller.navigation
        width: parent.width
        anchors.centerIn: parent
        radius: parent.radius
        clip: true
      }

      Rectangle {
        width: 50
        height: 2
        radius: 4
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 2
        color: Theme.mainTextColor
      }

      MouseArea {
        anchors.fill: details

        property int dragStartY: 0

        onPressed: mouse => {
          dragStartY = mouse.y
        }
        onPositionChanged: mouse => {
         var deltaY = mouse.y - dragStartY
         if (deltaY < -details.height / 4){ // && positioningPv.canShow) {
            controller.height = controller.defaultHeight
          }
        }
      }
    }

    Rectangle {
      anchors.horizontalCenter: parent.horizontalCenter
      width: parent.width - 16
      height: positioningPv.height
      radius: 8
      color: "black"

      PositioningPreciseView {
        id: positioningPv
        precision: positioningSettings.preciseViewPrecision
        width: parent.width
        height: positioningPreciseViewHeight
      }

      Rectangle{
        visible: !positioningPreciseEnabled
        anchors.fill: positioningPv
        color: "#cc2e2e2e"

        Text{
          anchors.centerIn: parent
          text: "Enable Gps to show!"
          color: "white"
        }
      }
    }
  }
}
