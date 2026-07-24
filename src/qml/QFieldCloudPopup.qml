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
  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin
  closePolicy: Popup.CloseOnPressOutside

  property QFieldCloudStatus cloudServiceStatus: null
  property string pendingAction: ""
  property string pendingCreationTitle: ""
  property string pendingUploadPath: ""
  property string lastSubscriptionUser: ""

  property date currentDateTime: new Date()

  onAboutToShow: currentDateTime = new Date()

  onAboutToHide: {
    pendingAction = "";
    pendingCreationTitle = "";
    pendingUploadPath = "";
    swipeView.currentIndex = 0;
  }

  Page {
    id: page
    anchors.fill: parent

    header: QfPageHeader {
      title: swipeView.currentIndex === 1 ? qsTr('Danger Zone') : qsTr('QFieldCloud')

      showBackButton: true
      showCancelButton: false
      showApplyButton: false

      busyIndicatorState: cloudProjectsModel.currentProject && (cloudConnection.status === QFieldCloudConnection.Connecting || cloudProjectsModel.currentProject.status === QFieldCloudProject.Pushing || cloudProjectsModel.currentProject.status === QFieldCloudProject.Downloading) ? 'on' : 'off'

      topMargin: mainWindow.sceneTopMargin

      onFinished: popup.goBack()
    }

    focus: true

    Keys.onReleased: event => {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        event.accepted = true;
        popup.goBack();
      }
    }

    RowLayout {
      id: connectionInformation
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: parent.right

      QfMeterBar {
        id: storageMeterBar
        Layout.fillWidth: true
        Layout.margins: 10
        Layout.alignment: Qt.AlignVCenter
        visible: false
      }

      Item {
        Layout.fillWidth: true
        visible: !storageMeterBar.visible
      }

      Rectangle {
        id: cloudAvatarRect
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.margins: 10
        width: 48
        height: 48
        radius: width / 2
        color: Theme.controlBackgroundAlternateColor
        layer.enabled: true

        Rectangle {
          id: cloudAvatarMask
          anchors.centerIn: parent
          width: cloudAvatar.width
          height: cloudAvatar.height
          radius: width / 2
          color: "white"
          visible: false
          layer.enabled: true
        }

        Image {
          id: cloudAvatar
          anchors.centerIn: parent
          fillMode: Image.PreserveAspectCrop
          smooth: true
          source: cloudConnection.avatarUrl !== '' ? cloudConnection.avatarUrl : 'qrc:/images/nyuki.svg'
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
              source = 'qrc:/images/nyuki.svg';
            }
          }

          MouseArea {
            anchors.fill: parent

            onClicked: {
              if (cloudConnection.status !== QFieldCloudConnection.LoggedIn || !cloudProjectsModel.currentProject || cloudProjectsModel.currentProject.status !== QFieldCloudProject.Idle)
                return;
              connectionSettings.visible = !connectionSettings.visible;
              storageMeterBar.visible = Qt.binding(() => (storageMeterBar.value > 0 || storageMeterBar.loading) && !connectionSettings.visible);
            }
          }
        }
      }
    }

    SwipeView {
      id: swipeView
      anchors.top: connectionInformation.bottom
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      clip: true
      interactive: false

      ScrollView {
        id: scrollView

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

          QFieldCloudStatusBanner {
            cloudServiceStatus: popup.cloudServiceStatus
            Layout.margins: 10
            visible: cloudServiceStatus && cloudServiceStatus.hasProblem && !connectionSettings.visible
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
            iconSource: Theme.getThemeVectorIcon('ic_error_outline_24dp')

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
            spacing: 10

            readonly property bool isCloudifying: cloudProjectsModel.isCreating || !!cloudProjectCreationConnection.target
            readonly property real cloudifyProgress: cloudProjectCreationConnection.target ? cloudProjectCreationConnection.target.uploadProgress : 0

            QfContainerCard {
              id: cloudifyCard
              Layout.fillWidth: true
              accentColor: Theme.cloudColor
              iconSource: Theme.getThemeVectorIcon('ic_cloud_active_24dp')
              title: qsTr('Cloudify project')
              description: localProjectGrid.isCloudifying ? qsTr('Uploading the current project to QFieldCloud.') : qsTr('The current project is not stored on QFieldCloud. Storing projects on QFieldCloud offers seamless synchronization, offline editing, and team management.') + ' <a href="https://qfield.cloud/">' + qsTr('Learn more about QFieldCloud') + '</a>.'

              QfButton {
                Layout.fillWidth: true
                Layout.topMargin: 4
                bgcolor: Theme.cloudColor
                color: Theme.light
                text: localProjectGrid.isCloudifying ? (localProjectGrid.cloudifyProgress > 0 ? qsTr('Cloudifying %1%').arg(Math.round(localProjectGrid.cloudifyProgress * 100)) : qsTr('Cloudifying')) : qsTr('Cloudify project')
                enabled: !localProjectGrid.isCloudifying
                showProgress: localProjectGrid.isCloudifying
                progressValue: localProjectGrid.cloudifyProgress

                onClicked: {
                  if (qgisProject.fileName != "") {
                    cloudify(ProjectUtils.title(qgisProject), FileUtils.absolutePath(qgisProject.fileName));
                  }
                }
              }
            }
          }

          ColumnLayout {
            id: cloudProjectGrid
            Layout.margins: 10
            Layout.maximumWidth: 525
            Layout.alignment: Qt.AlignHCenter
            visible: !connectionSettings.visible && cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.Idle

            readonly property bool hasDeltaFileWrapper: !!cloudProjectsModel.layerObserver.deltaFileWrapper
            readonly property bool hasDeltaError: hasDeltaFileWrapper && cloudProjectsModel.layerObserver.deltaFileWrapper.hasError
            readonly property int changesCount: hasDeltaFileWrapper ? cloudProjectsModel.layerObserver.deltaFileWrapper.count : 0
            readonly property bool isProjectIdle: !!(cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.Idle)
            readonly property bool canUpload: isProjectIdle && changesCount > 0 && !hasDeltaError
            readonly property bool canSynchronize: isProjectIdle && hasDeltaFileWrapper && !hasDeltaError

            Text {
              id: changesText
              visible: cloudProjectGrid.hasDeltaError
              font: Theme.tipFont
              color: Theme.errorColor
              text: visible ? qsTr('The locally stored cloud project has been corrupted') + '\n' + cloudProjectsModel.layerObserver.deltaFileWrapper.errorString : ''
              wrapMode: Text.WordWrap
              horizontalAlignment: Text.AlignHCenter
              Layout.bottomMargin: 20
              Layout.fillWidth: true
            }

            ColumnLayout {
              id: cloudActions
              Layout.fillWidth: true
              Layout.bottomMargin: 10
              spacing: 10
              visible: cloudProjectGrid.hasDeltaFileWrapper && !cloudProjectGrid.hasDeltaError

              QfContainerCard {
                id: uploadCard
                Layout.fillWidth: true
                accentColor: Theme.cloudColor
                iconSource: Theme.getThemeVectorIcon('ic_cloud_upload_24dp')
                title: qsTr('Upload local changes')
                indicatorVisible: cloudProjectGrid.changesCount > 0
                indicatorCount: cloudProjectGrid.changesCount
                description: qsTr('Sends your edits and attachments to the cloud without downloading project updates. Fast and low on data.')
                footnote: {
                  if (!cloudProjectsModel.currentProject) {
                    return '';
                  }
                  const uploadedAt = cloudProjectsModel.currentProject.lastLocalPushDeltas;
                  const timeDeltaMinutes = parseInt(Math.max(popup.currentDateTime - uploadedAt, 0) / (60 * 1000));
                  if (isNaN(timeDeltaMinutes)) {
                    return qsTr('No changes uploaded yet');
                  } else if (timeDeltaMinutes < 1) {
                    return qsTr('Last uploaded just now');
                  } else if (timeDeltaMinutes < 60) {
                    return qsTr('Last uploaded %1 minutes ago').arg(timeDeltaMinutes);
                  } else if (uploadedAt.toLocaleDateString() === popup.currentDateTime.toLocaleDateString()) {
                    return qsTr('Last uploaded today at %1').arg(uploadedAt.toLocaleTimeString(Qt.locale(), Locale.ShortFormat));
                  }
                  return qsTr('Last uploaded on %1').arg(uploadedAt.toLocaleString(Qt.locale(), Locale.ShortFormat));
                }

                QfButton {
                  Layout.fillWidth: true
                  Layout.topMargin: 4
                  bgcolor: Theme.cloudColor
                  color: Theme.light
                  text: qsTr('Upload')
                  enabled: cloudProjectGrid.canUpload

                  onClicked: projectPush(false)
                }

                Rectangle {
                  Layout.fillWidth: true
                  Layout.topMargin: 12
                  Layout.bottomMargin: 6
                  Layout.preferredHeight: 1

                  color: Theme.controlBorderColor
                }

                RowLayout {
                  Layout.fillWidth: true
                  spacing: 0

                  Label {
                    id: autoPushText
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                    font: Theme.tipFont
                    wrapMode: Text.WordWrap
                    color: autoPush.checked ? Theme.mainTextColor : Theme.secondaryTextColor

                    text: qsTr('Auto-upload every %n minute(s)', '', 0 + (cloudProjectsModel.currentProject ? cloudProjectsModel.currentProject.autoPushIntervalMins : 0))

                    MouseArea {
                      anchors.fill: parent
                      onClicked: {
                        if (cloudProjectsModel.currentProject) {
                          if (!!cloudProjectsModel.currentProject.forceAutoPush) {
                            displayToast(qsTr('The current project does not allow for auto-upload to be turned off'));
                          } else {
                            cloudProjectsModel.currentProject.autoPushEnabled = !autoPush.checked;
                          }
                        }
                      }
                    }
                  }

                  QfSwitch {
                    id: autoPush

                    verticalPadding: 0

                    Layout.preferredWidth: 38
                    Layout.preferredHeight: 24
                    Layout.alignment: Qt.AlignVCenter
                    enabled: !(cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.forceAutoPush)
                    checked: !!(cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.autoPushEnabled)

                    onClicked: {
                      if (cloudProjectsModel.currentProject) {
                        cloudProjectsModel.currentProject.autoPushEnabled = checked;
                      }
                    }
                  }
                }
              }

              QfContainerCard {
                id: synchronizeCard
                Layout.fillWidth: true
                accentColor: Theme.mainColor
                iconSource: Theme.getThemeVectorIcon('ic_cloud_synchronize_24dp')
                title: qsTr('Synchronize project')
                indicatorVisible: !!(cloudProjectsModel.currentProject && (cloudProjectsModel.currentProject.isOutdated || cloudProjectsModel.currentProject.isProjectOutdated))
                description: qsTr('Uploads your edits, then downloads the latest project from QFieldCloud so everything is up to date.')
                footnote: {
                  if (!cloudProjectsModel.currentProject) {
                    return '';
                  }
                  const synchronizedAt = cloudProjectsModel.currentProject.lastLocalExportedAt;
                  const timeDeltaMinutes = parseInt(Math.max(popup.currentDateTime - synchronizedAt, 0) / (60 * 1000));
                  if (isNaN(timeDeltaMinutes)) {
                    return '';
                  } else if (timeDeltaMinutes < 1) {
                    return qsTr('Last synchronized just now');
                  } else if (timeDeltaMinutes < 60) {
                    return qsTr('Last synchronized %1 minutes ago').arg(timeDeltaMinutes);
                  } else if (synchronizedAt.toLocaleDateString() === popup.currentDateTime.toLocaleDateString()) {
                    return qsTr('Last synchronized today at %1').arg(synchronizedAt.toLocaleTimeString(Qt.locale(), Locale.ShortFormat));
                  }
                  return qsTr('Last synchronized on %1').arg(synchronizedAt.toLocaleString(Qt.locale(), Locale.ShortFormat));
                }

                QfButton {
                  Layout.fillWidth: true
                  Layout.topMargin: 4
                  bgcolor: "transparent"
                  color: Theme.mainColor
                  text: qsTr('Synchronize')
                  enabled: cloudProjectGrid.canSynchronize

                  onClicked: projectPush(true)
                }
              }
            }

            Timer {
              id: autoPushTimer
              running: !!(cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.autoPushEnabled)
              interval: (cloudProjectsModel.currentProject ? cloudProjectsModel.currentProject.autoPushIntervalMins : 0) * 60 * 1000
              repeat: true

              onRunningChanged: {
                if (running && cloudProjectGrid.canUpload && cloudProjectsModel.currentProject) {
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
                if (cloudProjectGrid.canUpload) {
                  projectPush(false);
                }
              }
            }

            RowLayout {
              Layout.topMargin: 5
              Layout.bottomMargin: 10
              Layout.alignment: Qt.AlignHCenter
              Layout.maximumWidth: uploadCard.width - 80
              spacing: 20

              ColumnLayout {
                Layout.preferredWidth: uploadCard.width / 2 - 60
                Layout.alignment: Qt.AlignTop
                spacing: 5

                QfToolButton {
                  id: uploadHistoryEntryButton
                  Layout.alignment: Qt.AlignHCenter
                  round: true
                  bgcolor: Qt.rgba(Theme.cloudColor.r, Theme.cloudColor.g, Theme.cloudColor.b, 0.1)
                  iconSource: Theme.getThemeVectorIcon('ic_baseline-list_white_24dp')
                  iconColor: Theme.cloudColor

                  onClicked: qfieldCloudDeltaHistory.open()
                }

                Text {
                  Layout.fillWidth: true
                  font: Theme.tipFont
                  color: Theme.mainTextColor
                  horizontalAlignment: Text.AlignHCenter
                  wrapMode: Text.WordWrap
                  text: qsTr('Upload history')

                  MouseArea {
                    anchors.fill: parent
                    onClicked: qfieldCloudDeltaHistory.open()
                  }
                }
              }

              ColumnLayout {
                Layout.preferredWidth: uploadCard.width / 2 - 60
                Layout.alignment: Qt.AlignTop
                spacing: 5

                QfToolButton {
                  id: dangerZoneEntryButton
                  Layout.alignment: Qt.AlignHCenter
                  round: true
                  bgcolor: Qt.rgba(Theme.darkRed.r, Theme.darkRed.g, Theme.darkRed.b, 0.1)
                  iconSource: Theme.getThemeVectorIcon('ic_error_outline_24dp')
                  iconColor: Theme.darkRed

                  onClicked: swipeView.currentIndex = 1
                }

                Text {
                  Layout.fillWidth: true
                  Layout.alignment: Qt.AlignHCenter
                  font: Theme.tipFont
                  color: Theme.mainTextColor
                  horizontalAlignment: Text.AlignHCenter
                  wrapMode: Text.WordWrap
                  text: qsTr('Danger zone')

                  MouseArea {
                    anchors.fill: parent
                    onClicked: swipeView.currentIndex = 1
                  }
                }
              }
            }
          }

          ColumnLayout {
            id: connectionSettings
            Layout.fillWidth: true
            Layout.fillHeight: true
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
                cloudServiceStatus: popup.cloudServiceStatus
              }
            }

            Item {
              Layout.fillHeight: true
              height: 15
            }
          }
        }
      }

      QFieldCloudDangerZone {
        onDiscardRequested: revertDialog.open()
        onResetRequested: resetDialog.open()
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
        fetchSubscriptionInformation();
        if (popup.pendingAction === "cloudify") {
          popup.pendingAction = "";
          cloudify(pendingCreationTitle, pendingUploadPath);
        } else if (popup.pendingAction == "connect") {
          popup.visible = false;
        }
      } else if (cloudConnection.status === QFieldCloudConnection.Disconnected) {
        lastSubscriptionUser = "";
        storageMeterBar.visible = false;
        storageMeterBar.value = 0;
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

    function onSubscriptionInformationReceived(subscriptionInformation) {
      storageMeterBar.loading = false;
      if (subscriptionInformation.storageTotal > 0) {
        showStorageBar(subscriptionInformation.storageUsed, subscriptionInformation.storageTotal, subscriptionInformation.plan, subscriptionInformation.storageThresholdWarning, subscriptionInformation.storageThresholdCritical);
      }
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

    function onProjectDownloaded(projectId, projectName, projectOwner, hasError, errorString) {
      if (hasError) {
        if (errorString.indexOf(`"code":"${QFieldCloudUtils.errorCodeOverQuota}"`) >= 0) {
          // Let the storage meter and the toast message inviting users to upgrade subscription
          transferError.hasError = false;
          if (transferError.visible) {
            transferError.detailsText = "";
          }
        } else {
          transferError.hasError = hasError;
          if (transferError.visible) {
            transferError.detailsText = errorString;
          }
        }
      }
      const cloudProject = cloudProjectsModel.findProject(projectId);
      if (cloudProject.packagedLayerErrors.length !== 0) {
        cloudPackageLayersFeedback.packagedLayersListViewModel = cloudProject.packagedLayerErrors;
        cloudPackageLayersFeedback.visible = true;
      }
    }

    function onProjectCreated(projectId, fromProjectId, hasError, errorString) {
      if (fromProjectId !== "") {
        return;
      }
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
      iface.loadFile(cloudProjectCreationConnection.target.localPath, cloudProjectCreationConnection.target.name);
      cloudProjectCreationConnection.target = null;
      popup.close();
    }
  }

  QfDialog {
    id: revertDialog
    parent: mainWindow.contentItem

    property int selectedCount: 0
    property bool isDeleted: false

    title: qsTr("Discard local changes")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("Should local changes be discarded?")
    }

    onAccepted: {
      revertLocalChangesFromCurrentProject();
      swipeView.currentIndex = 0;
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
      swipeView.currentIndex = 0;
    }
    onRejected: {
      visible = false;
      popup.focus = true;
    }
  }

  function goBack() {
    if (swipeView.currentIndex === 1) {
      swipeView.currentIndex = 0;
    } else if (connectionSettings.visible) {
      if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
        connectionSettings.visible = false;
      } else {
        popup.close();
      }
    } else {
      popup.close();
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
      cloudConnection.getServerInformation();
    }
    if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
      fetchSubscriptionInformation();
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
    if (shouldDownloadUpdates && storageMeterBar.value >= 1.0) {
      if (storageMeterBar.relatedUrl != "") {
        displayToast(qsTr("Project %1 cannot be packaged as your available storage is full.").arg(ProjectUtils.title(qgisProject)), 'info', qsTr('Upgrade storage'), function () {
          Qt.openUrlExternally(storageMeterBar.relatedUrl);
        });
      } else {
        displayToast(qsTr("Project %1 cannot be packaged as the project owner's available storage is full.").arg(ProjectUtils.title(qgisProject)), 'warning');
      }
      return;
    }

    if (cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.Idle) {
      cloudProjectsModel.projectPush(cloudProjectsModel.currentProjectId, shouldDownloadUpdates);
    }
  }

  function revertLocalChangesFromCurrentProject() {
    if (cloudProjectsModel.currentProject && cloudProjectsModel.currentProject.status === QFieldCloudProject.Idle) {
      if (cloudProjectsModel.revertLocalChangesFromCurrentProject()) {
        displayToast(qsTr('Local changes discarded'));
      } else {
        displayToast(qsTr('Failed to discard changes'), 'error');
      }
    } else {
      displayToast(qsTr('No changes to discard'));
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

  function fetchSubscriptionInformation() {
    if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
      const owner = cloudProjectsModel.currentProject ? cloudProjectsModel.currentProject.owner : cloudConnection.username;
      const isOwnSubscription = !cloudProjectsModel.currentProject || owner === cloudConnection.username;
      if (owner !== lastSubscriptionUser) {
        storageMeterBar.visible = false;
        storageMeterBar.value = 0;
      }
      if (isOwnSubscription) {
        storageMeterBar.loading = true;
        storageMeterBar.visible = true;
      }
      cloudConnection.getSubscriptionInformation(owner);
    }
  }

  function showStorageBar(usedBytes, totalBytes, plan, thresholdWarningBytes, thresholdCriticalBytes) {
    const owner = cloudProjectsModel.currentProject ? cloudProjectsModel.currentProject.owner : cloudConnection.username;
    lastSubscriptionUser = owner;
    const usageRatio = usedBytes / totalBytes;
    const warnRatio = thresholdWarningBytes > 0 ? 1.0 - (thresholdWarningBytes / totalBytes) : 0.8;
    const critRatio = thresholdCriticalBytes > 0 ? 1.0 - (thresholdCriticalBytes / totalBytes) : 0.95;
    storageMeterBar.value = usageRatio;
    storageMeterBar.usageText = qsTr("Used %1 of %2").arg(FileUtils.representFileSize(usedBytes, true)).arg(FileUtils.representFileSize(totalBytes, true));
    storageMeterBar.relatedUrl = QFieldCloudUtils.subscriptionManagementUrl(cloudConnection.url, plan, cloudProjectsModel.currentProject ? cloudProjectsModel.currentProject.owner : "", cloudConnection.username);
    storageMeterBar.warningThreshold = warnRatio;
    storageMeterBar.criticalThreshold = critRatio;
    storageMeterBar.visible = true;
  }
}
