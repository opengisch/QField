

# File QfCloudScreen.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfCloudScreen.qml**](QfCloudScreen_8qml.md)

[Go to the documentation of this file](QfCloudScreen_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import org.qfield.core
import org.qfield.gui
import org.qfield.app

Page {
  id: qfieldCloudScreen

  signal finished
  signal viewProjectFolder(string projectPath)

  property string requestedProjectDetails: ""
  property string pendingCreatedProjectId: ""
  property bool pendingCreatedDownloadStarted: false
  property QfCloudStatus cloudServiceStatus: null
  property QfCloudProjectsModel cloudProjectsModel: null

  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

  header: QfPageHeader {
    title: qsTr("QFieldCloud")

    showBackButton: true
    showApplyButton: false
    showCancelButton: false
    showMenuButton: true

    busyIndicatorState: cloudConnection.status === QfCloudConnection.Connecting ? 'on' : 'off'
    busyIndicatorValue: busyIndicatorState === 'on' ? 0 : 1

    topMargin: mainWindow.sceneTopMargin

    onFinished: {
      if (connectionSettings.visible) {
        if (cloudConnection.status === QfCloudConnection.LoggedIn || table.count > 0) {
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
      visible: projectsSwipeView.currentIndex !== 1

      QfTabBar {
        id: filterBar
        Layout.fillWidth: true
        Layout.preferredHeight: defaultHeight
        Layout.leftMargin: 68
        visible: !connectionSettings.visible
        boundsBehavior: Flickable.StopAtBounds
        model: cloudProjectsModel.hasTemplates && !filterModel.showLocalOnly ? [qsTr("Projects"), qsTr("Templates")] : [qsTr("Projects")]

        Material.accent: cloudProjectsModel.hasTemplates ? QfTheme.mainColor : QfTheme.mainTextColor
        highlight: Item {
          Rectangle {
            height: cloudProjectsModel.hasTemplates ? 2 : 0
            color: QfTheme.mainColor
            radius: 4
            width: parent.width
            anchors.bottom: parent.bottom
          }
        }

        onCurrentIndexChanged: {
          filterModel.showTemplates = currentIndex == 1;
        }
      }

      Item {
        id: topSpacer
        Layout.fillWidth: true
        visible: !filterBar.visible
      }

      Rectangle {
        id: cloudAvatarRect
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.margins: 10
        Layout.preferredWidth: 48
        Layout.preferredHeight: 48
        radius: width / 2
        color: QfTheme.controlBackgroundAlternateColor
        layer.enabled: true

        Rectangle {
          id: cloudAvatarMask
          anchors.centerIn: parent
          width: cloudAvatar.width * 2
          height: cloudAvatar.height * 2
          radius: width / 2
          color: "white"
          visible: false
          layer.enabled: true
        }

        Image {
          id: cloudAvatar
          anchors.centerIn: parent
          fillMode: Image.PreserveAspectFit
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

        QfCloudLogin {
          id: qfieldCloudLogin
          isVisible: connectionSettings.visible
          width: connectionSettings.width
          cloudServiceStatus: qfieldCloudScreen.cloudServiceStatus
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

        QfCloudStatusBanner {
          cloudServiceStatus: qfieldCloudScreen.cloudServiceStatus
        }

        QfSearchBar {
          id: searchBar
          Layout.fillWidth: true
          Layout.preferredHeight: searchHeight
          enableFilterButton: true
          filterActive: projectFilter.visible
          placeHolderText: filterModel.showTemplates ? qsTr("Search for templates") : qsTr("Search for projects")
          parameterKeys: ["owner", "include"]
          z: 10

          QfCloudProjectFilter {
            id: projectFilter
            width: parent.width
            height: Math.min(mainWindow.height - mainWindow.sceneTopMargin - mainWindow.sceneBottomMargin - 186, 400)
            visible: false

            currentUsername: cloudConnection.username
            isTemplateSearch: filterBar.currentIndex === 1

            onQueryStringChanged: {
              if (visible) {
                searchBar.setSearchTerm(queryString);
              }
            }

            onApplyFilter: {
              table.model.textFilter = queryString;
              searchBar.setSearchTerm(queryString);
              visible = false;
            }
          }

          onFilterClicked: {
            if (!projectFilter.visible) {
              projectFilter.visible = true;
              projectFilter.updateQueryFromString(searchBar.searchTerm);
            } else {
              projectFilter.applyFilter();
            }
          }

          onSearchTermChanged: {
            if (!projectFilter.visible) {
              table.model.textFilter = searchBar.searchTerm;
            }
          }

          onSearchTermEdited: {
            if (projectFilter.visible) {
              projectFilter.updateQueryFromString(searchBar.searchTerm);
            }
          }

          onSearchTriggered: {
            if (projectFilter.visible) {
              projectFilter.applyFilter();
            } else {
              table.model.textFilter = searchBar.searchTerm;
            }
          }

          onCleared: {
            if (projectFilter.visible) {
              projectFilter.clear();
            } else {
              table.model.textFilter = "";
            }
          }
        }

        Rectangle {
          Layout.fillWidth: true
          Layout.fillHeight: true
          color: QfTheme.controlBackgroundColor
          border.color: QfTheme.controlBorderColor
          border.width: 1

          ListView {
            id: table

            property bool overshootRefresh: false

            model: QfCloudProjectsFilterModel {
              id: filterModel
              projectsModel: cloudProjectsModel
              showLocalOnly: cloudConnection.status !== QfCloudConnection.LoggedIn
              showInValidProjects: settings ? settings.valueBool("/QField/showInvalidProjects", false) : false
              showTemplates: false
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
                color: QfTheme.controlBorderColor

                Text {
                  anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                  }
                  font: QfTheme.strongResultFont
                  color: QfTheme.mainTextColor
                  text: section
                }
              }
            }
            enabled: !projectFilter.visible
            opacity: enabled ? 1 : 0.5
            clip: true

            onMovingChanged: {
              if (!moving && overshootRefresh && cloudConnection.state === QfCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0) {
                refreshProjectsList(false);
              }
              overshootRefresh = false;
            }

            onVerticalOvershootChanged: {
              if (verticalOvershoot < -100) {
                overshootRefresh = true;
              }
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
              property int projectType: ProjectType

              width: parent ? parent.width : undefined
              height: line.height
              color: "transparent"
              border.color: QfTheme.controlBorderColor
              border.width: 1
              radius: 2

              ProgressBar {
                anchors.bottom: line.bottom
                anchors.left: line.left
                anchors.right: parent.right
                height: 4
                indeterminate: PackagingStatus !== QfCloudProject.PackagingFinishedStatus && DownloadProgress === 0.0
                value: DownloadProgress
                visible: Status === QfCloudProject.ProjectStatus.Downloading
                z: 1
              }

              Row {
                id: line
                width: parent.width
                leftPadding: 6
                rightPadding: 6
                topPadding: 6
                bottomPadding: 6
                spacing: 2

                QfParameterizedImage {
                  id: type
                  anchors.verticalCenter: line.verticalCenter

                  strokeColor: QfTheme.mainColor
                  parameters: {
                    "cloud": QfTheme.cloudColor
                  }

                  source: {
                    if (cloudConnection.status !== QfCloudConnection.LoggedIn) {
                      return QfTheme.getThemeVectorIcon('ic_cloud_project_localonly_param_48dp');
                    } else {
                      if (projectDelegate.projectType === QfCloudProject.ProjectType.Template) {
                        return QfTheme.getThemeVectorIcon('ic_cloud_project_template_param_48dp');
                      }
                      var status = '';
                      switch (Status) {
                      case QfCloudProject.ProjectStatus.Downloading:
                        return QfTheme.getThemeVectorIcon('ic_cloud_project_download_param_48dp');
                      case QfCloudProject.ProjectStatus.Pushing:
                        return QfTheme.getThemeVectorIcon('ic_cloud_project_upload_param_48dp');
                      case QfCloudProject.ProjectStatus.Failing:
                        return QfTheme.getThemeVectorIcon('ic_cloud_project_failed_param_48dp');
                      default:
                        break;
                      }
                      switch (Checkout) {
                      case QfCloudProject.LocalCheckout:
                        return QfTheme.getThemeVectorIcon('ic_cloud_project_localonly_param_48dp');
                      case QfCloudProject.RemoteCheckout:
                        return QfTheme.getThemeVectorIcon('ic_cloud_project_download_param_48dp');
                      default:
                        break;
                      }
                      return QfTheme.getThemeVectorIcon('ic_cloud_project_param_48dp');
                    }
                  }
                  width: 40
                  height: 40
                  opacity: Status === QfCloudProject.ProjectStatus.Downloading ? 0.3 : 1

                  QfToolButton {
                    anchors.top: parent.top
                    anchors.topMargin: -3
                    anchors.left: parent.left
                    anchors.leftMargin: -2
                    width: 24
                    height: 24
                    padding: 2
                    iconSource: QfTheme.getThemeVectorIcon('ic_star_filled_white_24dp')
                    iconColor: QfTheme.mainColor
                    enabled: false
                    bgcolor: QfTheme.controlBackgroundColor
                    round: true
                    visible: Featured
                  }
                }

                ColumnLayout {
                  id: inner
                  width: projectDelegate.width - type.width - menuButton.width - 16
                  anchors.verticalCenter: line.verticalCenter
                  spacing: 2

                  Text {
                    id: projectTitle
                    Layout.fillWidth: true
                    leftPadding: 3
                    text: Name
                    font.pointSize: QfTheme.tipFont.pointSize
                    color: QfTheme.mainTextColor
                    opacity: projectDelegate.isPressed ? 0.8 : 1
                    wrapMode: Text.Wrap
                  }
                  Text {
                    id: projectNote
                    Layout.fillWidth: true
                    leftPadding: 3
                    text: {
                      if (cloudConnection.status !== QfCloudConnection.LoggedIn) {
                        return QfStringUtils.snippet(Description);
                      } else {
                        var status = '';

                        // TODO I think these should be shown as UI badges
                        switch (Status) {
                        case QfCloudProject.Idle:
                          break;
                        case QfCloudProject.Downloading:
                          if (PackagingStatus === QfCloudProject.PackagingBusyStatus) {
                            status = qsTr('QFieldCloud is packaging the latest data just for you; this might take some time, please hold tight');
                          } else {
                            if (PackagingStatus === QfCloudProject.PackagingFinishedStatus || DownloadProgress > 0.0) {
                              if (DownloadSize > 0) {
                                status = qsTr('Downloading, %1% of %2 fetched').arg(Math.round(DownloadProgress * 100)).arg(QfFileUtils.representFileSize(DownloadSize));
                              } else {
                                status = qsTr('Downloading, %1% fetched').arg(Math.round(DownloadProgress * 100));
                              }
                            } else {
                              status = qsTr('Reaching out to QFieldCloud to download project');
                            }
                          }
                          break;
                        case QfCloudProject.Pushing:
                          status = qsTr('Uploading…');
                          break;
                        default:
                          break;
                        }
                        switch (ErrorStatus) {
                        case QfCloudProject.NoErrorStatus:
                          break;
                        case QfCloudProject.DownloadErrorStatus:
                          status = qsTr('Downloading error. ') + QfCloudUtils.userFriendlyErrorString(ErrorString);
                          break;
                        case QfCloudProject.PushErrorStatus:
                          status = qsTr('Uploading error. ') + QfCloudUtils.userFriendlyErrorString(ErrorString);
                          break;
                        }

                        if (!status) {
                          status = QfStringUtils.snippet(Description);
                        }

                        var str = status;
                        return str.trim();
                      }
                    }
                    visible: text !== ""
                    font.pointSize: QfTheme.tipFont.pointSize - 2
                    color: QfTheme.secondaryTextColor
                    wrapMode: Text.WordWrap
                  }
                }

                Item {
                  width: QfTheme.toolButtonSize
                  height: QfTheme.toolButtonSize
                  anchors.verticalCenter: line.verticalCenter

                  QfToolButton {
                    id: downloadActionButton

                    readonly property bool isTemplate: projectDelegate.projectType === QfCloudProject.ProjectType.Template

                    visible: LocalPath === ''
                    iconSource: {
                      if (isTemplate) {
                        return QfTheme.getThemeVectorIcon("ic_add_white_24dp");
                      }
                      return Status === QfCloudProject.ProjectStatus.Downloading ? QfTheme.getThemeVectorIcon("ic_clear_white_24dp") : QfTheme.getThemeVectorIcon("ic_download_white_24dp");
                    }
                    iconColor: Status === QfCloudProject.ProjectStatus.Downloading ? QfTheme.mainTextColor : QfTheme.mainColor
                    opacity: Status === QfCloudProject.ProjectStatus.Downloading ? 0.5 : 1

                    onClicked: {
                      if (isTemplate) {
                        cloneProjectDialog.sourceProjectId = Id;
                        cloneProjectName.text = Name;
                        cloneProjectDialog.open();
                      } else if (Status === QfCloudProject.ProjectStatus.Downloading) {
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
                    width: QfTheme.toolButtonSize
                    height: QfTheme.toolButtonSize
                    visible: LocalPath !== ''

                    bgcolor: "transparent"
                    iconSource: QfTheme.getThemeVectorIcon("ic_dot_menu_black_24dp")
                    iconColor: QfTheme.mainTextColor

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
                    color: showSync ? QfTheme.mainColor : QfTheme.cloudColor
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

            MouseArea {
              property Item pressedItem
              propagateComposedEvents: false
              anchors.fill: parent
              anchors.rightMargin: QfTheme.toolButtonSize
              onClicked: mouse => {
                var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
                if (item) {
                  if (item.projectLocalPath !== '' && item.projectType !== QfCloudProject.ProjectType.Template) {
                    qfieldCloudScreen.visible = false;
                    iface.loadFile(item.projectLocalPath, item.projectName);
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

          Label {
            anchors.fill: parent
            anchors.margins: 20
            visible: cloudConnection.status === QfCloudConnection.LoggedIn && table.count === 0
            text: {
              const isTemplate = filterBar.currentIndex === 1;
              let labelText = "";
              if (cloudProjectsModel.isRefreshing) {
                labelText = isTemplate ? qsTr("Refreshing templates list...") : qsTr("Refreshing projects list...");
              } else if (table.model.isSearching) {
                labelText = isTemplate ? qsTr("Searching for templates...") : qsTr("Searching for projects...");
              } else if (searchBar.searchTerm.trim() !== "") {
                labelText = isTemplate ? qsTr("No templates found.") : qsTr("No cloud projects found.");
                const parameters = projectFilter.getQueryParametersFromString(searchBar.searchTerm);
                if (parameters["includePublic"] === false) {
                  labelText += "\n\n";
                  if (cloudConnection.url == cloudConnection.defaultUrl) {
                    labelText += isTemplate ? qsTr("Try to %1include public templates%2 and see what the community has to offer.").arg("<a href=\"#includePublic\">").arg("</a>") : qsTr("Try to %1include public projects%2 and see what the community has to offer.").arg("<a href=\"#includePublic\">").arg("</a>");
                  } else {
                    labelText += isTemplate ? qsTr("Try to %1include public templates%2.").arg("<a href=\"#includePublic\">").arg("</a>") : qsTr("Try to %1include public projects%2.").arg("<a href=\"#includePublic\">").arg("</a>");
                  }
                }
              } else {
                labelText = qsTr("No cloud projects found.");
                if (Qt.platform !== "ios") {
                  labelText += "\n\n" + qsTr("To get started, %1read the documentation%2.").arg("<a href=\"https://docs.qfield.org/get-started/tutorials/get-started-qfc/\">").arg("</a>");
                }
              }
              return labelText;
            }
            textFormat: Text.MarkdownText
            font: QfTheme.defaultFont
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            onLinkActivated: link => {
              if (link === "#includePublic") {
                searchBar.setSearchTerm("include:public " + searchBar.searchTerm.trim());
              } else {
                Qt.openUrlExternally(link);
              }
            }
          }
        }

        RowLayout {
          Layout.fillWidth: true
          Layout.topMargin: 5
          visible: !projectFilter.visible

          QfButton {
            id: refreshProjectsListBtn
            Layout.fillWidth: true
            text: filterBar.currentIndex === 1 ? qsTr("Refresh templates list") : qsTr("Refresh projects list")
            enabled: cloudConnection.status === QfCloudConnection.LoggedIn && cloudConnection.state === QfCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0 && pendingCreatedProjectId === ""
            showProgress: cloudProjectsModel.isRefreshing || table.model.isSearching
            progressValue: 0
            onClicked: {
              refreshProjectsList(true);
            }
          }

          QfToolButton {
            id: scanProjectBtn
            enabled: cloudConnection.status === QfCloudConnection.LoggedIn && cloudConnection.state === QfCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0 && pendingCreatedProjectId === ""
            visible: enabled

            bgcolor: "transparent"
            iconSource: QfTheme.getThemeVectorIcon("ic_qr_code_black_24dp")
            iconColor: QfTheme.mainTextColor

            onClicked: {
              codeReaderConnection.enabled = true;
              codeReader.open();
            }
          }
        }
      }

      QfCloudProjectDetails {
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
      text: cloudConnection.status === QfCloudConnection.LoggedIn ? qsTr('Sign out') : qsTr('Sign in')
      font: QfTheme.defaultFont
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding
      icon.source: cloudConnection.status === QfCloudConnection.LoggedIn ? QfTheme.getThemeVectorIcon('ic_logout_24dp') : QfTheme.getThemeVectorIcon('ic_login_24dp')
      enabled: cloudConnection.state !== QfCloudConnection.Busy
      onTriggered: {
        if (cloudConnection.status === QfCloudConnection.LoggedIn) {
          cloudConnection.logout();
        } else {
          prepareCloudScreen();
        }
      }
    }

    MenuItem {
      property bool isVisible: Qt.platform.os !== "ios" && cloudConnection.url === cloudConnection.defaultUrl

      text: qsTr('Settings page')
      font: QfTheme.defaultFont
      visible: isVisible
      height: isVisible ? 48 : 0
      leftPadding: QfTheme.menuItemLeftPadding
      icon.source: QfTheme.getThemeVectorIcon('ic_tune_white_24dp')
      enabled: cloudConnection.state !== QfCloudConnection.Busy && cloudConnection.status === QfCloudConnection.LoggedIn
      onTriggered: {
        const link = cloudConnection.defaultUrl + '/settings/' + cloudConnection.username;
        if (Qt.platform.os === "ios" || Qt.platform.os === "android") {
          browserPopup.url = link;
          browserPopup.fullscreen = true;
          browserPopup.open();
        } else {
          Qt.openUrlExternally(link);
        }
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr('Show invalid projects')
      font: QfTheme.defaultFont
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding
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

      font: QfTheme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: QfTheme.menuItemLeftPadding

      text: qsTr("Open Project")
      onTriggered: {
        if (projectActions.projectLocalPath != '') {
          qfieldCloudScreen.visible = false;
          iface.loadFile(projectActions.projectLocalPath, projectActions.projectName);
        }
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: syncProject

      font: QfTheme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: QfTheme.menuItemLeftPadding

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
        color: QfTheme.mainColor
        border.color: "transparent"
      }
    }
    MenuItem {
      id: pushProject

      font: QfTheme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: QfTheme.menuItemLeftPadding
      enabled: projectActions.localDeltasCount > 0

      text: qsTr("Upload local changes")
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
        color: QfTheme.cloudColor
        badgeText.text: projectActions.localDeltasCount
        badgeText.color: QfTheme.light
        border.color: "transparent"
      }
    }

    MenuItem {
      id: cloneProject

      font: QfTheme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: QfTheme.menuItemLeftPadding

      text: qsTr("Clone Project")
      onTriggered: {
        cloneProjectDialog.sourceProjectId = projectActions.projectId;
        cloneProjectName.text = projectActions.projectName;
        cloneProjectDialog.open();
      }
    }

    MenuItem {
      id: removeProject

      font: QfTheme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: QfTheme.menuItemLeftPadding

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

      font: QfTheme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: QfTheme.menuItemLeftPadding

      text: qsTr("View Project Details")
      onTriggered: {
        projectDetails.cloudProject = cloudProjectsModel.findProject(projectActions.projectId);
        projectsSwipeView.currentIndex = 1;
      }
    }
    MenuItem {
      id: viewProjectFolder

      font: QfTheme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: QfTheme.menuItemLeftPadding

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
      welcomeScreen.model.removeRecentProject(projectActions.projectLocalPath);
      welcomeScreen.model.reloadModel();
      if (projectActions.projectLocalPath === qgisProject.fileName) {
        iface.clearProject();
      }
    }
    onRejected: {
      visible = false;
    }
  }

  QfDialog {
    id: cloneProjectDialog
    parent: mainWindow.contentItem
    title: qsTr("Project Creation")
    width: mainWindow.width - 40
    property string sourceProjectId: ""

    ColumnLayout {
      width: cloneProjectDialog.availableWidth
      spacing: 10

      Label {
        Layout.fillWidth: true
        wrapMode: Text.WordWrap
        text: qsTr("Enter your project name")
      }

      QfTextField {
        id: cloneProjectName
        Layout.fillWidth: true
      }

      Label {
        Layout.fillWidth: true
        wrapMode: Text.WordWrap
        text: qsTr("Allowed characters: 0-9A-Za-z_-")
        font: QfTheme.tipFont
        color: QfTheme.secondaryTextColor
      }
    }

    onAccepted: {
      const trimmedName = cloneProjectName.text.trim();
      if (trimmedName !== "") {
        busyOverlay.text = qsTr("Creating project…");
        busyOverlay.state = "visible";
        cloudProjectsModel.createProject(trimmedName, sourceProjectId);
      }
    }
  }

  Connections {
    id: codeReaderConnection
    target: codeReader
    enabled: false

    function onDecoded(string) {
      const details = QfUrlUtils.getActionDetails(string);
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
      if (cloudConnection.status === QfCloudConnection.LoggedIn) {
        prepareCloudScreen();
      } else if (cloudConnection.status === QfCloudConnection.Disconnected) {
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
      if (requestedProjectDetails != "") {
        requestedProjectDetails = "";
        if (hasError) {
          displayToast(qsTr("QFieldCloud project details fetching failed"));
        } else {
          projectDetails.cloudProject = cloudProjectsModel.findProject(projectId);
          projectsSwipeView.currentIndex = 1;
        }
      }
    }

    function onProjectCreated(projectId, fromProjectId, hasError, errorString) {
      const isClone = fromProjectId !== "";
      if (hasError) {
        busyOverlay.state = "hidden";
        displayToast(isClone ? qsTr("Project cloning failed: %1").arg(errorString) : qsTr("Project creation failed: %1").arg(errorString));
        return;
      }
      pendingCreatedProjectId = projectId;
      pendingCreatedDownloadStarted = false;
      busyOverlay.text = qsTr("Preparing project…");
    }

    function onProjectDownloaded(projectId, projectName, projectOwner, hasError, errorString) {
      // Only react to the download started as part of a create-from-source flow
      if (projectId !== pendingCreatedProjectId) {
        return;
      }
      pendingCreatedProjectId = "";
      pendingCreatedDownloadStarted = false;
      busyOverlay.state = "hidden";

      if (hasError) {
        displayToast(qsTr("Project created but downloading failed: %1").arg(QfCloudUtils.userFriendlyErrorString(errorString)));
        return;
      }

      projectDetails.cloudProject = cloudProjectsModel.findProject(projectId);
      projectsSwipeView.currentIndex = 1;
    }
  }

  Connections {
    target: pendingCreatedProjectId !== "" ? cloudProjectsModel.findProject(pendingCreatedProjectId) : null
    ignoreUnknownSignals: true

    function onStatusChanged() {
      // The project leaves the Creating state once its create job settles.
      // On success we start the download and on failure we surface the error and clear the overlay
      if (target.status === QfCloudProject.ProjectStatus.Creating) {
        return;
      }
      if (target.status === QfCloudProject.ProjectStatus.Failing) {
        pendingCreatedProjectId = "";
        pendingCreatedDownloadStarted = false;
        busyOverlay.state = "hidden";
        displayToast(qsTr("The newly-created project could not be prepared."), 'warning');
        return;
      }
      if (target.status === QfCloudProject.ProjectStatus.Idle && !pendingCreatedDownloadStarted) {
        pendingCreatedDownloadStarted = true;
        busyOverlay.text = qsTr("Downloading project…");
        cloudProjectsModel.projectPackageAndDownload(pendingCreatedProjectId);
      }
    }

    function onDownloadProgressChanged() {
      busyOverlay.progress = target.downloadProgress;
    }
  }

  function refreshProjectsList(shouldResetModel) {
    if (cloudConnection.state !== QfCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0) {
      return;
    }
    cloudProjectsModel.refreshProjectsList(shouldResetModel);
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
      case QfCloudConnection.Disconnected:
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
        cloudConnection.getServerInformation();
        break;
      case QfCloudConnection.Connecting:
        const hasProjects = table.count !== 0;
        projectsSwipeView.visible = hasProjects;
        connectionSettings.visible = !hasProjects;
        break;
      case QfCloudConnection.LoggedIn:
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
      if (projectFilter.visible) {
        projectFilter.applyFilter();
        return;
      }

      header.onFinished();
    }
  }
}
```


