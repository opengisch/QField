import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

Rectangle {
  id: positioningInformationView

  property alias positionSource: positioningModel.positioningSource
  property alias antennaHeight: positioningModel.antennaHeight

  property color backgroundColor: "transparent"
  property color alternateBackgroundColor: Theme.positionBackgroundColor
  property color textColor: positionSource.currentness ? Theme.mainTextColor : Theme.secondaryTextColor
  property double cellHeight: 26
  property double cellPadding: 6
  property real contentHeight: grid.count / grid.numberOfColumns * cellHeight

  color: Theme.mainBackgroundColorSemiOpaque
  anchors.margins: 20
  width: parent.width

  GridView {
    id: grid

    readonly property real numberOfColumns: parent.width / cellWidth

    flow: GridView.FlowTopToBottom
    model: PositioningModel {
      id: positioningModel
      distanceUnits: projectInfo.distanceUnits
      coordinateDisplayCrs: projectInfo.coordinateDisplayCrs
    }
    anchors.fill: parent
    cellHeight: positioningInformationView.cellHeight
    cellWidth: {
      if (parent.width >= 1000)
        return parent.width / 4;
      else if (parent.width >= 620)
        return parent.width / 3;
      else if (parent.width >= 200)
        return parent.width / 2;
      return parent.width;
    }
    clip: true
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.vertical: QfScrollBar {
    }
    Component.onCompleted: {
      positioningModel.setupConnections();
    }
    delegate: Rectangle {
      readonly property real currentColumn: parseInt(index / (grid.count / grid.numberOfColumns))

      width: grid.cellWidth
      height: grid.cellHeight
      color: {
        if (currentColumn % 2 == 0) {
          return index % 2 == 0 ? alternateBackgroundColor : backgroundColor;
        } else {
          return index % 2 == 0 ? backgroundColor : alternateBackgroundColor;
        }
      }

      RowLayout {
        anchors.margins: cellPadding
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right

        Text {
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: Name
        }

        Text {
          Layout.fillWidth: true
          font: Theme.tipFont
          color: positioningInformationView.textColor
          text: Value ? Value : qsTr("N/A")
          verticalAlignment: Text.AlignVCenter
          elide: Text.ElideRight
        }
      }
    }
  }
}
