import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
QfDialog {
  property int selectedCount: 0
  property bool isDeleted: false
  property alias packagedLayersListViewModel: packagedLayersListView.model

  parent: mainWindow.contentItem
  width: mainWindow.width - Theme.popupScreenEdgeVerticalMargin * 2
  height: Math.min(300 + packagedLayersListView.contentHeight, mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4))

  ColumnLayout {
    id: layout
    anchors.fill: parent

    Label {
      Layout.fillWidth: true
      text: qsTr("Some layers have not been packaged correctly. These layers might be misconfigured or their data source is not accessible from the QFieldCloud server.")
      font: Theme.defaultFont
      wrapMode: Text.WordWrap
    }

    Label {
      Layout.fillWidth: true
      text: qsTr("Please check the detailed feedback below and the latest packaging job logs on the QFieldCloud website.")
      font: Theme.defaultFont
      wrapMode: Text.WordWrap
    }

    Rectangle {
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.topMargin: 10
      Layout.preferredHeight: packagedLayersListView.contentHeight
      color: Theme.controlBackgroundColor
      border.color: Theme.controlBorderColor
      border.width: 1

      ListView {
        id: packagedLayersListView
        anchors.fill: parent
        clip: true

        model: []

        delegate: Item {
          width: ListView.view.width
          height: descriptionText.contentHeight + 10

          Text {
            id: descriptionText
            anchors.centerIn: parent
            width: parent.width - 20
            text: modelData
            font: Theme.resultFont
            color: Theme.secondaryTextColor
            wrapMode: Text.Wrap
          }
        }
      }
    }
  }

  standardButtons: Dialog.Ok
}
