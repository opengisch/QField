

# File QfCloudProjectDetails.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfCloudProjectDetails.qml**](QfCloudProjectDetails_8qml.md)

[Go to the documentation of this file](QfCloudProjectDetails_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield.core
import org.qfield.gui

ColumnLayout {
  id: projectDetails
  spacing: 5

  signal synchronize
  signal pushChanges

  property var cloudProject: undefined
  property string lastSubscriptionUser: ""

  readonly property bool isTemplate: cloudProject != undefined && cloudProject.type === QfCloudProject.ProjectType.Template

  onCloudProjectChanged: {
    if (cloudProject != undefined) {
      cloudProject.downloadThumbnail();
      updateStorageMeter();
    } else {
      detailsStorageMeter.visible = false;
      detailsStorageMeter.value = 0;
      projectsSwipeView.currentIndex = 0;
    }
  }

  Connections {
    target: cloudProject !== undefined ? cloudProject : null

    function onLocalPathChanged() {
      updateStorageMeter();
    }
  }

  Connections {
    target: cloudConnection

    function onSubscriptionInformationReceived(subscriptionInformation) {
      detailsStorageMeter.loading = false;
      if (projectDetails.cloudProject !== undefined && subscriptionInformation.storageTotal > 0) {
        lastSubscriptionUser = projectDetails.cloudProject.owner;
        detailsStorageMeter.value = subscriptionInformation.storageUsed / subscriptionInformation.storageTotal;
        detailsStorageMeter.usageText = qsTr("Using %1 of %2").arg(QfFileUtils.representFileSize(subscriptionInformation.storageUsed, true)).arg(QfFileUtils.representFileSize(subscriptionInformation.storageTotal, true));
        detailsStorageMeter.relatedUrl = Qt.platform !== "ios" ? QfCloudUtils.subscriptionManagementUrl(cloudConnection.url, subscriptionInformation.plan, projectDetails.cloudProject.owner, cloudConnection.username) : "";
        detailsStorageMeter.warningThreshold = subscriptionInformation.storageThresholdWarning > 0 ? 1.0 - (subscriptionInformation.storageThresholdWarning / subscriptionInformation.storageTotal) : 0.8;
        detailsStorageMeter.criticalThreshold = subscriptionInformation.storageThresholdCritical > 0 ? 1.0 - (subscriptionInformation.storageThresholdCritical / subscriptionInformation.storageTotal) : 0.95;
        detailsStorageMeter.visible = true;
      }
    }
  }

  function updateStorageMeter() {
    if (cloudConnection.status === QfCloudConnection.LoggedIn && cloudProject.localPath !== "") {
      if (cloudProject.owner === lastSubscriptionUser || cloudProject.owner === cloudConnection.username) {
        detailsStorageMeter.visible = true;
        detailsStorageMeter.loading = true;
      } else {
        detailsStorageMeter.visible = false;
        detailsStorageMeter.value = 0;
      }
      cloudConnection.getSubscriptionInformation(cloudProject.owner);
    } else {
      detailsStorageMeter.visible = false;
      detailsStorageMeter.value = 0;
    }
  }

  ColumnLayout {
    id: projectDetailsLayout
    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 10

    RowLayout {
      Layout.fillWidth: true
      spacing: 2

      Text {
        id: projectDetailsName
        Layout.fillWidth: true
        font.pointSize: QfTheme.titleFont.pointSize * 1.25
        font.bold: true
        color: QfTheme.mainTextColor
        wrapMode: Text.Wrap

        text: cloudProject != undefined ? cloudProject.name : ""
      }

      Rectangle {
        id: projectDetailsThumbnailRect
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.margins: 10
        width: 48
        height: 48
        radius: width / 2
        color: QfTheme.controlBackgroundAlternateColor
        layer.enabled: true

        Rectangle {
          id: projectDetailsThumbnailMask
          anchors.centerIn: parent
          width: projectDetailsThumbnail.width
          height: projectDetailsThumbnail.height
          radius: width / 2
          color: "white"
          visible: false
          layer.enabled: true
        }

        Image {
          id: projectDetailsThumbnail
          anchors.centerIn: parent
          fillMode: Image.PreserveAspectFit
          smooth: true
          source: cloudProject != undefined && cloudProject.thumbnailPath !== "" ? 'file://' + cloudProject.thumbnailPath : ""
          visible: source !== "" && status === Image.Ready
          width: 48
          height: 48
          sourceSize.width: width * screen.devicePixelRatio
          sourceSize.height: height * screen.devicePixelRatio
          layer.enabled: true
          layer.effect: QfOpacityMask {
            maskSource: projectDetailsThumbnailMask
          }
        }
      }
    }

    ScrollView {
      Layout.fillWidth: true
      Layout.fillHeight: true
      contentWidth: width
      contentHeight: projectDetailsBodyLayout.height
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical: QfScrollBar {}

      ColumnLayout {
        id: projectDetailsBodyLayout
        width: parent.width - 10
        spacing: 10

        Text {
          id: projectDetailsDescription
          Layout.fillWidth: true
          font: QfTheme.defaultFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          textFormat: Text.MarkdownText
          visible: text !== ""

          text: cloudProject != undefined ? cloudProject.description.trim() : ""

          onLinkActivated: link => {
            Qt.openUrlExternally(link);
          }
        }

        ColumnLayout {
          Layout.fillWidth: true
          spacing: 5

          Text {
            id: projectDetailsStorageSizeLabel
            Layout.fillWidth: true
            font: QfTheme.strongFont
            color: QfTheme.mainTextColor

            text: qsTr("Storage size")
          }

          Text {
            id: projectDetailsStorageSize
            Layout.fillWidth: true
            font: QfTheme.defaultFont
            color: QfTheme.secondaryTextColor
            wrapMode: Text.WordWrap

            text: cloudProject != undefined ? QfFileUtils.representFileSize(cloudProject.remoteSizeBytes) : ""
          }
        }

        ColumnLayout {
          Layout.fillWidth: true
          spacing: 5

          Text {
            id: projectDetailsOwnerLabel
            Layout.fillWidth: true
            font: QfTheme.strongFont
            color: QfTheme.mainTextColor

            text: qsTr("Owner")
          }

          Text {
            id: projectDetailsOwner
            Layout.fillWidth: true
            font: QfTheme.defaultFont
            color: QfTheme.secondaryTextColor
            wrapMode: Text.WordWrap

            text: cloudProject != undefined ? cloudProject.owner : ""
          }
        }

        QfMeterBar {
          id: detailsStorageMeter
          Layout.fillWidth: true
          visible: false
          showTitleLabel: false
        }

        ColumnLayout {
          Layout.fillWidth: true
          spacing: 5

          Text {
            id: projectDetailsCreationDateLabel
            Layout.fillWidth: true
            font: QfTheme.strongFont
            color: QfTheme.mainTextColor

            text: qsTr("Creation date")
          }

          Text {
            id: projectDetailsCreationDate
            Layout.fillWidth: true
            font: QfTheme.defaultFont
            color: QfTheme.secondaryTextColor
            wrapMode: Text.WordWrap

            text: cloudProject != undefined ? Qt.formatDateTime(new Date(cloudProject.createdAt), "dddd, MMMM dd, yyyy - hh:mm") : ""
          }
        }

        ColumnLayout {
          Layout.fillWidth: true
          spacing: 5

          Text {
            id: projectDetailsUpdateDateLabel
            Layout.fillWidth: true
            visible: cloudProject != undefined && !isNaN(cloudProject.dataLastUpdatedAt.getTime())
            font: QfTheme.strongFont
            color: QfTheme.mainTextColor

            text: qsTr("Latest update date")
          }

          Text {
            id: projectDetailsUpdateDate
            Layout.fillWidth: true
            font: QfTheme.defaultFont
            color: QfTheme.secondaryTextColor
            wrapMode: Text.WordWrap

            text: cloudProject != undefined ? Qt.formatDateTime(new Date(cloudProject.dataLastUpdatedAt), "dddd, MMMM dd, yyyy - hh:mm") : ""
          }
        }

        ColumnLayout {
          Layout.topMargin: 20
          Layout.bottomMargin: 20
          Layout.preferredWidth: projectDetailsCode.desiredWidth
          Layout.alignment: Qt.AlignHCenter
          spacing: 5

          Image {
            id: projectDetailsCode
            Layout.preferredWidth: desiredWidth
            Layout.preferredHeight: desiredWidth
            fillMode: Image.PreserveAspectFit

            sourceSize.width: desiredWidth * Screen.devicePixelRatio
            sourceSize.height: desiredWidth * Screen.devicePixelRatio
            source: cloudProject != undefined ? "image://barcode/?text=" + encodeURIComponent(QfUrlUtils.createActionUrl("qfield", "cloud", {
              "project": cloudProject.owner + '/' + cloudProject.name
            })) + "&color=" + encodeURIComponent(QfTheme.mainTextColor) : ""
            property int desiredWidth: Math.min(mainWindow.width - 40, 250)
          }

          Text {
            id: projectDetailsCodeLabel
            Layout.preferredWidth: projectDetailsCode.desiredWidth - 20
            font: QfTheme.tinyFont
            color: QfTheme.secondaryTextColor
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter

            text: qsTr("This QR code can be scanned for users with the appropriate access to download and open this project")
          }
        }
      }
    }
  }

  RowLayout {
    Layout.fillWidth: true
    visible: !projectDetails.isTemplate && cloudProject != undefined && cloudProject.localPath !== "" && (((Date.now() - cloudProject.lastLocalExportedAt) / 1000 > 60) || (cloudProject.deltaFileWrapper === undefined || cloudProject.deltasCount > 0))

    QfButton {
      id: syncButton
      Layout.fillWidth: true
      Layout.preferredWidth: 1
      enabled: cloudProject != undefined && cloudProject.deltaFileWrapper !== null && cloudProject.status === QfCloudProject.Idle && !cloudProject.deltaFileWrapper.hasError
      progressValue: cloudProject != undefined && cloudProject.localPath !== "" ? cloudProject.downloadProgress : 0
      showProgress: cloudProject != undefined && cloudProject.localPath !== "" && cloudProject.status === QfCloudProject.ProjectStatus.Downloading
      text: {
        if (showProgress) {
          if (cloudProject.downloadProgress > 0) {
            return qsTr("Synchronizing") + " (%1%)".arg(Math.round(cloudProject.downloadProgress * 100));
          } else {
            return qsTr("Synchronizing");
          }
        }
        return qsTr('Synchronize');
      }

      onClicked: {
        synchronize();
      }
    }

    QfButton {
      id: pushButton
      Layout.fillWidth: true
      Layout.preferredWidth: 1
      enabled: cloudProject != undefined && cloudProject.deltaFileWrapper !== undefined && cloudProject.deltasCount > 0 && cloudProject.status === QfCloudProject.Idle && !cloudProject.deltaFileWrapper.hasError
      visible: cloudProject != undefined && cloudProject.userRole !== "reader"
      text: qsTr('Upload local changes')

      onClicked: {
        pushChanges();
      }
    }
  }

  QfButton {
    id: downloadProjectInDetailsLayout
    Layout.fillWidth: true
    dropdown: !projectDetails.isTemplate && !showProgress
    progressValue: !projectDetails.isTemplate && cloudProject ? cloudProject.downloadProgress : 0
    showProgress: (projectDetails.isTemplate && cloudProjectsModel.isCreating) || (cloudProject != undefined && cloudProject.status === QfCloudProject.ProjectStatus.Downloading)
    text: {
      if (projectDetails.isTemplate) {
        if (cloudProjectsModel.isCreating) {
          return qsTr("Creating project");
        }
        return qsTr("Create project from template");
      }
      if (showProgress) {
        if (cloudProject.packagingStatus === QfCloudProject.PackagingBusyStatus) {
          return qsTr("QFieldCloud is packaging project, hold tight");
        } else {
          if (cloudProject.downloadProgress > 0) {
            return qsTr("Downloading project") + " (%1%)".arg(Math.round(cloudProject.downloadProgress * 100));
          } else {
            return qsTr("Downloading project");
          }
        }
      }
      return qsTr("Download project");
    }
    visible: cloudProject != undefined && cloudProject.localPath === ""
    enabled: (projectDetails.isTemplate && !cloudProjectsModel.isCreating) || (cloudProject != undefined && cloudProject.status !== QfCloudProject.ProjectStatus.Downloading)

    onClicked: {
      if (cloudProject == undefined) {
        return;
      }
      if (projectDetails.isTemplate) {
        cloneProjectDialog.sourceProjectId = cloudProject.id;
        cloneProjectName.text = cloudProject.name;
        cloneProjectDialog.open();
      } else {
        displayToast(qsTr("Downloading project %1").arg(cloudProject.name));
        cloudProjectsModel.projectPackageAndDownload(cloudProject.id);
      }
    }

    onDropdownClicked: {
      projectDetailsMenu.popup(downloadProjectInDetailsLayout.width - projectDetailsMenu.width, downloadProjectInDetailsLayout.y);
    }
  }

  QfButton {
    id: openProjectBtn
    Layout.fillWidth: true
    dropdown: true
    text: qsTr("Open project")
    visible: cloudProject != undefined && cloudProject.localPath !== ""
    enabled: cloudProject != undefined && cloudProject.localPath !== "" && cloudProject.status === QfCloudProject.Idle

    onClicked: {
      if (cloudProject != undefined) {
        qfieldCloudScreen.visible = false;
        iface.loadFile(cloudProject.localPath, cloudProject.name);
      }
      projectsSwipeView.currentIndex = 0;
      cloudProject = undefined;
    }

    onDropdownClicked: {
      projectDetailsMenu.popup(openProjectBtn.width - projectDetailsMenu.width, openProjectBtn.y);
    }
  }

  QfMenu {
    id: projectDetailsMenu

    MenuItem {
      font: QfTheme.defaultFont
      width: parent.width
      height: 48
      leftPadding: QfTheme.menuItemLeftPadding
      text: qsTr("Clone project")
      onTriggered: {
        cloneProjectDialog.sourceProjectId = cloudProject.id;
        cloneProjectName.text = cloudProject.name;
        cloneProjectDialog.open();
      }
    }
  }
}
```


