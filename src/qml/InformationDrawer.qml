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
        onClicked: stakeoutMenu.popup(preciseViewSettings.width - stakeoutMenu.width, preciseViewSettings.height)

        Menu {
          id: stakeoutMenu
          width: 320

          MenuItem {
            text: qsTr("Audio proximity feedback")
            font: Theme.defaultFont
            height: 48
            leftPadding: Theme.menuItemCheckLeftPadding
            checkable: true
            checked: positioningSettings.preciseViewProximityAlarm
            indicator.height: 20
            indicator.width: 20
            indicator.implicitHeight: 24
            indicator.implicitWidth: 24
            onCheckedChanged: positioningSettings.preciseViewProximityAlarm = checked
          }

          MenuItem {
            text: qsTr("Rotate view")
            font: Theme.defaultFont
            height: 48
            leftPadding: Theme.menuItemCheckLeftPadding
            checkable: true
            checked: positioningSettings.preciseViewAutoRotate
            indicator.height: 20
            indicator.width: 20
            indicator.implicitHeight: 24
            indicator.implicitWidth: 24
            onCheckedChanged: positioningSettings.preciseViewAutoRotate = checked
          }

          MenuSeparator {
            width: parent.width
          }

          Item {
            width: 1
            height: 8
          }

          Text {
            text: qsTr("Rotation source")
            color: Theme.mainTextColor
            font: Theme.defaultFont
            leftPadding: Theme.menuItemCheckLeftPadding
          }

          Item {
            width: 1
            height: 8
          }

          ListView {
            id: rotationSources
            height: 35
            anchors {
              left: parent.left
              right: parent.right
              leftMargin: Theme.menuItemCheckLeftPadding
              rightMargin: 4
            }
            spacing: 3
            orientation: ListView.Horizontal
            model: [qsTr("Compass"), qsTr("Movement")]

            delegate: Item {
              id: sourceDelegate
              width: (rotationSources.width - rotationSources.spacing) / 2
              height: 35
              enabled: !selected

              property bool selected: index === (positioningSettings.preciseViewRotationSource === PositioningSettings.RotationSource.Compass ? 0 : 1)

              Rectangle {
                anchors.fill: parent
                radius: 4
                color: sourceDelegate.selected ? Theme.mainColor : "transparent"
              }

              Text {
                text: modelData
                font: sourceDelegate.selected ? Theme.strongTipFont : Theme.tipFont
                anchors.centerIn: parent
                color: sourceDelegate.selected ? Theme.buttonColor : Theme.mainTextColor
                elide: Text.ElideRight
                width: parent.width - 8
                horizontalAlignment: Text.AlignHCenter
              }

              Ripple {
                clip: true
                anchors.fill: parent
                clipRadius: 4
                pressed: sourceMouseArea.pressed
                anchor: parent
                active: sourceMouseArea.pressed
                color: "#22aaaaaa"
              }

              MouseArea {
                id: sourceMouseArea
                anchors.fill: parent
                onClicked: {
                  if (sourceDelegate.selected) {
                    return;
                  }
                  positioningSettings.preciseViewRotationSource = index === 0 ? PositioningSettings.RotationSource.Compass : PositioningSettings.RotationSource.Movement;
                }
              }
            }
          }

          Item {
            width: 1
            height: 8
          }

          Text {
            text: qsTr("Precision")
            color: Theme.mainTextColor
            font: Theme.defaultFont
            leftPadding: Theme.menuItemCheckLeftPadding
          }

          Item {
            width: 1
            height: 8
          }

          Grid {
            id: precisions
            anchors {
              left: parent.left
              right: parent.right
              leftMargin: Theme.menuItemCheckLeftPadding
              rightMargin: 4
            }
            columns: 4
            rowSpacing: 4
            columnSpacing: 3

            property var model: [0.10, 0.25, 0.50, 1, 2.5, 5, 10]

            Repeater {
              model: precisions.model

              delegate: Item {
                id: precisionDelegate
                width: (precisions.width - precisions.columnSpacing * (precisions.columns - 1)) / precisions.columns
                height: 35
                enabled: !selected

                property bool selected: modelData === positioningSettings.preciseViewPrecision

                Rectangle {
                  anchors.fill: parent
                  radius: 4
                  color: precisionDelegate.selected ? Theme.mainColor : "transparent"
                }

                Text {
                  id: precisionText
                  text: UnitTypes.formatDistance(modelData, 2, projectInfo.distanceUnits)
                  font: precisionDelegate.selected ? Theme.strongTipFont : Theme.tipFont
                  anchors.centerIn: parent
                  color: precisionDelegate.selected ? Theme.buttonColor : Theme.mainTextColor
                }

                Ripple {
                  clip: true
                  anchors.fill: parent
                  clipRadius: 4
                  pressed: precisionMouseArea.pressed
                  anchor: parent
                  active: precisionMouseArea.pressed
                  color: "#22aaaaaa"
                }

                MouseArea {
                  id: precisionMouseArea
                  anchors.fill: parent
                  onClicked: {
                    if (precisionDelegate.selected) {
                      return;
                    }
                    positioningSettings.preciseViewPrecision = modelData;
                  }
                }
              }
            }
          }

          Item {
            width: 1
            height: 8
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
