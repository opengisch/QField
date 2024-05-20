import QtQuick 2.14
import QtQuick.Controls 2.14

import org.qfield 1.0
import Theme 1.0

Item {
  id: controller

  anchors.bottom: parent.bottom
  anchors.leftMargin: 10
  anchors.rightMargin: 10

  width: parent.width
  height: mainContent.height + (mainContent.height > 0 ? 5 : 0) + mainWindow.sceneBottomMargin
  Behavior on height {
    PropertyAnimation {
      easing.type: Easing.OutQuart
    }
  }

  property real itemRadius: 8

  // SensorInformationView
  property bool sensorInformationViewEnabled: sensorInformationView.activeSensors > 0

  // NavigationInformationView
  property bool navigationInformationViewEnabled: navigation.isActive && !elevationProfile.visible

  // PositioningInformationView
  property Navigation navigation
  property bool positioningInformationViewEnabled: positioningSettings.showPositionInformation && !elevationProfile.visible

  // PositioningPreciseView
  property alias positioningPreciseView: positioningPreciseView
  property PositioningSettings positioningSettings
  property Positioning positionSource
  property real positioningPreciseViewHeight
  property bool positioningPreciseEnabled: !elevationProfile.visible
                                           && !isNaN(navigation.distance)
                                           && navigation.isActive
                                           && (positioningSettings.alwaysShowPreciseView
                                              || ( positioningPreciseView.hasAcceptableAccuracy
                                              &&  positioningPreciseView.projectDistance < positioningPreciseView.precision ))

  // ElevationProfile
  property alias elevationProfile: elevationProfile

  Column {
    id: mainContent
    width: parent.width - 10
    anchors.horizontalCenter: parent.horizontalCenter
    spacing: 8

    NavigationInformationView {
      id: navigationInformationView
      width: parent.width
      height: navigationInformationViewEnabled ? contentHeight : 0
      radius: itemRadius
      clip: true
      navigation: controller.navigation
    }

    PositioningInformationView {
      id: positioningInformationView
      width: parent.width
      height: positioningInformationViewEnabled ? contentHeight : 0
      radius: itemRadius
      clip: true
      visible: positioningInformationViewEnabled
      positionSource: controller.positionSource
      antennaHeight: positioningSettings.antennaHeightActivated ? positioningSettings.antennaHeight : NaN
    }

    PositioningPreciseView {
      id: positioningPreciseView
      width: parent.width
      height: positioningPreciseEnabled ? positioningPreciseViewHeight : 0
      clip: true
      precision: positioningSettings.preciseViewPrecision
    }

    SensorInformationView {
      id: sensorInformationView
      height: sensorInformationViewEnabled ? contentHeight : 0
      radius: itemRadius
      clip: true
    }

    ElevationProfile {
        id: elevationProfile

        visible: stateMachine.state === 'measure' && elevationProfileButton.elevationProfileActive

        width: parent.width
        height: Math.max(220, mainWindow.height / 4)
        radius: itemRadius
        clip: true

        project: qgisProject
        crs: mapCanvas.mapSettings.destinationCrs
    }
  }
}
