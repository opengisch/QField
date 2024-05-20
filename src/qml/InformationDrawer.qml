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
    leftPadding: 5
    rightPadding: 5
    spacing: 8

    Rectangle {
      visible: navigationInformationViewEnabled
      width: parent.width
      height: childrenRect.height
      color: Theme.mainBackgroundColorSemiOpaque
      radius: itemRadius
      clip: true

      Column {
        width: parent.width - 10
        anchors.horizontalCenter: parent.horizontalCenter
        topPadding: 5
        bottomPadding: 5
        spacing: 4

        Text {
          text: "Navigation"
          font: Theme.strongTipFont
          color: Theme.mainTextColor
          leftPadding: 6
        }

        NavigationInformationView {
          id: navigationInformationView
          width: parent.width
          height: contentHeight
          navigation: controller.navigation
        }
      }
    }

    Rectangle {
      visible: positioningInformationViewEnabled
      width: parent.width
      height: childrenRect.height
      color: Theme.mainBackgroundColorSemiOpaque
      radius: itemRadius
      clip: true

      Column {
        width: parent.width - 10
        anchors.horizontalCenter: parent.horizontalCenter
        topPadding: 5
        bottomPadding: 5
        spacing: 4

        Text {
          text: "Positioning"
          font: Theme.strongTipFont
          color: Theme.mainTextColor
          leftPadding: 6
        }

        PositioningInformationView {
          id: positioningInformationView
          width: parent.width
          height: contentHeight
          visible: positioningInformationViewEnabled
          positionSource: controller.positionSource
          antennaHeight: positioningSettings.antennaHeightActivated ? positioningSettings.antennaHeight : NaN
        }
      }
    }

    Rectangle {
      visible: positioningPreciseEnabled
      width: parent.width
      height: childrenRect.height
      color: Theme.mainBackgroundColorSemiOpaque

      Column {
        width: parent.width - 10
        anchors.horizontalCenter: parent.horizontalCenter
        topPadding: 5
        bottomPadding: 5
        spacing: 4

        Text {
          text: "Precise view"
          font: Theme.strongTipFont
          color: Theme.mainTextColor
          leftPadding: 6
        }

        PositioningPreciseView {
          id: positioningPreciseView
          width: parent.width
          height: Math.min(mainWindow.height / 2.5, 400)
          clip: true
          precision: positioningSettings.preciseViewPrecision
        }
      }
    }

    Rectangle {
      visible: sensorInformationViewEnabled
      width: parent.width
      height: childrenRect.height
      color: Theme.mainBackgroundColorSemiOpaque
      radius: itemRadius
      clip: true

      Column {
        width: parent.width - 10
        anchors.horizontalCenter: parent.horizontalCenter
        topPadding: 5
        bottomPadding: 5
        spacing: 4

        Text {
          text: "Sensors"
          font: Theme.strongTipFont
          color: Theme.mainTextColor
          leftPadding: 6
        }

        SensorInformationView {
          id: sensorInformationView
          height: contentHeight
        }
      }
    }

    Rectangle {
      visible: stateMachine.state === 'measure' && elevationProfileButton.elevationProfileActive
      width: parent.width
      height: childrenRect.height
      color: Theme.mainBackgroundColorSemiOpaque
      radius: itemRadius
      clip: true

      Column {
        width: parent.width - 10
        anchors.horizontalCenter: parent.horizontalCenter
        topPadding: 5
        bottomPadding: 5
        spacing: 4

        Text {
          text: "Elevation profile"
          font: Theme.strongTipFont
          color: Theme.mainTextColor
          leftPadding: 6
        }

        ElevationProfile {
          id: elevationProfile

          width: parent.width
          height: Math.max(220, mainWindow.height / 4)

          project: qgisProject
          crs: mapCanvas.mapSettings.destinationCrs
        }
      }
    }
  }
}
