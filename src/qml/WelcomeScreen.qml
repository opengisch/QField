import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Particles 2.0

import Theme 1.0

Page {
  id: welcomeScreen

  property bool firstShown: false

  property alias model: table.model
  signal showOpenProjectDialog

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

    Text {
      Layout.leftMargin: 6
      Layout.rightMargin: 6
      Layout.topMargin: 2
      Layout.bottomMargin: 2
      id: welcomeText
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      text: ""
      font: Theme.defaultFont
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.WordWrap
      Layout.fillWidth: true
    }

    Rectangle {
      Layout.leftMargin: 6
      Layout.rightMargin: 6
      Layout.topMargin: 2
      Layout.bottomMargin: 2
      Layout.fillWidth: true
      Layout.maximumWidth: 410
      Layout.minimumHeight: welcomeActions.height
      Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
      color: "transparent"

      ColumnLayout {
        id: welcomeActions
        width: parent.width
        spacing: 12

        QfButton {
          id: localProjectButton
          Layout.fillWidth: true
          text: qsTr( "Open local file" )
          onClicked: {
            showOpenProjectDialog()
          }
        }
        QfButton {
          id: cloudProjectButton
          Layout.fillWidth: true
          text: qsTr( "QField Cloud projects, coming soon" )
          enabled: false
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
            height: table.height
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
          height: table.model.rowCount() * ( 80)

          delegate: Rectangle {
            id: rectangle
            property string path: ProjectPath
            property string title: ProjectTitle
            width: parent ? parent.width : undefined
            height: line.height
            color: "transparent"

            Row {
              id: line
              Layout.fillWidth: true
              leftPadding: 6
              rightPadding: 10
              topPadding: 9
              bottomPadding: 3
              spacing: 0
              Image {
                id: type
                anchors.verticalCenter: inner.verticalCenter
                source: switch(ProjectType) {
                        case 0: return Theme.getThemeIcon('ic_map_green_48dp'); // local project
                        case 1: return ''; // cloud project
                        case 2: return Theme.getThemeIcon('ic_file_green_48dp'); // local dataset
                        default: return'';
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
                    if (index == 0) {
                        var firstRun = !settings.value( "/QField/FirstRunFlag", false )
                        return !firstRun && firstShown === false ? qsTr( "Last session" ) : ""
                    }
                    else
                    {
                        return ""
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
              if (item)
                iface.loadFile(item.path,item.title)
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
                    recentProjectActions.popup(mouse.x, mouse.y)
                }
            }
          }

          Menu {
            id: recentProjectActions

            property string recentProjectPath: ''

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
              id: removeProject

              font: Theme.defaultFont
              width: parent.width
              height: visible ? 48: 0
              leftPadding: 10

              text: qsTr( "Remove Recent Project" )
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
          welcomeText.text = qsTr( "Welcome to QField. First time using this application? Try out a few demos listed in the recent projects below." )
        } else {
          welcomeText.text = qsTr( "Welcome back to QField." )
        }
        currentProjectButton.visible = false
        firstShown = true
      }
    }
  }

  Component.onCompleted: {
    adjustWelcomeScreen()
  }

  onVisibleChanged: {
    adjustWelcomeScreen()
  }
}
