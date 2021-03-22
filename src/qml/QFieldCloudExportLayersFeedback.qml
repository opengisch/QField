import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qfield 1.0
import Theme 1.0

// QFieldCloudExportLayersFeedback
Dialog {
  parent: mainWindow.contentItem

  property int selectedCount: 0
  property bool isDeleted: false
  property alias exportedLayersListViewModel: exportedLayersListView.model

  visible: false
  modal: true
  width: mainWindow.width - 20

  x: ( mainWindow.width - width ) / 2
  y: ( mainWindow.height - height ) / 2

  title: qsTr("Problematic QFieldCloud layer exports")

  ColumnLayout {
    id: layout

    anchors.fill: parent

    Label {
      Layout.fillWidth: true

      text: qsTr( "Some layers have not been exported correctly on QFieldCloud. These layers might be misconfigured or the data source is not accessible from the QFieldCloud server." )
      wrapMode: Text.WordWrap
    }

    ListView {
      id: exportedLayersListView
      model: []

      Layout.fillWidth: true
      Layout.topMargin: 10
      Layout.preferredHeight: Math.min(childrenRect.height, 300)

      delegate: Text {
        width: parent.width
        text: modelData
        font: Theme.resultFont
        wrapMode: Text.WordWrap
      }
    }
  }

  standardButtons: Dialog.Ok
}

