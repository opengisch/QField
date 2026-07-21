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

  readonly property bool hasDeltaError: !!(cloudProjectsModel.layerObserver.deltaFileWrapper && cloudProjectsModel.layerObserver.deltaFileWrapper.hasError)

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

      QfContainerCard {
        Layout.fillWidth: true
        Layout.margins: 10
        Layout.maximumWidth: 525
        Layout.alignment: Qt.AlignHCenter
        accentColor: Theme.darkRed
        iconSource: Theme.getThemeVectorIcon('ic_delete_forever_white_24dp')
        title: dangerZone.hasDeltaError ? qsTr('Reset project') : qsTr('Discard local changes')
        description: dangerZone.hasDeltaError ? qsTr('The local copy of this cloud project has been corrupted. Resetting the project will re-download the cloud version and will remove any local changes, make sure those were copied first if needed.\n\nWhile you can still view and use the project, it is strongly recommended to reset to avoid any accidental data loss as none of the changes made will be pushed back to the cloud.') : qsTr('Removes all your local edits that have not yet been uploaded.')

        QfButton {
          Layout.fillWidth: true
          Layout.topMargin: 4

          bgcolor: Theme.darkRed
          color: Theme.light
          text: dangerZone.hasDeltaError ? qsTr('Reset') : qsTr('Discard')
          enabled: cloudProjectsModel.layerObserver.deltaFileWrapper && (cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0 || dangerZone.hasDeltaError)

          onClicked: {
            if (!dangerZone.hasDeltaError) {
              dangerZone.discardRequested();
            } else {
              dangerZone.resetRequested();
            }
          }
        }
      }
    }
  }
}
