import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

GridLayout {
  property alias hours: hoursSpinBox.value
  property alias minutes: minutesSpinBox.value
  property alias seconds: secondsSpinBox.value
  property bool verticalView: false

  columns: verticalView ? 1 : 3

  Label {
    text: qsTr("Hours")
    font: Theme.strongTipFont
    Layout.fillWidth: true
    horizontalAlignment: Text.AlignHCenter
  }

  Label {
    text: qsTr("Minutes")
    font: Theme.strongTipFont
    Layout.fillWidth: true
    horizontalAlignment: Text.AlignHCenter
    Layout.column: verticalView ? 0 : 1
    Layout.row: verticalView ? 2 : 0
  }

  Label {
    text: qsTr("Seconds")
    font: Theme.strongTipFont
    Layout.fillWidth: true
    horizontalAlignment: Text.AlignHCenter
    Layout.column: verticalView ? 0 : 2
    Layout.row: verticalView ? 4 : 0
  }

  SpinBox {
    id: hoursSpinBox
    editable: true
    from: 0
    to: 23
    value: 12
    inputMethodHints: Qt.ImhTime
    font: Theme.tipFont
    Layout.fillWidth: true
    Layout.column: 0
    Layout.row: 1
  }

  SpinBox {
    id: minutesSpinBox
    editable: true
    from: 0
    to: 59
    value: 30
    inputMethodHints: Qt.ImhTime
    font: Theme.tipFont
    Layout.fillWidth: true
    Layout.column: verticalView ? 0 : 1
    Layout.row: verticalView ? 3 : 1
  }

  SpinBox {
    id: secondsSpinBox
    editable: true
    from: 0
    to: 59
    value: 30
    inputMethodHints: Qt.ImhTime
    font: Theme.tipFont
    Layout.fillWidth: true
    Layout.column: verticalView ? 0 : 2
    Layout.row: verticalView ? 5 : 1
  }
}
