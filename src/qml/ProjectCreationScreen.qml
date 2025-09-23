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

  signal triggerCloudify(string title, string path)
  signal triggerProjectLoad(string title, string path)

  header: QfPageHeader {
    title: qsTr("Create a new project")

    backgroundFill: false
    showBackButton: true
    showApplyButton: false
    showCancelButton: false
    showMenuButton: false

    topMargin: mainWindow.sceneTopMargin
    titleColor: Theme.mainTextColor
    titleFont: Theme.strongTitleFont

    onBack: {
      projectCreation.visible = false;
    }
  }

  Flickable {
    anchors.fill: parent
    anchors.margins: 10
    contentHeight: newProjectConfigColumn.height + bottomRow.height + 20
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
        text: qsTr("My project")
      }

      QfExpandableGroupBox {
        id: baseMapExpandablePanel
        title: qsTr("Select your basemap")
        width: parent.width
        checked: true
        interactive: false
        content: Column {
          id: basemapColumn
          anchors.left: parent.left
          anchors.right: parent.right
          spacing: 10

          Label {
            text: qsTr("Choose a basemap for your project. Pick from the available options or provide your own URL.")
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
                "name": "colorful"
              }, {
                "icon": "qrc:/pictures/pictures/dark.jpg",
                "name": "dark"
              }, {
                "icon": "qrc:/pictures/pictures/lightgray.jpg",
                "name": "light"
              }, {
                "icon": "",
                "name": "blank"
              }, {
                "icon": "",
                "name": "custom"
              }]

            clip: true

            delegate: QfProjectThumbnail {
              width: 150
              implicitHeight: 95
              radius: 4
              bgColor: modelData.name === "Blank" ? "white" : Theme.groupBoxSurfaceColor
              previewImageSource: modelData.icon
              projectTitle.text: modelData.name
              projectTitle.color: Theme.mainTextColor
              projectTitle.font.underline: false
              showType: false
              selected: baseMapList.currentIndex == index
              fillHeight: true
              showCustomizeIcon: projectTitle.text === "Custom"

              MouseArea {
                anchors.fill: parent
                onClicked: {
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
      }

      QfExpandableGroupBox {
        id: trackPositionGroupBox
        title: qsTr("Track your position?")
        width: parent.width
        checked: true
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
            checked: true
          }
        }
      }

      QfExpandableGroupBox {
        id: qfieldCloudGroupBox
        title: qsTr("Backup & collaborate?")
        width: parent.width
        checked: true
        icon: "qrc:/themes/qfield/nodpi/ic_cloud_active_24dp.svg"

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
      }
    }
  }

  Rectangle {
    id: bottomRow
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right

    height: childrenRect.height + 20
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
        const projectFilePath = ProjectUtils.createProject(projectConfig);
        projectCreation.visible = false;
        if (qfieldCloudGroupBox.checked) {
          triggerCloudify(projectName.text, projectFilePath);
        } else {
          triggerProjectLoad(projectName.text, projectFilePath);
        }
      }
    }
  }
}
