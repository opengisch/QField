import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import Theme 1.0
import org.qfield 1.0

Item {
    id: aboutPanel

    Rectangle {
        color: "black"
        opacity: 0.8
        anchors.fill: parent
    }

    ColumnLayout {
        id: aboutContainer
        anchors.fill: parent
        anchors.margins: 20

        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
            ScrollBar.vertical.policy: ScrollBar.AsNeeded
            contentItem: information
            contentWidth: information.width
            contentHeight: information.height
            clip: true

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    aboutPanel.visible = false
                }
            }

            ColumnLayout {
                id: information
                width: aboutContainer.width
                height: Math.max(mainWindow.height - linksButton.height * 2 - 60,
                                 opengisLogo.height + qfieldLogo.height + 100)

                ColumnLayout {
                    Layout.fillHeight: true
                    width: aboutContainer.width

                    MouseArea {
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 138
                        Layout.preferredHeight: 138
                        Image {
                            id: qfieldLogo
                            width: parent.width
                            height: parent.height
                            source: "qrc:/images/qfield_logo.svg"
                            sourceSize.width: width * screen.devicePixelRatio
                            sourceSize.height: height * screen.devicePixelRatio
                        }
                        onClicked: Qt.openUrlExternally("https://qfield.org/")
                    }

                    Label {
                        Layout.fillWidth: true
                        Layout.maximumWidth: parent.width
                        Layout.alignment: Qt.AlignCenter
                        horizontalAlignment: Text.AlignHCenter
                        font: Theme.strongFont
                        color: Theme.light
                        textFormat: Text.RichText
                        text: {
                            var links = '<a href="https://github.com/opengisch/QField/commit/' + gitRev + '">' + gitRev.substr(0, 6) + '</a>'

                            if (appVersion && appVersion !== '1.0.0')
                                links += ' <a href="https://github.com/opengisch/QField/releases/tag/' + appVersion + '">' + appVersion + '</a>'

                            return "QField<br>" + appVersionStr + " (" + links + ")"
                        }
                        onLinkActivated: Qt.openUrlExternally(link)
                    }
                }

                ColumnLayout {
                    Layout.fillHeight: true
                    width: aboutContainer.width

                    MouseArea {
                        Layout.alignment: Qt.AlignCenter
                        Layout.preferredWidth: 91
                        Layout.preferredHeight:113
                        Image {
                            id: opengisLogo
                            width: parent.width
                            height: parent.height
                            source: "qrc:/images/opengis-logo.svg"
                            sourceSize.width: width * screen.devicePixelRatio
                            sourceSize.height: height * screen.devicePixelRatio
                        }
                        onClicked: Qt.openUrlExternally("https://opengis.ch")
                    }

                    Label {
                        Layout.fillWidth: true
                        Layout.maximumWidth: parent.width
                        Layout.alignment: Qt.AlignCenter
                        horizontalAlignment: Text.AlignHCenter
                        font: Theme.strongFont
                        color: Theme.light
                        textFormat: Text.RichText
                        text: qsTr( "Developed by" ) + '<br><a href="https://opengis.ch">OPENGIS.ch</a>'
                        onLinkActivated: Qt.openUrlExternally(link)
                    }
                }
            }
        }

        Label {
            Layout.fillWidth: true
            Layout.maximumWidth: parent.width
            Layout.alignment: Qt.AlignCenter
            horizontalAlignment: Text.AlignHCenter
            font: Theme.tinyFont
            color: Theme.light
            opacity: 0.6

            text: {
                var dataDirs = platformUtilities.appDataDirs();
                if (dataDirs.length > 0) {
                  return (dataDirs.length > 1
                          ? 'QField app directories'
                          : 'QField app directory')
                         + '\n' + dataDirs.join('\n');
                }
                return '';
            }
        }

        QfButton {
            id: sponsorshipButton
            Layout.fillWidth: true
            icon.source: Theme.getThemeVectorIcon( 'ic_sponsor_white_24dp' )

            text: qsTr( 'Support QField')
            onClicked: Qt.openUrlExternally("https://github.com/sponsors/opengisch")
        }

        QfButton {
            id: linksButton
            dropdown: true
            Layout.fillWidth: true
            icon.source: Theme.getThemeVectorIcon( 'ic_book_white_24dp' )

            text: qsTr( 'Documentation' )


            onClicked: {
                Qt.openUrlExternally("https://docs.qfield.org/")
            }

            onDropdownClicked: {
              linksMenu.popup(linksButton.width - linksMenu.width + 10, linksButton.y + 10)
            }
        }
    }

    Menu {
        id: linksMenu
        title: qsTr( "Links Menu" )

        width: {
          var result = 0;
          var padding = 0;
          for (var i = 0; i < count; ++i) {
            var item = itemAt(i);
            result = Math.max(item.contentItem.implicitWidth, result);
            padding = Math.max(item.padding, padding);
          }
          return result + padding * 2;
        }

        MenuItem {
            text: qsTr('Changelog')

            font: Theme.defaultFont
            height: 48
            leftPadding: 10
            icon.source: Theme.getThemeVectorIcon( 'ic_speaker_white_24dp' )

            onTriggered: {
                changelogPopup.open()
            }
        }
    }
}
