

# File QfSensorInformationView.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfSensorInformationView.qml**](QfSensorInformationView_8qml.md)

[Go to the documentation of this file](QfSensorInformationView_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

Rectangle {
  id: sensorInformationView

  property alias activeSensors: grid.count

  property int ceilsCount: 4
  property double cellHeight: 26
  property double cellPadding: 6
  property color backgroundColor: QfTheme.mainBackgroundColor
  property color alternateBackgroundColor: QfTheme.sensorBackgroundColor
  property color textColor: QfTheme.mainTextColor
  property real contentHeight: parent.width > 620 ? cellHeight * Math.ceil(grid.count / 3) : cellHeight * Math.ceil(grid.count / 2)

  width: parent.width
  anchors.margins: 20

  color: QfTheme.mainBackgroundColor

  GridView {
    id: grid
    Layout.fillWidth: true
    Layout.preferredHeight: childrenRect.height
    width: parent.width
    height: parent.height
    cellWidth: parent.width > 620 ? parent.width / 3 : parent.width / 2
    cellHeight: sensorInformationView.cellHeight
    flow: GridLayout.TopToBottom

    model: QfSensorListModel {
      project: qgisProject
      showConnectedOnly: true
    }

    delegate: Rectangle {
      width: grid.cellWidth
      height: grid.cellHeight
      color: index % 2 == 0 ? sensorInformationView.alternateBackgroundColor : sensorInformationView.backgroundColor

      RowLayout {
        anchors.margins: cellPadding
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: parent.right

        Text {
          Layout.fillWidth: false
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          text: SensorName
        }

        Text {
          Layout.fillWidth: true
          font: QfTheme.tipFont
          color: sensorInformationView.textColor
          text: SensorLastValue ? (SensorLastValue + '').trim() : qsTr("N/A")
          verticalAlignment: Text.AlignVCenter
          elide: Text.ElideRight
        }
      }
    }
  }
}
```


