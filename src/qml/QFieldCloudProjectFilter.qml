import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import org.qfield
import Theme

Pane {
  id: filterPanel

  readonly property var presets: [
    {label: qsTr("My Own Projects")},
    {label: qsTr("Projects shared with me")},
    {label: qsTr("OPENGIS.ch projects")},
    {label: qsTr("Showcased projects")}
  ]

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: 10
    spacing: 14

    Label {
      Layout.fillWidth: true
      text: qsTr("Predefined Filters")
      font.pointSize: Theme.secondaryTitleFont.pointSize
      color: Theme.mainTextColor
    }

    ListView {
      Layout.fillWidth: true
      Layout.preferredHeight: 40
      orientation: ListView.Horizontal
      spacing: 10
      clip: true
      boundsBehavior: Flickable.StopAtBounds
      model: filterPanel.presets

      delegate: QfButton {
        id: presetButtons
        width: implicitWidth + 32
        radius: 4
        //color: Theme.mainColor
        font.pointSize: Theme.tipFont.pointSize
        text: modelData.label
      }
    }

    Label {
      Layout.fillWidth: true
      text: qsTr("Criteria")
      font.pointSize: Theme.secondaryTitleFont.pointSize
      color: Theme.mainTextColor
    }

    Label {
      Layout.fillWidth: true
      text: qsTr("Title")
      font.pointSize: Theme.tipFont.pointSize
      color: Theme.mainTextColor
    }

    QfTextField {
      id: titleField
      Layout.fillWidth: true
    }

    Label {
      Layout.fillWidth: true
      text: qsTr("Owner")
      font.pointSize: Theme.tipFont.pointSize
      color: Theme.mainTextColor
    }

    QfComboBox {
      id: ownerCombo
      Layout.fillWidth: true
      editable: true
    }

    Label {
      Layout.fillWidth: true
      text: qsTr("Type")
      font.pointSize: Theme.tipFont.pointSize
      color: Theme.mainTextColor
    }

    QfComboBox {
      id: typeCombo
      Layout.fillWidth: true
    }

    RowLayout {
      Layout.alignment: Qt.AlignRight
      Layout.topMargin: 8

      Label {
        Layout.fillWidth: true
        text: qsTr("Include public projects")
        font.pointSize: Theme.tipFont.pointSize
        color: Theme.mainTextColor
      }

      Switch { id: publicSwitch }
    }

    RowLayout {
      Layout.alignment: Qt.AlignRight
      Layout.topMargin: 8
      spacing: 10

      QfButton {
        text: qsTr("Reset")
        onClicked: filterPanel.clear()
      }

      QfButton {
        text: qsTr("Search")
        onClicked: filterPanel.applied()
      }
    }
  }
}
