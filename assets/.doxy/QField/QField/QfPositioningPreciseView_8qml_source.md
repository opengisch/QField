

# File QfPositioningPreciseView.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfPositioningPreciseView.qml**](QfPositioningPreciseView_8qml.md)

[Go to the documentation of this file](QfPositioningPreciseView_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material.impl
import QtQuick.Layouts
import QtQuick.Shapes
import org.qfield.core
import org.qfield.gui

Item {
  id: positioningPreciseView

  enum RotationSource {
    Compass,
    Movement
  }

  property real maximumHeight: Math.min(Math.min(mainWindow.height / 3, 250))

  property double precision: 1
  property double projectDistance: navigation.distance * UnitTypes.fromUnitToUnitFactor(navigation.distanceUnits, projectInfo.distanceUnits)
  property double projectVerticalDistance: navigation.verticalDistance * UnitTypes.fromUnitToUnitFactor(navigation.distanceUnits, projectInfo.distanceUnits)

  property bool hasZ: !isNaN(projectVerticalDistance)
  property bool hasAcceptableAccuracy: positionSource.positionInformation.haccValid && positionSource.positionInformation.hacc < precision / 2.5
  property bool hasReachedTarget: hasAcceptableAccuracy && projectDistance - positionSource.positionInformation.hacc - (precision / 10) <= 0
  property bool hasAlarmSnoozed: false
  property QtObject positioningSettings

  property alias menu: settingsMenu

  readonly property alias preciseTargetDiameter: preciseTarget.width

  property real lastValidDirection: NaN
  readonly property real rotationAngle: {
    if (!positioningSettings.preciseViewAutoRotate) {
      return NaN;
    }
    if (positioningSettings.preciseViewRotationSource === QfPositioningPreciseView.RotationSource.Movement) {
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
    color: QfTheme.mainBackgroundColorSemiOpaque
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
        font: QfTheme.tinyFont
        color: QfTheme.navigationColor
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
        strokeColor: QfTheme.navigationColorSemiOpaque
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
        strokeColor: QfTheme.navigationColorSemiOpaque
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
        strokeColor: QfTheme.navigationColorSemiOpaque
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
        strokeColor: QfTheme.navigationColorSemiOpaque
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
        strokeColor: QfTheme.navigationBackgroundColor
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
        strokeColor: QfTheme.navigationBackgroundColor
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
        strokeColor: QfTheme.navigationBackgroundColor
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
        strokeColor: hasReachedTarget ? QfTheme.mainColor : QfTheme.mainTextColor
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
        color: QfTheme.navigationColor
        font: QfTheme.tinyFont
        style: Text.Outline
        styleColor: QfTheme.mainBackgroundColor
        text: '0'
      }
      Text {
        anchors.right: parent.right
        anchors.rightMargin: 2
        anchors.verticalCenter: parent.verticalCenter
        color: QfTheme.navigationColor
        font: QfTheme.tinyFont
        style: Text.Outline
        styleColor: QfTheme.mainBackgroundColor
        text: '90'
      }
      Text {
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        anchors.horizontalCenter: parent.horizontalCenter
        color: QfTheme.navigationColor
        font: QfTheme.tinyFont
        style: Text.Outline
        styleColor: QfTheme.mainBackgroundColor
        text: '180'
      }
      Text {
        anchors.left: parent.left
        anchors.leftMargin: 2
        anchors.verticalCenter: parent.verticalCenter
        color: QfTheme.navigationColor
        font: QfTheme.tinyFont
        style: Text.Outline
        styleColor: QfTheme.mainBackgroundColor
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
        font: QfTheme.tinyFont
        color: hasZ ? QfTheme.navigationColor : QfTheme.secondaryTextColor
        text: positiveLabel
      }
      Text {
        id: labelElevationBottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        font: QfTheme.tinyFont
        color: hasZ ? QfTheme.navigationColor : QfTheme.secondaryTextColor
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
      border.color: QfTheme.mainTextColor
      border.width: 1

      Rectangle {
        x: 1
        y: parent.height / 2
        width: parent.width - 2
        height: 1
        color: hasZ ? QfTheme.navigationColorSemiOpaque : QfTheme.mainTextColor
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
          fillColor: QfTheme.positionColor
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
          fillColor: QfTheme.positionColor
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
        color: QfTheme.mainTextColor
        font: QfTheme.strongTipFont
        style: Text.Outline
        styleColor: QfTheme.mainBackgroundColor

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
      fillColor: hasReachedTarget ? Qt.hsla(QfTheme.mainColor.hslHue, QfTheme.mainColor.hslSaturation, QfTheme.mainColor.hslLightness, 0.4) : QfTheme.positionColor
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
      fillColor: QfTheme.positionColor
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
      strokeColor: QfTheme.mainTextColor
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

    color: QfTheme.mainTextColor
    font: QfTheme.strongTipFont
    style: Text.Outline
    styleColor: QfTheme.mainBackgroundColor

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
      color: QfTheme.warningColor
      font: QfTheme.tinyFont
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.WordWrap
      text: qsTr('Positioning accuracy too low for this precision level')
      style: Text.Outline
      styleColor: QfTheme.mainBackgroundColor
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
    bgcolor: positioningPreciseView.hasAlarmSnoozed ? "transparent" : QfTheme.navigationColor
    iconSource: positioningPreciseView.hasAlarmSnoozed ? QfTheme.getThemeVectorIcon('ic_alarm_purple_24dp') : QfTheme.getThemeVectorIcon('ic_alarm_white_24dp')

    onClicked: {
      positioningPreciseView.hasAlarmSnoozed = !positioningPreciseView.hasAlarmSnoozed;
    }
  }

  Menu {
    id: settingsMenu
    width: 330

    MenuItem {
      text: qsTr("Audio proximity feedback")
      font: QfTheme.defaultFont
      height: 48
      leftPadding: QfTheme.menuItemCheckLeftPadding
      rightPadding: QfTheme.menuItemCheckLeftPadding
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
      font: QfTheme.defaultFont
      height: 48
      leftPadding: QfTheme.menuItemCheckLeftPadding
      rightPadding: QfTheme.menuItemCheckLeftPadding
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
      color: QfTheme.mainTextColor
      font: QfTheme.defaultFont
      leftPadding: QfTheme.menuItemIconlessLeftPadding
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
        leftMargin: QfTheme.menuItemIconlessLeftPadding
        rightMargin: QfTheme.menuItemCheckLeftPadding
      }
      spacing: 3
      orientation: ListView.Horizontal
      model: [qsTr("Compass"), qsTr("Movement")]

      delegate: Item {
        id: sourceDelegate
        width: (rotationSources.width - rotationSources.spacing) / 2
        height: 35
        enabled: !selected

        property bool selected: index === (positioningSettings.preciseViewRotationSource === QfPositioningPreciseView.RotationSource.Compass ? 0 : 1)

        Rectangle {
          anchors.fill: parent
          radius: 4
          color: sourceDelegate.selected ? QfTheme.mainColor : "transparent"
        }

        Text {
          text: modelData
          font: sourceDelegate.selected ? QfTheme.strongTipFont : QfTheme.tipFont
          anchors.centerIn: parent
          color: sourceDelegate.selected ? QfTheme.buttonColor : QfTheme.mainTextColor
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
            positioningSettings.preciseViewRotationSource = index === 0 ? QfPositioningPreciseView.RotationSource.Compass : QfPositioningPreciseView.RotationSource.Movement;
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
      color: QfTheme.mainTextColor
      font: QfTheme.defaultFont
      leftPadding: QfTheme.menuItemIconlessLeftPadding
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
        leftMargin: QfTheme.menuItemIconlessLeftPadding
        rightMargin: QfTheme.menuItemCheckLeftPadding
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
            color: precisionDelegate.selected ? QfTheme.mainColor : "transparent"
          }

          Text {
            id: precisionText
            text: UnitTypes.formatDistance(modelData, modelData < 1 ? 2 : 1, projectInfo.distanceUnits)
            font: precisionDelegate.selected ? QfTheme.strongTipFont : QfTheme.tipFont
            anchors.centerIn: parent
            color: precisionDelegate.selected ? QfTheme.buttonColor : QfTheme.mainTextColor
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
    enabled: positioningSettings.preciseViewRotationSource === QfPositioningPreciseView.RotationSource.Movement

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
```


