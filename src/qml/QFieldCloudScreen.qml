import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQml.Models
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Page {
  id: qfieldCloudScreen

  signal finished

  property LayerObserver layerObserver
  property string requestedProjectDetails: ""

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
        connectionSettings.visible = false;
        projectsSwipeView.visible = true;
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
      visible: cloudConnection.status === QFieldCloudConnection.LoggedIn || table.count > 0

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
        ScrollBar.vertical: QfScrollBar {
        }
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
          delegate: TabButton {
            text: modelData
            height: filterBar.defaultHeight
            width: projects.width / filterBar.count
            font: Theme.defaultFont
            onClicked: {
              filterBar.currentIndex = index;
            }
          }
        }

        QfSearchBar {
          id: searchBar
          Layout.fillWidth: true
          Layout.preferredHeight: 41
          placeHolderText: qsTr("Search for project")
        }

        Rectangle {
          Layout.fillWidth: true
          Layout.fillHeight: true
          color: Theme.controlBackgroundColor
          border.color: Theme.controlBorderColor
          border.width: 1

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
              id: rectangle

              property bool isPressed: false
              property string projectId: Id
              property string projectOwner: Owner
              property string projectName: Name
              property string projectLocalPath: LocalPath
              property int status: Status

              width: parent ? parent.width : undefined
              height: line.height
              color: "transparent"

              ProgressBar {
                anchors.bottom: line.bottom
                anchors.bottomMargin: -4
                anchors.left: line.left
                anchors.leftMargin: line.leftPadding
                width: line.width - 20
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
                      case QFieldCloudProject.ProjectStatus.Uploading:
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
                  width: rectangle.width - type.width - menuButton.width - 16

                  Text {
                    id: projectTitle
                    Layout.fillWidth: true
                    topPadding: 5
                    leftPadding: 3
                    text: Name
                    font.pointSize: Theme.tipFont.pointSize
                    font.underline: true
                    color: Theme.mainColor
                    opacity: rectangle.isPressed ? 0.8 : 1
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
                        case QFieldCloudProject.Uploading:
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
                        case QFieldCloudProject.UploadErrorStatus:
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
                    font.italic: true
                    color: Theme.secondaryTextColor
                    wrapMode: Text.WordWrap
                  }
                }

                QfToolButton {
                  id: menuButton
                  round: true
                  opacity: 0.5
                  width: 48
                  height: 48

                  anchors.verticalCenter: line.verticalCenter

                  bgcolor: "transparent"
                  iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
                  iconColor: Theme.mainTextColor

                  onClicked: mouse => {
                    let gc = mapToItem(qfieldCloudScreen, 0, 0);
                    projectActions.projectId = Id;
                    projectActions.projectOwner = Owner;
                    projectActions.projectName = Name;
                    projectActions.projectLocalPath = LocalPath;
                    downloadProject.visible = LocalPath === '' && Status !== QFieldCloudProject.ProjectStatus.Downloading;
                    openProject.visible = LocalPath !== '';
                    removeProject.visible = LocalPath !== '';
                    cancelDownloadProject.visible = Status === QFieldCloudProject.ProjectStatus.Downloading;
                    projectActions.popup(gc.x + width - projectActions.width, gc.y - height);
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
                var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
                if (item) {
                  projectActions.projectId = item.projectId;
                  projectActions.projectOwner = item.projectOwner;
                  projectActions.projectName = item.projectName;
                  projectActions.projectLocalPath = item.projectLocalPath;
                  downloadProject.visible = item.projectLocalPath === '' && item.status !== QFieldCloudProject.ProjectStatus.Downloading;
                  openProject.visible = item.projectLocalPath !== '';
                  removeProject.visible = item.projectLocalPath !== '';
                  cancelDownloadProject.visible = item.status === QFieldCloudProject.ProjectStatus.Downloading;
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

      ColumnLayout {
        id: projectDetails
        spacing: 10

        property var cloudProject: undefined

        onCloudProjectChanged: {
          if (cloudProject != undefined) {
            cloudProject.downloadThumbnail();
          } else {
            projectsSwipeView.currentIndex = 0;
          }
        }

        ColumnLayout {
          id: projectDetailsLayout
          Layout.fillWidth: true
          Layout.fillHeight: true
          spacing: 10

          RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Rectangle {
              id: projectDetailsThumbnailRect
              width: 48
              height: 48
              border.color: Theme.mainBackgroundColor
              border.width: 1
              radius: width / 2
              clip: true

              Image {
                id: projectDetailsThumbnail
                anchors.fill: parent
                anchors.margins: 1
                fillMode: Image.PreserveAspectFit
                smooth: true
                source: projectDetails.cloudProject != undefined && projectDetails.cloudProject.thumbnailPath !== "" ? 'file://' + projectDetails.cloudProject.thumbnailPath : ""
                visible: source !== "" && status === Image.Ready
                width: 48
                height: 48
                sourceSize.width: width * screen.devicePixelRatio
                sourceSize.height: height * screen.devicePixelRatio
                layer.enabled: true
                layer.effect: QfOpacityMask {
                  maskSource: roundMask
                }
              }
            }

            Text {
              id: projectDetailsName
              Layout.fillWidth: true
              font.pointSize: Theme.titleFont.pointSize * 1.25
              font.bold: true
              color: Theme.mainTextColor
              wrapMode: Text.Wrap

              text: projectDetails.cloudProject != undefined ? projectDetails.cloudProject.name : ""
            }
          }

          ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            contentWidth: width
            contentHeight: projectDetailsBodyLayout.height
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical: QfScrollBar {
            }

            ColumnLayout {
              id: projectDetailsBodyLayout
              width: parent.width - 10
              spacing: 10

              ColumnLayout {
                Layout.fillWidth: true
                spacing: 5

                Text {
                  id: projectDetailsDescriptionLabel
                  Layout.fillWidth: true
                  font: Theme.strongFont
                  color: Theme.mainTextColor

                  text: qsTr("Description")
                }

                Text {
                  id: projectDetailsDescription
                  Layout.fillWidth: true
                  font: Theme.defaultFont
                  color: Theme.secondaryTextColor
                  wrapMode: Text.WordWrap
                  textFormat: Text.MarkdownText

                  text: projectDetails.cloudProject != undefined ? projectDetails.cloudProject.description.trim() : ""

                  onLinkActivated: link => {
                    Qt.openUrlExternally(link);
                  }
                }
              }

              ColumnLayout {
                Layout.fillWidth: true
                spacing: 5

                Text {
                  id: projectDetailsOwnerLabel
                  Layout.fillWidth: true
                  font: Theme.strongFont
                  color: Theme.mainTextColor

                  text: qsTr("Owner")
                }

                Text {
                  id: projectDetailsOwner
                  Layout.fillWidth: true
                  font: Theme.defaultFont
                  color: Theme.secondaryTextColor
                  wrapMode: Text.WordWrap

                  text: projectDetails.cloudProject != undefined ? projectDetails.cloudProject.owner : ""
                }
              }

              ColumnLayout {
                Layout.fillWidth: true
                spacing: 5

                Text {
                  id: projectDetailsCreationDateLabel
                  Layout.fillWidth: true
                  font: Theme.strongFont
                  color: Theme.mainTextColor

                  text: qsTr("Creation date")
                }

                Text {
                  id: projectDetailsCreationDate
                  Layout.fillWidth: true
                  font: Theme.defaultFont
                  color: Theme.secondaryTextColor
                  wrapMode: Text.WordWrap

                  text: projectDetails.cloudProject != undefined ? projectDetails.cloudProject.createdAt : ""
                }
              }

              ColumnLayout {
                Layout.fillWidth: true
                spacing: 5

                Text {
                  id: projectDetailsUpdateDateLabel
                  Layout.fillWidth: true
                  font: Theme.strongFont
                  color: Theme.mainTextColor

                  text: qsTr("Latest update date")
                }

                Text {
                  id: projectDetailsUpdateDate
                  Layout.fillWidth: true
                  font: Theme.defaultFont
                  color: Theme.secondaryTextColor
                  wrapMode: Text.WordWrap

                  text: projectDetails.cloudProject != undefined ? projectDetails.cloudProject.updatedAt : ""
                }
              }

              ColumnLayout {
                Layout.topMargin: 20
                Layout.bottomMargin: 20
                Layout.preferredWidth: projectDetailsCodeContainer.desiredWidth
                Layout.alignment: Qt.AlignHCenter
                spacing: 5

                Rectangle {
                  id: projectDetailsCodeContainer

                  property int desiredWidth: Math.min(mainWindow.width - 40, 250)
                  Layout.preferredWidth: desiredWidth
                  Layout.preferredHeight: desiredWidth

                  color: "transparent"
                  radius: 4
                  border.width: 1
                  border.color: Theme.mainTextColor

                  Image {
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit

                    sourceSize.width: projectDetailsCodeContainer.desiredWidth * Screen.devicePixelRatio
                    sourceSize.height: projectDetailsCodeContainer.desiredWidth * Screen.devicePixelRatio
                    source: projectDetails.cloudProject != undefined ? "image://barcode/?text=" + encodeURIComponent(UrlUtils.createActionUrl("qfield", "cloud", {
                          "project": projectDetails.cloudProject.id
                        })) + "&color=%2380cc28" : ""
                  }
                }

                Text {
                  id: projectDetailsCodeLabel
                  Layout.preferredWidth: projectDetailsCodeContainer.desiredWidth - 20
                  font: Theme.tinyFont
                  color: Theme.secondaryTextColor
                  wrapMode: Text.WordWrap
                  horizontalAlignment: Text.AlignHCenter

                  text: qsTr("This QR code can be scanned for users with the appropriate access to download and open this project")
                }
              }
            }
          }
        }

        QfButton {
          id: downloadProjectBtn
          Layout.fillWidth: true
          text: {
            if (projectDetails.cloudProject != undefined && projectDetails.cloudProject.status === QFieldCloudProject.ProjectStatus.Downloading) {
              if (projectDetails.cloudProject.packagingStatus === QFieldCloudProject.PackagingBusyStatus) {
                return qsTr("QFieldCloud is packaging project, hold tight");
              } else {
                if (projectDetails.cloudProject.downloadProgress > 0) {
                  return qsTr("Downloading project") + " (%1%)".arg(Math.round(projectDetails.cloudProject.downloadProgress * 100));
                } else {
                  return qsTr("Downloading project");
                }
              }
            }
            return qsTr("Download project");
          }
          visible: projectDetails.cloudProject != undefined && projectDetails.cloudProject.localPath === ""
          enabled: projectDetails.cloudProject != undefined && projectDetails.cloudProject.status !== QFieldCloudProject.ProjectStatus.Downloading

          onClicked: {
            if (projectDetails.cloudProject != undefined) {
              displayToast(qsTr("Downloading project %1").arg(projectDetails.cloudProject.name));
              cloudProjectsModel.projectPackageAndDownload(projectDetails.cloudProject.id);
            }
          }
        }

        QfButton {
          id: openProjectBtn
          Layout.fillWidth: true
          text: qsTr("Open project")
          visible: projectDetails.cloudProject != undefined && projectDetails.cloudProject.localPath !== ""

          onClicked: {
            if (projectDetails.cloudProject != undefined) {
              qfieldCloudScreen.visible = false;
              iface.loadFile(projectDetails.cloudProject.localPath);
            }
            projectsSwipeView.currentIndex = 0;
            projectDetails.cloudProject = undefined;
          }
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

    title: qsTr('Project Actions')

    topMargin: mainWindow.sceneTopMargin
    bottomMargin: mainWindow.sceneBottomMargin

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

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: downloadProject

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      text: qsTr("Download Project")
      onTriggered: {
        cloudProjectsModel.projectPackageAndDownload(projectActions.projectId);
      }
    }

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

    MenuItem {
      id: removeProject

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      text: qsTr("Remove Stored Project")
      onTriggered: {
        cloudProjectsModel.removeLocalProject(projectActions.projectId);
        iface.removeRecentProject(projectActions.projectLocalPath);
        welcomeScreen.model.reloadModel();
        if (projectActions.projectLocalPath === qgisProject.fileName) {
          iface.clearProject();
        }
      }
    }

    MenuItem {
      id: cancelDownloadProject
      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding
      text: qsTr("Cancel Project Download")
      onTriggered: {
        cloudProjectsModel.projectCancelDownload(projectActions.projectId);
      }
    }
  }

  Connections {
    id: codeReaderConnection
    target: codeReader
    enabled: false

    function onDecoded(string) {
      const results = UrlUtils.getActionDetails(string);
      if (results.type !== undefined && results.type === "cloud" && results.project !== undefined && results.project !== "") {
        codeReader.close();
        let cloudProject = cloudProjectsModel.findProject(requestedProjectDetails);
        if (cloudProject) {
          projectDetails.cloudProject = cloudProjectsModel.findProject(projectId);
          projectsSwipeView.currentIndex = 1;
        } else {
          cloudProjectsModel.appendProject(results.project);
        }
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

  function prepareCloudScreen() {
    if (visible) {
      if (cloudConnection.status == QFieldCloudConnection.Disconnected) {
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
      } else {
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
