import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Particles
import QtCore
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Page {
  id: welcomeScreen

  property bool firstShown: false

  property alias model: table.model
  signal showLocalDataPicker
  signal showQFieldCloudScreen
  signal showSettings
  signal showProjectCreationScreen

  visible: false
  focus: visible

  Settings {
    id: registry
    category: 'QField'

    property string baseMapProject: ''
    property string defaultProject: ''
    property bool loadProjectOnLaunch: false
  }

  Rectangle {
    id: welcomeBackground
    anchors.fill: parent
    gradient: Gradient {
      GradientStop {
        position: 0.0
        color: Theme.darkTheme ? "#99000000" : "#99A5A5A5"
      }
      GradientStop {
        position: 0.33
        color: Theme.mainBackgroundColor
      }
    }
  }

  ScrollView {
    padding: 0
    topPadding: Math.max(0, Math.min(80, (mainWindow.height - welcomeGrid.height) / 2 - 45))
    leftPadding: mainWindow.sceneLeftMargin
    rightPadding: mainWindow.sceneRighMargin
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical: QfScrollBar {
      opacity: active
      _maxSize: 4
      _minSize: 2

      Behavior on opacity {
        NumberAnimation {
          duration: 200
        }
      }
    }
    contentItem: welcomeGrid
    contentWidth: welcomeGrid.width
    contentHeight: welcomeGrid.height
    anchors.fill: parent
    clip: true

    GridLayout {
      id: welcomeGrid
      columns: 1
      rowSpacing: 4

      width: mainWindow.width - mainWindow.sceneLeftMargin - mainWindow.sceneRightMargin

      ImageDial {
        id: imageDialLogo
        value: 1

        Layout.margins: 6
        Layout.topMargin: 14 + mainWindow.sceneTopMargin
        Layout.bottomMargin: 6
        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
        Layout.preferredWidth: Math.min(138, mainWindow.height / 8)
        Layout.preferredHeight: Math.min(138, mainWindow.height / 8)

        source: "qrc:/images/app_logo.svg"
        rotationOffset: 220
      }

      SwipeView {
        id: feedbackView
        visible: false

        Layout.margins: 6
        Layout.topMargin: 10
        Layout.bottomMargin: 10
        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
        Layout.preferredWidth: Math.min(410, welcomeGrid.width - 30)
        Layout.preferredHeight: Math.max(ohno.childrenRect.height, intro.childrenRect.height, ohyeah.childrenRect.height)
        clip: true

        Behavior on Layout.preferredHeight {
          NumberAnimation {
            duration: 100
            easing.type: Easing.InQuad
          }
        }

        interactive: false
        currentIndex: 1
        Item {
          id: ohno

          Rectangle {
            anchors.fill: parent
            gradient: Gradient {
              GradientStop {
                position: 0.0
                color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.26)
              }
              GradientStop {
                position: 0.88
                color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.02)
              }
            }

            radius: 6
          }

          ColumnLayout {
            spacing: 0
            anchors.centerIn: parent

            Text {
              Layout.margins: 6
              Layout.topMargin: 12
              Layout.maximumWidth: feedbackView.width - 12
              text: qsTr("We're sorry to hear that. Click on the button below to comment or seek support.")
              font: Theme.defaultFont
              color: Theme.mainTextColor
              horizontalAlignment: Text.AlignHCenter
              wrapMode: Text.WordWrap
            }

            RowLayout {
              spacing: 6
              Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
              Layout.bottomMargin: 10

              QfButton {
                topPadding: 8
                bottomPadding: 8
                leftPadding: 10
                rightPadding: 10

                text: qsTr("Reach out")
                icon.source: Theme.getThemeVectorIcon('ic_create_white_24dp')

                onClicked: {
                  Qt.openUrlExternally("https://www.qfield.org/");
                  feedbackView.Layout.preferredHeight = 0;
                }
              }
            }
          }
        }

        Item {
          id: intro

          Rectangle {
            anchors.fill: parent
            gradient: Gradient {
              GradientStop {
                position: 0.0
                color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.26)
              }
              GradientStop {
                position: 0.88
                color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.02)
              }
            }

            radius: 6
          }

          ColumnLayout {
            spacing: 0
            anchors.centerIn: parent

            Text {
              Layout.margins: 6
              Layout.topMargin: 12
              Layout.maximumWidth: feedbackView.width - 12
              text: qsTr("Hey there, how do you like your experience with QField so far?")
              font: Theme.defaultFont
              color: Theme.mainTextColor
              horizontalAlignment: Text.AlignHCenter
              wrapMode: Text.WordWrap
            }

            RowLayout {
              spacing: 6
              Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
              Layout.bottomMargin: 10
              QfToolButton {
                iconSource: Theme.getThemeVectorIcon('ic_dissatisfied_white_24dp')
                iconColor: Theme.mainOverlayColor
                bgcolor: Theme.mainColor
                round: true

                onClicked: {
                  feedbackView.currentIndex = 0;
                }
              }
              QfToolButton {
                iconSource: Theme.getThemeVectorIcon('ic_satisfied_white_24dp')
                iconColor: Theme.mainOverlayColor
                bgcolor: Theme.mainColor
                round: true

                onClicked: {
                  if (Qt.platform.os === "android" || Qt.platform.os === "ios" || Qt.platform.os === "windows") {
                    feedbackView.currentIndex = 2;
                  } else {
                    feedbackView.Layout.preferredHeight = 0;
                  }
                }
              }
            }
          }
        }
        Item {
          id: ohyeah

          Rectangle {
            anchors.fill: parent
            gradient: Gradient {
              GradientStop {
                position: 0.0
                color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.26)
              }
              GradientStop {
                position: 0.88
                color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.02)
              }
            }

            radius: 6
          }

          ColumnLayout {
            spacing: 0
            anchors.centerIn: parent

            Text {
              Layout.margins: 6
              Layout.topMargin: 12
              Layout.maximumWidth: feedbackView.width - 12
              text: qsTr("That's great! We'd love for you to click on the button below and leave a review.")
              font: Theme.defaultFont
              color: Theme.mainTextColor
              horizontalAlignment: Text.AlignHCenter
              wrapMode: Text.WordWrap
            }

            RowLayout {
              spacing: 6
              Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
              Layout.margins: 6
              Layout.bottomMargin: 10
              QfButton {
                topPadding: 8
                bottomPadding: 8
                leftPadding: 10
                rightPadding: 10

                text: qsTr("Rate us")
                icon.source: Theme.getThemeVectorIcon('ic_star_white_24dp')

                onClicked: {
                  if (Qt.platform.os === "windows") {
                    Qt.openUrlExternally("https://apps.microsoft.com/detail/xp99h3bcx4bw7f");
                  } else if (Qt.platform.os === "android") {
                    Qt.openUrlExternally("market://details?id=ch.opengis.qfield");
                  } else if (Qt.platform.os === "ios") {
                    Qt.openUrlExternally("itms-apps://itunes.apple.com/app/qfield-for-qgis/id1531726814");
                  }
                  feedbackView.Layout.preferredHeight = 0;
                }
              }
            }
          }
        }
      }

      SwipeView {
        id: collectionView
        visible: false

        Layout.margins: 0
        Layout.topMargin: 10
        Layout.bottomMargin: 10
        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
        Layout.preferredWidth: Math.min(410, welcomeGrid.width - 20)
        Layout.preferredHeight: Math.max(collectionOhno.childrenRect.height, collectionIntro.childrenRect.height)
        clip: true

        Behavior on Layout.preferredHeight {
          NumberAnimation {
            duration: 100
            easing.type: Easing.InQuad
          }
        }

        interactive: false
        currentIndex: 1
        Item {
          id: collectionOhno

          Rectangle {
            anchors.fill: parent
            gradient: Gradient {
              GradientStop {
                position: 0.0
                color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.26)
              }
              GradientStop {
                position: 0.88
                color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.02)
              }
            }

            radius: 6
          }

          ColumnLayout {
            spacing: 0
            anchors.centerIn: parent

            Text {
              Layout.margins: 6
              Layout.topMargin: 12
              Layout.maximumWidth: collectionView.width - 12
              text: qsTr("Anonymized metrics collection has been disabled. You can re-enable through the settings panel.")
              font: Theme.defaultFont
              color: Theme.mainTextColor
              horizontalAlignment: Text.AlignHCenter
              wrapMode: Text.WordWrap
            }
          }
        }

        Item {
          id: collectionIntro

          Rectangle {
            anchors.fill: parent
            gradient: Gradient {
              GradientStop {
                position: 0.0
                color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.26)
              }
              GradientStop {
                position: 0.88
                color: Qt.hsla(Theme.mainColor.hslHue, Theme.mainColor.hslSaturation, Theme.mainColor.hslLightness, 0.02)
              }
            }

            radius: 6
          }

          ColumnLayout {
            spacing: 0
            anchors.centerIn: parent

            Text {
              Layout.margins: 6
              Layout.topMargin: 12
              Layout.maximumWidth: collectionView.width - 12
              text: qsTr("To improve stability for everyone, QField collects and sends anonymized metrics.")
              font: Theme.defaultFont
              color: Theme.mainTextColor
              horizontalAlignment: Text.AlignHCenter
              wrapMode: Text.WordWrap
            }

            RowLayout {
              spacing: 6
              Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
              Layout.bottomMargin: 10
              QfButton {
                topPadding: 8
                bottomPadding: 8
                leftPadding: 10
                rightPadding: 10

                text: qsTr('I agree')

                onClicked: {
                  qfieldSettings.enableInfoCollection = true;
                  collectionView.visible = false;
                }
              }

              QfButton {
                topPadding: 8
                bottomPadding: 8
                leftPadding: 10
                rightPadding: 10

                text: qsTr('I prefer not')
                bgcolor: "transparent"
                color: Theme.mainColor

                onClicked: {
                  qfieldSettings.enableInfoCollection = false;
                  collectionView.visible = false;
                }
              }
            }
          }
        }
      }

      Text {
        id: welcomeText
        visible: !feedbackView.visible
        Layout.leftMargin: 6
        Layout.rightMargin: 6
        Layout.topMargin: 2
        Layout.bottomMargin: 8
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
        text: ""
        font: Theme.defaultFont
        color: Theme.mainTextColor
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
      }

      Rectangle {
        Layout.leftMargin: 6
        Layout.rightMargin: 6
        Layout.topMargin: 2
        Layout.bottomMargin: 8
        Layout.fillWidth: true
        Layout.maximumWidth: 410
        Layout.preferredHeight: welcomeActions.height
        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
        color: "transparent"

        ColumnLayout {
          id: welcomeActions
          width: parent.width - 12
          spacing: 12

          GridLayout {
            Layout.fillWidth: true
            columns: 3
            rows: 2

            Repeater {
              id: actionsRepeater
              model: [
                {
                  "icon": Theme.getThemeVectorIcon("ic_cloud_active_24dp"),
                  "iconColor": "transparent",
                  "action": function () {
                    showQFieldCloudScreen();
                  }
                },
                {
                  "icon": Theme.getThemeVectorIcon("ic_folder_open_black_24dp"),
                  "iconColor": Theme.mainColor,
                  "action": function () {
                    platformUtilities.requestStoragePermission();
                    showLocalDataPicker();
                  }
                },
                {
                  "icon": Theme.getThemeVectorIcon("ic_add_white_24dp"),
                  "iconColor": Theme.mainColor,
                  "action": function () {
                    showProjectCreationScreen();
                  }
                }
              ]

              delegate: QfToolButton {
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredWidth: Math.min(mainWindow.height / 4, welcomeActions.width / actionsRepeater.count / 1.5)
                Layout.preferredHeight: Layout.preferredWidth
                icon.width: width / 2.2
                icon.height: height / 2.2
                bgcolor: Theme.controlBackgroundAlternateColor
                round: false
                roundborder: true
                iconSource: modelData.icon
                iconColor: modelData.iconColor
                smooth: true
                onClicked: modelData.action()
              }
            }

            Repeater {
              model: [qsTr("QFieldCloud\nprojects"), qsTr("Local projects and\n datasets"), qsTr("Create new\nproject")]

              delegate: Text {
                Layout.preferredWidth: welcomeActions.width / 3
                text: modelData
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                color: Theme.mainTextColor
              }
            }
          }

          Text {
            id: recentText
            text: qsTr("Recent Projects")
            font.pointSize: Theme.tipFont.pointSize
            font.bold: true
            color: Theme.mainTextColor
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            Layout.topMargin: 10
          }

          Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: table.height
            color: "transparent"

            ListView {
              id: table
              ScrollBar.vertical: QfScrollBar {}
              flickableDirection: Flickable.AutoFlickIfNeeded
              boundsBehavior: Flickable.StopAtBounds
              clip: true
              width: parent.width
              height: contentItem.childrenRect.height

              delegate: QfProjectThumbnail {
                property string path: ProjectPath
                property var type: ProjectType
                property int changesCount: {
                  const project = cloudProjectsModel.findProject(QFieldCloudUtils.getProjectId(ProjectPath));
                  if (project) {
                    return project.deltasCount;
                  }
                  return 0;
                }
                property bool isOutdated: {
                  const project = cloudProjectsModel.findProject(QFieldCloudUtils.getProjectId(ProjectPath));
                  if (project) {
                    return project.isOutdated;
                  }
                  return 0;
                }

                objectName: "loadProjectItem_1" // todo, suffix with e.g. ProjectTitle
                previewImageSource: welcomeScreen.visible ? 'image://projects/' + ProjectPath : ''
                showType: true

                primaryBadge.badgeText.text: changesCount > 0 ? changesCount : ''
                primaryBadge.badgeText.color: Theme.light
                primaryBadge.visible: showSync || showPush
                primaryBadge.color: showSync ? Theme.mainColor : Theme.cloudColor
                primaryBadge.border.color: Theme.mainBackgroundColor
                primaryBadge.border.width: 1
                primaryBadge.enableGradient: showSync && showPush

                readonly property bool showSync: isOutdated
                readonly property bool showPush: changesCount > 0

                projectTypeSource: switch (ProjectType) {
                case 0:
                  return Theme.getThemeVectorIcon('ic_map_green_48dp');     // local project
                case 1:
                  return Theme.getThemeVectorIcon('ic_cloud_project_48dp'); // cloud project
                case 2:
                  return Theme.getThemeVectorIcon('ic_file_green_48dp');    // local dataset
                default:
                  return '';
                }
                projectTitle.text: ProjectTitle
                projectNote: {
                  var notes = [];
                  if (index == 0) {
                    var firstRun = settings && !settings.value("/QField/FirstRunFlag", false);
                    if (!firstRun && firstShown === false)
                      notes.push(qsTr("Last session"));
                  }
                  if (ProjectPath === registry.defaultProject) {
                    notes.push(qsTr("Default project"));
                  }
                  if (ProjectPath === registry.baseMapProject) {
                    notes.push(qsTr("Base map"));
                  }
                  if (notes.length > 0) {
                    return notes.join('; ');
                  } else {
                    return "";
                  }
                }
              }

              MouseArea {
                property Item pressedItem
                anchors.fill: parent
                onClicked: mouse => {
                  var item = table.itemAt(mouse.x, mouse.y);
                  if (item) {
                    if (item.type === 1 && cloudConnection.hasToken && cloudConnection.status !== QFieldCloudConnection.LoggedIn) {
                      cloudConnection.login();
                    }
                    iface.loadFile(item.path, item.projectTitle.text);
                  }
                }
                onPressed: mouse => {
                  var item = table.itemAt(mouse.x, mouse.y);
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
                  var item = table.itemAt(mouse.x, mouse.y);
                  if (item) {
                    recentProjectActions.recentProjectPath = item.path;
                    recentProjectActions.recentProjectType = item.type;
                    recentProjectActions.popup(mouse.x, mouse.y);
                  }
                }
              }

              QfMenu {
                id: recentProjectActions

                property string recentProjectPath: ''
                property int recentProjectType: 0

                title: qsTr('Recent Project Actions')

                topMargin: mainWindow.sceneTopMargin
                bottomMargin: mainWindow.sceneBottomMargin

                MenuItem {
                  id: defaultProject
                  visible: recentProjectActions.recentProjectType != 2

                  font: Theme.defaultFont
                  width: parent.width
                  height: visible ? 48 : 0
                  leftPadding: Theme.menuItemCheckLeftPadding
                  checkable: true
                  checked: recentProjectActions.recentProjectPath === registry.defaultProject

                  text: qsTr("Default Project")
                  onTriggered: {
                    registry.defaultProject = recentProjectActions.recentProjectPath === registry.defaultProject ? '' : recentProjectActions.recentProjectPath;
                  }
                }

                MenuItem {
                  id: baseMapProject
                  visible: recentProjectActions.recentProjectType != 2

                  font: Theme.defaultFont
                  width: parent.width
                  height: visible ? 48 : 0
                  leftPadding: Theme.menuItemCheckLeftPadding
                  checkable: true
                  checked: recentProjectActions.recentProjectPath === registry.baseMapProject

                  text: qsTr("Individual Datasets Base Map")
                  onTriggered: {
                    registry.baseMapProject = recentProjectActions.recentProjectPath === registry.baseMapProject ? '' : recentProjectActions.recentProjectPath;
                  }
                }

                MenuSeparator {
                  visible: baseMapProject.visible
                  width: parent.width
                  height: visible ? undefined : 0
                }

                MenuItem {
                  id: removeProject

                  font: Theme.defaultFont
                  width: parent.width
                  height: visible ? 48 : 0
                  leftPadding: Theme.menuItemIconlessLeftPadding

                  text: qsTr("Remove from Recent Projects")
                  onTriggered: {
                    iface.removeRecentProject(recentProjectActions.recentProjectPath);
                    model.reloadModel();
                  }
                }
              }
            }
          }

          RowLayout {
            Layout.leftMargin: 10
            Layout.rightMargin: 10
            Layout.bottomMargin: mainWindow.sceneBottomMargin
            Label {
              Layout.fillWidth: true
              Layout.alignment: Qt.AlignVCenter
              topPadding: 10
              bottomPadding: 10
              font: Theme.tipFont
              wrapMode: Text.WordWrap
              color: Theme.mainTextColor

              text: registry.defaultProject != '' ? qsTr('Load default project on launch') : qsTr('Load last opened project on launch')

              MouseArea {
                anchors.fill: parent
                onClicked: reloadOnLaunch.checked = !reloadOnLaunch.checked
              }
            }

            QfSwitch {
              id: reloadOnLaunch
              Layout.preferredWidth: implicitContentWidth
              Layout.alignment: Qt.AlignVCenter
              width: implicitContentWidth

              checked: registry.loadProjectOnLaunch
              onCheckedChanged: {
                registry.loadProjectOnLaunch = checked;
              }
            }
          }
        }
      }
    }
  }

  Column {
    spacing: 4
    anchors {
      top: parent.top
      left: parent.left
      topMargin: mainWindow.sceneTopMargin + 4
      leftMargin: mainWindow.sceneLeftMargin + 4
    }

    QfActionButton {
      id: currentProjectButton
      toolImage: Theme.getThemeVectorIcon('ic_arrow_left_white_24dp')
      toolText: welcomeScreen.width > 420 ? qsTr('Return to map') : ""
      visible: qgisProject && !!qgisProject.homePath
      innerActionIcon.visible: false

      onClicked: {
        welcomeScreen.visible = false;
      }
    }

    QfToolButton {
      id: settingsButton
      iconSource: Theme.getThemeVectorIcon('ic_tune_white_24dp')
      iconColor: Theme.mainTextColor
      bgcolor: "transparent"
      round: true

      onClicked: {
        showSettings();
      }
    }
  }

  QfToolButton {
    id: exitButton
    visible: (Qt.platform.os === "ios" || Qt.platform.os === "android" || mainWindow.sceneBorderless)
    anchors {
      top: parent.top
      right: parent.right
      topMargin: mainWindow.sceneTopMargin + 4
      rightMargin: mainWindow.sceneRightMargin + 4
    }
    iconSource: Theme.getThemeVectorIcon('ic_shutdown_24dp')
    iconColor: Theme.mainTextColor
    bgcolor: "transparent"
    round: true

    onClicked: {
      mainWindow.closeAlreadyRequested = true;
      mainWindow.close();
    }
  }

  // Sparkles & unicorns
  Rectangle {
    anchors.fill: parent
    color: "#00000000"
    visible: imageDialLogo.value < 0.1

    MouseArea {
      id: mouseArea
      anchors.fill: parent
      acceptedButtons: Qt.LeftButton | Qt.RightButton
      hoverEnabled: true
      propagateComposedEvents: true
      onReleased: mouse.accepted = false
      onDoubleClicked: mouse.accepted = false
      onPressAndHold: mouse.accepted = false
      onClicked: mouse => {
        burstSomeSparkles(mouse.x, mouse.y);
        mouse.accepted = false;
      }
      onPressed: mouse => {
        burstSomeSparkles(mouse.x, mouse.y);
        mouse.accepted = false;
      }
      onPositionChanged: mouse => {
        burstSomeSparkles(mouse.x, mouse.y);
        mouse.accepted = false;
      }
    }

    ParticleSystem {
      id: particles
      running: imageDialLogo.value < 0.1
    }

    ParticleSystem {
      id: unicorns
      running: imageDialLogo.value < 0.1
    }

    ImageParticle {
      anchors.fill: parent
      system: particles
      source: "qrc:///particleresources/star.png"
      sizeTable: "qrc:///images/sparkleSize.png"
      alpha: 1
      colorVariation: 0.3
    }

    ImageParticle {
      anchors.fill: parent
      system: unicorns
      source: "qrc:///images/icons/unicorn.png"
      alpha: 1
      redVariation: 0
      blueVariation: 0
      greenVariation: 0
      rotation: 0
      rotationVariation: 360
    }

    Emitter {
      id: emitterParticles
      x: -100
      y: -100
      system: particles
      emitRate: 60
      lifeSpan: 700
      size: 50
      sizeVariation: 10
      maximumEmitted: 100
      velocity: AngleDirection {
        angle: 0
        angleVariation: 360
        magnitude: 100
        magnitudeVariation: 50
      }
    }

    Emitter {
      id: emitterUnicorns
      x: -100
      y: -100
      system: unicorns
      emitRate: 20
      lifeSpan: 900
      size: 70
      sizeVariation: 10
      maximumEmitted: 100
      velocity: AngleDirection {
        angle: 90
        angleVariation: 20
        magnitude: 200
        magnitudeVariation: 50
      }
    }
  }

  function burstSomeSparkles(x, y) {
    emitterParticles.burst(50, x, y);
    emitterUnicorns.burst(1, x, y);
  }

  function adjustWelcomeScreen() {
    if (visible) {
      if (firstShown) {
        welcomeText.text = " ";
      } else {
        var firstRun = !settings.valueBool("/QField/FirstRunDone", false);
        if (firstRun) {
          welcomeText.text = qsTr("Welcome to QField. First time using this application? Try the sample projects listed below.");
          settings.setValue("/QField/FirstRunDone", true);
          settings.setValue("/QField/showMapCanvasGuide", true);
        } else {
          welcomeText.text = qsTr("Welcome back to QField.");
        }
      }
    }
  }

  Component.onCompleted: {
    adjustWelcomeScreen();
    var runCount = settings.value("/QField/RunCount", 0) * 1;
    var feedbackFormShown = settings.value("/QField/FeedbackFormShown", false);
    if (!feedbackFormShown) {
      var now = new Date();
      var dt = settings.value("/QField/FirstRunDate", "");
      if (dt != "") {
        dt = new Date(dt);
        var daysToPrompt = 30;
        var runsToPrompt = 5;
        if (runCount >= runsToPrompt && (now - dt) >= (daysToPrompt * 24 * 60 * 60 * 1000)) {
          feedbackView.visible = true;
          settings.setValue("/QField/FeedbackFormShown", true);
        }
      } else {
        settings.setValue("/QField/FirstRunDate", now.toISOString());
      }
    }
    if (platformUtilities.capabilities & PlatformUtilities.SentryFramework) {
      var collectionFormShown = settings.value("/QField/CollectionFormShownV2", false);
      if (!collectionFormShown) {
        collectionView.visible = true;
        settings.setValue("/QField/CollectionFormShownV2", true);
      }
    }
    settings.setValue("/QField/RunCount", runCount + 1);
    if (registry.defaultProject != '') {
      if (!FileUtils.fileExists(registry.defaultProject)) {
        registry.defaultProject = '';
      }
    }
  }

  onVisibleChanged: {
    adjustWelcomeScreen();
    if (!visible) {
      feedbackView.visible = false;
      collectionView.visible = false;
      firstShown = true;
    }
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      if (qgisProject.fileName != '') {
        event.accepted = true;
        visible = false;
      } else {
        event.accepted = false;
      }
    }
  }
}
