import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Popup {
    id: bookmarkProperties

    property string bookmarkId: ''
    property string bookmarkName: ''
    property string bookmarkGroup: ''


    width: Math.min(350, mainWindow.width - 20 )
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    padding: 0

    onAboutToShow: {
        nameField.text = bookmarkName;
        groupField.value = bookmarkGroup;
    }

    Page {
        width: parent.width
        padding: 10
        header: Label {
            padding: 10
            topPadding: 15
            bottomPadding: 0
            width: parent.width - 20
            text: qsTr('Bookmark Properties')
            font: Theme.strongFont
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
        }

        ColumnLayout {
            spacing: 4
            width: parent.width

            Label {
                Layout.fillWidth: true
                text: qsTr('Name')
                font: Theme.defaultFont
            }

            TextField {
                id: nameField
                Layout.fillWidth: true
                font: Theme.defaultFont
                horizontalAlignment: TextInput.AlignHLeft
                text: ''

                background: Rectangle {
                    y: nameField.height - height * 2 - nameField.bottomPadding / 2
                    implicitWidth: 120
                    height: nameField.activeFocus ? 2 : 1
                    color: nameField.activeFocus ? "#4CAF50" : "#C8E6C9"
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

                property int iconSize: 24
                property string value: ''

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
                text: 'Update bookmark details'

                onClicked: {
                    bookmarkModel.updateBookmarkDetails(bookmarkProperties.bookmarkId, nameField.text, groupField.value)
                    bookmarkProperties.close();
                }
            }

            QfButton {
                id: deleteBookmarkButton
                Layout.fillWidth: true
                text: 'Remove bookmark'

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
