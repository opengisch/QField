import QtQuick 2.12
import QtQuick.Controls 2.12
import QtPositioning 5.8
import QtQuick.Layouts 1.12

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

Rectangle {
  id: navigationInformationView

  property Navigation navigation
  property int ceilsCount: 4
  property double rowHeight: 30
  property color backgroundColor: "white"
  property color alternateBackgroundColor: "#ffd3d3"
  property color textColor: "black"

  height: grid.rows * navigationInformationView.rowHeight
  width: parent.width
  anchors.margins: 20

  Grid {
    id: grid
    flow: GridLayout.TopToBottom
    rows: parent.width > 620? 1 : 2
    width: parent.width
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
        text: navigation.mapSettings.destinationCrs.isGeographic ?
                qsTr( "Lat." ) + ': ' + ( Number( navigation.destination.x ).toLocaleString( Qt.locale(), 'f', 7 ) )
              : qsTr( "X" )    + ': ' + ( Number( navigation.destination.x ).toLocaleString( Qt.locale(), 'f', 3 ) )
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
        text: positionSource.destinationCrs.isGeographic ?
                qsTr( "Lon." ) + ': ' + ( Number( navigation.destination.x ).toLocaleString( Qt.locale(), 'f', 7 ) )
              : qsTr( "Y" )    + ': ' + ( Number( navigation.destination.y ).toLocaleString( Qt.locale(), 'f', 3 ) )
      }
    }

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
        text: qsTr( "Dist." ) + ': ' + ( UnitTypes.formatDistance( navigation.distance, 3, navigation.distanceUnits ) )
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
        text: qsTr( "Bearing" ) + ': ' + ( Number( navigation.bearing ).toLocaleString( Qt.locale(), 'f', 1 ) ) + '°'
      }
    }
  }
}
