import QtQuick 2.14
import QtQuick.Controls 2.14

import org.qfield 1.0
import Theme 1.0

Drawer {
  id: controller
  width: parent.width
  height: defaultHeight
  Overlay.modal: null
  modal: false
  edge: Qt.BottomEdge
  dragMargin: 0
  closePolicy: Popup.NoAutoClose

  property alias positioningPreciseView: positioningPv
  property Navigation navigation
  property PositioningSettings positioningSettings
  property Positioning positionSource
  property double antennaHeight
  property real realtimeHeight: controller.height * controller.position
  property real positioningPreciseViewHeight
  property real defaultHeight: details.height + positioningIfo.height + positioningPreciseViewRect.height + 24
  property bool closeRequested: true
  property bool navigationInformationViewEnabled: navigation.isActive
  property bool positioningInformationViewEnabled: false
  property bool shouldOpen: false
  property bool positioningPreciseEnabled: false

  function resetHeight() {
    let newHeight = 0
    newHeight += (details.height + 8)
    newHeight += (positioningIfo.height + 8)
    newHeight += (positioningPreciseViewRect.height + 8)
    newHeight += 16
    controller.height = newHeight
  }

  onNavigationInformationViewEnabledChanged: {
    if (!navigationInformationViewEnabled) {
      controller.height -= navigationInformationView.height
      controller.open()
    } else if (shouldOpen) {
      controller.height += navigationInformationView.height
      controller.open()
    }
  }

  onPositioningInformationViewEnabledChanged: {
    if (!positioningInformationViewEnabled) {
      controller.height -= positioningInformationView.height
      controller.open()
    } else if (shouldOpen) {
      controller.height += positioningInformationView.height
      controller.open()
    }
  }

  onPositioningPreciseEnabledChanged: {
    if (!positioningPreciseEnabled) {
      controller.height -= positioningPreciseViewHeight
      controller.open()
    } else if (shouldOpen) {
      controller.height += positioningPreciseViewHeight
      controller.open()
    }
  }

  onShouldOpenChanged: {
    if (shouldOpen) {
      closeRequested = false
      resetHeight()
      open()
    } else {
      closeRequested = true
      close()
    }
  }

  onOpened: {
    if (closeRequested) {
      controller.close()
    }
  }

  onClosed: {
    if (!closeRequested) {
      // view is dragged down but we must bring it back
      resetHeight()
      controller.open()
    }
  }

  background: Rectangle {
    anchors.fill: parent
    color: "transparent"
  }

  Column {
    id: mainContent
    spacing: 8
    width: parent.width

    Rectangle {
      id: details
      anchors.horizontalCenter: parent.horizontalCenter
      width: parent.width - 8
      height: navigationInformationViewEnabled ? navigationInformationView.height : 0
      radius: 8
      color: Theme.mainBackgroundColor
      clip: true

      NavigationInformationView {
        id: navigationInformationView
        navigation: controller.navigation
        width: parent.width
        anchors.centerIn: parent
        radius: parent.radius
        clip: true
      }
    }

    Rectangle {
      id: positioningIfo
      anchors.horizontalCenter: parent.horizontalCenter
      width: parent.width - 8
      height: positioningInformationViewEnabled ? positioningInformationView.height : 0
      radius: 8
      color: Theme.mainBackgroundColor
      clip: true

      PositioningInformationView {
        id: positioningInformationView
        visible: positioningInformationViewEnabled
        positionSource: controller.positionSource
        antennaHeight: positioningSettings.antennaHeightActivated ? positioningSettings.antennaHeight : NaN
        radius: parent.radius
      }
    }

    Rectangle {
      id: positioningPreciseViewRect
      anchors.horizontalCenter: parent.horizontalCenter
      width: parent.width - 8
      height: positioningPv.height
      radius: 8
      color: "black"
      clip: true

      PositioningPreciseView {
        id: positioningPv
        precision: positioningSettings.preciseViewPrecision
        width: parent.width
        height: positioningPreciseEnabled ? positioningPreciseViewHeight : 0
      }
    }
  }

  MouseArea {
    anchors.fill: parent
    property int dragStartY: 0

    onPressed: mouse => {
      dragStartY = mouse.y
    }

    onPositionChanged: mouse => {
      var deltaY = mouse.y - dragStartY
      if (deltaY < -details.height / 6 && positioningPreciseEnabled) {
        resetHeight()
        controller.open()
      }
    }
  }
}
