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
  height: mainContent.height + (mainContent.height > 0 ? 10 : 0)
  Behavior on height {
    PropertyAnimation {
      easing.type: Easing.OutQuart
    }
  }

  property real itemRadius: 8
  property bool uiConflictFree: false

  // SensorInformationView
  property bool sensorInformationViewEnabled: sensorInformationView.activeSensors > 0

  // NavigationInformationView
  property bool navigationInformationViewEnabled: navigation.isActive

  // PositioningInformationView
  property Navigation navigation
  property bool positioningInformationViewEnabled: positioningSettings.showPositionInformation && uiConflictFree

  // PositioningPreciseView
  property alias positioningPreciseView: positioningPreciseView
  property PositioningSettings positioningSettings
  property Positioning positionSource
  property real positioningPreciseViewHeight
  property bool positioningPreciseEnabled: uiConflictFree
                                           && !isNaN(navigation.distance)
                                           && navigation.isActive
                                           && (positioningSettings.alwaysShowPreciseView
                                              || ( positioningPreciseView.hasAcceptableAccuracy
                                              &&  positioningPreciseView.projectDistance < positioningPreciseView.precision ))

  Column {
    id: mainContent
    width: parent.width - 8
    anchors.horizontalCenter: parent.horizontalCenter
    spacing: 8

    NavigationInformationView {
      id: navigationInformationView
      width: parent.width
      height: navigationInformationViewEnabled ? contentHeight : 0
      clip: true
      navigation: controller.navigation
      radius: itemRadius
    }

    PositioningInformationView {
      id: positioningInformationView
      width: parent.width
      height: positioningInformationViewEnabled ? contentHeight : 0
      clip: true
      visible: positioningInformationViewEnabled
      positionSource: controller.positionSource
      antennaHeight: positioningSettings.antennaHeightActivated ? positioningSettings.antennaHeight : NaN
      radius: itemRadius
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
    }
  }
}
