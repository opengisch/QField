

# File InformationDrawer.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**InformationDrawer.qml**](InformationDrawer_8qml.md)

[Go to the documentation of this file](InformationDrawer_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

Item {
  id: controller

  anchors.bottom: parent.bottom
  anchors.left: parent.left
  anchors.leftMargin: mainWindow.sceneLeftMargin
  anchors.right: parent.right
  anchors.rightMargin: mainWindow.sceneRightMargin

  height: mainContent.height + (mainContent.height > 0 ? 5 : 0) + mainWindow.sceneBottomMargin
  Behavior on height {
    PropertyAnimation {
      easing.type: Easing.OutQuart
    }
  }

  // COGO
  property alias cogoOperationSettings: cogoOperationSettings

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
  property bool positioningPreciseEnabled: !elevationProfile.visible && !isNaN(navigation.distance) && navigation.isActive && (positioningSettings.alwaysShowPreciseView || (positioningPreciseView.hasAcceptableAccuracy && positioningPreciseView.projectDistance < positioningPreciseView.precision))

  // ElevationProfile
  property alias elevationProfile: elevationProfile

  Column {
    id: mainContent
    width: parent.width - 10
    leftPadding: 5
    rightPadding: 5
    spacing: 8

    CogoOperationSettings {
      id: cogoOperationSettings
      visible: false
    }

    QfOverlayContainer {
      visible: navigationInformationViewEnabled

      title: qsTr("Navigation")

      NavigationInformationView {
        id: navigationInformationView
        width: parent.width
        height: contentHeight
        navigation: controller.navigation
      }
    }

    QfOverlayContainer {
      visible: positioningInformationViewEnabled

      title: qsTr("Positioning")

      PositioningInformationView {
        id: positioningInformationView
        width: parent.width
        height: Math.min(contentHeight, mainWindow.height / 3)
        visible: positioningInformationViewEnabled
        positionSource: controller.positionSource
        antennaHeight: positioningSettings.antennaHeightActivated ? positioningSettings.antennaHeight : 0
      }
    }

    QfOverlayContainer {
      visible: positioningPreciseEnabled

      title: qsTr("Precise view")

      PositioningPreciseView {
        id: positioningPreciseView
        width: parent.width
        height: Math.min(mainWindow.height / 2.5, 400)
        precision: positioningSettings.preciseViewPrecision
      }
    }

    QfOverlayContainer {
      visible: sensorInformationViewEnabled

      title: qsTr("Sensors")

      SensorInformationView {
        id: sensorInformationView
        height: contentHeight
      }
    }

    QfOverlayContainer {
      visible: stateMachine.state === 'measure' && elevationProfileButton.elevationProfileActive

      title: qsTr("Elevation profile")

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
```


