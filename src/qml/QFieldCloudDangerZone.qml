import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Item {
  id: dangerZone

  signal discardRequested
  signal resetRequested

  ScrollView {
    anchors.fill: parent

    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical: QfScrollBar {}
    contentWidth: width
    contentHeight: dangerZoneColumn.height
    clip: true

    ColumnLayout {
      id: dangerZoneColumn
      width: parent.width
      spacing: 10

      QfCollapsibleMessage {
        Layout.fillWidth: true
        Layout.margins: 10
        color: Theme.darkRed
        font: Theme.tipFont
        iconSource: Theme.getThemeVectorIcon('ic_error_outline_24dp')
        titleText: qsTr('The action below is irreversible and permanently affects your local data. Proceed with caution.')
      }

      QfButton {
        id: discardButton
        Layout.fillWidth: true
        Layout.leftMargin: 10
        Layout.rightMargin: 10
        bgcolor: Theme.darkRed
        text: cloudProjectsModel.layerObserver.deltaFileWrapper && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError ? qsTr('Discard local changes') : qsTr('Reset project')
        enabled: cloudProjectsModel.layerObserver.deltaFileWrapper && (cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0 || cloudProjectsModel.layerObserver.deltaFileWrapper.hasError)
        icon.source: Theme.getThemeVectorIcon('ic_undo_black_24dp')

        onClicked: {
          if (!cloudProjectsModel.layerObserver.deltaFileWrapper.hasError) {
            dangerZone.discardRequested();
          } else {
            dangerZone.resetRequested();
          }
        }
      }

      Text {
        id: discardText
        Layout.fillWidth: true
        Layout.leftMargin: 10
        Layout.rightMargin: 10
        Layout.bottomMargin: 10
        font: Theme.tipFont
        color: Theme.secondaryTextColor
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter
        text: cloudProjectsModel.layerObserver.deltaFileWrapper && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError ? qsTr('Revert all modified features in the local layers. You cannot restore those changes.') : qsTr('The local copy of this cloud project has been corrupted. Resetting the project will re-download the cloud version and will remove any local changes, make sure those were copied first if needed.\n\nWhile you can still view and use the project, it is strongly recommended to reset to avoid any accidental data loss as none of the changes made will be pushed back to the cloud.')
      }
    }
  }
}
