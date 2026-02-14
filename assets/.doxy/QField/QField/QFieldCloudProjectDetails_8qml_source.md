

# File QFieldCloudProjectDetails.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**QFieldCloudProjectDetails.qml**](QFieldCloudProjectDetails_8qml.md)

[Go to the documentation of this file](QFieldCloudProjectDetails_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

ColumnLayout {
  id: projectDetails
  spacing: 5

  signal synchronize
  signal pushChanges

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
      spacing: 2

      Text {
        id: projectDetailsName
        Layout.fillWidth: true
        font.pointSize: Theme.titleFont.pointSize * 1.25
        font.bold: true
        color: Theme.mainTextColor
        wrapMode: Text.Wrap

        text: cloudProject != undefined ? cloudProject.name : ""
      }

      Rectangle {
        id: projectDetailsThumbnailRect
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
        Layout.margins: 10
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
          source: cloudProject != undefined && cloudProject.thumbnailPath !== "" ? 'file://' + cloudProject.thumbnailPath : ""
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
          font: Theme.defaultFont
          color: Theme.mainTextColor
          wrapMode: Text.WordWrap
          textFormat: Text.MarkdownText

          text: cloudProject != undefined ? cloudProject.description.trim() : ""

          onLinkActivated: link => {
            Qt.openUrlExternally(link);
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

            text: cloudProject != undefined ? cloudProject.owner : ""
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

            text: cloudProject != undefined ? Qt.formatDateTime(new Date(cloudProject.createdAt), "dddd, MMMM dd, yyyy - hh:mm") : ""
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

            text: cloudProject != undefined ? Qt.formatDateTime(new Date(cloudProject.updatedAt), "dddd, MMMM dd, yyyy - hh:mm") : ""
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
            source: cloudProject != undefined ? "image://barcode/?text=" + encodeURIComponent(UrlUtils.createActionUrl("qfield", "cloud", {
              "project": cloudProject.id
            })) + "&color=%2380cc28" : ""
            property int desiredWidth: Math.min(mainWindow.width - 40, 250)
          }

          Text {
            id: projectDetailsCodeLabel
            Layout.preferredWidth: projectDetailsCode.desiredWidth - 20
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

  RowLayout {
    visible: openProjectBtn.visible
    Layout.fillWidth: true

    QfButton {
      id: syncButton
      Layout.fillWidth: true
      Layout.preferredWidth: 1
      progressValue: cloudProject != undefined && cloudProject.localPath !== "" ? cloudProject.downloadProgress : 0
      showProgress: cloudProject != undefined && cloudProject.localPath !== "" && cloudProject.status === QFieldCloudProject.ProjectStatus.Downloading
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
      visible: true
      enabled: cloudProject != undefined && cloudProject.deltaFileWrapper !== undefined && cloudProject.status === QFieldCloudProject.Idle && !cloudProject.deltaFileWrapper.hasError

      onClicked: {
        synchronize();
      }
    }

    QfButton {
      id: pushButton
      Layout.fillWidth: true
      Layout.preferredWidth: 1
      enabled: cloudProject != undefined && cloudProject.deltaFileWrapper !== undefined && cloudProject.deltasCount > 0 && cloudProject.status === QFieldCloudProject.Idle && !cloudProject.deltaFileWrapper.hasError
      text: qsTr('Push changes')
      visible: true

      onClicked: {
        pushChanges();
      }
    }
  }

  QfButton {
    id: downloadProjectInDetailsLayout
    Layout.fillWidth: true
    progressValue: cloudProject ? cloudProject.downloadProgress : 0
    showProgress: cloudProject != undefined && cloudProject.status === QFieldCloudProject.ProjectStatus.Downloading
    text: {
      if (showProgress) {
        if (cloudProject.packagingStatus === QFieldCloudProject.PackagingBusyStatus) {
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
    enabled: cloudProject != undefined && cloudProject.status !== QFieldCloudProject.ProjectStatus.Downloading

    onClicked: {
      if (cloudProject != undefined) {
        displayToast(qsTr("Downloading project %1").arg(cloudProject.name));
        cloudProjectsModel.projectPackageAndDownload(cloudProject.id);
      }
    }
  }

  QfButton {
    id: openProjectBtn
    Layout.fillWidth: true
    text: qsTr("Open project")
    visible: cloudProject != undefined && cloudProject.localPath !== ""
    enabled: cloudProject != undefined && cloudProject.localPath !== "" && cloudProject.status === QFieldCloudProject.Idle

    onClicked: {
      if (cloudProject != undefined) {
        qfieldCloudScreen.visible = false;
        iface.loadFile(cloudProject.localPath);
      }
      projectsSwipeView.currentIndex = 0;
      cloudProject = undefined;
    }
  }
}
```


