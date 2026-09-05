

# File QfPositioningInformationView.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfPositioningInformationView.qml**](QfPositioningInformationView_8qml.md)

[Go to the documentation of this file](QfPositioningInformationView_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

Rectangle {
  id: positioningInformationView

  property alias positionSource: positioningInformationModel.positioningSource
  property alias antennaHeight: positioningInformationModel.antennaHeight

  property color backgroundColor: "transparent"
  property color alternateBackgroundColor: QfTheme.positionBackgroundColor
  property color textColor: positionSource.currentness ? QfTheme.mainTextColor : QfTheme.secondaryTextColor
  property double cellHeight: 26
  property double cellPadding: 6
  property real contentHeight: grid.numberOfRows * cellHeight

  color: QfTheme.mainBackgroundColorSemiOpaque
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
      model: QfPositioningInformationModel {
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
            Layout.maximumWidth: parent.width * 0.66
            font: QfTheme.tipFont
            color: QfTheme.secondaryTextColor
            text: Name
            verticalAlignment: Text.AlignVCenter
            elide: Text.ElideRight
          }

          Text {
            Layout.fillWidth: true
            font: QfTheme.tipFont
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


