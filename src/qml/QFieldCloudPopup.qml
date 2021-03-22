import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qfield 1.0
import Theme 1.0

Popup {
  id: popup
  padding: 0

  Page {
    anchors.fill: parent

    header: PageHeader {
      title: qsTr('QFieldCloud')

      showCancelButton: false
      showApplyButton: cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Idle
      busyIndicatorState: cloudConnection.status === QFieldCloudConnection.Connecting
            || cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Uploading
            || cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Downloading
            ? 'on'
            : 'off'

      onFinished: {
        popup.close()
      }
    }

    ColumnLayout {
      visible: cloudProjectsModel.currentProjectId && cloudConnection.status !== QFieldCloudConnection.LoggedIn
      id: connectionSettings
      anchors.fill: parent
      spacing: 2

      ScrollView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: 10
        height: parent.height
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical.policy: ScrollBar.AsNeeded
        contentWidth: qfieldCloudLogin.width
        contentHeight: qfieldCloudLogin.childrenRect.height
        clip: true

        QFieldCloudLogin {
          id: qfieldCloudLogin
          width: parent.parent.width
        }
      }

      Item {
          Layout.fillHeight: true
          height: 15
      }
    }

    ColumnLayout {
      visible: !cloudProjectsModel.currentProjectId
      anchors.fill: parent
      anchors.margins: 20
      anchors.topMargin: 50
      spacing: 2

      Text {
        Layout.fillWidth: true
        font: Theme.defaultFont
        text: '<style>a, a:hover, a:visited { color:' + Theme.mainColor + '; }></style>' +
              qsTr('The current project is not stored on QFieldCloud.<br><br>') +
              qsTr('Storing projects on QFieldCloud offers seamless synchornization, offline editing, and team management.<br><br>') +
              ' <a href="https://qfield.cloud/">' + qsTr( 'Learn more about QFieldCloud' ) + '</a>.'
        textFormat: Text.RichText
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter

        onLinkActivated: Qt.openUrlExternally(link)
      }

      Item {
          Layout.fillHeight: true
          height: 15
      }
    }

    ScrollView {
      visible: cloudProjectsModel.currentProjectId && cloudConnection.status === QFieldCloudConnection.LoggedIn
      padding: 0
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical.policy: ScrollBar.AsNeeded
      contentWidth: mainGrid.width
      contentHeight: mainGrid.height
      anchors.fill: parent
      clip: true

      GridLayout {
        id: mainGrid
        width: parent.parent.width
        columns: 1
        columnSpacing: 2
        rowSpacing: 2

        RowLayout {
          Text {
              Layout.fillWidth: true
              Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
              id: welcomeText
              padding: 10
              text: switch(cloudConnection.status) {
                    case 0: qsTr( 'Disconnected from the cloud.' ); break;
                    case 1: qsTr( 'Connecting to the cloud.' ); break;
                    case 2: qsTr( 'Greetings %1.' ).arg( cloudConnection.username ); break;
                  }
              wrapMode: Text.WordWrap
              font: Theme.tipFont
          }

          Rectangle {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.margins: 10
            width: 48
            height: 48
            color: 'transparent'

            Image {
              id: cloudAvatar
              anchors.fill: parent
              fillMode: Image.PreserveAspectFit
              smooth: true
              source: 'qrc:/images/qfieldcloud_logo.svg'
              width: 48
              height: 48
              sourceSize.width: width * screen.devicePixelRatio
              sourceSize.height: height * screen.devicePixelRatio
            }
          }
        }

        Text {
          id: statusText
          visible: cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Downloading ||
                   cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Uploading
          font: Theme.defaultFont
          text: switch(cloudProjectsModel.currentProjectData.Status ) {
                  case QFieldCloudProjectsModel.Downloading:
                    switch ( cloudProjectsModel.currentProjectData.ExportStatus ) {
                      case QFieldCloudProjectsModel.ExportFinishedStatus:
                        return qsTr('Downloading %1%…').arg( parseInt(cloudProjectsModel.currentProjectData.DownloadProgress * 100) )
                      default:
                        return qsTr('QFieldCloud is preparing the latest data just for you.\nThis might take some time, please hold tight…')
                    }
                  case QFieldCloudProjectsModel.Uploading:
                    switch ( cloudProjectsModel.currentProjectData.UploadDeltaStatus ) {
                      case QFieldCloudProjectsModel.DeltaFileLocalStatus:
                        return qsTr('Uploading %1%…').arg( parseInt(cloudProjectsModel.currentProjectData.UploadDeltaProgress * 100) );
                      default:
                        return qsTr('QFieldCloud is applying the latest uploaded changes.\nThis might take some time, please hold tight…')
                    }
                  default: '';
                }

          wrapMode: Text.WordWrap
          horizontalAlignment: Text.AlignHCenter
          Layout.fillWidth: true
          Layout.leftMargin: 10
          Layout.rightMargin: 10
        }

        Rectangle {
          Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
          Layout.margins: 10
          width: 64
          height: 64
          color: 'transparent'
          visible: cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Downloading ||
                   cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Uploading

          Image {
            id: statusIcon
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            smooth: true
            source: switch(cloudProjectsModel.currentProjectData.Status ) {
                    case QFieldCloudProjectsModel.Downloading:
                      switch ( cloudProjectsModel.currentProjectData.ExportStatus ) {
                        case QFieldCloudProjectsModel.ExportFinishedStatus:
                          return Theme.getThemeVectorIcon('ic_cloud_download_24dp');
                        default:
                          return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
                      }
                    case QFieldCloudProjectsModel.Uploading:
                      switch ( cloudProjectsModel.currentProjectData.UploadDeltaStatus ) {
                        case QFieldCloudProjectsModel.DeltaFileLocalStatus:
                          return Theme.getThemeVectorIcon('ic_cloud_upload_24dp');
                        default:
                          return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
                      }
                    default: '';
                  }
            width: parent.width
            height: parent.height
            sourceSize.width: width * screen.devicePixelRatio
            sourceSize.height: height * screen.devicePixelRatio
            opacity: 1

            SequentialAnimation {
              OpacityAnimator {
                  from: 1
                  to: 0.2
                  duration: 2000
                  target: statusIcon
              }
              OpacityAnimator {
                  from: 0.2
                  to: 1
                  duration: 2000
                  target: statusIcon
              }
              running: true
              loops: Animation.Infinite
            }
          }
        }

        Text {
          id: transferErrorText
          visible: false
          font: Theme.defaultFont
          text: ''
          color: Theme.darkRed
          wrapMode: Text.WordWrap
          horizontalAlignment: Text.AlignHCenter
          Layout.fillWidth: true

          Connections {
            target: cloudProjectsModel

            function onPushFinished(projectId, hasError, errorString) {
              transferErrorText.visible = hasError && cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Idle;

              if (transferErrorText.visible)
                transferErrorText.text = errorString
            }

            function onProjectDownloaded(projectId, projectName, hasError, errorString) {
              const projectData = cloudProjectsModel.getProjectData(projectId)

              transferErrorText.visible = hasError && projectData.Status === QFieldCloudProjectsModel.Idle;

              if (transferErrorText.visible)
                transferErrorText.text = errorString

              if (projectData.ExportedLayerErrors.length !== 0)
              {
                cloudExportLayersFeedback.exportedLayersListViewModel = projectData.ExportedLayerErrors;
                cloudExportLayersFeedback.visible = true;
              }
            }

            function onDataChanged() {
              transferErrorText.visible = cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Idle;
            }
          }
        }

        GridLayout {
          Layout.margins: 10
          Layout.maximumWidth: 525
          Layout.alignment: Qt.AlignHCenter
          id: mainInnerGrid
          width: parent.width
          visible: cloudConnection.status === QFieldCloudConnection.LoggedIn &&
                   cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Idle
          columns: 1
          columnSpacing: parent.columnSpacing
          rowSpacing: parent.rowSpacing

          Text {
            property int changesCount: cloudProjectsModel.layerObserver.deltaFileWrapper.count
            id: changesText
            font: Theme.tipFont
            color: Theme.gray
            text: changesCount === 0
                  ? qsTr('There are no local changes.')
                  : changesCount === 1
                    ? qsTr('There is a single local change.')
                    : qsTr('There are %1 local changes.').arg( changesCount )
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.bottomMargin: 20
            Layout.fillWidth: true
          }

          QfButton {
            id: syncButton
            Layout.fillWidth: true
            font: Theme.defaultFont
            text: qsTr('Synchronize')
            enabled: !!(cloudProjectsModel.currentProjectData && cloudProjectsModel.currentProjectData.CanSync)
            icon.source: Theme.getThemeIcon('ic_cloud_download_24dp')
            icon.color: 'white'

            onClicked: uploadProject(true)
          }

          Text {
            id: syncText
            font: Theme.tipFont
            color: Theme.gray
            text: qsTr('Synchronize the whole project with all modified features and download the freshly updated project with all the applied changes from QFieldCloud.')
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.bottomMargin: 20
            Layout.fillWidth: true
          }

          QfButton {
            id: pushButton
            Layout.fillWidth: true
            font: Theme.defaultFont
            text: qsTr('Push changes')
            enabled: !!(cloudProjectsModel.currentProjectData && cloudProjectsModel.currentProjectData.CanSync) && cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0
            icon.source: Theme.getThemeIcon('ic_cloud_upload_24dp')
            icon.color: 'white'

            onClicked: uploadProject(false)
          }

          Text {
            id: pushText
            font: Theme.tipFont
            color: Theme.gray
            text: qsTr('Save internet bandwidth by only pushing the local features and pictures to the cloud, without updating the whole project.')
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.bottomMargin: 20
            Layout.fillWidth: true
          }

          QfButton {
            id: discardButton
            Layout.fillWidth: true
            font: Theme.defaultFont
            bgcolor: Theme.darkRed
            text: qsTr('Revert local changes')
            enabled: cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0
            icon.source: Theme.getThemeIcon('ic_undo_white_24dp')
            icon.color: 'white'

            onClicked: {
              revertDialog.open();
            }

            onPressAndHold: {
              discardDialog.open();
            }
          }

          Text {
            id: discardText
            font: Theme.tipFont
            color: Theme.gray
            text: qsTr('Revert all modified features in the local cloud layers. You cannot restore those changes.')
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.bottomMargin: 10
            Layout.fillWidth: true
          }

          Rectangle {
            color: Theme.lightGray
            height: 1
            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.bottomMargin: 5
          }

          Text {
            id: lastExportText
            font: Theme.tipFont
            color: Theme.gray
            text: {
              var dt = cloudProjectsModel.currentProjectData.LastLocalExport

              if (!dt)
                return ''

              dt = new Date(dt)

              if (dt.toLocaleDateString() === new Date().toLocaleDateString())
                return qsTr( 'Last cloud export at ' ) + dt.toLocaleTimeString()
              else
                return qsTr( 'Last cloud export on ' ) + dt.toLocaleString()
            }
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
          }

          Text {
            id: lastPushDeltasText
            font: Theme.tipFont
            color: Theme.gray
            text: {
              var dt = cloudProjectsModel.currentProjectData.LastLocalPushDeltas

              if (!dt)
                return qsTr( 'No changes pushed yet' )

              dt = new Date(dt)

              if (dt.toLocaleDateString() === new Date().toLocaleDateString())
                return qsTr( 'Last changes push at ' ) + dt.toLocaleTimeString()
              else
                return qsTr( 'Last changes push on ' ) + dt.toLocaleString()
            }
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.bottomMargin: 10
            Layout.fillWidth: true
          }
        }
      }
    }
  }

  Connections {
    target: cloudConnection

    function onStatusChanged() {
      if (cloudConnection.status === QFieldCloudConnection.Disconnected) {
        visible = false
        displayToast(qsTr('Logged out'))
      } else if (cloudConnection.status === QFieldCloudConnection.Connecting) {
        displayToast(qsTr('Connecting...'))
      } else if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
        displayToast(qsTr('Logged in'))
      }
    }
  }


  Dialog {
    id: revertDialog
    parent: mainWindow.contentItem

    property int selectedCount: 0
    property bool isDeleted: false

    visible: false
    modal: true

    x: ( mainWindow.width - width ) / 2
    y: ( mainWindow.height - height ) / 2

    title: qsTr( "Revert local changes" )
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr( "Should local changes be reverted?" )
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
      revertLocalChangesFromCurrentProject();
    }
    onRejected: {
      visible = false
      popup.focus = true;
    }
  }

  Dialog {
    id: discardDialog
    parent: mainWindow.contentItem

    property int selectedCount: 0
    property bool isDeleted: false

    visible: false
    modal: true

    x: ( mainWindow.width - width ) / 2
    y: ( mainWindow.height - height ) / 2

    title: qsTr( "Discard local changes" )
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr( "Discarding local changes may result in QFieldCloud conflicts. Should local changes be discarded?" )
    }

    standardButtons: Dialog.Ok | Dialog.Cancel

    onAccepted: {
      discardLocalChangesFromCurrentProject();
    }
    onRejected: {
      visible = false
      popup.focus = true;
    }
  }

  function show() {
    visible = !visible

    if ( cloudProjectsModel.currentProjectId && cloudConnection.hasToken && cloudConnection.status === QFieldCloudConnection.Disconnected )
      cloudConnection.login();

    if ( cloudConnection.status === QFieldCloudConnection.Connectiong )
      displayToast(qsTr('Connecting cloud'))
  }

  function uploadProject(shouldDownloadUpdates) {
    if (cloudProjectsModel.currentProjectData && cloudProjectsModel.currentProjectData.CanSync) {
      cloudProjectsModel.uploadProject(cloudProjectsModel.currentProjectId, shouldDownloadUpdates)
      return
    }
  }

  function revertLocalChangesFromCurrentProject() {
    if (cloudProjectsModel.currentProjectData && cloudProjectsModel.currentProjectData.CanSync) {
      if ( cloudProjectsModel.revertLocalChangesFromCurrentProject(cloudProjectsModel.currentProjectId) )
        displayToast(qsTr('Local changes reverted'))
      else
        displayToast(qsTr('Failed to revert changes'))

      return
    }

    displayToast(qsTr('No changes to revert'))
  }

  function discardLocalChangesFromCurrentProject() {
    if (cloudProjectsModel.currentProjectData && cloudProjectsModel.currentProjectData.CanSync) {
      if ( cloudProjectsModel.discardLocalChangesFromCurrentProject(cloudProjectsModel.currentProjectId) )
        displayToast(qsTr('Local changes discarded'))
      else
        displayToast(qsTr('Failed to discard changes'))

      return
    }

    displayToast(qsTr('No changes to discard'))
  }
}
