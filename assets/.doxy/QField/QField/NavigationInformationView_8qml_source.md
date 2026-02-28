

# File NavigationInformationView.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**NavigationInformationView.qml**](NavigationInformationView_8qml.md)

[Go to the documentation of this file](NavigationInformationView_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

Rectangle {
  id: navigationInformationView

  property Navigation navigation

  property var coordinates: GeometryUtils.reprojectPoint(navigation.destination, navigation.mapSettings.destinationCrs, projectInfo.coordinateDisplayCrs)
  property bool coordinatesIsXY: CoordinateReferenceSystemUtils.defaultCoordinateOrderForCrsIsXY(projectInfo.coordinateDisplayCrs)
  property bool coordinatesIsGeographic: projectInfo.coordinateDisplayCrs.isGeographic

  property int ceilsCount: 4
  property double cellHeight: 26
  property double cellPadding: 6
  property color backgroundColor: "transparent"
  property color alternateBackgroundColor: Theme.navigationBackgroundColor
  property color textColor: Theme.mainTextColor
  property real contentHeight: content.height

  color: Theme.mainBackgroundColorSemiOpaque

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
          iconSource: Theme.getThemeVectorIcon("ic_chevron_left_white_24dp")
          iconColor: Theme.mainTextColor

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
          font: Theme.strongTipFont
          elide: Text.ElideMiddle
          wrapMode: Text.NoWrap
          color: Theme.mainTextColor
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
          iconSource: Theme.getThemeVectorIcon("ic_chevron_right_white_24dp")
          iconColor: Theme.mainTextColor

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
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            text: coordinatesIsXY ? coordinatesIsGeographic ? qsTr("Lon") : qsTr("X") : coordinatesIsGeographic ? qsTr("Lat") : qsTr("Y")
          }

          Text {
            Layout.fillWidth: true
            font: Theme.tipFont
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
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            text: coordinatesIsXY ? coordinatesIsGeographic ? qsTr("Lat") : qsTr("Y") : coordinatesIsGeographic ? qsTr("Lon") : qsTr("X")
          }

          Text {
            Layout.fillWidth: true
            font: Theme.tipFont
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
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            text: qsTr("Dist.")
          }

          Text {
            Layout.fillWidth: true
            font: Theme.tipFont
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
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            text: qsTr("Bearing")
          }

          Text {
            Layout.fillWidth: true
            font: Theme.tipFont
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


