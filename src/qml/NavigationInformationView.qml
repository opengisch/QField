import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

Rectangle {
  id: navigationInformationView

  property var coordinates: projectInfo.reprojectDisplayCoordinatesToWGS84
                                    ? GeometryUtils.reprojectPointToWgs84(navigation.destination, navigation.mapSettings.destinationCrs)
                                    : navigation.destination
  property bool coordinatesIsXY: !projectInfo.reprojectDisplayCoordinatesToWGS84
                                && CoordinateReferenceSystemUtils.defaultCoordinateOrderForCrsIsXY(navigation.mapSettings.destinationCrs)
  property bool coordinatesIsGeographic: projectInfo.reprojectDisplayCoordinatesToWGS84
                                        || navigation.mapSettings.destinationCrs.isGeographic

  property Navigation navigation
  property int ceilsCount: 4
  property double rowHeight: 30
  property color backgroundColor: "white"
  property color alternateBackgroundColor: Theme.navigationBackgroundColor
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
