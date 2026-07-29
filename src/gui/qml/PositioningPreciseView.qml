import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import QtQuick.Shapes
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Item {
  id: positioningPreciseView

  property real maximumHeight: Math.min(Math.min(mainWindow.height / 3, 250))

  property double precision: 1
  property double projectDistance: navigation.distance * UnitTypes.fromUnitToUnitFactor(navigation.distanceUnits, projectInfo.distanceUnits)
  property double projectVerticalDistance: navigation.verticalDistance * UnitTypes.fromUnitToUnitFactor(navigation.distanceUnits, projectInfo.distanceUnits)

  property bool hasZ: !isNaN(projectVerticalDistance)
  property bool hasAcceptableAccuracy: positionSource.positionInformation.haccValid && positionSource.positionInformation.hacc < precision / 2.5
  property bool hasReachedTarget: hasAcceptableAccuracy && projectDistance - positionSource.positionInformation.hacc - (precision / 10) <= 0
  property bool hasAlarmSnoozed: false
  property PositioningSettings positioningSettings

  property alias menu: settingsMenu

  readonly property alias preciseTargetDiameter: preciseTarget.width

  property real lastValidDirection: NaN
  readonly property real rotationAngle: {
    if (!positioningSettings.preciseViewAutoRotate) {
      return NaN;
    }
    if (positioningSettings.preciseViewRotationSource === PositioningSettings.RotationSource.Movement) {
      return lastValidDirection;
    }
    return positionSource.orientation;
  }

  property double positionX: Math.min(precision, projectDistance) * Math.cos((navigation.bearing - (!isNaN(rotationAngle) ? rotationAngle : 0) - 90) * Math.PI / 180) * (preciseTarget.width / 2) / precision
  property double positionY: Math.min(precision, projectDistance) * Math.sin((navigation.bearing - (!isNaN(rotationAngle) ? rotationAngle : 0) - 90) * Math.PI / 180) * (preciseTarget.width / 2) / precision
  property double positionZ: hasZ ? Math.min(precision, Math.max(-precision, -projectVerticalDistance)) * ((preciseElevation.height - 15) / 2) / precision : 0.0
  property point positionCenter: Qt.point(preciseTarget.width / 2 + preciseTarget.x + preciseTarget.parent.x, preciseTarget.height / 2 + preciseTarget.y + preciseTarget.parent.y)

  property string negativeLabel: UnitTypes.formatDistance(-precision, 2, projectInfo.distanceUnits)
  property string positiveLabel: UnitTypes.formatDistance(precision, 2, projectInfo.distanceUnits)

  Rectangle {
    anchors.fill: parent
    color: Theme.mainBackgroundColorSemiOpaque
  }

  Row {
    anchors.centerIn: parent
    anchors.margins: 10
    spacing: 5

    Rectangle {
      width: labelTarget.contentWidth
      height: preciseElevation.height
      color: "transparent"
      Text {
        id: labelTarget
        anchors.centerIn: parent
        font: Theme.tinyFont
        color: Theme.navigationColor
        text: positiveLabel
      }
    }

    Shape {
      id: preciseTarget
      width: Math.min(180, positioningPreciseView.maximumHeight - 40, positioningPreciseView.width - preciseElevation.width - labelTarget.contentWidth - labelElevation.width - 40)
      height: width
      rotation: !isNaN(rotationAngle) ? -rotationAngle + positionSource.bearingTrueNorth : 0

      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationColorSemiOpaque
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: (preciseTarget.width / 2) + (preciseTarget.width / 2) * Math.cos(5.49779)
        startY: (preciseTarget.width / 2) + (preciseTarget.width / 2) * Math.sin(5.49779)
        PathLine {
          x: (preciseTarget.width / 2) + (preciseTarget.width / 2) * Math.cos(2.35619)
          y: (preciseTarget.width / 2) + (preciseTarget.width / 2) * Math.sin(2.35619)
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationColorSemiOpaque
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: (preciseTarget.width / 2) + (preciseTarget.width / 2) * Math.cos(0.78539)
        startY: (preciseTarget.width / 2) + (preciseTarget.width / 2) * Math.sin(0.78539)
        PathLine {
          x: (preciseTarget.width / 2) + (preciseTarget.width / 2) * Math.cos(3.92699)
          y: (preciseTarget.width / 2) + (preciseTarget.width / 2) * Math.sin(3.92699)
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationColorSemiOpaque
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: preciseTarget.width / 2
        startY: 0
        PathLine {
          x: preciseTarget.width / 2
          y: preciseTarget.height
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationColorSemiOpaque
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: 0
        startY: preciseTarget.height / 2
        PathLine {
          x: preciseTarget.width
          y: preciseTarget.height / 2
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationBackgroundColor
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: 0
        startY: 0
        PathAngleArc {
          centerX: preciseTarget.width / 2
          centerY: centerX
          radiusX: preciseTarget.width / 8
          radiusY: radiusX
          startAngle: 0
          sweepAngle: 360
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationBackgroundColor
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: 0
        startY: 0
        PathAngleArc {
          centerX: preciseTarget.width / 2
          centerY: centerX
          radiusX: preciseTarget.width / 4
          radiusY: radiusX
          startAngle: 0
          sweepAngle: 360
        }
      }
      ShapePath {
        strokeWidth: 1
        strokeColor: Theme.navigationBackgroundColor
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: 0
        startY: 0
        PathAngleArc {
          centerX: preciseTarget.width / 2
          centerY: centerX
          radiusX: preciseTarget.width / 2.66
          radiusY: radiusX
          startAngle: 0
          sweepAngle: 360
        }
      }
      ShapePath {
        strokeWidth: hasReachedTarget ? 3 : 1
        strokeColor: hasReachedTarget ? Theme.mainColor : Theme.mainTextColor
        strokeStyle: ShapePath.SolidLine
        fillColor: "transparent"
        startX: 0
        startY: 0
        PathAngleArc {
          centerX: preciseTarget.width / 2
          centerY: centerX
          radiusX: preciseTarget.width / 2
          radiusY: radiusX
          startAngle: 0
          sweepAngle: 360
        }
      }

      Text {
        anchors.top: parent.top
        anchors.topMargin: 2
        anchors.horizontalCenter: parent.horizontalCenter
        color: Theme.navigationColor
        font: Theme.tinyFont
        style: Text.Outline
        styleColor: Theme.mainBackgroundColor
        text: '0'
      }
      Text {
        anchors.right: parent.right
        anchors.rightMargin: 2
        anchors.verticalCenter: parent.verticalCenter
        color: Theme.navigationColor
        font: Theme.tinyFont
        style: Text.Outline
        styleColor: Theme.mainBackgroundColor
        text: '90'
      }
      Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        anchors.horizontalCenter: parent.horizontalCenter
        color: Theme.navigationColor
        font: Theme.tinyFont
        style: Text.Outline
        styleColor: Theme.mainBackgroundColor
        text: '180'
      }
      Text {
        anchors.left: parent.left
        anchors.leftMargin: 2
        anchors.verticalCenter: parent.verticalCenter
        color: Theme.navigationColor
        font: Theme.tinyFont
        style: Text.Outline
        styleColor: Theme.mainBackgroundColor
        text: '270'
      }
    }

    Rectangle {
      id: labelElevation
      width: Math.max(labelElevationTop.contentWidth, labelElevationBottom.contentWidth)
      height: preciseElevation.height
      opacity: hasZ ? 1 : 0.25
      color: "transparent"

      Text {
        id: labelElevationTop
        anchors.top: parent.top
        anchors.right: parent.right
        font: Theme.tinyFont
        color: hasZ ? Theme.navigationColor : Theme.secondaryTextColor
        text: positiveLabel
      }
      Text {
        id: labelElevationBottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        font: Theme.tinyFont
        color: hasZ ? Theme.navigationColor : Theme.secondaryTextColor
        text: negativeLabel
      }
    }

    Rectangle {
      id: preciseElevation
      width: 14
      height: preciseTarget.height
      radius: 7
      opacity: hasZ ? 1 : 0.25
      color: "transparent"
      border.color: Theme.mainTextColor
      border.width: 1

      Rectangle {
        x: 1
        y: parent.height / 2
        width: parent.width - 2
        height: 1
        color: hasZ ? Theme.navigationColorSemiOpaque : Theme.mainTextColor
      }

      Shape {
        id: preciseVerticalPosition
        x: -1
        y: (preciseElevation.height - height) / 2 + positionZ
        width: preciseElevation.width + 2
        height: width
        visible: hasZ
        rotation: projectVerticalDistance < 0 ? 180 : 0

        ShapePath {
          strokeWidth: 0
          strokeColor: "transparent"
          strokeStyle: ShapePath.SolidLine
          fillColor: Theme.positionColor
          fillRule: ShapePath.WindingFill
          joinStyle: ShapePath.MiterJoin
          startX: preciseVerticalPosition.width / 2
          startY: startX
          scale: Math.abs(projectVerticalDistance) <= precision ? Qt.size(1, 1) : Qt.size(0, 0)
          PathAngleArc {
            centerX: preciseVerticalPosition.width / 2
            centerY: centerX
            radiusX: preciseVerticalPosition.width / 2
            radiusY: radiusX
            startAngle: 0
            sweepAngle: 360
          }
        }

        ShapePath {
          strokeWidth: 0
          strokeColor: "transparent"
          strokeStyle: ShapePath.SolidLine
          fillColor: Theme.positionColor
          fillRule: ShapePath.WindingFill
          joinStyle: ShapePath.MiterJoin
          startX: preciseVerticalPosition.width / 2
          startY: 0
          scale: Math.abs(projectVerticalDistance) > precision ? Qt.size(1, 1) : Qt.size(0, 0)
          PathLine {
            x: preciseVerticalPosition.width - 2
            y: preciseVerticalPosition.width
          }
          PathLine {
            x: 2
            y: preciseVerticalPosition.width
          }
          PathLine {
            x: preciseVerticalPosition.width / 2
            y: 0
          }
        }
      }

      Text {
        id: preciseVerticalPositionInfo
        x: -contentWidth - 10
        y: (preciseElevation.height - height) / 2 + positionZ
        visible: hasZ
        color: Theme.mainTextColor
        font: Theme.strongTipFont
        style: Text.Outline
        styleColor: Theme.mainBackgroundColor

        property int decimals: projectVerticalDistance >= 1000 ? 3 : projectVerticalDistance >= 0.1 ? 2 : 1
        text: projectVerticalDistance != 0.0 ? UnitTypes.formatDistance(projectVerticalDistance, decimals, projectInfo.distanceUnits) : 0
      }
    }
  }

  Shape {
    id: preciseHorizontalPosition

    x: positionCenter.x + positionX - width / 2
    y: positionCenter.y + positionY - width / 2
    width: 28
    height: width
    rotation: navigation.bearing - (!isNaN(rotationAngle) ? rotationAngle : 0)

    ShapePath {
      strokeWidth: 1
      strokeColor: "transparent"
      strokeStyle: ShapePath.SolidLine
      fillColor: hasReachedTarget ? Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.4) : Theme.positionColor
      fillRule: ShapePath.WindingFill
      startX: preciseHorizontalPosition.width / 2
      startY: startX
      scale: projectDistance <= precision ? Qt.size(1, 1) : Qt.size(0, 0)
      PathAngleArc {
        centerX: preciseHorizontalPosition.width / 2
        centerY: centerX
        radiusX: preciseHorizontalPosition.width / 2
        radiusY: radiusX
        startAngle: 0
        sweepAngle: 360
      }
    }

    ShapePath {
      strokeWidth: 1
      strokeColor: "transparent"
      strokeStyle: ShapePath.SolidLine
      fillColor: Theme.positionColor
      fillRule: ShapePath.WindingFill
      joinStyle: ShapePath.MiterJoin
      startX: preciseHorizontalPosition.width / 2
      startY: 0
      scale: projectDistance > precision ? Qt.size(1, 1) : Qt.size(0, 0)
      PathLine {
        x: preciseHorizontalPosition.width - 2
        y: preciseHorizontalPosition.width
      }
      PathLine {
        x: 2
        y: preciseHorizontalPosition.width
      }
      PathLine {
        x: preciseHorizontalPosition.width / 2
        y: 0
      }
    }
  }

  Shape {
    id: preciseHorizontalPositionCross

    anchors.centerIn: preciseHorizontalPosition

    width: 28
    height: width

    ShapePath {
      strokeWidth: 2
      strokeColor: Theme.mainTextColor
      strokeStyle: ShapePath.SolidLine
      fillColor: "transparent"
      startX: preciseHorizontalPosition.width / 2
      startY: 0
      scale: hasReachedTarget ? Qt.size(1, 1) : Qt.size(0, 0)

      PathLine {
        x: preciseHorizontalPosition.width / 2
        y: preciseHorizontalPosition.height
      }
      PathMove {
        x: 0
        y: preciseHorizontalPosition.height / 2
      }
      PathLine {
        x: preciseHorizontalPosition.width
        y: preciseHorizontalPosition.height / 2
      }
    }
  }

  Text {
    id: preciseHorizontalPositionInfo

    x: positionCenter.x + positionX + (positionX >= 0 ? -contentWidth - 10 : preciseHorizontalPosition.width / 2)
    y: positionCenter.y + positionY + (positionY >= 0 ? -preciseHorizontalPosition.height : preciseHorizontalPosition.height / 2)

    color: Theme.mainTextColor
    font: Theme.strongTipFont
    style: Text.Outline
    styleColor: Theme.mainBackgroundColor

    property int decimals: projectDistance >= 1000 ? 3 : projectDistance >= 0.10 ? 2 : 1
    text: qsTr('Dist.') + ': ' + UnitTypes.formatDistance(projectDistance, decimals, projectInfo.distanceUnits)
  }

  Rectangle {
    id: accuracyWarning

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 20

    visible: !hasAcceptableAccuracy
    width: parent.width - 20
    height: accuracyWarningLabel.contentHeight + 4
    color: "#22000000"
    radius: 3

    Text {
      id: accuracyWarningLabel
      anchors.centerIn: parent
      width: parent.width
      color: Theme.warningColor
      font: Theme.tinyFont
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.WordWrap
      text: qsTr('Positioning accuracy too low for this precision level')
      style: Text.Outline
      styleColor: Theme.mainBackgroundColor
    }
  }

  QfToolButton {
    id: alarmSnoozeButton

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.margins: 5

    visible: (navigation.proximityAlarm || positioningPreciseView.hasAlarmSnoozed) && projectDistance <= positioningPreciseView.precision
    enabled: visible

    round: true
    bgcolor: positioningPreciseView.hasAlarmSnoozed ? "transparent" : Theme.navigationColor
    iconSource: positioningPreciseView.hasAlarmSnoozed ? Theme.getThemeVectorIcon('ic_alarm_purple_24dp') : Theme.getThemeVectorIcon('ic_alarm_white_24dp')

    onClicked: {
      positioningPreciseView.hasAlarmSnoozed = !positioningPreciseView.hasAlarmSnoozed;
    }
  }

  Menu {
    id: settingsMenu
    width: 330

    MenuItem {
      text: qsTr("Audio proximity feedback")
      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding
      rightPadding: Theme.menuItemCheckLeftPadding
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
      rightPadding: Theme.menuItemCheckLeftPadding
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
      leftPadding: Theme.menuItemIconlessLeftPadding
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
        leftMargin: Theme.menuItemIconlessLeftPadding
        rightMargin: Theme.menuItemCheckLeftPadding
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
      leftPadding: Theme.menuItemIconlessLeftPadding
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
        leftMargin: Theme.menuItemIconlessLeftPadding
        rightMargin: Theme.menuItemCheckLeftPadding
      }
      columns: 4
      rowSpacing: 4
      columnSpacing: 3

      property var model: [0.10, 0.25, 0.50, 1, 2.5, 5, 10, 25]

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
            text: UnitTypes.formatDistance(modelData, modelData < 1 ? 2 : 1, projectInfo.distanceUnits)
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

  Connections {
    target: positionSource
    enabled: positioningSettings.preciseViewRotationSource === PositioningSettings.RotationSource.Movement

    function onPositionInformationChanged() {
      const info = positionSource.positionInformation;
      const movementSpeedThreshold = 0.8;
      if (info && info.directionValid && (!info.speedValid || info.speed >= movementSpeedThreshold)) {
        positioningPreciseView.lastValidDirection = info.direction;
      }
    }
  }

  Connections {
    target: navigation
    enabled: positioningPreciseView.hasAlarmSnoozed

    function onDistanceChanged() {
      if (projectDistance > precision) {
        positioningPreciseView.hasAlarmSnoozed = false;
      }
    }
  }
}
