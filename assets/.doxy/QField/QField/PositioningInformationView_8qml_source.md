

# File PositioningInformationView.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**PositioningInformationView.qml**](PositioningInformationView_8qml.md)

[Go to the documentation of this file](PositioningInformationView_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

Rectangle {
  id: positioningInformationView

  property alias positionSource: positioningInformationModel.positioningSource
  property alias antennaHeight: positioningInformationModel.antennaHeight

  property color backgroundColor: "transparent"
  property color alternateBackgroundColor: Theme.positionBackgroundColor
  property color textColor: positionSource.currentness ? Theme.mainTextColor : Theme.secondaryTextColor
  property double cellHeight: 26
  property double cellPadding: 6
  property real contentHeight: grid.numberOfRows * cellHeight

  color: Theme.mainBackgroundColorSemiOpaque
  anchors.margins: 20
  width: parent.width
  clip: true

  Flickable {
    width: parent.width
    height: positioningInformationView.height
    contentHeight: grid.height
    boundsBehavior: Flickable.StopAtBounds
    ScrollBar.vertical: QfScrollBar {}

    GridView {
      id: grid

      readonly property real numberOfColumns: parent.width > 1000 ? 6 : parent.width > 620 ? 3 : 2
      readonly property real numberOfRows: Math.ceil(grid.count / numberOfColumns)

      flow: GridView.FlowTopToBottom
      boundsBehavior: Flickable.StopAtBounds
      model: PositioningInformationModel {
        id: positioningInformationModel
        distanceUnits: projectInfo.distanceUnits
        coordinateDisplayCrs: projectInfo.coordinateDisplayCrs
      }
      height: grid.numberOfRows * cellHeight
      width: parent.width
      cellHeight: positioningInformationView.cellHeight
      cellWidth: parent.width / numberOfColumns
      clip: true

      delegate: Rectangle {
        readonly property real currentColumn: parseInt(index / grid.numberOfRows)
        readonly property real currentRow: index % grid.numberOfRows

        width: grid.cellWidth
        height: grid.cellHeight
        color: {
          if (currentColumn % 2 == 0) {
            return currentRow % 2 == 0 ? alternateBackgroundColor : backgroundColor;
          } else {
            return currentRow % 2 == 0 ? backgroundColor : alternateBackgroundColor;
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
}
```


