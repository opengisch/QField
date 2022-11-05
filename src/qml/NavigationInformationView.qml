import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

Rectangle {
  id: navigationInformationView

  property Navigation navigation

  property var coordinates: GeometryUtils.reprojectPoint(navigation.destination, navigation.mapSettings.destinationCrs, projectInfo.coordinateDisplayCrs)
  property bool coordinatesIsXY: CoordinateReferenceSystemUtils.defaultCoordinateOrderForCrsIsXY(projectInfo.coordinateDisplayCrs)
  property bool coordinatesIsGeographic: projectInfo.coordinateDisplayCrs.isGeographic

  property int ceilsCount: 4
  property double rowHeight: 30
  property color backgroundColor: "white"
  property color alternateBackgroundColor: Theme.navigationBackgroundColor
  property color textColor: "black"

  height: childrenRect.height
  width: parent.width
  anchors.margins: 20

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

      if (interval > 100) interval = interval * 0.8;
    }
  }

  ColumnLayout {
    width: parent.width
    spacing: 0

    Rectangle {
      Layout.fillWidth: true
      Layout.preferredHeight: childrenRect.height
      color: Theme.navigationColor

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
          iconSource: Theme.getThemeIcon("ic_chevron_left_white_24dp")

          onPressed: {
            navigation.previousDestinationVertex()
            featureVertexTimer.moveForward = false;
            featureVertexTimer.interval = 700
            featureVertexTimer.restart()
          }
          onReleased: {
            featureVertexTimer.stop()
          }
          onCanceled: {
            featureVertexTimer.stop()
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
          color: "#FFFFFF"
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
          iconSource: Theme.getThemeIcon("ic_chevron_right_white_24dp")

          onPressed: {
            navigation.nextDestinationVertex()
            featureVertexTimer.moveForward = true;
            featureVertexTimer.interval = 700
            featureVertexTimer.restart()
          }
          onReleased: {
            featureVertexTimer.stop()
          }
          onCanceled: {
            featureVertexTimer.stop()
          }
        }
      }
    }

    Grid {
      id: grid
      Layout.fillWidth: true
      Layout.preferredHeight: childrenRect.height
      width: parent.width
      height: grid.rows * navigationInformationView.rowHeight
      flow: GridLayout.TopToBottom
      rows: parent.width > 620? 1 : 2
      property double cellWidth: grid.width / ( ceilsCount / grid.rows )

      Rectangle {
        height: rowHeight
        width: grid.cellWidth
        color: alternateBackgroundColor

        Text {
          anchors.margins:  10
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          font: Theme.tipFont
          color: textColor
          text: coordinatesIsXY
                ? (coordinatesIsGeographic ? qsTr( "Lon" ) : qsTr( "X" )) + ': '
                  + Number( coordinates.x ).toLocaleString( Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3 )
                : (coordinatesIsGeographic ? qsTr( "Lat" ) : qsTr( "Y" )) + ': '
                  + Number( coordinates.y ).toLocaleString( Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3 )
        }
      }

      Rectangle {
        height: rowHeight
        width: grid.cellWidth
        color: backgroundColor

        Text {
          anchors.margins:  10
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          font: Theme.tipFont
          color: textColor
          text: coordinatesIsXY
                ? (coordinatesIsGeographic ? qsTr( "Lat" ) : qsTr( "Y" )) + ': '
                  + Number( coordinates.y ).toLocaleString( Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3 )
                : (coordinatesIsGeographic ? qsTr( "Lon" ) : qsTr( "X" )) + ': '
                  + Number( coordinates.x ).toLocaleString( Qt.locale(), 'f', coordinatesIsGeographic ? 7 : 3 )
        }
      }

      Rectangle {
        height: rowHeight
        width: grid.cellWidth
        color: grid.rows == 2 ? backgroundColor : alternateBackgroundColor

        Text {
          anchors.margins:  10
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          font: Theme.tipFont
          color: textColor
          text: qsTr( "Dist." ) + ': ' +
                ( positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid
                 ? ( UnitTypes.formatDistance( navigation.distance, 3, navigation.distanceUnits ) )
                 : qsTr( "N/A" ) )
        }
      }

      Rectangle {
        height: rowHeight
        width: grid.cellWidth
        color: grid.rows == 2 ? alternateBackgroundColor : backgroundColor

        Text {
          anchors.margins:  10
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          font: Theme.tipFont
          color: textColor
          text: qsTr( "Bearing" ) + ': ' +
                ( positionSource.active && positionSource.positionInformation && positionSource.positionInformation.latitudeValid
                 ? ( Number( navigation.bearing ).toLocaleString( Qt.locale(), 'f', 1 ) ) + '°'
                 : qsTr( "N/A" ) )
        }
      }
    }
  }
}
