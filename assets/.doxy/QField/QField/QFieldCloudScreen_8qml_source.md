

# File QFieldCloudScreen.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**QFieldCloudScreen.qml**](QFieldCloudScreen_8qml.md)

[Go to the documentation of this file](QFieldCloudScreen_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import org.qfield
import Theme

Page {
  id: qfieldCloudScreen

  signal finished
  signal viewProjectFolder(string projectPath)

  property string requestedProjectDetails: ""

  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

  header: QfPageHeader {
    title: qsTr("QFieldCloud Projects")

    showBackButton: true
    showApplyButton: false
    showCancelButton: false
    showMenuButton: true

    busyIndicatorState: cloudConnection.status === QFieldCloudConnection.Connecting || cloudConnection.state === QFieldCloudConnection.Busy ? 'on' : 'off' || cloudProjectsModel.busyProjectIds.length > 0
    busyIndicatorValue: busyIndicatorState === 'on' ? 0 : 1

    topMargin: mainWindow.sceneTopMargin

    onFinished: {
      if (connectionSettings.visible) {
        if (cloudConnection.status === QFieldCloudConnection.LoggedIn || table.count > 0) {
          connectionSettings.visible = false;
          projectsSwipeView.visible = true;
        } else {
          parent.finished();
        }
      } else if (projectsSwipeView.currentIndex === 1) {
        projectDetails.cloudProject = undefined;
        projectsSwipeView.currentIndex = 0;
      } else {
        parent.finished();
      }
    }

    onOpenMenu: qfieldCloudScreenOption.open()
  }

  ColumnLayout {
    anchors.fill: parent
    anchors.bottomMargin: mainWindow.sceneBottomMargin
    spacing: 2

    RowLayout {
      id: connectionInformation
      spacing: 2
      Layout.fillWidth: true
      visible: (cloudConnection.status === QFieldCloudConnection.LoggedIn || table.count > 0) && projectsSwipeView.currentIndex !== 1

      Label {
        Layout.fillWidth: true
        padding: 10
        opacity: projectsSwipeView.visible ? 1 : 0
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
        border.color: Theme.mainColor
        border.width: 1
        radius: width / 2
        clip: true

        Rectangle {
          id: roundMask
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
          fillMode: Image.PreserveAspectFit
          smooth: true
          source: cloudConnection.avatarUrl !== '' ? cloudConnection.avatarUrl : 'qrc:/images/qfieldcloud_logo.svg'
          width: 48
          height: 48
          sourceSize.width: width * screen.devicePixelRatio
          sourceSize.height: height * screen.devicePixelRatio
          layer.enabled: true
          layer.effect: QfOpacityMask {
            maskSource: roundMask
          }

          onStatusChanged: {
            // In case the avatar URL fails to load or the image is corrupted, revert to our lovely Nyuki
            if (status == Image.Error) {
              source = 'qrc:/images/qfieldcloud_logo.svg';
            }
          }
        }

        MouseArea {
          anchors.fill: parent

          onClicked: {
            if (!connectionSettings.visible) {
              connectionSettings.visible = true;
              projectsSwipeView.visible = false;
            } else {
              connectionSettings.visible = false;
              projectsSwipeView.visible = true;
              refreshProjectsListBtn.forceActiveFocus();
            }
          }
        }
      }
    }

    ColumnLayout {
      id: connectionSettings
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.margins: 0
      Layout.topMargin: connectionInformation.visible ? 10 : connectionInformation.childrenRect.height
      spacing: 2
      visible: !connectionInformation.visible

      ScrollView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical: QfScrollBar {}
        contentWidth: qfieldCloudLogin.width
        contentHeight: qfieldCloudLogin.height
        clip: true

        QFieldCloudLogin {
          id: qfieldCloudLogin
          isVisible: connectionSettings.visible
          width: connectionSettings.width
        }
      }

      Item {
        Layout.fillHeight: true
        height: 15
      }
    }

    SwipeView {
      id: projectsSwipeView
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.margins: 10
      Layout.topMargin: 0
      visible: connectionInformation.visible
      clip: true
      interactive: false

      ColumnLayout {
        id: projects
        spacing: 2

        QfTabBar {
          id: filterBar
          model: [qsTr("My Projects"), qsTr("Community")]
          Layout.fillWidth: true
          Layout.preferredHeight: defaultHeight
        }

        QfSearchBar {
          id: searchBar
          Layout.fillWidth: true
          Layout.preferredHeight: 41
          placeHolderText: qsTr("Search for project")
        }

        Item {
          Layout.fillWidth: true
          Layout.fillHeight: true

          ListView {
            id: table

            property bool overshootRefresh: false

            model: QFieldCloudProjectsFilterModel {
              projectsModel: cloudProjectsModel
              filter: filterBar.currentIndex === 0 ? QFieldCloudProjectsFilterModel.PrivateProjects : QFieldCloudProjectsFilterModel.PublicProjects
              showLocalOnly: cloudConnection.status !== QFieldCloudConnection.LoggedIn
              showInValidProjects: settings ? settings.valueBool("/QField/showInvalidProjects", false) : false
              showFeaturedOnTop: filterBar.currentIndex === 1
              textFilter: searchBar.searchTerm
              onFilterChanged: {
                if (cloudConnection.state === QFieldCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0) {
                  refreshProjectsList(false, filter === QFieldCloudProjectsFilterModel.PublicProjects);
                }
              }
            }

            ScrollBar.vertical: QfScrollBar {
              verticalPadding: 15
            }

            anchors.fill: parent
            anchors.margins: 1
            section.property: "Owner"
            section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
            spacing: -1
            section.delegate: Component {
              Rectangle {
                width: parent.width
                height: 30
                color: Theme.controlBorderColor

                Text {
                  anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                  }
                  font.bold: true
                  font.pointSize: Theme.resultFont.pointSize
                  color: Theme.mainTextColor
                  text: section
                }
              }
            }
            clip: true

            onMovingChanged: {
              if (!moving && overshootRefresh && cloudConnection.state === QFieldCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0) {
                refreshProjectsList(false, filterBar.currentIndex !== 0);
              }
              overshootRefresh = false;
            }

            onVerticalOvershootChanged: {
              if (verticalOvershoot < -100 || verticalOvershoot > 100)
                overshootRefresh = true;
            }

            delegate: Rectangle {
              id: projectDelegate

              property bool isPressed: false
              property string projectId: Id
              property string projectOwner: Owner
              property string projectName: Name
              property string projectLocalPath: LocalPath
              property int status: Status
              property int localDeltasCount: LocalDeltasCount
              property bool projectOutdated: ProjectOutdated

              width: parent ? parent.width : undefined
              height: line.height
              color: Theme.controlBackgroundColor
              border.color: Theme.controlBorderColor
              border.width: 1
              radius: 2

              ProgressBar {
                anchors.bottom: line.bottom
                anchors.left: line.left
                anchors.right: parent.right
                height: 6
                indeterminate: PackagingStatus !== QFieldCloudProject.PackagingFinishedStatus && DownloadProgress === 0.0
                value: DownloadProgress
                visible: Status === QFieldCloudProject.ProjectStatus.Downloading
                z: 1
              }

              Row {
                id: line
                width: parent.width
                leftPadding: 6
                rightPadding: 6
                topPadding: 4
                bottomPadding: 8
                spacing: 2

                Image {
                  id: type
                  anchors.verticalCenter: line.verticalCenter
                  source: {
                    if (cloudConnection.status !== QFieldCloudConnection.LoggedIn) {
                      return Theme.getThemeVectorIcon('ic_cloud_project_localonly_48dp');
                    } else {
                      var status = '';
                      switch (Status) {
                      case QFieldCloudProject.ProjectStatus.Downloading:
                        return Theme.getThemeVectorIcon('ic_cloud_project_download_48dp');
                      case QFieldCloudProject.ProjectStatus.Pushing:
                        return Theme.getThemeVectorIcon('ic_cloud_project_upload_48dp');
                      case QFieldCloudProject.ProjectStatus.Failing:
                        return Theme.getThemeVectorIcon('ic_cloud_project_failed_48dp');
                      default:
                        break;
                      }
                      switch (Checkout) {
                      case QFieldCloudProject.LocalCheckout:
                        return Theme.getThemeVectorIcon('ic_cloud_project_localonly_48dp');
                      case QFieldCloudProject.RemoteCheckout:
                        return Theme.getThemeVectorIcon('ic_cloud_project_download_48dp');
                      default:
                        break;
                      }
                      return Theme.getThemeVectorIcon('ic_cloud_project_48dp');
                    }
                  }
                  sourceSize.width: 80
                  sourceSize.height: 80
                  width: 40
                  height: 40
                  opacity: Status === QFieldCloudProject.ProjectStatus.Downloading ? 0.3 : 1

                  QfToolButton {
                    anchors.top: parent.top
                    anchors.topMargin: -3
                    anchors.left: parent.left
                    anchors.leftMargin: -2
                    width: 24
                    height: 24
                    padding: 2
                    iconSource: Theme.getThemeVectorIcon('ic_star_filled_white_24dp')
                    iconColor: Theme.mainColor
                    enabled: false
                    bgcolor: Theme.controlBackgroundColor
                    round: true
                    visible: Featured
                  }
                }

                ColumnLayout {
                  id: inner
                  width: projectDelegate.width - type.width - menuButton.width - 16

                  Text {
                    id: projectTitle
                    Layout.fillWidth: true
                    topPadding: 5
                    leftPadding: 3
                    text: Name
                    font.pointSize: Theme.tipFont.pointSize
                    color: Theme.mainTextColor
                    opacity: projectDelegate.isPressed ? 0.8 : 1
                    wrapMode: Text.Wrap
                  }
                  Text {
                    id: projectNote
                    Layout.fillWidth: true
                    leftPadding: 3
                    text: {
                      if (cloudConnection.status !== QFieldCloudConnection.LoggedIn) {
                        return qsTr('Available locally');
                      } else {
                        var status = '';

                        // TODO I think these should be shown as UI badges
                        switch (Status) {
                        case QFieldCloudProject.Idle:
                          break;
                        case QFieldCloudProject.Downloading:
                          if (PackagingStatus === QFieldCloudProject.PackagingBusyStatus) {
                            status = qsTr('QFieldCloud is packaging the latest data just for you; this might take some time, please hold tight');
                          } else {
                            if (PackagingStatus === QFieldCloudProject.PackagingFinishedStatus || DownloadProgress > 0.0) {
                              if (DownloadSize > 0) {
                                status = qsTr('Downloading, %1% of %2 fetched').arg(Math.round(DownloadProgress * 100)).arg(FileUtils.representFileSize(DownloadSize));
                              } else {
                                status = qsTr('Downloading, %1% fetched').arg(Math.round(DownloadProgress * 100));
                              }
                            } else {
                              status = qsTr('Reaching out to QFieldCloud to download project');
                            }
                          }
                          break;
                        case QFieldCloudProject.Pushing:
                          status = qsTr('Uploading…');
                          break;
                        default:
                          break;
                        }
                        switch (ErrorStatus) {
                        case QFieldCloudProject.NoErrorStatus:
                          break;
                        case QFieldCloudProject.DownloadErrorStatus:
                          status = qsTr('Downloading error. ') + ErrorString;
                          break;
                        case QFieldCloudProject.PushErrorStatus:
                          status = qsTr('Uploading error. ') + ErrorString;
                          break;
                        }
                        if (!status) {
                          switch (Checkout) {
                          case QFieldCloudProject.LocalCheckout:
                            status = UserRoleOrigin === "public" ? qsTr('Available locally') : qsTr('Available locally, missing on the cloud');
                            break;
                          case QFieldCloudProject.RemoteCheckout:
                            status = qsTr('Available on the cloud');
                            break;
                          case QFieldCloudProject.LocalAndRemoteCheckout:
                            status = qsTr('Available locally');
                            if (ProjectOutdated) {
                              status += qsTr(', updated data available on the cloud');
                            }
                            break;
                          default:
                            break;
                          }
                        }
                        var localChanges = (LocalDeltasCount > 0) ? qsTr(', has changes locally') : '';
                        var str = status + localChanges;
                        return str.trim();
                      }
                    }
                    visible: text != ""
                    font.pointSize: Theme.tipFont.pointSize - 2
                    color: Theme.secondaryTextColor
                    wrapMode: Text.WordWrap
                  }
                }

                Item {
                  width: 48
                  height: 48
                  anchors.verticalCenter: line.verticalCenter

                  QfToolButton {
                    id: downloadActionButton

                    visible: LocalPath === ''
                    iconSource: Status === QFieldCloudProject.ProjectStatus.Downloading ? Theme.getThemeVectorIcon("ic_clear_white_24dp") : Theme.getThemeVectorIcon("ic_download_white_24dp")
                    iconColor: Status === QFieldCloudProject.ProjectStatus.Downloading ? Theme.mainTextColor : Theme.mainColor
                    opacity: Status === QFieldCloudProject.ProjectStatus.Downloading ? 0.5 : 1

                    onClicked: {
                      if (Status === QFieldCloudProject.ProjectStatus.Downloading) {
                        cloudProjectsModel.projectCancelDownload(Id);
                      } else {
                        cloudProjectsModel.projectPackageAndDownload(Id);
                      }
                    }
                  }

                  QfToolButton {
                    id: menuButton
                    round: true
                    opacity: 0.5
                    width: 48
                    height: 48
                    visible: LocalPath !== ''

                    bgcolor: "transparent"
                    iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
                    iconColor: Theme.mainTextColor

                    onClicked: mouse => {
                      projectActions.projectId = Id;
                      projectActions.projectOwner = Owner;
                      projectActions.projectName = Name;
                      projectActions.projectLocalPath = LocalPath;
                      projectActions.localDeltasCount = projectDelegate.localDeltasCount;
                      projectActions.projectOutdated = projectDelegate.projectOutdated;
                      openProject.visible = LocalPath !== '';
                      viewProjectFolder.visible = LocalPath !== '';
                      removeProject.visible = LocalPath !== '';
                      const gc = mapToItem(qfieldCloudScreen, 0, 0);
                      projectActions.popup(gc.x + width - projectActions.width, gc.y - height);
                    }
                  }

                  QfBadge {
                    alignment: QfBadge.Alignment.TopRight
                    visible: showSync || showPush
                    color: showSync ? Theme.mainColor : Theme.cloudColor
                    topMargin: 5
                    rightMargin: 5
                    enableGradient: showSync && showPush
                    width: 14

                    readonly property bool showSync: LocalPath !== '' && projectDelegate.projectOutdated
                    readonly property bool showPush: projectDelegate.localDeltasCount > 0
                  }
                }
              }
            }

            Label {
              anchors.fill: parent
              anchors.margins: 20
              visible: cloudConnection.status === QFieldCloudConnection.LoggedIn && parent.count === 0 && filterBar.currentIndex === 0
              text: cloudProjectsModel.isRefreshing ? qsTr("Refreshing projects list") : qsTr("No cloud projects found. To get started, %1read the documentation%2.").arg("<a href=\"https://docs.qfield.org/get-started/tutorials/get-started-qfc/\">").arg("</a>")
              font: Theme.defaultFont
              wrapMode: Text.WordWrap
              horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
              onLinkActivated: link => Qt.openUrlExternally(link)
            }

            MouseArea {
              property Item pressedItem
              propagateComposedEvents: false
              anchors.fill: parent
              anchors.rightMargin: 48
              onClicked: mouse => {
                var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
                if (item) {
                  if (item.projectLocalPath !== '') {
                    qfieldCloudScreen.visible = false;
                    iface.loadFile(item.projectLocalPath);
                  } else {
                    projectDetails.cloudProject = cloudProjectsModel.findProject(item.projectId);
                    projectsSwipeView.currentIndex = 1;
                  }
                }
              }
              onPressed: mouse => {
                var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
                if (item) {
                  pressedItem = item;
                  pressedItem.isPressed = true;
                }
              }
              onCanceled: {
                if (pressedItem) {
                  pressedItem.isPressed = false;
                  pressedItem = null;
                }
              }
              onReleased: {
                if (pressedItem) {
                  pressedItem.isPressed = false;
                  pressedItem = null;
                }
              }

              onPressAndHold: mouse => {
                const item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
                if (item && item.projectLocalPath !== "") {
                  projectActions.projectId = item.projectId;
                  projectActions.projectOwner = item.projectOwner;
                  projectActions.projectName = item.projectName;
                  projectActions.projectLocalPath = item.projectLocalPath;
                  projectActions.localDeltasCount = item.localDeltasCount;
                  projectActions.projectOutdated = item.projectOutdated;
                  openProject.visible = item.projectLocalPath !== '';
                  removeProject.visible = item.projectLocalPath !== '';
                  projectActions.popup(mouse.x, mouse.y);
                }
              }
            }
          }
        }

        RowLayout {
          Layout.fillWidth: true
          Layout.topMargin: 5

          QfButton {
            id: refreshProjectsListBtn
            Layout.fillWidth: true
            text: qsTr("Refresh projects list")
            enabled: cloudConnection.status === QFieldCloudConnection.LoggedIn && cloudConnection.state === QFieldCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0
            onClicked: {
              refreshProjectsList(true, filterBar.currentIndex !== 0);
            }
          }

          QfToolButton {
            id: scanProjectBtn
            enabled: cloudConnection.status === QFieldCloudConnection.LoggedIn && cloudConnection.state === QFieldCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0
            visible: enabled

            bgcolor: "transparent"
            iconSource: Theme.getThemeVectorIcon("ic_qr_code_black_24dp")
            iconColor: Theme.mainTextColor

            onClicked: {
              codeReaderConnection.enabled = true;
              codeReader.open();
            }
          }
        }
      }

      QFieldCloudProjectDetails {
        id: projectDetails

        onSynchronize: {
          cloudProjectsModel.projectPush(projectDetails.cloudProject.id, true);
        }

        onPushChanges: {
          cloudProjectsModel.projectPush(projectDetails.cloudProject.id, false);
        }
      }
    }
  }

  QfMenu {
    id: qfieldCloudScreenOption
    x: parent.width - width - 8

    MenuItem {
      text: cloudConnection.status == QFieldCloudConnection.LoggedIn ? qsTr('Sign out') : qsTr('Sign in')
      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      icon.source: cloudConnection.status == QFieldCloudConnection.LoggedIn ? Theme.getThemeVectorIcon('ic_logout_24dp') : Theme.getThemeVectorIcon('ic_login_24dp')
      enabled: cloudConnection.state != QFieldCloudConnection.Busy
      onTriggered: {
        if (cloudConnection.status == QFieldCloudConnection.LoggedIn) {
          cloudConnection.logout();
        } else {
          prepareCloudScreen();
        }
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr('Show invalid projects')
      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      checkable: true
      checked: settings ? settings.valueBool("/QField/showInvalidProjects", false) : false
      onTriggered: {
        settings.setValue("/QField/showInvalidProjects", checked);
        table.model.showInValidProjects = checked;
      }
    }
  }

  QfMenu {
    id: projectActions

    property string projectId: ''
    property string projectOwner: ''
    property string projectName: ''
    property string projectLocalPath: ''
    property int localDeltasCount: 0
    property bool projectOutdated: false

    title: qsTr('Project Actions')

    topMargin: mainWindow.sceneTopMargin
    bottomMargin: mainWindow.sceneBottomMargin

    MenuItem {
      id: openProject

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      text: qsTr("Open Project")
      onTriggered: {
        if (projectActions.projectLocalPath != '') {
          qfieldCloudScreen.visible = false;
          iface.loadFile(projectActions.projectLocalPath);
        }
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: syncProject

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      text: qsTr("Synchronize")
      onTriggered: {
        cloudProjectsModel.projectPush(projectActions.projectId, true);
      }

      QfBadge {
        width: 16
        height: width
        topMargin: 5
        rightMargin: 5
        alignment: QfBadge.Alignment.TopRight
        visible: projectActions.projectOutdated
        color: Theme.mainColor
        border.color: "transparent"
      }
    }
    MenuItem {
      id: pushProject

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding
      enabled: projectActions.localDeltasCount > 0

      text: qsTr("Push changes")
      onTriggered: {
        cloudProjectsModel.projectPush(projectActions.projectId, false);
      }

      QfBadge {
        width: 16
        height: width
        topMargin: 5
        rightMargin: 5
        alignment: QfBadge.Alignment.TopRight
        visible: projectActions.localDeltasCount > 0
        color: Theme.cloudColor
        badgeText.text: projectActions.localDeltasCount
        badgeText.color: Theme.light
        border.color: "transparent"
      }
    }
    MenuItem {
      id: removeProject

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      text: qsTr("Remove Stored Project")
      onTriggered: {
        confirmRemoveDialog.open();
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: viewProjectDetails

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      text: qsTr("View Project Details")
      onTriggered: {
        projectDetails.cloudProject = cloudProjectsModel.findProject(projectActions.projectId);
        projectsSwipeView.currentIndex = 1;
      }
    }
    MenuItem {
      id: viewProjectFolder

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      text: qsTr("View Project Folder")
      onTriggered: {
        qfieldCloudScreen.viewProjectFolder(projectActions.projectLocalPath);
      }
    }
  }

  QfDialog {
    id: confirmRemoveDialog
    parent: mainWindow.contentItem
    title: removeProject.text

    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("Are you sure you want to remove `%1`?").arg(projectActions.projectName)
    }
    onAccepted: {
      cloudProjectsModel.removeLocalProject(projectActions.projectId);
      iface.removeRecentProject(projectActions.projectLocalPath);
      welcomeScreen.model.reloadModel();
      if (projectActions.projectLocalPath === qgisProject.fileName) {
        iface.clearProject();
      }
    }
    onRejected: {
      visible = false;
    }
  }

  Connections {
    id: codeReaderConnection
    target: codeReader
    enabled: false

    function onDecoded(string) {
      const details = UrlUtils.getActionDetails(string);
      if (details.type !== undefined && details.type === "cloud" && details.project !== undefined && details.project !== "") {
        codeReader.close();
        prepareProjectRequest(details);
      }
    }

    function onAboutToHide() {
      codeReaderConnection.enabled = false;
    }
  }

  Connections {
    target: cloudConnection

    function onStatusChanged() {
      if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
        prepareCloudScreen();
      } else if (cloudConnection.status === QFieldCloudConnection.Disconnected) {
        if (table.count === 0) {
          projectsSwipeView.visible = false;
          connectionSettings.visible = true;
        }
      }
    }
  }

  Connections {
    target: cloudProjectsModel

    function onProjectAppended(projectId, hasError, errorString) {
      requestedProjectDetails = "";
      if (hasError) {
        displayToast(qsTr("QFieldCloud project details fetching failed"));
      } else {
        projectDetails.cloudProject = cloudProjectsModel.findProject(projectId);
        projectsSwipeView.currentIndex = 1;
      }
    }
  }

  function refreshProjectsList(shouldResetModel, shouldFetchPublic) {
    if (cloudConnection.state !== QFieldCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0) {
      return;
    }
    cloudProjectsModel.refreshProjectsList(shouldResetModel, shouldFetchPublic);
    displayToast(qsTr("Refreshing projects list"));
  }

  function prepareProjectRequest(details) {
    if (details.url !== undefined && details.url !== cloudConnection.url) {
      cloudConnection.url = details.url;
    }
    if (details.username !== undefined && details.username !== cloudConnection.username) {
      cloudConnection.username = details.username;
    }
    requestedProjectDetails = details.project;
    if (!visible) {
      visible = true;
    } else {
      prepareCloudScreen();
    }
  }

  function prepareCloudScreen() {
    if (visible) {
      switch (cloudConnection.status) {
      case QFieldCloudConnection.Disconnected:
        if (cloudConnection.hasToken || cloudConnection.hasProviderConfiguration) {
          cloudConnection.login();
          if (requestedProjectDetails != "") {
            // Project details requested, jump on the login screen then project details
            projectsSwipeView.visible = false;
            connectionSettings.visible = true;
          } else {
            // Show projects list while we are logging in
            projectsSwipeView.visible = true;
            connectionSettings.visible = false;
          }
        } else {
          projectsSwipeView.visible = false;
          connectionSettings.visible = true;
        }
        cloudConnection.getAuthenticationProviders();
        break;
      case QFieldCloudConnection.Connecting:
        const hasProjects = table.count !== 0;
        projectsSwipeView.visible = hasProjects;
        connectionSettings.visible = !hasProjects;
        break;
      case QFieldCloudConnection.LoggedIn:
        projectsSwipeView.visible = true;
        connectionSettings.visible = false;
        if (requestedProjectDetails != "") {
          let cloudProject = cloudProjectsModel.findProject(requestedProjectDetails);
          if (cloudProject) {
            requestedProjectDetails = "";
            projectDetails.cloudProject = cloudProject;
            projectsSwipeView.currentIndex = 1;
          } else {
            cloudProjectsModel.appendProject(requestedProjectDetails);
          }
        }
        break;
      }
    }
  }

  Component.onCompleted: {
    prepareCloudScreen();
  }

  onVisibleChanged: {
    prepareCloudScreen();
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true;
      header.onFinished();
    }
  }
}
```


