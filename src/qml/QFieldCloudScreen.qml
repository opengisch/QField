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

  header: QfPageHeader {
    title: qsTr("QFieldCloud Projects")

    showBackButton: true
    showApplyButton: false
    showCancelButton: false

    busyIndicatorState: cloudConnection.status === QFieldCloudConnection.Connecting || cloudConnection.state === QFieldCloudConnection.Busy ? 'on' : 'off' || cloudProjectsModel.busyProjectIds.length > 0
    busyIndicatorValue: busyIndicatorState === 'on' ? 0 : 1

    topMargin: mainWindow.sceneTopMargin

    onFinished: parent.finished()
  }

  ColumnLayout {
    anchors.fill: parent
    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 2

    RowLayout {
      id: connectionInformation
      spacing: 2
      Layout.fillWidth: true
      visible: cloudConnection.hasToken || cloudProjectsModel.rowCount() > 0

      Label {
        Layout.fillWidth: true
        padding: 10
        opacity: projects.visible ? 1 : 0
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
          fillMode: Image.PreserveAspectFit
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
        }

        MouseArea {
          anchors.fill: parent

          onClicked: {
            if (!connectionSettings.visible) {
              connectionSettings.visible = true;
              projects.visible = false;
            } else {
              connectionSettings.visible = false;
              projects.visible = true;
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
      Layout.margins: 10
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

    ColumnLayout {
      id: projects
      Layout.fillWidth: true
      Layout.fillHeight: true
      Layout.margins: 10
      Layout.topMargin: 0
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
          property bool refreshing: false

          model: QFieldCloudProjectsFilterModel {
            projectsModel: cloudProjectsModel
            filter: filterBar.currentIndex === 0 ? QFieldCloudProjectsFilterModel.PrivateProjects : QFieldCloudProjectsFilterModel.PublicProjects
            showLocalOnly: cloudConnection.status !== QFieldCloudConnection.LoggedIn
            textFilter: searchBar.searchTerm
            onFilterChanged: {
              if (cloudConnection.state === QFieldCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0) {
                refreshProjectsList(filter === QFieldCloudProjectsFilterModel.PublicProjects);
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
              refreshProjectsList(filterBar.currentIndex !== 0);
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
            height: line.height + 6
            color: "transparent"

            ProgressBar {
              anchors.bottom: line.bottom
              anchors.bottomMargin: -4
              anchors.left: line.left
              anchors.leftMargin: line.leftPadding
              width: line.width - 20
              height: 6
              indeterminate: PackagingStatus !== QFieldCloudProjectsModel.PackagingFinishedStatus && DownloadProgress === 0.0
              value: DownloadProgress
              visible: Status === QFieldCloudProjectsModel.ProjectStatus.Downloading
              z: 1
            }

            Row {
              id: line
              Layout.fillWidth: true
              leftPadding: 6
              rightPadding: 10
              topPadding: 4
              bottomPadding: 8
              spacing: 0

              Image {
                id: type
                anchors.verticalCenter: inner.verticalCenter
                source: {
                  if (cloudConnection.status !== QFieldCloudConnection.LoggedIn) {
                    return Theme.getThemeVectorIcon('ic_cloud_project_offline_48dp');
                  } else {
                    var status = '';
                    switch (Status) {
                    case QFieldCloudProjectsModel.ProjectStatus.Downloading:
                      return Theme.getThemeVectorIcon('ic_cloud_project_download_48dp');
                    case QFieldCloudProjectsModel.ProjectStatus.Uploading:
                      return Theme.getThemeVectorIcon('ic_cloud_project_upload_48dp');
                    default:
                      break;
                    }
                    switch (Checkout) {
                    case QFieldCloudProjectsModel.LocalCheckout:
                      return Theme.getThemeVectorIcon('ic_cloud_project_localonly_48dp');
                    case QFieldCloudProjectsModel.RemoteCheckout:
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
                opacity: Status === QFieldCloudProjectsModel.ProjectStatus.Downloading ? 0.3 : 1
              }
              ColumnLayout {
                id: inner
                width: rectangle.width - type.width - 10
                Text {
                  id: projectTitle
                  topPadding: 5
                  leftPadding: 3
                  text: Name
                  font.pointSize: Theme.tipFont.pointSize
                  font.underline: true
                  color: Theme.mainColor
                  opacity: rectangle.isPressed ? 0.8 : 1
                  wrapMode: Text.WordWrap
                  Layout.fillWidth: true
                }
                Text {
                  id: projectNote
                  leftPadding: 3
                  text: {
                    if (cloudConnection.status !== QFieldCloudConnection.LoggedIn) {
                      return qsTr('(Available locally)');
                    } else {
                      var status = '';

                      // TODO I think these should be shown as UI badges
                      switch (Status) {
                      case QFieldCloudProjectsModel.ProjectStatus.Idle:
                        break;
                      case QFieldCloudProjectsModel.ProjectStatus.Downloading:
                        if (PackagingStatus === QFieldCloudProjectsModel.PackagingBusyStatus) {
                          status = qsTr('QFieldCloud is packaging the latest data just for you; this might take some time, please hold tight');
                        } else {
                          if (PackagingStatus === QFieldCloudProjectsModel.PackagingFinishedStatus || DownloadProgress > 0.0) {
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
                      case QFieldCloudProjectsModel.ProjectStatus.Uploading:
                        status = qsTr('Uploading…');
                        break;
                      default:
                        break;
                      }
                      switch (ErrorStatus) {
                      case QFieldCloudProjectsModel.NoErrorStatus:
                        break;
                      case QFieldCloudProjectsModel.DownloadErrorStatus:
                        status = qsTr('Downloading error. ') + ErrorString;
                        break;
                      case QFieldCloudProjectsModel.UploadErrorStatus:
                        status = qsTr('Uploading error. ') + ErrorString;
                        break;
                      }
                      if (!status) {
                        switch (Checkout) {
                        case QFieldCloudProjectsModel.LocalCheckout:
                          status = qsTr('Available locally, missing on the cloud');
                          break;
                        case QFieldCloudProjectsModel.RemoteCheckout:
                          status = qsTr('Available on the cloud, missing locally');
                          break;
                        case QFieldCloudProjectsModel.LocalAndRemoteCheckout:
                          status = qsTr('Available locally');
                          if (ProjectOutdated) {
                            status += qsTr(', updated data available on the cloud');
                          }
                          break;
                        default:
                          break;
                        }
                      }
                      var localChanges = (LocalDeltasCount > 0) ? qsTr('Has changes. ') : '';
                      var str = '%1 (%2%3)'.arg(Description).arg(localChanges).arg(status);
                      return str.trim();
                    }
                  }
                  visible: text != ""
                  font.pointSize: Theme.tipFont.pointSize - 2
                  font.italic: true
                  color: Theme.secondaryTextColor
                  wrapMode: Text.WordWrap
                  Layout.fillWidth: true
                }
              }
            }
          }

          Label {
            anchors.fill: parent
            anchors.margins: 20
            visible: parent.count == 0 && filterBar.currentIndex === 0
            text: table.refreshing ? qsTr("Refreshing projects list") : qsTr("No cloud projects found. To get started, %1read the documentation%2.").arg("<a href=\"https://docs.qfield.org/get-started/tutorials/get-started-qfc/\">").arg("</a>")
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
            onClicked: mouse => {
              var item = table.itemAt(table.contentX + mouse.x, table.contentY + mouse.y);
              if (item) {
                if (item.projectLocalPath != '') {
                  qfieldCloudScreen.visible = false;
                  iface.loadFile(item.projectLocalPath);
                } else {
                  // fetch remote project
                  displayToast(qsTr("Downloading project %1").arg(item.projectName));
                  cloudProjectsModel.projectPackageAndDownload(item.projectId);
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
                downloadProject.visible = item.projectLocalPath === '' && item.status !== QFieldCloudProjectsModel.ProjectStatus.Downloading;
                openProject.visible = item.projectLocalPath !== '';
                removeProject.visible = item.projectLocalPath !== '';
                cancelDownloadProject.visible = item.status === QFieldCloudProjectsModel.ProjectStatus.Downloading;
                projectActions.popup(mouse.x, mouse.y);
              }
            }
          }
        }
      }

      Menu {
        id: projectActions

        property string projectId: ''
        property string projectOwner: ''
        property string projectName: ''
        property string projectLocalPath: ''

        title: qsTr('Project Actions')

        width: {
          let result = 50;
          let padding = 0;
          for (let i = 0; i < count; ++i) {
            let item = itemAt(i);
            result = Math.max(item.contentItem.implicitWidth, result);
            padding = Math.max(item.leftPadding + item.rightPadding, padding);
          }
          return mainWindow.width > 0 ? Math.min(result + padding, mainWindow.width - 20) : result + padding;
        }

        topMargin: mainWindow.sceneTopMargin
        bottomMargin: mainWindow.sceneBottomMargin

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

      Text {
        id: projectsTips
        Layout.alignment: Qt.AlignLeft
        Layout.fillWidth: true
        Layout.topMargin: 5
        Layout.bottomMargin: 5
        text: qsTr("Press and hold over a cloud project for a menu of additional actions.")
        font: Theme.tipFont
        color: Theme.secondaryTextColor
        wrapMode: Text.WordWrap
      }

      QfButton {
        id: refreshProjectsListBtn
        Layout.fillWidth: true
        Layout.bottomMargin: mainWindow.sceneBottomMargin
        text: qsTr("Refresh projects list")
        enabled: cloudConnection.status === QFieldCloudConnection.LoggedIn && cloudConnection.state === QFieldCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0
        onClicked: {
          refreshProjectsList(filterBar.currentIndex !== 0);
        }
      }
    }
  }

  Connections {
    target: cloudConnection

    function onStatusChanged() {
      if (table.refreshing) {
        table.refreshing = false;
      }
      if (cloudConnection.status === QFieldCloudConnection.LoggedIn)
        prepareCloudLogin();
    }
  }

  function refreshProjectsList(shouldRefreshPublic) {
    if (cloudConnection.state !== QFieldCloudConnection.Idle && cloudProjectsModel.busyProjectIds.length === 0) {
      return;
    }
    table.refreshing = true;
    cloudProjectsModel.refreshProjectsList(shouldRefreshPublic);
    displayToast(qsTr("Refreshing projects list"));
  }

  function prepareCloudLogin() {
    if (visible) {
      if (cloudConnection.status == QFieldCloudConnection.Disconnected) {
        if (cloudConnection.hasToken) {
          cloudConnection.login();
          projects.visible = true;
          connectionSettings.visible = false;
        } else {
          projects.visible = false;
          connectionSettings.visible = true;
        }
      } else {
        projects.visible = true;
        connectionSettings.visible = false;
      }
    }
  }

  Component.onCompleted: {
    prepareCloudLogin();
  }

  onVisibleChanged: {
    prepareCloudLogin();
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true;
      finished();
    }
  }
}
