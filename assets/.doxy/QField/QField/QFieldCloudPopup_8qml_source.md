

# File QFieldCloudPopup.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**QFieldCloudPopup.qml**](QFieldCloudPopup_8qml.md)

[Go to the documentation of this file](QFieldCloudPopup_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

Popup {
  id: popup
  padding: 0
  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

  property string pendingAction: ""
  property string pendingCreationTitle: ""
  property string pendingUploadPath: ""

  onAboutToHide: {
    pendingAction = "";
    pendingCreationTitle = "";
    pendingUploadPath = "";
  }

  Page {
    anchors.fill: parent

    header: QfPageHeader {
      title: qsTr('QFieldCloud')

      showBackButton: true
      showCancelButton: false
      showApplyButton: false

      busyIndicatorState: cloudProjectsModel.currentProject && (cloudConnection.status === QFieldCloudConnection.Connecting || cloudProjectsModel.currentProject.status === QFieldCloudProject.Pushing || cloudProjectsModel.currentProject.status === QFieldCloudProject.Downloading) ? 'on' : 'off'

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

    ScrollView {
      id: scrollView
      anchors.fill: parent

      ScrollBar.horizontal: QfScrollBar {}
      ScrollBar.vertical: QfScrollBar {}
      contentWidth: mainGrid.width
      contentHeight: mainGrid.height
      padding: 0
      bottomPadding: uploadFeedback.isVisible ? uploadFeedback.height + 20 : 0
      clip: true

      GridLayout {
        id: mainGrid
        width: popup.width - popup.leftPadding - popup.rightPadding
        columns: 1
        columnSpacing: 2
        rowSpacing: 2

        RowLayout {
          id: connectionInformation

          Text {
            id: welcomeText
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            padding: 10
            text: switch (cloudConnection.status) {
            case QFieldCloudConnection.Disconnected:
              '';
              break;
            case QFieldCloudConnection.Connecting:
              qsTr('Connecting to the cloud.');
              break;
            case QFieldCloudConnection.LoggedIn:
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
              anchors.fill: parent
              anchors.margins: 1
              radius: width / 2
              color: "white"
              visible: false
              layer.enabled: true
            }

            Image {
              id: cloudAvatar
              anchors.fill: parent
              anchors.margins: 1
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
                  if (cloudConnection.status !== QFieldCloudConnection.LoggedIn || !cloudProjectsModel.currentProject || cloudProjectsModel.currentProject.status !== QFieldCloudProject.Idle)
                    return;
                  if (!connectionSettings.visible) {
                    connectionSettings.visible = true;
                  } else {
                    connectionSettings.visible = false;
                  }
                }
              }
            }
          }
        }

        Text {
          id: wrongAccountText
          visible: cloudProjectsModel.currentProjectId != '' && cloudProjectsModel.currentProject && cloudProjectsModel.currentProjectId !== cloudProjectsModel.currentProject.id
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
          visible: (cloudProjectsModel.currentProject && (cloudProjectsModel.currentProject.status === QFieldCloudProject.Downloading || cloudProjectsModel.currentProject.status === QFieldCloudProject.Pushing)) || (cloudProjectCreationConnection.target && (cloudProjectCreationConnection.target.status === QFieldCloudProject.Downloading))
          font: Theme.tipFont
          color: Theme.secondaryTextColor
          text: {
            let status = QFieldCloudProject.Idle;
            let packagingStatus = QFieldCloudProject.PackagingFinishedStatus;
            let downloadProgress = 0.0;
            let downloadBytesTotal = 0;
            let deltaFilePushStatus = QFieldCloudProject.DeltaFileLocalStatus;
            if (cloudProjectsModel.currentProject) {
              status = cloudProjectsModel.currentProject.status;
              packagingStatus = cloudProjectsModel.currentProject.packagingStatus;
              downloadProgress = cloudProjectsModel.currentProject.downloadProgress;
              downloadBytesTotal = cloudProjectsModel.currentProject.downloadBytesTotal;
              deltaFilePushStatus = cloudProjectsModel.currentProject.deltaFilePushStatus;
            } else if (cloudProjectCreationConnection.target) {
              status = cloudProjectCreationConnection.target.status;
              packagingStatus = cloudProjectCreationConnection.target.packagingStatus;
              downloadProgress = cloudProjectCreationConnection.target.downloadProgress;
              downloadBytesTotal = cloudProjectCreationConnection.target.downloadBytesTotal;
              deltaFilePushStatus = cloudProjectCreationConnection.target.deltaFilePushStatus;
            } else {
              return '';
            }
            switch (status) {
            case QFieldCloudProject.Downloading:
              if (packagingStatus === QFieldCloudProject.PackagingBusyStatus) {
                return qsTr('QFieldCloud is packaging the latest data just for you; this might take some time, please hold tight');
              } else {
                if (packagingStatus === QFieldCloudProject.PackagingFinishedStatus || downloadProgress > 0.0) {
                  if (downloadBytesTotal > 0) {
                    return qsTr('Downloading, %1% of %2 fetched').arg(Math.round(downloadProgress * 100)).arg(FileUtils.representFileSize(downloadBytesTotal));
                  } else {
                    return qsTr('Downloading, %1% fetched').arg(Math.round(downloadProgress * 100));
                  }
                } else {
                  return qsTr('Reaching out to QFieldCloud to download project');
                }
              }
            case QFieldCloudProject.Pushing:
              switch (deltaFilePushStatus) {
              case QFieldCloudProject.DeltaFileLocalStatus:
                return qsTr('Pushing changes, %1%…').arg(Math.round(cloudProjectsModel.currentProject.pushDeltaProgress * 100));
              default:
                return qsTr('QFieldCloud is applying the latest pushed changes. This might take some time, please hold tight…');
              }
            default:
              '';
            }
            return '';
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
          visible: statusText.visible

          Image {
            id: statusIcon
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            smooth: true
            source: {
              let status = QFieldCloudProject.Idle;
              let packagingStatus = QFieldCloudProject.PackagingFinishedStatus;
              let deltaFilePushStatus = QFieldCloudProject.DeltaFileLocalStatus;
              if (cloudProjectsModel.currentProject) {
                status = cloudProjectsModel.currentProject.status;
                packagingStatus = cloudProjectsModel.currentProject.packagingStatus;
                deltaFilePushStatus = cloudProjectsModel.currentProject.deltaFilePushStatus;
              } else if (cloudProjectCreationConnection.target) {
                status = cloudProjectCreationConnection.target.status;
                packagingStatus = cloudProjectCreationConnection.target.packagingStatus;
                deltaFilePushStatus = cloudProjectCreationConnection.target.deltaFilePushStatus;
              } else {
                return '';
              }
              switch (status) {
              case QFieldCloudProject.Downloading:
                switch (packagingStatus) {
                case QFieldCloudProject.PackagingFinishedStatus || cloudProjectsModel.currentProject.downloadProgress > 0.0:
                  return Theme.getThemeVectorIcon('ic_cloud_download_24dp');
                default:
                  return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
                }
              case QFieldCloudProject.Pushing:
                switch (deltaFilePushStatus) {
                case QFieldCloudProject.DeltaFileLocalStatus:
                  return Theme.getThemeVectorIcon('ic_cloud_upload_24dp');
                default:
                  return Theme.getThemeVectorIcon('ic_cloud_active_24dp');
                }
              default:
                '';
              }
              return '';
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
            indeterminate: cloudProjectsModel.currentProject && (cloudProjectsModel.currentProject.packagingStatus !== QFieldCloudProject.PackagingFinishedStatus && cloudProjectsModel.currentProject.downloadProgress === 0.0)
            value: cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.downloadProgress ? cloudProjectsModel.currentProject.downloadProgress : 0
            visible: cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.ProjectStatus.Downloading
          }
        }

        QfCollapsibleMessage {
          id: transferError

          property bool hasError: false

          visible: hasError && cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.Idle && !connectionSettings.visible

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
        }

        ColumnLayout {
          id: localProjectGrid
          visible: !connectionSettings.visible && !cloudProjectsModel.currentProject && !statusText.visible
          Layout.margins: 10
          Layout.maximumWidth: 525
          Layout.alignment: Qt.AlignHCenter

          Text {
            Layout.fillWidth: true
            Layout.bottomMargin: 20
            font: Theme.tipFont
            color: Theme.mainTextColor
            text: cloudProjectsModel.isCreating || (cloudProjectCreationConnection.target && cloudProjectCreationConnection.target.status === QFieldCloudProject.Uploading) ? qsTr('Uploading the current project to QFieldCloud.') : qsTr('The current project is not stored on QFieldCloud.')
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
          }

          QfButton {
            id: cloudifyButton
            Layout.fillWidth: true
            text: {
              if (!enabled) {
                if (cloudProjectCreationConnection.target && cloudProjectCreationConnection.target.uploadProgress > 0) {
                  return qsTr("Cloudifying project") + " (%1%)".arg(Math.round(cloudProjectCreationConnection.target.uploadProgress * 100));
                } else {
                  return qsTr("Cloudifying project");
                }
              }
              return qsTr('Cloudify!');
            }
            enabled: !cloudProjectsModel.isCreating && !cloudProjectCreationConnection.target
            progressValue: cloudProjectCreationConnection.target ? cloudProjectCreationConnection.target.uploadProgress : 0
            icon.source: Theme.getThemeVectorIcon('ic_cloud_white_24dp')

            onClicked: {
              if (qgisProject.fileName != "") {
                cloudify(ProjectUtils.title(qgisProject), FileUtils.absolutePath(qgisProject.fileName));
              }
            }
          }

          Text {
            Layout.fillWidth: true
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            text: qsTr('Storing projects on QFieldCloud offers seamless synchronization, offline editing, and team management.<br><br>') + ' <a href="https://qfield.cloud/">' + qsTr('Learn more about QFieldCloud') + '</a>.'
            textFormat: Text.RichText
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter

            onLinkActivated: link => {
              Qt.openUrlExternally(link);
            }
          }
        }

        ColumnLayout {
          id: cloudProjectGrid
          Layout.margins: 10
          Layout.maximumWidth: 525
          Layout.alignment: Qt.AlignHCenter
          visible: !connectionSettings.visible && cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.Idle

          Text {
            id: changesText
            property bool hasError: cloudProjectsModel.layerObserver.deltaFileWrapper && cloudProjectsModel.layerObserver.deltaFileWrapper.hasError
            property int changesCount: cloudProjectsModel.layerObserver.deltaFileWrapper ? cloudProjectsModel.layerObserver.deltaFileWrapper.count : 0
            font: Theme.tipFont
            color: hasError ? Theme.errorColor : Theme.mainTextColor
            text: {
              if (!hasError) {
                return changesCount !== 0 ? qsTr('There is/are %n local change(s)', '', changesCount) : qsTr('There are no local changes');
              } else {
                return qsTr('The locally stored cloud project has been corrupted') + '\n' + cloudProjectsModel.layerObserver.deltaFileWrapper.errorString;
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
            visible: cloudProjectsModel.layerObserver.deltaFileWrapper && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError
            enabled: !!(cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.Idle) && cloudProjectsModel.layerObserver.deltaFileWrapper && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError
            icon.source: Theme.getThemeVectorIcon('ic_cloud_synchronize_24dp')

            onClicked: projectPush(true)
          }

          Text {
            id: syncText
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            visible: cloudProjectsModel.layerObserver.deltaFileWrapper && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError
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
            visible: cloudProjectsModel.layerObserver.deltaFileWrapper && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError
            enabled: !!(cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.Idle) && cloudProjectsModel.layerObserver.deltaFileWrapper && cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0 && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError
            icon.source: Theme.getThemeVectorIcon('ic_cloud_upload_24dp')

            onClicked: projectPush(false)
          }

          Text {
            id: pushText
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            visible: cloudProjectsModel.layerObserver.deltaFileWrapper && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError
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
            text: cloudProjectsModel.layerObserver.deltaFileWrapper && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError ? qsTr('Revert local changes') : qsTr('Reset project')
            enabled: cloudProjectsModel.layerObserver.deltaFileWrapper && (cloudProjectsModel.layerObserver.deltaFileWrapper.count > 0 || cloudProjectsModel.layerObserver.deltaFileWrapper.hasError)
            icon.source: Theme.getThemeVectorIcon('ic_undo_black_24dp')

            onClicked: {
              if (!cloudProjectsModel.layerObserver.deltaFileWrapper.hasError) {
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
            text: cloudProjectsModel.layerObserver.deltaFileWrapper && !cloudProjectsModel.layerObserver.deltaFileWrapper.hasError ? qsTr('Revert all modified features in the local layers. You cannot restore those changes.') : qsTr('The local copy of this cloud project has been corrupted. Resetting the project will re-download the cloud version and will remove any local changes, make sure those were copied first if needed.\n\nWhile you can still view and use the project, it is strongly recommended to reset to avoid any accidental data loss as none of the changes made will be pushed back to the cloud.')
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

              text: qsTr('Automatically push changes every %n minute(s)', '', 0 + (cloudProjectsModel.currentProject ? cloudProjectsModel.currentProject.autoPushIntervalMins : 0))

              MouseArea {
                anchors.fill: parent
                onClicked: {
                  if (cloudProjectsModel.currentProject) {
                    if (!!cloudProjectsModel.currentProject.forceAutoPush) {
                      displayToast(qsTr('The current project does not allow for auto-push to be turned off'));
                    } else {
                      cloudProjectsModel.currentProject.autoPushEnabled = !autoPush.checked;
                    }
                  }
                }
              }
            }

            QfSwitch {
              id: autoPush
              Layout.preferredWidth: implicitContentWidth
              Layout.alignment: Qt.AlignVCenter
              width: implicitContentWidth
              enabled: !(cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.forceAutoPush)
              checked: !!(cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.autoPushEnabled)

              onClicked: {
                if (cloudProjectsModel.currentProject) {
                  cloudProjectsModel.currentProject.autoPushEnabled = checked;
                }
              }
            }

            Timer {
              id: autoPushTimer
              running: !!(cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.autoPushEnabled)
              interval: (cloudProjectsModel.currentProject ? cloudProjectsModel.currentProject.autoPushIntervalMins : 0) * 60 * 1000
              repeat: true

              onRunningChanged: {
                if (running && pushButton.enabled && cloudProjectsModel.currentProject) {
                  const dtStr = cloudProjectsModel.currentProject.lastLocalPushDeltas;
                  if (dtStr) {
                    const dt = new Date(dtStr);
                    const now = new Date();
                    if ((now - dt) >= interval) {
                      projectPush(false);
                    }
                  }
                }
              }

              onTriggered: {
                if (pushButton.enabled) {
                  projectPush(false);
                }
              }
            }
          }

          Text {
            id: lastExportPushText
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            text: {
              if (!cloudProjectsModel.currentProject) {
                return '';
              }
              var exportText = '';
              var exportDt = cloudProjectsModel.currentProject.lastLocalExportedAt;
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
              var pushDt = cloudProjectsModel.currentProject.lastLocalPushDeltas;
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

          visible: (cloudProjectsModel.currentProjectId || popup.pendingAction != "") && cloudConnection.status !== QFieldCloudConnection.LoggedIn

          ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.margins: 0
            height: parent.height
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical: QfScrollBar {}
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

    Rectangle {
      id: uploadFeedback

      property bool isVisible: uploadLabel.text !== ""

      anchors.left: parent.left
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      anchors.margins: 10
      height: isVisible ? uploadLabel.contentHeight + uploadProgress.height + 25 : 0

      radius: 8
      color: Theme.groupBoxBackgroundColor
      clip: true
      opacity: isVisible ? 1 : 0

      Behavior on opacity {
        PropertyAnimation {
          easing.type: Easing.OutQuart
        }
      }

      Column {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 5

        Text {
          id: uploadLabel
          width: parent.width
          wrapMode: Text.WordWrap
          text: ""
          font: Theme.tipFont
          color: Theme.mainTextColor
        }

        ProgressBar {
          id: uploadProgress
          width: parent.width
          indeterminate: false
          from: 0
          to: 1
        }
      }
    }
  }

  Connections {
    target: cloudConnection

    function onStatusChanged() {
      if (cloudConnection.status == QFieldCloudConnection.LoggedIn) {
        if (popup.pendingAction === "cloudify") {
          popup.pendingAction = "";
          cloudify(pendingCreationTitle, pendingUploadPath);
        } else if (popup.pendingAction == "connect") {
          popup.visible = false;
        }
      }
    }

    function onPendingAttachmentsUploadStatus(fileName, fileProgress, uploadPending) {
      let text = qsTr("Uploading %1").arg(fileName);
      if (uploadPending > 0) {
        text += " — " + qsTr("%n file(s) remaining", "", uploadPending);
      }
      uploadLabel.text = text;
      uploadProgress.value = fileProgress;
    }

    function onPendingAttachmentsUploadFinished() {
      uploadLabel.text = "";
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
      const cloudProject = cloudProjectsModel.findProject(projectId);
      if (cloudProject.packagedLayerErrors.length !== 0) {
        cloudPackageLayersFeedback.packagedLayersListViewModel = cloudProject.packagedLayerErrors;
        cloudPackageLayersFeedback.visible = true;
      }
    }

    function onProjectCreated(projectId, hasError, errorString) {
      if (!qfieldCloudPopup.visible) {
        qfieldCloudPopup.visible = true;
      }
      if (hasError) {
        displayToast(errorString, 'error');
        return;
      }
      const createdCloudProject = cloudProjectsModel.findProject(projectId);
      if (createdCloudProject) {
        cloudProjectCreationConnection.target = createdCloudProject;
        createdCloudProject.uploadLocalPath(pendingUploadPath, true);
      }
    }
  }

  Connections {
    id: cloudProjectCreationConnection
    target: null

    function onUploadFinished(error) {
      if (error !== '') {
        displayToast(error, 'error');
        cloudProjectCreationConnection.target = null;
        return;
      }
      cloudProjectCreationConnection.target.packageAndDownload();
    }

    function onDownloadFinished(error) {
      if (error !== '') {
        displayToast(error, 'error');
      }
      iface.loadFile(cloudProjectCreationConnection.target.localPath);
      cloudProjectCreationConnection.target = null;
      popup.close();
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
    if (opened) {
      return;
    }
    if (cloudConnection.status === QFieldCloudConnection.Disconnected) {
      if ((cloudConnection.hasToken || cloudConnection.hasProviderConfiguration)) {
        cloudConnection.login();
      }
      cloudConnection.getAuthenticationProviders();
    }
    if (cloudConnection.status === QFieldCloudConnection.Connecting) {
      displayToast(qsTr('Connecting cloud'));
    } else if (cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.isProjectOutdated) {
      displayToast(qsTr('This project has an updated project file on the cloud, you are advised to synchronize.'), 'warning');
    } else if (cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.isOutdated) {
      displayToast(qsTr('This project has updated data on the cloud, you should synchronize.'));
    }
    open();
  }

  function projectPush(shouldDownloadUpdates) {
    if (cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.Idle) {
      cloudProjectsModel.projectPush(cloudProjectsModel.currentProjectId, shouldDownloadUpdates);
    }
  }

  function revertLocalChangesFromCurrentProject() {
    if (cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.Idle) {
      if (cloudProjectsModel.revertLocalChangesFromCurrentProject()) {
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
    cloudProjectsModel.projectPackageAndDownload(cloudProjectsModel.currentProjectId);
  }

  function cloudify(title, path) {
    if (!opened) {
      show();
    }
    pendingCreationTitle = title;
    pendingUploadPath = path;
    if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
      pendingCreationTitle = title;
      pendingUploadPath = path;
      cloudProjectsModel.createProject(title);
    } else {
      popup.pendingAction = "cloudify";
    }
  }
}
```


