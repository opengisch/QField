import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

Rectangle {
  id: sensorInformationView

  property alias activeSensors: grid.count

  property int ceilsCount: 4
  property double rowHeight: 30
  property color backgroundColor: Theme.mainBackgroundColor
  property color alternateBackgroundColor: Theme.sensorBackgroundColor
  property color textColor: Theme.mainTextColor

  width: parent.width
  height: parent.width > 620
          ? rowHeight * Math.ceil(grid.count / 3)
          : rowHeight * Math.ceil(grid.count / 2)
  anchors.margins: 20

  color: Theme.mainBackgroundColor

  GridView {
    id: grid
    Layout.fillWidth: true
    Layout.preferredHeight: childrenRect.height
    width: parent.width
    height: childrenRect.height
    cellWidth: parent.width > 620
               ? parent.width / 3
               : parent.width / 2
    cellHeight: rowHeight
    flow: GridLayout.TopToBottom

    model: SensorListModel {
      project: qgisProject
      showConnectedOnly: true
    }

    delegate: Rectangle {
      width: grid.cellWidth
      height: grid.cellHeight
      color: index % 2 == 0 ? alternateBackgroundColor : backgroundColor

      Text {
        anchors.margins:  10
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        width: grid.cellWidth - 20
        height: grid.cellHeight - 20
        font: Theme.tipFont
        color: textColor
        text: SensorName + ': ' + (SensorLastValue ? (SensorLastValue + '').trim() : 'N/A')
        verticalAlignment: Text.AlignVCenter
        elide: Text.ElideRight
      }
    }
  }
}
