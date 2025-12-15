import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import org.qfield
import Theme

Page {
  id: projectCreation

  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

  signal triggerConnection
  signal triggerCloudify(string title, string path)
  signal triggerProjectLoad(string title, string path)

  header: QfPageHeader {
    title: qsTr("Create a new project")

    showBackButton: true
    showApplyButton: false
    showCancelButton: false
    showMenuButton: false

    topMargin: mainWindow.sceneTopMargin

    onBack: {
      projectCreation.visible = false;
    }
  }

  Flickable {
    id: contentFlickable
    anchors.fill: parent
    anchors.margins: 10
    contentHeight: newProjectConfigColumn.height + bottomRow.height + mainWindow.sceneBottomMargin + 50
    clip: true

    Column {
      id: newProjectConfigColumn
      width: parent.width
      spacing: 10

      Label {
        text: qsTr("This page will guide you through the creation of a simple project through simple configuration toggles. For more advanced needs, we advise you to create projects in QGIS using QFieldSync.")
        font: Theme.defaultFont
        color: Theme.mainTextColor
        wrapMode: Text.WordWrap
        width: parent.width
      }

      TextField {
        id: projectName
        width: parent.width
        height: 50
        font: Theme.defaultFont
        placeholderText: qsTr("New project name")
        text: ""
      }

      QfExpandableGroupBox {
        id: baseMapExpandablePanel
        title: qsTr("Select your basemap")
        width: parent.width
        checked: true
        interactive: false
        icon: Theme.getThemeVectorIcon("ic_map_white_24dp")
        iconColor: Theme.mainTextColor

        content: Column {
          id: basemapColumn
          anchors.left: parent.left
          anchors.right: parent.right
          spacing: 10

          Label {
            text: qsTr("Choose a basemap for your project. Pick from the available options or provide your own custom URL.")
            font: Theme.defaultFont
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            width: parent.width
          }

          ListView {
            id: baseMapList
            width: parent.width
            height: 105
            orientation: ListView.Horizontal
            spacing: 10
            model: [{
                "icon": "qrc:/pictures/pictures/colorful.jpg",
                "name": "colorful",
                "displayName": qsTr("Colorful")
              }, {
                "icon": "qrc:/pictures/pictures/dark.jpg",
                "name": "darkgray",
                "displayName": qsTr("Darkgray")
              }, {
                "icon": "qrc:/pictures/pictures/lightgray.jpg",
                "name": "lightgray",
                "displayName": qsTr("Lightgray")
              }, {
                "icon": "",
                "name": "blank",
                "displayName": qsTr("Blank")
              }, {
                "icon": "",
                "name": "custom",
                "displayName": qsTr("Custom")
              }]

            clip: true

            delegate: QfProjectThumbnail {
              width: 150
              implicitHeight: 95
              radius: 4
              bgColor: modelData.name === "Blank" ? "white" : Theme.groupBoxSurfaceColor
              previewImageSource: modelData.icon
              projectTitle.text: modelData.displayName
              showType: false
              selected: baseMapList.currentIndex == index
              fillHeight: true
              showCustomizeIcon: modelData.name === "custom"

              MouseArea {
                anchors.fill: parent
                onClicked: {
                  Qt.inputMethod.hide();
                  baseMapList.currentIndex = index;
                }

                onPressed: {
                  parent.isPressed = true;
                }
                onReleased: {
                  parent.isPressed = false;
                }
                onCanceled: {
                  parent.isPressed = false;
                }
              }
            }
          }

          Column {
            spacing: 4
            width: parent.width
            visible: baseMapList.currentIndex === 4

            Label {
              text: qsTr("Custom basemap URL")
              font: Theme.defaultFont
              color: Theme.mainTextColor
              wrapMode: Text.WordWrap
              width: parent.width
            }

            TextField {
              id: baseMapURL
              font: Theme.defaultFont
              placeholderText: text === "" && !focus ? "e.g., https://your-map-service.com/{z}/{x}/{y}.png" : ""
              placeholderTextColor: Theme.secondaryTextColor
              width: parent.width
            }
          }
        }
      }

      QfExpandableGroupBox {
        id: takeNotesGroupBox
        title: qsTr("Take notes?")
        width: parent.width
        checked: true
        icon: Theme.getThemeVectorIcon("ic_marker_white_24dp")
        iconColor: Theme.mainTextColor

        content: Column {
          id: takeNotesColumn
          anchors.left: parent.left
          anchors.right: parent.right

          Label {
            text: qsTr("Quickly capture notes with date, time, and comments. Optionally, attach multimedia items such as images and videos to enrich your notes.")
            font: Theme.defaultFont
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            width: parent.width
          }

          CheckBox {
            id: takeMediaCheckBox
            text: qsTr("Take image and video attachments")
            font: Theme.defaultFont
            indicator.height: 16
            indicator.width: 16
            indicator.implicitHeight: 24
            indicator.implicitWidth: 24
            leftPadding: 1
            checked: true
          }
        }

        onClicked: {
          Qt.inputMethod.hide();
        }
      }

      QfExpandableGroupBox {
        id: trackPositionGroupBox
        title: qsTr("Track your position?")
        width: parent.width
        checked: false
        icon: Theme.getThemeVectorIcon("directions_walk_24dp")
        iconColor: Theme.mainTextColor

        content: Column {
          id: trackPositionColumn
          anchors.left: parent.left
          anchors.right: parent.right

          Label {
            text: qsTr("Record your location every second, along with date and time, to keep a precise track of your movements.")
            font: Theme.defaultFont
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            width: parent.width
          }

          CheckBox {
            id: autoTrackPositionCheckBox
            text: qsTr("Begin tracking automatically")
            font: Theme.defaultFont
            indicator.height: 16
            indicator.width: 16
            indicator.implicitHeight: 24
            indicator.implicitWidth: 24
            leftPadding: 1
            checked: false
          }
        }

        onClicked: {
          Qt.inputMethod.hide();
        }
      }

      QfExpandableGroupBox {
        id: qfieldCloudGroupBox
        title: qsTr("Backup & collaborate?")
        width: parent.width
        checked: false
        icon: Theme.getThemeVectorIcon("ic_cloud_active_24dp")

        content: Column {
          id: databaseAndColabrationColumn
          width: parent.width

          Label {
            text: qsTr("QFieldCloud allows to synchronize and merge the data collected by your team in QField. From small individual projects to large data collection campaigns. ") + "<a href=\"https://qfield.cloud/\">" + qsTr("Learn more about QFieldCloud here") + "</a>."
            font: Theme.defaultFont
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            width: parent.width
            textFormat: Text.RichText
            onLinkActivated: link => Qt.openUrlExternally(link)
          }
        }

        onClicked: {
          Qt.inputMethod.hide();
        }
      }
    }
  }

  Rectangle {
    id: bottomRow
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right

    height: childrenRect.height + 20 + mainWindow.sceneBottomMargin
    color: Theme.darkTheme ? Theme.mainBackgroundColorSemiOpaque : Theme.lightestGraySemiOpaque

    QfButton {
      id: createProjectButton
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.margins: 10
      bgcolor: Theme.mainColor

      text: qsTr("Create Project")

      onClicked: {
        if (projectName.text === "") {
          projectName.text = qsTr("My Project");
        }
        let projectConfig = {
          "title": projectName.text,
          "basemap": baseMapList.model[Math.max(0, baseMapList.currentIndex)].name,
          "basemap_url": baseMapURL.text,
          "notes": takeNotesGroupBox.checked,
          "camera_capture": takeMediaCheckBox.checked,
          "tracks": trackPositionGroupBox.checked,
          "track_on_launch": autoTrackPositionCheckBox.checked
        };
        if (qfieldCloudGroupBox.checked) {
          if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
            const projectFilePath = ProjectUtils.createProject(projectConfig, positionSource.positionInformation);
            triggerCloudify(projectName.text, projectFilePath);
          } else {
            connection.enabled = true;
            triggerConnection();
          }
        } else {
          const projectFilePath = ProjectUtils.createProject(projectConfig, positionSource.positionInformation);
          triggerProjectLoad(projectName.text, projectFilePath);
        }
      }
    }
  }

  Connections {
    id: connection
    target: cloudConnection
    enabled: false

    function onStatusChanged() {
      if (cloudConnection.status === QFieldCloudConnection.LoggedIn) {
        createProjectButton.click();
      }
    }
  }

  function show() {
    if (visible) {
      return;
    }
    projectName.text = "";
    contentFlickable.contentY = 0;
    visible = true;
  }

  onVisibleChanged: {
    connection.enabled = false;
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true;
      header.onBack();
    }
  }
}
