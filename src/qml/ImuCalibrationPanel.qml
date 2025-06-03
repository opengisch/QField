import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0
import Theme 1.0

Item {
  id: imuPanel
  width: parent.width

  property var positionSource

  GridLayout {
    id: grid
    columns: 2
    columnSpacing: 6
    rowSpacing: 4

    Label { text: qsTr("Offset X") }
    QfTextField { id: offsetX; text: "0" }

    Label { text: qsTr("Offset Y") }
    QfTextField { id: offsetY; text: "0" }

    Label { text: qsTr("Offset Z") }
    QfTextField { id: offsetZ; text: "0" }

    Label { text: qsTr("Std X") }
    QfTextField { id: stdX; text: "0" }

    Label { text: qsTr("Std Y") }
    QfTextField { id: stdY; text: "0" }

    Label { text: qsTr("Std Z") }
    QfTextField { id: stdZ; text: "0" }

    QfButton {
      text: qsTr("Calibrate")
      Layout.columnSpan: 2
      onClicked: {
        if (positionSource && positionSource.device)
          positionSource.device.configImuToAntOffset(parseFloat(offsetX.text), parseFloat(offsetY.text), parseFloat(offsetZ.text),
                                                    parseFloat(stdX.text), parseFloat(stdY.text), parseFloat(stdZ.text))
      }
    }
  }
}
