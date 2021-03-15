import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qfield 1.0
import Theme 1.0

Popup {
  id: popup
  padding: 0

  property alias exportedLayersListViewModel: exportedLayersListView.model

  Page {
    anchors.fill: parent

    header: PageHeader {
      title: qsTr('QFieldCloud')

      showCancelButton: false
      showApplyButton: cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Idle
      busyIndicatorState: 'off'

      onFinished: {
        popup.close()
      }
    }

      GridLayout {
        id: mainGrid
        anchors.fill: parent
        columns: 1
        columnSpacing: 2
        rowSpacing: 2

        GridLayout {
          Layout.margins: 10
          Layout.maximumWidth: 525
          Layout.alignment: Qt.AlignHCenter
          id: mainInnerGrid
          width: parent.width
          visible: cloudConnection.status === QFieldCloudConnection.LoggedIn
                   && cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Idle
          columns: 1
          columnSpacing: parent.columnSpacing
          rowSpacing: parent.rowSpacing

          Text {
            id: transferErrorText
            visible: true
            font: Theme.defaultFont
            text: exportedLayersListViewModel.length === 1
                  ? qsTr("One layer was not exported correctly:")
                  : qsTr("Some layers were not exported correctly:")
            color: Theme.darkRed
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
          }

          Item {

            Layout.bottomMargin: 20
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
              id: exportedLayersListView
              anchors.fill: parent

              delegate:
                Text {
                  width: parent.width
                  text: modelData
                  wrapMode: Text.WordWrap
                }
            }
          }
        }
      }
  }

  function show() {
    visible = true
  }
}
