import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
    id: bookmarkProperties

    property string bookmarkId: ''
    property string bookmarkName: ''
    property string bookmarkGroup: ''

    width: Math.min(350, mainWindow.width - Theme.popupScreenEdgeMargin)
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    padding: 0

    onAboutToShow: {
        nameField.text = bookmarkName;
        groupField.value = bookmarkGroup;
    }

    function saveBookmark() {
        bookmarkModel.updateBookmarkDetails(bookmarkProperties.bookmarkId, nameField.text, groupField.value)
    }

    Page {
        width: parent.width
        padding: 10
        header: ToolBar {
            id: toolBar

            background: Rectangle {
                color: "transparent"
                height: 48
            }

            RowLayout {
                width: parent.width
                height: 48

                Label {
                    Layout.leftMargin: 48
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                    text: qsTr('Bookmark Properties')
                    font: Theme.strongFont
                    color: Theme.mainColor
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                }

                QfToolButton {
                    id: closeButton
                    Layout.alignment: Qt.AlignVCenter
                    iconSource: Theme.getThemeIcon( 'ic_close_black_24dp' )
                    bgcolor: "transparent"

                    onClicked: {
                        bookmarkProperties.close();
                    }
                }
            }
        }

        ColumnLayout {
            id: propertiesLayout
            spacing: 4
            width: parent.width

            Label {
                Layout.fillWidth: true
                text: qsTr('Name')
                font: Theme.defaultFont
            }

            QfTextField {
                id: nameField
                Layout.fillWidth: true
                font: Theme.defaultFont
                text: ''

                onTextChanged: {
                    saveBookmark();
                }
            }

            Label {
                Layout.fillWidth: true
                text: qsTr('Color')
                font: Theme.defaultFont
            }

            RowLayout {
                id: groupField
                spacing: 8
                Layout.fillWidth: true

                property int iconSize: 32
                property string value: ''

                onValueChanged: {
                    saveBookmark();
                }

                Rectangle {
                    id: defaultColor
                    Layout.alignment: Qt.AlignVCenter
                    width: groupField.iconSize
                    height: groupField.iconSize
                    color: Theme.bookmarkDefault
                    border.width: 4
                    border.color: groupField.value != 'orange' &&
                                  groupField.value != 'red' &&
                                  groupField.value != 'blue' ? "black" : "transparent"
                    radius: 2

                    MouseArea {
                        anchors.fill: parent
                        onClicked: groupField.value = '';
                    }
                }
                Rectangle {
                    id: orangeColor
                    width: groupField.iconSize
                    height: groupField.iconSize
                    color: Theme.bookmarkOrange
                    border.width: 4
                    border.color: groupField.value === 'orange' ? "black" : "transparent"
                    radius: 2

                    MouseArea {
                        anchors.fill: parent
                        onClicked: groupField.value = 'orange';
                    }
                }
                Rectangle {
                    id: redColor
                    width: groupField.iconSize
                    height: groupField.iconSize
                    color: Theme.bookmarkRed
                    border.width: 4
                    border.color: groupField.value === 'red' ? "black" : "transparent"
                    radius: 2

                    MouseArea {
                        anchors.fill: parent
                        onClicked: groupField.value = 'red';
                    }
                }
                Rectangle {
                    id: blueColor
                    width: groupField.iconSize
                    height: groupField.iconSize
                    color: Theme.bookmarkBlue
                    border.width: 4
                    border.color: groupField.value === 'blue' ? "black" : "transparent"
                    radius: 2

                    MouseArea {
                        anchors.fill: parent
                        onClicked: groupField.value = 'blue';
                    }
                }
                Item {
                    Layout.fillWidth: true
                }
            }

            QfButton {
                id: updateBookmarkButton
                Layout.fillWidth: true
                Layout.topMargin: 10
                text: qsTr('Copy bookmark details')

                onClicked: {
                    var point = bookmarkModel.getBookmarkPoint(bookmarkProperties.bookmarkId)
                    var crs = bookmarkModel.getBookmarkCrs(bookmarkProperties.bookmarkId)
                    var coordinates = StringUtils.pointInformation(point, crs)

                    platformUtilities.copyTextToClipboard(nameField.text + '\n' + coordinates)
                    displayToast(qsTr('Bookmark details copied to clipboard'));
                }
            }

            QfButton {
                id: deleteBookmarkButton
                Layout.fillWidth: true
                bgcolor: 'transparent'
                color: Theme.darkRed
                text: qsTr('Remove bookmark')

                onClicked: {
                    removeBookmarkDialog.open();
                }
            }
        }
    }

    Dialog {
        id: removeBookmarkDialog
        parent: mainWindow.contentItem

        visible: false
        modal: true
        font: Theme.defaultFont

        z: 10000 // 1000s are embedded feature forms, user a higher value to insure the dialog will always show above embedded feature forms
        x: ( mainWindow.width - width ) / 2
        y: ( mainWindow.height - height ) / 2

        title: qsTr( "Remove bookmark" )
        Label {
            width: parent.width
            wrapMode: Text.WordWrap
            text: qsTr( "You are about to remove a bookmark, proceed?" )
        }

        standardButtons: Dialog.Ok | Dialog.Cancel
        onAccepted: {
            bookmarkModel.removeBookmark(bookmarkProperties.bookmarkId);
            bookmarkProperties.close();
        }
    }
}
