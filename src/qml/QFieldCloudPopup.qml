import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Popup {
  id: popup
  padding: 0

  Page {
    anchors.fill: parent

    header: QfPageHeader {
      title: qsTr('QFieldCloud')

      showBackButton: true
      showCancelButton: false
      showApplyButton: false

      busyIndicatorState: cloudConnection.status === QFieldCloudConnection.Connecting || cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Uploading || cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Downloading ? 'on' : 'off'

      topMargin: mainWindow.sceneTopMargin

      onFinished: {
        if (connectionSettings.visible) {
          if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
            connectionSettings.visible = false;
          } else {
            popup.close();
          }
        } else {
          popup.close();
        }
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
        color: Theme.mainTextColor
        text: qsTr('The current project is not stored on QFieldCloud.<br><br>') + qsTr('Storing projects on QFieldCloud offers seamless synchronization, offline editing, and team management.<br><br>') + ' <a href="https://qfield.cloud/">' + qsTr('Learn more about QFieldCloud') + '</a>.'
        textFormat: Text.RichText
        wrapMode: Text.WordWrap
        horizontalAlignment: Text.AlignHCenter

        onLinkActivated: link => {
          Qt.openUrlExternally(link);
        }
      }

      Item {
        Layout.fillHeight: true
        height: 15
      }
    }

    ScrollView {
      visible: cloudProjectsModel.currentProjectId
      padding: 0
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical: QfScrollBar {
      }
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
          id: connectionInformation
          visible: cloudConnection.status === QFieldCloudConnection.LoggedIn

          Text {
            id: welcomeText
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            padding: 10
            text: switch (cloudConnection.status) {
            case 0:
              qsTr('Disconnected from the cloud.');
              break;
            case 1:
              qsTr('Connecting to the cloud.');
              break;
            case 2:
              qsTr('Greetings <strong>%1</strong>.').arg(cloudConnection.username);
              break;
            }
            wrapMode: Text.WordWrap
            font: Theme.tipFont
            color: Theme.mainTextColor
          }

          Rectangle {
            id: cloudAvatarRect
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.margins: 10
            width: 48
            height: 48
            radius: width / 2
            border.color: Theme.mainColor
            border.width: 1
            clip: true

            Rectangle {
              id: cloudAvatarMask
              anchors.centerIn: parent
              width: 46
              height: 46
              radius: width / 2
              color: "white"
              visible: false
              layer.enabled: true
            }

            Image {
              id: cloudAvatar
              anchors.fill: parent
              anchors.margins: 2
              fillMode: Image.PreserveAspectCrop
              smooth: true
              source: cloudConnection.avatarUrl !== '' ? cloudConnection.avatarUrl : 'qrc:/images/qfieldcloud_logo.svg'
              width: 48
              height: 48
              sourceSize.width: width * screen.devicePixelRatio
              sourceSize.height: height * screen.devicePixelRatio
              layer.enabled: true
              layer.effect: QfOpacityMask {
                maskSource: cloudAvatarMask
              }

              onStatusChanged: {
                // In case the avatar URL fails to load or the image is corrupted, revert to our lovely Nyuki
                if (status == Image.Error) {
                  source = 'qrc:/images/qfieldcloud_logo.svg';
                }
              }

              MouseArea {
                anchors.fill: parent

                onClicked: {
                  if (cloudConnection.status !== QFieldCloudConnection.LoggedIn || cloudProjectsModel.currentProjectData.Status !== QFieldCloudProjectsModel.Idle)
                    return;
                  if (!connectionSettings.visible) {
                    connectionSettings.visible = true;
                    projects.visible = false;
                  } else {
                    connectionSettings.visible = false;
                    projects.visible = true;
                  }
                }
              }
            }
          }
        }

        Text {
          id: wrongAccountText
          visible: cloudProjectsModel.currentProjectId != '' && cloudProjectsModel.currentProjectId !== cloudProjectsModel.currentProjectData.Id
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: qsTr('This QFieldCloud project was first downloaded with another cloud account. Please sign in with the original account for this project to use the QFieldCloud functionality.')

          wrapMode: Text.WordWrap
          horizontalAlignment: Text.AlignHCenter
          Layout.fillWidth: true
          Layout.margins: 10
        }

        Text {
          id: statusText
          visible: cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Downloading || cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Uploading
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: switch (cloudProjectsModel.currentProjectData.Status) {
          case QFieldCloudProjectsModel.Downloading:
            if (cloudProjectsModel.currentProjectData.PackagingStatus === QFieldCloudProjectsModel.PackagingBusyStatus) {
              return qsTr('QFieldCloud is packaging the latest data just for you; this might take some time, please hold tight');
            } else {
              if (cloudProjectsModel.currentProjectData.PackagingStatus === QFieldCloudProjectsModel.PackagingFinishedStatus || cloudProjectsModel.currentProjectData.DownloadProgress > 0.0) {
                if (cloudProjectsModel.currentProjectData.DownloadSize > 0) {
                  return qsTr('Downloading, %1% of %2 fetched').arg(Math.round(cloudProjectsModel.currentProjectData.DownloadProgress * 100)).arg(FileUtils.representFileSize(cloudProjectsModel.currentProjectData.DownloadSize));
                } else {
                  return qsTr('Downloading, %1% fetched').arg(Math.round(cloudProjectsModel.currentProjectData.DownloadProgress * 100));
                }
              } else {
                return qsTr('Reaching out to QFieldCloud to download project');
              }
            }
          case QFieldCloudProjectsModel.Uploading:
            switch (cloudProjectsModel.currentProjectData.UploadDeltaStatus) {
            case QFieldCloudProjectsModel.DeltaFileLocalStatus:
              return qsTr('Uploading %1%…').arg(Math.round(cloudProjectsModel.currentProjectData.UploadDeltaProgress * 100));
            default:
              return qsTr('QFieldCloud is applying the latest uploaded changes. This might take some time, please hold tight…');
            }
          default:
            '';
          }

          wrapMode: Text.WordWrap
          horizontalAlignment: Text.AlignHCenter
          Layout.fillWidth: true
          Layout.leftMargin: 10
          Layout.rightMargin: 10
          Layout.topMargin: 60
          Layout.bottomMargin: 20
        }

        Rectangle {
          id: cloudAnimation
          Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
          Layout.margins: 10
          width: 128
          height: 128
          color: 'transparent'
          visible: cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Downloading || cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Uploading

          Image {
            id: statusIcon
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            smooth: true
            source: switch (cloudProjectsModel.currentProjectData.Status) {
            case QFieldCloudProjectsModel.Downloading:
              switch (cloudProjectsModel.currentProjectData.PackagingStatus) {
              case QFieldCloudProjectsModel.PackagingFinishedStatus || cloudProjectsModel.currentProjectData.DownloadProgress > 0.0:
                return Theme.getThemeVectorIcon('ic_cloud_download_24dp');
              default:
                return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
              }
            case QFieldCloudProjectsModel.Uploading:
              switch (cloudProjectsModel.currentProjectData.UploadDeltaStatus) {
              case QFieldCloudProjectsModel.DeltaFileLocalStatus:
                return Theme.getThemeVectorIcon('ic_cloud_upload_24dp');
              default:
                return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
              }
            default:
              '';
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
              running: cloudAnimation.visible
              loops: Animation.Infinite
            }
          }

          ProgressBar {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: 6
            indeterminate: cloudProjectsModel.currentProjectData.PackagingStatus !== QFieldCloudProjectsModel.PackagingFinishedStatus && cloudProjectsModel.currentProjectData.DownloadProgress === 0.0
            value: cloudProjectsModel.currentProjectData.DownloadProgress ? cloudProjectsModel.currentProjectData.DownloadProgress : 0
            visible: cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.ProjectStatus.Downloading
          }
        }

        QfCollapsibleMessage {
          id: transferError

          property bool hasError: false

          visible: hasError && cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Idle && !connectionSettings.visible

          Layout.fillWidth: true
          Layout.leftMargin: 10
          Layout.rightMargin: 10

          color: Theme.darkRed
          detailsColor: Theme.secondaryTextColor
          font: Theme.tipFont

          externalLink: QFieldCloudUtils.documentationFromErrorString(detailsText)
          titleText: QFieldCloudUtils.userFriendlyErrorString(detailsText)
          detailsText: ''

          Connections {
            target: iface

            function onLoadProjectEnded() {
              transferError.hasError = false;
            }
          }

          Connections {
            target: cloudProjectsModel

            function onPushFinished(projectId, hasError, errorString) {
              transferError.hasError = hasError;
              if (transferError.visible) {
                transferError.detailsText = errorString;
              }
            }

            function onProjectDownloaded(projectId, projectName, hasError, errorString) {
              transferError.hasError = hasError;
              if (transferError.visible) {
                transferError.detailsText = errorString;
              }
              const projectData = cloudProjectsModel.getProjectData(projectId);
              if (projectData.PackagedLayerErrors.length !== 0) {
                cloudPackageLayersFeedback.packagedLayersListViewModel = projectData.PackagedLayerErrors;
                cloudPackageLayersFeedback.visible = true;
              }
            }
          }
        }

        GridLayout {
          id: mainInnerGrid
          Layout.margins: 10
          Layout.maximumWidth: 525
          Layout.alignment: Qt.AlignHCenter
          width: parent.width
          visible: !connectionSettings.visible && cloudProjectsModel.currentProjectData.Status === QFieldCloudProjectsModel.Idle
          columns: 1
          columnSpacing: parent.columnSpacing
          rowSpacing: parent.rowSpacing

          Text {
            id: changesText
            property bool hasError: cloudProjectsModel.layerObserver.deltaFileWrapper.hasError()
            property int changesCount: cloudProjectsModel.layerObserver.deltaFileWrapper.count
            font: Theme.tipFont
            color: hasError ? Theme.errorColor : Theme.mainTextColor
            text: {
              if (!hasError) {
                return changesCount !== 0 ? qsTr('There is/are %n local change(s)', '', changesCount) : qsTr('There are no local changes');
              } else {
                return qsTr('The locally stored cloud project has been corrupted');
              }
            }
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.bottomMargin: 20
            Layout.fillWidth: true
          }

          QfButton {
            id: syncButton
            Layout.fillWidth: true
            text: qsTr('Synchronize')
            visible: !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError()
            enabled: !!(cloudProjectsModel.currentProjectData && cloudProjectsModel.currentProjectData.CanSync) && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError()
            icon.source: Theme.getThemeVectorIcon('ic_cloud_synchronize_24dp')

            onClicked: projectUpload(true)
          }

          Text {
            id: syncText
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            visible: !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError()
            text: qsTr('Synchronize the whole project with all modified features and download the freshly updated project with all the applied changes from QFieldCloud.')
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.bottomMargin: 20
            Layout.fillWidth: true
          }

          QfButton {
            id: pushButton
            Layout.fillWidth: true
            text: qsTr('Push changes')
            visible: !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError()
            enabled: !!(cloudProjectsModel.currentProjectData && cloudProjectsModel.currentProjectData.CanSync) && cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0 && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError()
            icon.source: Theme.getThemeVectorIcon('ic_cloud_upload_24dp')

            onClicked: projectUpload(false)
          }

          Text {
            id: pushText
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            visible: !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError()
            text: qsTr('Save internet bandwidth by only pushing the local features and pictures to the cloud, without updating the whole project.')
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.bottomMargin: 20
            Layout.fillWidth: true
          }

          QfButton {
            id: discardButton
            Layout.fillWidth: true
            bgcolor: Theme.darkRed
            text: !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError() ? qsTr('Revert local changes') : qsTr('Reset project')
            enabled: cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0 || cloudProjectsModel.layerObserver.deltaFileWrapper.hasError()
            icon.source: Theme.getThemeVectorIcon('ic_undo_black_24dp')

            onClicked: {
              if (!cloudProjectsModel.layerObserver.deltaFileWrapper.hasError()) {
                revertDialog.open();
              } else {
                resetDialog.open();
              }
            }
          }

          Text {
            id: discardText
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            text: !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError() ? qsTr('Revert all modified features in the local layers. You cannot restore those changes.') : qsTr('The local copy of this cloud project has been corrupted. Resetting the project will re-download the cloud version and will remove any local changes, make sure those were copied first if needed.\n\nWhile you can still view and use the project, it is strongly recommended to reset to avoid any accidental data loss as none of the changes made will be pushed back to the cloud.')
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.bottomMargin: 10
            Layout.fillWidth: true
          }

          Rectangle {
            color: Theme.controlBorderColor
            height: 1
            Layout.fillWidth: true
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.bottomMargin: 5
          }

          RowLayout {
            Layout.leftMargin: 10
            Layout.rightMargin: 10

            Label {
              Layout.fillWidth: true
              Layout.alignment: Qt.AlignVCenter
              topPadding: 10
              bottomPadding: 10
              font: Theme.tipFont
              wrapMode: Text.WordWrap
              color: autoPush.checked ? Theme.mainTextColor : Theme.secondaryTextColor

              text: qsTr('Automatically push changes every %n minute(s)', '', 0 + cloudProjectsModel.currentProjectData.AutoPushIntervalMins)

              MouseArea {
                anchors.fill: parent
                onClicked: {
                  if (!!cloudProjectsModel.currentProjectData.ForceAutoPush) {
                    displayToast(qsTr('The current project does not allow for auto-push to be turned off'));
                  } else {
                    cloudProjectsModel.projectSetAutoPushEnabled(cloudProjectsModel.currentProjectId, !autoPush.checked);
                  }
                }
              }
            }

            QfSwitch {
              id: autoPush
              Layout.preferredWidth: implicitContentWidth
              Layout.alignment: Qt.AlignVCenter
              width: implicitContentWidth
              small: true
              enabled: !cloudProjectsModel.currentProjectData.ForceAutoPush
              checked: !!cloudProjectsModel.currentProjectData.AutoPushEnabled

              onClicked: {
                cloudProjectsModel.projectSetAutoPushEnabled(cloudProjectsModel.currentProjectId, checked);
              }
            }

            Timer {
              id: autoPushTimer
              running: !!cloudProjectsModel.currentProjectData.AutoPushEnabled
              interval: (cloudProjectsModel.currentProjectData.AutoPushIntervalMins) * 60 * 1000
              repeat: true

              onRunningChanged: {
                if (running && pushButton.enabled) {
                  const dtStr = cloudProjectsModel.currentProjectData.LastLocalPushDeltas;
                  if (dtStr) {
                    const dt = new Date(dtStr);
                    const now = new Date();
                    if ((now - dt) >= interval) {
                      projectUpload(false);
                    }
                  }
                }
              }

              onTriggered: {
                if (pushButton.enabled) {
                  projectUpload(false);
                }
              }
            }
          }

          Text {
            id: lastExportPushText
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            text: {
              var exportText = '';
              var exportDt = cloudProjectsModel.currentProjectData.LastLocalExportedAt;
              var timeDeltaMinutes = null;
              if (exportDt) {
                exportDt = new Date(exportDt);
                timeDeltaMinutes = parseInt(Math.max(new Date() - exportDt, 0) / (60 * 1000));
                if (timeDeltaMinutes < 1)
                  exportText = qsTr('Last synchronized just now');
                else if (timeDeltaMinutes < 60)
                  exportText = qsTr('Last synchronized %1 minutes ago').arg(timeDeltaMinutes);
                else if (exportDt.toLocaleDateString() === new Date().toLocaleDateString())
                  exportText = qsTr('Last synchronized at %1').arg(exportDt.toLocaleTimeString());
                else
                  exportText = qsTr('Last synchronized on %1').arg(exportDt.toLocaleString());
              }
              var pushText = '';
              var pushDt = cloudProjectsModel.currentProjectData.LastLocalPushDeltas;
              if (pushDt) {
                pushDt = new Date(pushDt);
                timeDeltaMinutes = parseInt(Math.max(new Date() - pushDt, 0) / (60 * 1000));
                if (timeDeltaMinutes < 1)
                  pushText = qsTr('Last changes pushed just now');
                else if (timeDeltaMinutes < 60)
                  pushText = qsTr('Last changes pushed %1 minutes ago').arg(timeDeltaMinutes);
                else if (pushDt.toLocaleDateString() === new Date().toLocaleDateString())
                  pushText = qsTr('Last changes pushed at %1').arg(pushDt.toLocaleTimeString());
                else
                  pushText = qsTr('Last changes pushed on %1').arg(pushDt.toLocaleString());
              } else {
                pushText = qsTr('No changes pushed yet');
              }
              return pushDt > exportDt ? pushText + '\n' + exportText : exportText + '\n' + pushText;
            }
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true

            MouseArea {
              anchors.fill: parent
              onClicked: {
                qfieldCloudDeltaHistory.open();
              }
            }
          }
        }

        ColumnLayout {
          id: connectionSettings
          Layout.fillWidth: true
          Layout.fillHeight: true
          Layout.topMargin: connectionInformation.visible ? 0 : connectionInformation.childrenRect.height
          spacing: 2

          property bool visibility: false
          visible: visibility || (cloudProjectsModel.currentProjectId && cloudConnection.status !== QFieldCloudConnection.LoggedIn)

          ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 10
            height: parent.height
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical: QfScrollBar {
            }
            contentWidth: qfieldCloudLogin.width
            contentHeight: qfieldCloudLogin.childrenRect.height
            clip: true

            QFieldCloudLogin {
              id: qfieldCloudLogin
              isVisible: connectionSettings.visible
              width: parent.parent.width
            }
          }

          Item {
            Layout.fillHeight: true
            height: 15
          }
        }
      }
    }
  }

  QfDialog {
    id: revertDialog
    parent: mainWindow.contentItem

    property int selectedCount: 0
    property bool isDeleted: false

    title: qsTr("Revert local changes")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("Should local changes be reverted?")
    }

    onAccepted: {
      revertLocalChangesFromCurrentProject();
    }
    onRejected: {
      visible = false;
      popup.focus = true;
    }
  }

  QfDialog {
    id: resetDialog
    parent: mainWindow.contentItem

    property int selectedCount: 0
    property bool isDeleted: false

    title: qsTr("Reset cloud project")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("Last warning, resetting the cloud project will erase any local changes, are you sure you want to go ahead?")
    }

    onAccepted: {
      resetCurrentProject();
    }
    onRejected: {
      visible = false;
      popup.focus = true;
    }
  }

  function show() {
    visible = !visible;
    if (cloudProjectsModel.currentProjectId && cloudConnection.hasToken && cloudConnection.status === QFieldCloudConnection.Disconnected) {
      cloudConnection.login();
    }
    if (cloudConnection.status === QFieldCloudConnection.Connecting) {
      displayToast(qsTr('Connecting cloud'));
    } else if (cloudProjectsModel.currentProjectData.ProjectFileOutdated) {
      displayToast(qsTr('This project has an updated project file on the cloud, you are advised to synchronize.'), 'warning');
    } else if (cloudProjectsModel.currentProjectData.ProjectOutdated) {
      displayToast(qsTr('This project has updated data on the cloud, you should synchronize.'));
    }
  }

  function projectUpload(shouldDownloadUpdates) {
    if (cloudProjectsModel.currentProjectData && cloudProjectsModel.currentProjectData.CanSync) {
      cloudProjectsModel.projectUpload(cloudProjectsModel.currentProjectId, shouldDownloadUpdates);
    }
  }

  function revertLocalChangesFromCurrentProject() {
    if (cloudProjectsModel.currentProjectData && cloudProjectsModel.currentProjectData.CanSync) {
      if (cloudProjectsModel.revertLocalChangesFromCurrentProject(cloudProjectsModel.currentProjectId)) {
        displayToast(qsTr('Local changes reverted'));
      } else {
        displayToast(qsTr('Failed to revert changes'), 'error');
      }
    } else {
      displayToast(qsTr('No changes to revert'));
    }
  }

  function resetCurrentProject() {
    cloudProjectsModel.discardLocalChangesFromCurrentProject(cloudProjectsModel.currentProjectId);
    cloudProjectsModel.downloadProject(cloudProjectsModel.currentProjectId, true);
  }
}
