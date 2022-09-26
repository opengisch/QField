import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import Theme 1.0
import org.qfield 1.0



Popup {
    id: changelogPopup

    parent: ApplicationWindow.overlay
    x: Theme.popupScreenEdgeMargin
    y: Theme.popupScreenEdgeMargin
    width: parent.width - Theme.popupScreenEdgeMargin * 2
    height: parent.height - Theme.popupScreenEdgeMargin * 2
    padding: 0
    modal: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
    focus: visible

    Page {
        focus: true
        anchors.fill: parent

        header: PageHeader {
            title: qsTr("What's new in QField")

            showApplyButton: false
            showCancelButton: false
            showBackButton: true

            onBack: {
                changelogPopup.close()
            }
        }

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: 10

            Flickable {
                id: changelogFlickable
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.topMargin: 10
                Layout.bottomMargin: 10
                flickableDirection: Flickable.VerticalFlick
                interactive: true
                contentWidth: parent.width;
                contentHeight: changelogGrid.height
                clip: true

                GridLayout {
                    id: changelogGrid

                    anchors.left: parent.left
                    anchors.right: parent.right

                    columns: 1

                    Text {
                        id: changelogBody
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.minimumHeight: contentHeight
                        Layout.maximumHeight: contentHeight
                        visible: changelogContents.status != ChangelogContents.LoadingStatus

                        color: '#95000000'
                        font: Theme.tipFont

                        fontSizeMode: Text.VerticalFit
                        textFormat: Text.MarkdownText
                        wrapMode: Text.WordWrap

                        text: {
                            switch ( changelogContents.status ) {
                            case ChangelogContents.IdleStatus:
                            case ChangelogContents.LoadingStatus:
                                return ''
                            case ChangelogContents.SuccessStatus:
                                return changelogContents.markdown
                            case ChangelogContents.ErrorStatus:
                                return qsTr( 'Error while fetching changelog, try again later.' )
                            }
                        }

                        onLinkActivated: Qt.openUrlExternally(link)
                    }

                    BusyIndicator {
                        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                        visible: changelogContents.status == ChangelogContents.LoadingStatus
                        running: visible
                    }
                }
            }

            QfButton {
                id: sponsorshipButton
                Layout.fillWidth: true
                icon.source: Theme.getThemeVectorIcon( 'ic_sponsor_white_24dp' )

                text: qsTr( 'Support QField')
                onClicked: Qt.openUrlExternally("https://github.com/sponsors/opengisch")
            }
        }
    }

    ChangelogContents {
        id: changelogContents
        onMarkdownChanged: {
            if ( changelogContents.markdown ) {
                settings.setValue( "/QField/isLoadingChangelog", false )
                settings.remove( "/QField/isCrashingSslDevice" )
            }
        }
    }

    onClosed: {
        settings.setValue( "/QField/ChangelogVersion", appVersion )
        changelogFlickable.contentY = 0
    }

    onOpened: {
        if ( settings.valueBool( "/QField/isLoadingChangelog", false ) ) {
            settings.setValue( "/QField/isCrashingSslDevice", true )
        } else {
            settings.remove( "/QField/isCrashingSslDevice" )
        }

        if ( settings.valueBool( "/QField/isCrashingSslDevice", false ) === true ) {
            changelogBody.text = qsTr( "Check the latest QField changes on " )
                    + ' <a href="https://github.com/opengisch/qfield/releases">' + qsTr( 'QField releases page' ) + '</a>.'
            return
        }

        if ( changelogContents.status === ChangelogContents.SuccessStatus || changelogContents.status === ChangelogContents.LoadingStatus )
            return

        settings.remove( "/QField/isLoadingChangelog" )
        settings.setValue( "/QField/isLoadingChangelog", true )
        settings.sync()

        changelogContents.request()
    }
}
