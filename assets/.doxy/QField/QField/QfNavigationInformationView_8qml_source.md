

# File QfNavigationInformationView.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfNavigationInformationView.qml**](QfNavigationInformationView_8qml.md)

[Go to the documentation of this file](QfNavigationInformationView_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

Rectangle {
  id: navigationInformationView

  property QfNavigation navigation

  property var coordinates: QfGeometryUtils.reprojectPoint(navigation.destination, navigation.mapSettings.destinationCrs, projectInfo.coordinateDisplayCrs)
  property bool coordinatesIsXY: QfCoordinateReferenceSystemUtils.defaultCoordinateOrderForCrsIsXY(projectInfo.coordinateDisplayCrs)
  property bool coordinatesIsGeographic: projectInfo.coordinateDisplayCrs.isGeographic

  property int ceilsCount: 4
  property double cellHeight: 26
  property double cellPadding: 6
  property color backgroundColor: "transparent"
  property color alternateBackgroundColor: QfTheme.navigationBackgroundColor
  property color textColor: QfTheme.mainTextColor
  property real contentHeight: content.height

  color: QfTheme.mainBackgroundColorSemiOpaque

  Timer {
    id: featureVertexTimer
    interval: 700
    repeat: true

    property bool moveForward: true

    onTriggered: {
      if (moveForward) {
        navigation.nextDestinationVertex();
      } else {
        navigation.previousDestinationVertex();
      }
      if (interval > 100)
        interval = interval * 0.8;
    }
  }

  ColumnLayout {
    id: content
    width: parent.width
    spacing: 0

    Item {
      Layout.fillWidth: true
      Layout.preferredHeight: childrenRect.height

      RowLayout {
        width: parent.width

        QfToolButton {
          id: previousFeatureVertex
          visible: navigation.destinationFeatureVertexCount > 1
          Layout.alignment: Qt.AlignVCenter
          width: 36
          height: 36
          round: true
          bgcolor: "transparent"
          iconSource: QfTheme.getThemeVectorIcon("ic_chevron_left_white_24dp")
          iconColor: QfTheme.mainTextColor

          onPressed: {
            navigation.previousDestinationVertex();
            featureVertexTimer.moveForward = false;
            featureVertexTimer.interval = 700;
            featureVertexTimer.restart();
          }
          onReleased: {
            featureVertexTimer.stop();
          }
          onCanceled: {
            featureVertexTimer.stop();
          }
        }

        Text {
          Layout.fillWidth: true
          Layout.margins: 5
          visible: navigation.destinationName != ''
          horizontalAlignment: Text.AlignHCenter
          font: QfTheme.strongTipFont
          elide: Text.ElideMiddle
          wrapMode: Text.NoWrap
          color: QfTheme.mainTextColor
          text: navigation.destinationName
        }

        QfToolButton {
          id: nextFeatureVertex
          visible: navigation.destinationFeatureVertexCount > 1
          Layout.alignment: Qt.AlignVCenter
          width: 36
          height: 36
          round: true
          bgcolor: "transparent"
          iconSource: QfTheme.getThemeVectorIcon("ic_chevron_right_white_24dp")
          iconColor: QfTheme.mainTextColor

          onPressed: {
            navigation.nextDestinationVertex();
            featureVertexTimer.moveForward = true;
            featureVertexTimer.interval = 700;
            featureVertexTimer.restart();
          }
          onReleased: {
            featureVertexTimer.stop();
          }
          onCanceled: {
            featureVertexTimer.stop();
          }
        }
      }
    }

    Grid {
      id: grid
      Layout.fillWidth: true
      Layout.preferredHeight: childrenRect.height
      width: parent.width
      height: grid.rows * navigationInformationView.cellHeight
      flow: GridLayout.TopToBottom
      rows: parent.width > 620 ? 1 : 2
      property double cellWidth: grid.width / (ceilsCount / grid.rows)

      Rectangle {
        height: cellHeight
        width: grid.cellWidth
        color: alternateBackgroundColor

        RowLayout {
          anchors.margins: cellPadding
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          anchors.right: parent.right

          Text {
            Layout.fillWidth: false
            font: QfTheme.tipFont
            color: QfTheme.secondaryTextColor
            text: coordinatesIsXY ? coordinatesIsGeographic ? qsTr("Lon") : qsTr("X") : coordinatesIsGeographic ? qsTr("Lat") : qsTr("Y")
          }

          Text {
            Layout.fillWidth: true
            font: QfTheme.tipFont
            color: textColor
            text: coordinatesIsXY ? Number(coordinates.x).toLocaleString(Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3) : Number(coordinates.y).toLocaleString(Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3)
            elide: Text.ElideRight
          }
        }
      }

      Rectangle {
        height: cellHeight
        width: grid.cellWidth
        color: backgroundColor

        RowLayout {
          anchors.margins: cellPadding
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          anchors.right: parent.right

          Text {
            Layout.fillWidth: false
            font: QfTheme.tipFont
            color: QfTheme.secondaryTextColor
            text: coordinatesIsXY ? coordinatesIsGeographic ? qsTr("Lat") : qsTr("Y") : coordinatesIsGeographic ? qsTr("Lon") : qsTr("X")
          }

          Text {
            Layout.fillWidth: true
            font: QfTheme.tipFont
            color: textColor
            text: coordinatesIsXY ? Number(coordinates.y).toLocaleString(Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3) : Number(coordinates.x).toLocaleString(Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3)
            elide: Text.ElideRight
          }
        }
      }

      Rectangle {
        height: cellHeight
        width: grid.cellWidth
        color: grid.rows == 2 ? backgroundColor : alternateBackgroundColor

        RowLayout {
          anchors.margins: cellPadding
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          anchors.right: parent.right

          Text {
            Layout.fillWidth: false
            font: QfTheme.tipFont
            color: QfTheme.secondaryTextColor
            text: qsTr("Dist.")
          }

          Text {
            Layout.fillWidth: true
            font: QfTheme.tipFont
            color: textColor
            text: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid ? UnitTypes.formatDistance(navigation.distance * UnitTypes.fromUnitToUnitFactor(navigation.distanceUnits, projectInfo.distanceUnits), 3, projectInfo.distanceUnits) : qsTr("N/A")
            elide: Text.ElideRight
          }
        }
      }

      Rectangle {
        height: cellHeight
        width: grid.cellWidth
        color: grid.rows == 2 ? alternateBackgroundColor : backgroundColor

        RowLayout {
          anchors.margins: cellPadding
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          anchors.right: parent.right

          Text {
            Layout.fillWidth: false
            font: QfTheme.tipFont
            color: QfTheme.secondaryTextColor
            text: qsTr("Bearing")
          }

          Text {
            Layout.fillWidth: true
            font: QfTheme.tipFont
            color: textColor
            text: positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid ? Number(navigation.bearing).toLocaleString(Qt.locale(), 'f', 1) + '°' : qsTr("N/A")
            elide: Text.ElideRight
          }
        }
      }
    }
  }
}
```


