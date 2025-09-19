import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material.impl
import org.qfield
import Theme

Page {
  id: createNewProject

  signal create(var projectConfig)

  leftPadding: mainWindow.sceneLeftMargin
  rightPadding: mainWindow.sceneRightMargin

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
      createNewProject.visible = false;
    }
  }

  Flickable {
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: createProjectButton.top
    anchors.margins: 16
    contentHeight: newProjectConfigColumn.height
    clip: true

    Column {
      id: newProjectConfigColumn
      width: parent.width
      spacing: 8

      Label {
        text: qsTr("Project Name")
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
        placeholderText: text === "" && !focus ? "New project name" : ""
      }

      Item {
        width: 1
        height: 2
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

          Label {
            text: qsTr("Choose a basemap for your project. Pick from the available options or provide your own link.")
            font: Theme.defaultFont
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            width: parent.width
          }

          Item {
            width: 1
            height: 16
          }

          ListView {
            id: baseMapList
            width: parent.width
            height: 95
            orientation: ListView.Horizontal
            spacing: 4
            model: [{
                "icon": "qrc:/pictures/pictures/colorful.jpg",
                "name": "Colorful "
              }, {
                "icon": "qrc:/pictures/pictures/dark.jpg",
                "name": "Dark "
              }, {
                "icon": "qrc:/pictures/pictures/lightgray.jpg",
                "name": "Light "
              }, {
                "icon": "",
                "name": "Blank"
              }, {
                "icon": "",
                "name": "Custom"
              }]

            clip: true

            delegate: Rectangle {
              width: 150
              height: 95
              color: modelData.name == "Blank" ? "white" : Theme.groupBoxSurfaceColor
              radius: 4
              border.width: 2
              border.color: baseMapList.currentIndex === index ? Theme.mainColor : "transparent"

              Image {
                id: baseMapIcon
                anchors.fill: parent
                anchors.margins: 2
                fillMode: Image.PreserveAspectCrop
                source: modelData.icon
                sourceSize.width: width * screen.devicePixelRatio
                sourceSize.height: height * screen.devicePixelRatio
                visible: source !== ""
              }

              Rectangle {
                width: 25
                height: width
                radius: width / 2
                color: Theme.mainColor
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.margins: 8
                visible: baseMapList.currentIndex === index
                opacity: visible

                Behavior on opacity  {
                  NumberAnimation {
                    duration: 200
                  }
                }

                Image {
                  anchors.fill: parent
                  anchors.margins: 4
                  fillMode: Image.PreserveAspectCrop
                  source: "qrc:/themes/qfield/nodpi/ic_check_white_24dp.svg"
                  sourceSize.width: width * screen.devicePixelRatio
                  sourceSize.height: height * screen.devicePixelRatio
                }
              }

              Rectangle {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.margins: 2
                anchors.right: parent.right
                height: 20
                color: Qt.hsla(Theme.mainBackgroundColor.hslHue, Theme.mainBackgroundColor.hslSaturation, Theme.mainBackgroundColor.hslLightness, Theme.darkTheme ? 0.75 : 0.9)

                Label {
                  text: modelData.name
                  font: Theme.defaultFont
                  color: Theme.mainTextColor
                  wrapMode: Text.WordWrap
                  width: parent.width
                  anchors.verticalCenter: parent.verticalCenter
                  anchors.left: parent.left
                  anchors.leftMargin: 8
                }
              }

              MouseArea {
                id: baseMapItemMouseArea
                anchors.fill: parent
                onClicked: baseMapList.currentIndex = index

                Ripple {
                  clip: true
                  anchor: parent
                  width: parent.width
                  height: parent.height
                  pressed: parent.pressed
                  active: parent.pressed
                  color: Material.rippleColor
                }
              }
            }
          }

          Item {
            width: 1
            height: 8
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
              placeholderText: text == "" && !focus ? "e.g., https://your-map-service.com/{z}/" : ""
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
            text: qsTr("Quickly capture notes with date, time, and comments. Optionally, attach images to enrich your notes.")
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
          spacing: 0

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

  QfButton {
    id: createProjectButton
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 16
    text: qsTr("Create Project")

    onClicked: {
      let selectedBasemap = (baseMapList.currentIndex >= 0 && baseMapList.currentIndex < baseMapList.model.length) ? baseMapList.model[baseMapList.currentIndex].name : "Colorful";
      const isCustomBasemap = (selectedBasemap === "Custom");
      selectedBasemap = isCustomBasemap ? baseMapURL.text : selectedBasemap;
      var projectConfig = {
        "name": projectName.text,
        "isCustomBasemapSelected": isCustomBasemap,
        "basemap": selectedBasemap,
        "takeNotes": takeNotesGroupBox.checked,
        "takeMediaAttachments": takeMediaCheckBox.checked,
        "trackPosition": trackPositionGroupBox.checked,
        "autoTrackPosition": autoTrackPositionCheckBox.checked,
        "useCloud": qfieldCloudGroupBox.checked
      };
      create(projectConfig);
    }
  }
}
