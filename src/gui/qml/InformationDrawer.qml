import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
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

      header: QfToolButton {
        id: preciseViewSettings
        visible: positioningPreciseEnabled
        width: 24
        height: 24
        padding: 2
        iconSource: Theme.getThemeVectorIcon('ic_tune_white_24dp')
        iconColor: Theme.mainTextColor
        onClicked: {
          const globalPoint = preciseViewSettings.mapToGlobal(preciseViewSettings.width - positioningPreciseView.menu.width, preciseViewSettings.height);
          const localPoint = positioningPreciseView.menu.parent.mapFromGlobal(globalPoint.x, globalPoint.y);
          positioningPreciseView.menu.popup(localPoint.x, localPoint.y);
        }
      }

      Column {
        width: parent.width
        spacing: 0

        NavigationInformationView {
          id: navigationInformationView
          width: parent.width
          height: contentHeight
          navigation: controller.navigation
        }

        PositioningPreciseView {
          id: positioningPreciseView
          visible: positioningPreciseEnabled
          width: parent.width
          height: positioningPreciseEnabled ? preciseTargetDiameter + 20 : 0
          clip: true
          precision: positioningSettings.preciseViewPrecision
          positioningSettings: controller.positioningSettings

          Behavior on height {
            NumberAnimation {
              duration: 200
              easing.type: Easing.OutQuart
            }
          }
          Behavior on opacity {
            NumberAnimation {
              duration: 200
              easing.type: Easing.OutQuart
            }
          }
        }
      }
    }

    QfOverlayContainer {
      visible: positioningInformationViewEnabled

      title: qsTr("Positioning")

      header: RowLayout {
        Text {
          visible: positioningSettings.enableNtrip && positionSource.deviceCapabilities & AbstractGnssReceiver.NtripCorrection
          text: qsTr("NTRIP")
          font: Theme.tipFont
          color: positionSource.ntripState === Positioning.NtripState.Connected ? Theme.mainTextColor : Theme.secondaryTextColor
        }

        Rectangle {
          id: ntripIndicator
          Layout.alignment: Qt.AlignVCenter
          Layout.preferredWidth: 12
          Layout.preferredHeight: 12
          Layout.bottomMargin: 1
          visible: positioningSettings.enableNtrip && positionSource.deviceCapabilities & AbstractGnssReceiver.NtripCorrection
          radius: height / 2
          opacity: 1
          color: {
            if (positionSource.ntripState === Positioning.NtripState.Connected) {
              return positionSource.ntripCurrentness ? Theme.positionColor : Theme.warningColor;
            }
            return Theme.secondaryTextColor;
          }

          SequentialAnimation {
            running: positioningInformationViewEnabled && positionSource.ntripState === Positioning.NtripState.Connected && !positionSource.ntripCurrentness
            loops: Animation.Infinite

            onStopped: ntripIndicator.opacity = 1.0

            NumberAnimation {
              target: ntripIndicator
              property: "opacity"
              to: 0.0
              duration: 1000
              easing.type: Easing.InOutQuad
            }

            NumberAnimation {
              target: ntripIndicator
              property: "opacity"
              to: 1.0
              duration: 1000
              easing.type: Easing.InOutQuad
            }
          }
        }

        Rectangle {
          id: batteryIndicator
          Layout.alignment: Qt.AlignVCenter
          Layout.preferredWidth: 18
          Layout.preferredHeight: 12
          Layout.bottomMargin: 1
          visible: !isNaN(positionSource.deviceBatteryLevel)

          color: "transparent"
          border.width: 2
          border.color: Theme.mainTextColor
          radius: 2

          Rectangle {
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.topMargin: 3
            anchors.leftMargin: 3
            height: parent.height - 6
            width: (parent.width - 6) * positionSource.deviceBatteryLevel
            color: positionSource.deviceBatteryLevel > 0.20 ? Theme.goodColor : positionSource.deviceBatteryLevel > 0.05 ? Theme.warningColor : Theme.errorColor
          }
        }
      }

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
