import QtQuick
import QtQuick.Controls
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
      visible: positioningPreciseEnabled

      title: qsTr("Precise view")

      header: QfToolButton {
        id: preciseViewSettings
        width: 24
        height: 24
        padding: 2
        iconSource: Theme.getThemeVectorIcon('ic_tune_white_24dp')
        iconColor: Theme.mainTextColor
        onClicked: stakeoutSettingsPopup.open()

        QfPopup {
          id: stakeoutSettingsPopup
          padding: 12
          width: 220

          background: Rectangle {
            color: Theme.mainBackgroundColorSemiOpaque
            radius: 8
          }

          x: {
            const buttonPos = preciseViewSettings.mapToItem(parent, 0, 0);
            return buttonPos.x + preciseViewSettings.width - width;
          }
          y: {
            const buttonPos = preciseViewSettings.mapToItem(parent, 0, 0);
            const below = buttonPos.y + preciseViewSettings.height + 4;
            return below + height > parent.height ? buttonPos.y - height - 4 : below;
          }

          ButtonGroup {
            id: sourceGroup
          }

          contentItem: Column {
            width: parent.width
            spacing: 8

            QfSwitch {
              width: parent.width
              height: 28
              topPadding: 0
              bottomPadding: 0
              text: qsTr("Auto-rotate")
              checked: positioningSettings.preciseViewAutoRotate
              onToggled: positioningSettings.preciseViewAutoRotate = checked
            }

            MenuSeparator {
              width: parent.width
              padding: 0
            }

            Text {
              text: qsTr("Source")
              font: Theme.tipFont
              color: Theme.secondaryTextColor
            }

            Row {
              width: parent.width
              spacing: 4
              enabled: positioningSettings.preciseViewAutoRotate
              opacity: enabled ? 1.0 : 0.4

              QfButton {
                width: (parent.width - parent.spacing) / 2
                height: 32
                text: qsTr("Compass")
                checkable: true
                checked: positioningSettings.preciseViewRotationSource === PositioningSettings.RotationSource.Compass
                onClicked: positioningSettings.preciseViewRotationSource = PositioningSettings.RotationSource.Compass
                ButtonGroup.group: sourceGroup
                font.pointSize: Theme.tipFont.pointSize
                radius: 8
                bgcolor: (checked && enabled) ? Theme.mainColor : "transparent"
                color: (checked && enabled) ? Theme.mainBackgroundColor : Theme.mainColor
              }

              QfButton {
                width: (parent.width - parent.spacing) / 2
                height: 32
                text: qsTr("Movement")
                checkable: true
                checked: positioningSettings.preciseViewRotationSource === PositioningSettings.RotationSource.Movement
                onClicked: positioningSettings.preciseViewRotationSource = PositioningSettings.RotationSource.Movement
                ButtonGroup.group: sourceGroup
                font.pointSize: Theme.tipFont.pointSize
                radius: 8
                bgcolor: (checked && enabled) ? Theme.mainColor : "transparent"
                color: (checked && enabled) ? Theme.mainBackgroundColor : Theme.mainColor
              }
            }
          }
        }
      }

      PositioningPreciseView {
        id: positioningPreciseView
        width: parent.width
        height: Math.min(mainWindow.height / 2.5, 400)
        precision: positioningSettings.preciseViewPrecision
        positioningSettings: controller.positioningSettings
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
