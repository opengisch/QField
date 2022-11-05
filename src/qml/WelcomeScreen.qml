import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14
import QtQuick.Particles 2.14
import Qt.labs.settings 1.0

import org.qfield 1.0
import Theme 1.0

Page {
  id: welcomeScreen

  property bool firstShown: false

  property alias model: table.model
  signal openLocalDataPicker
  signal showQFieldCloudScreen

  Settings {
    id: registry
    category: 'QField'
    property string baseMapProject: ''
  }

  Rectangle {
    id: welcomeBackground
    anchors.fill: parent
    gradient: Gradient  {
      GradientStop  {
        position: 0.0
        color: "#ffdedede"
      }
      GradientStop  {
        position: 0.33
        color: "#00dedede"
      }
    }
  }

  ScrollView {
    padding: 0
    topPadding: Math.max(0, Math.min(80, (mainWindow.height - welcomeGrid.height) / 2 - 45))
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AsNeeded
    contentItem: welcomeGrid
    contentWidth: welcomeGrid.width
    contentHeight: welcomeGrid.height
    anchors.fill: parent
    clip: true

  GridLayout {
    id: welcomeGrid
    columns: 1
    rowSpacing: 4

    width: mainWindow.width

    ImageDial{
      id: imageDialLogo
      value: 1

      Layout.margins: 6
      Layout.topMargin: 14
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      Layout.preferredWidth: Math.min( 138, mainWindow.height / 4 )
      Layout.preferredHeight: Math.min( 138, mainWindow.height / 4 )

      source: "qrc:/images/qfield_logo.svg"
      rotationOffset: 220
    }

    SwipeView {
      id: feedbackView
      visible: false

      Layout.margins: 6
      Layout.topMargin: 10
      Layout.bottomMargin: 10
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      Layout.preferredWidth: Math.min( 410, mainWindow.width - 30 )
      Layout.preferredHeight: Math.max(ohno.childrenRect.height, intro.childrenRect.height, ohyeah.childrenRect.height)
      clip: true

      Behavior on Layout.preferredHeight {
          NumberAnimation { duration: 100; easing.type: Easing.InQuad; }
      }

      interactive: false
      currentIndex: 1
      Item {
        id: ohno

        Rectangle {
          anchors.fill: parent
          gradient: Gradient  {
            GradientStop  {
              position: 0.0
              color: "#4480cc28"
            }
            GradientStop  {
              position: 0.88
              color: "#0580cc28"
            }
          }

          radius: 6
        }

        ColumnLayout {
          spacing: 0
          anchors.centerIn: parent

          Text {
            Layout.margins: 6
            Layout.maximumWidth: feedbackView.width - 12
            text: qsTr("We're sorry to hear that. Click on the button below to comment or seek support.")
            font: Theme.defaultFont
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
          }

          RowLayout {
            spacing: 6
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            Layout.bottomMargin: 10
            QfButton {
              leftPadding: 20
              rightPadding: 20

              text: qsTr("Reach out")
              icon.source: Theme.getThemeIcon( 'ic_create_white_24dp' )

              onClicked: {
                Qt.openUrlExternally("https://www.qfield.org/")
                feedbackView.Layout.preferredHeight = 0
              }
            }
          }
        }
      }

      Item {
        id: intro

        Rectangle {
          anchors.fill: parent
          gradient: Gradient  {
            GradientStop  {
              position: 0.0
              color: "#4480cc28"
            }
            GradientStop  {
              position: 0.88
              color: "#0580cc28"
            }
          }

          radius: 6
        }

        ColumnLayout {
          spacing: 0
          anchors.centerIn: parent

          Text {
            Layout.margins: 6
            Layout.maximumWidth: feedbackView.width - 12
            text: qsTr("Hey there, how do you like your experience with QField so far?")
            font: Theme.defaultFont
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
          }

          RowLayout {
            spacing: 6
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            Layout.bottomMargin: 10
            QfToolButton {
              iconSource: Theme.getThemeVectorIcon('ic_dissatisfied_white_24dp')
              bgcolor: Theme.mainColor
              round: true

              onClicked: {
                feedbackView.currentIndex = 0
              }
            }
            QfToolButton {
              iconSource: Theme.getThemeVectorIcon('ic_satisfied_white_24dp')
              bgcolor: Theme.mainColor
              round: true

              onClicked: {
                feedbackView.currentIndex = 2
              }
            }
          }
        }
      }
      Item {
        id: ohyeah

        Rectangle {
          anchors.fill: parent
          gradient: Gradient  {
            GradientStop  {
              position: 0.0
              color: "#4480cc28"
            }
            GradientStop  {
              position: 0.88
              color: "#0580cc28"
            }
          }

          radius: 6
        }

        ColumnLayout {
          spacing: 0
          anchors.centerIn: parent

          Text {
            Layout.margins: 6
            Layout.maximumWidth: feedbackView.width - 12
            text: qsTr("That's great! We'd love for you to click on the button below and leave a comment on the store.")
            font: Theme.defaultFont
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
          }

          RowLayout {
            spacing: 6
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            Layout.margins: 6
            Layout.bottomMargin: 10
            QfButton {
              leftPadding: 20
              rightPadding: 20

              text: qsTr("Rate us")
              icon.source: Theme.getThemeVectorIcon( 'ic_star_white_24dp' )

              onClicked: {
                Qt.openUrlExternally("market://details?id=ch.opengis.qfield")
                feedbackView.Layout.preferredHeight = 0
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
      Layout.preferredWidth: Math.min( 410, mainWindow.width - 20 )
      Layout.preferredHeight: Math.max(collectionOhno.childrenRect.height, collectionIntro.childrenRect.height)
      clip: true

      Behavior on Layout.preferredHeight {
          NumberAnimation { duration: 100; easing.type: Easing.InQuad; }
      }

      interactive: false
      currentIndex: 1
      Item {
        id: collectionOhno

        Rectangle {
          anchors.fill: parent
          gradient: Gradient  {
            GradientStop  {
              position: 0.0
              color: "#4480cc28"
            }
            GradientStop  {
              position: 0.88
              color: "#0580cc28"
            }
          }

          radius: 6
        }

        ColumnLayout {
          spacing: 0
          anchors.centerIn: parent

          Text {
            Layout.margins: 6
            Layout.maximumWidth: collectionView.width - 12
            text: qsTr("Anonymized metrics collection has been disabled. You can re-enable through the settings panel.")
            font: Theme.defaultFont
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
          }
        }
      }

      Item {
        id: collectionIntro

        Rectangle {
          anchors.fill: parent
          gradient: Gradient  {
            GradientStop  {
              position: 0.0
              color: "#4480cc28"
            }
            GradientStop  {
              position: 0.88
              color: "#0580cc28"
            }
          }

          radius: 6
        }

        ColumnLayout {
          spacing: 0
          anchors.centerIn: parent

          Text {
            Layout.margins: 6
            Layout.maximumWidth: collectionView.width - 12
            text: qsTr("To improve stability for everyone, QField collects and sends anonymized metrics.")
            font: Theme.defaultFont
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
          }

          RowLayout {
            spacing: 6
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.bottomMargin: 10
            QfButton {
              text: qsTr('I agree')

              onClicked: {
                qfieldSettings.enableInfoCollection = true
                collectionView.visible = false
              }
            }

            QfButton {
              text: qsTr('I prefer not')
              bgcolor: "transparent"
              color: Theme.mainColor

              onClicked: {
                qfieldSettings.enableInfoCollection = false
                collectionView.visible = false
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
      Layout.bottomMargin: 2
      Layout.fillWidth: true
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      text: ""
      font: Theme.defaultFont
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.WordWrap
    }

    Rectangle {
      Layout.leftMargin: 6
      Layout.rightMargin: 6
      Layout.topMargin: 2
      Layout.bottomMargin: 2
      Layout.fillWidth: true
      Layout.maximumWidth: 410
      Layout.preferredHeight: welcomeActions.height
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      color: "transparent"

      ColumnLayout {
        id: welcomeActions
        width: parent.width
        spacing: 12

        QfButton {
          id: cloudProjectButton
          Layout.fillWidth: true
          text: qsTr( "QFieldCloud projects" )
          onClicked: {
            showQFieldCloudScreen()
          }
        }
        QfButton {
          id: localProjectButton
          Layout.fillWidth: true
          text: qsTr( "Open local file" )
          onClicked: {
            openLocalDataPicker()
          }
        }

        Text {
          id: recentText
          text: qsTr( "Recent Projects" )
          font.pointSize: Theme.tipFont.pointSize
          font.bold: true
          horizontalAlignment: Text.AlignHCenter
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: table.height
            color: "transparent"
            border.color: "transparent"
            border.width: 1

        ListView {
          id: table
          ScrollBar.vertical: ScrollBar {}
          flickableDirection: Flickable.VerticalFlick
          boundsBehavior: Flickable.StopAtBounds
          clip: true
          width: parent.width
          height: contentItem.childrenRect.height

          delegate: Rectangle {
            id: rectangle
            objectName: "loadProjectItem_1" // todo, suffix with e.g. ProjectTitle
            property string path: ProjectPath
            property string title: ProjectTitle
            property var type: ProjectType
            width: parent ? parent.width : undefined
            height: line.height
            color: "transparent"

            Row {
              id: line
              width: parent.width
              leftPadding: 6
              rightPadding: 10
              topPadding: 9
              bottomPadding: 3
              spacing: 0
              Image {
                id: type
                anchors.verticalCenter: inner.verticalCenter
                source: switch(ProjectType) {
                        case 0: return Theme.getThemeVectorIcon('ic_map_green_48dp');     // local project
                        case 1: return Theme.getThemeVectorIcon('ic_cloud_project_48dp'); // cloud project
                        case 2: return Theme.getThemeVectorIcon('ic_file_green_48dp');    // local dataset
                        default: return '';
                        }
                sourceSize.width: 80
                sourceSize.height: 80
                width: 40
                height: 40
              }
              ColumnLayout {
                id: inner
                width: rectangle.width - type.width - 10
                Text {
                  id: projectTitle
                  topPadding: 5
                  leftPadding: 3
                  text: ProjectTitle
                  font.pointSize: Theme.tipFont.pointSize
                  font.underline: true
                  color: Theme.mainColor
                  wrapMode: Text.WordWrap
                  Layout.fillWidth: true
                }
                Text {
                  id: projectNote
                  leftPadding: 3
                  text: {
                    var notes = [];

                    if ( index == 0 ) {
                        var firstRun = settings && !settings.value( "/QField/FirstRunFlag", false )
                        if (!firstRun && firstShown === false) notes.push( qsTr( "Last session" ) );
                    }

                    if ( ProjectPath === registry.baseMapProject ) {
                        notes.push( qsTr( "Base map project" ) );
                    }

                    if ( notes.length > 0 ) {
                        return notes.join( '; ' );
                    } else {
                        return "";
                    }
                  }
                  visible: text != ""
                  font.pointSize: Theme.tipFont.pointSize - 2
                  font.italic: true
                  wrapMode: Text.WordWrap
                  Layout.fillWidth: true
                }
              }
            }
          }
          MouseArea {
            property Item pressedItem
            anchors.fill: parent
            onClicked: {
              var item = table.itemAt(mouse.x, mouse.y)
              if (item) {
                if ( item.type == 1 && cloudConnection.hasToken && cloudConnection.status !== QFieldCloudConnection.LoggedIn ) {
                  cloudConnection.login()
                }
                iface.loadFile(item.path,item.title)
              }
            }
            onPressed: {
              var item = table.itemAt(mouse.x, mouse.y)
              if (item) {
                pressedItem = item.children[0].children[1].children[0];
                pressedItem.color = "#5a8725"
              }
            }
            onCanceled: {
              if (pressedItem) {
                pressedItem.color = Theme.mainColor
                pressedItem = null
              }
            }
            onReleased: {
              if (pressedItem) {
                pressedItem.color = Theme.mainColor
                pressedItem = null
              }
            }
            onPressAndHold: {
                var item = table.itemAt(mouse.x, mouse.y)
                if (item) {
                    recentProjectActions.recentProjectPath = item.path;
                    recentProjectActions.recentProjectType = item.type;
                    recentProjectActions.popup(mouse.x, mouse.y)
                }
            }
          }

          Menu {
            id: recentProjectActions

            property string recentProjectPath: ''
            property int recentProjectType: 0

            title: 'Recent Project Actions'

            width: {
                var result = 0;
                var padding = 0;
                for (var i = 0; i < count; ++i) {
                    var item = itemAt(i);
                    result = Math.max(item.contentItem.implicitWidth, result);
                    padding = Math.max(item.padding, padding);
                }
                return Math.min( result + padding * 2,mainWindow.width - 20);
            }

            MenuItem {
              id: baseMapProject
              visible: recentProjectActions.recentProjectType != 2;

              font: Theme.defaultFont
              width: parent.width
              height: visible ? 48: 0
              checkable: true
              checked: recentProjectActions.recentProjectPath === registry.baseMapProject

              text: qsTr( "Base Map Project" )
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
              height: visible ? 48: 0
              leftPadding: 10

              text: qsTr( "Remove from Recent Projects" )
              onTriggered: {
                  iface.removeRecentProject( recentProjectActions.recentProjectPath );
                  model.reloadModel();
              }
            }
          }
        }
        }
      }
    }
  }
  }

  QfToolButton {
    id: currentProjectButton
    visible: false
    anchors {
      top: parent.top
      left: parent.left
      topMargin: mainWindow.sceneTopMargin
    }
    iconSource: Theme.getThemeIcon( 'ic_chevron_left_black_24dp' )
    bgcolor: "transparent"

    onClicked: {
      welcomeScreen.visible = false;
      welcomeScreen.focus = false;
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
      onClicked:  {
        burstSomeSparkles(mouse.x, mouse.y)
        mouse.accepted = false
      }
      onPressed:  {
        burstSomeSparkles(mouse.x, mouse.y)
        mouse.accepted = false
      }
      onPositionChanged: {
        burstSomeSparkles(mouse.x, mouse.y)
        mouse.accepted = false
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
    emitterParticles.burst(50, x, y)
    emitterUnicorns.burst(1, x, y)
  }

  function adjustWelcomeScreen() {
    if (visible) {
      if (firstShown) {
        welcomeText.text = " ";
        currentProjectButton.visible = true
      } else {
        var firstRun = !settings.value( "/QField/FirstRunFlag", false )
        if ( firstRun ) {
          welcomeText.text = qsTr( "Welcome to QField. First time using this application? Try out sample projects listed in the list below." )
        } else {
          welcomeText.text = qsTr( "Welcome back to QField." )
        }
        currentProjectButton.visible = false
      }
    }
  }

  Component.onCompleted: {
    adjustWelcomeScreen()

    var runCount = settings.value("/QField/RunCount",0) * 1
    var feedbackFormShown = settings.value("/QField/FeedbackFormShown",false)
    if (!feedbackFormShown) {
      var now = new Date()
      var dt = settings.value("/QField/FirstRunDate", "")
      if (dt != "") {
        dt = new Date(dt)

        var daysToPrompt = 30;
        var runsToPrompt = 5;
        if (runCount >= runsToPrompt && (now - dt) >= (daysToPrompt * 24 * 60 * 60 * 1000)) {
          feedbackView.visible = true
          settings.setValue("/QField/FeedbackFormShown",true)
        }
      } else {
        settings.setValue("/QField/FirstRunDate", now.toISOString())
      }
    }

    if (platformUtilities.capabilities & PlatformUtilities.SentryFramework) {
      var collectionFormShown = settings.value("/QField/CollectionFormShownV2",false)
      if (!collectionFormShown) {
        collectionView.visible = true
        settings.setValue("/QField/CollectionFormShownV2",true)
      }
    }

    settings.setValue("/QField/RunCount",runCount + 1)
  }

  onVisibleChanged: {
    adjustWelcomeScreen()
    focus = visible
    if (!visible) {
      feedbackView.visible = false
      collectionView.visible = false
      firstShown = true
    }
  }
}
