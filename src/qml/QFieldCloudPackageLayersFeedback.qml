import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
QfDialog {
  parent: mainWindow.contentItem

  property int selectedCount: 0
  property bool isDeleted: false
  property alias packagedLayersListViewModel: packagedLayersListView.model

  width: mainWindow.width - 20
  title: qsTr("QFieldCloud had troubles packaging your project")

  ColumnLayout {
    id: layout
    anchors.fill: parent

    Label {
      Layout.fillWidth: true
      text: qsTr("Some layers have not been packaged correctly on QFieldCloud. These layers might be misconfigured or their data source is not accessible from the QFieldCloud server. Please check the logs of the latest packaging job on the qfield.cloud website.")
      wrapMode: Text.WordWrap
    }

    ListView {
      id: packagedLayersListView
      model: []

      Layout.fillWidth: true
      Layout.topMargin: 10
      Layout.preferredHeight: Math.min(childrenRect.height, 300)

      delegate: Text {
        width: parent.width
        text: modelData
        font: Theme.resultFont
        color: Theme.secondaryTextColor
        wrapMode: Text.WordWrap
      }
    }
  }

  standardButtons: Dialog.Ok
}
