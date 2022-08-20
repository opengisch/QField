import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Controls.Material 2.14
import QtQuick.Controls.Material.impl 2.14

import Theme 1.0

Item {
    property bool collapsed: true
    property alias color: titleText.color
    property alias font: titleText.font
    property alias titleText: titleText.text
    property alias detailsText: detailsText.text

    clip: true
    height: collapsed ? titleText.height + 1 : titleText.height + detailsText.height + 1
    implicitHeight: height

    Behavior on height {
        NumberAnimation { duration: 100; easing.type: Easing.InQuad; }
    }

    Rectangle {
        id: background
        anchors.fill: parent

        color: "transparent"
        border.color: titleText.color
        border.width: 1
        opacity: 0.25
        radius: 12
    }

    Text {
        id: titleText

        width: parent.width - 5
        anchors.top: parent.top
        anchors.left: parent.left
        padding: 5
        clip: true

        font: Theme.defaultFont
        color: "black"

        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
    }

    Rectangle {
        id: separator

        width: parent.width - 24
        anchors.top: titleText.bottom
        anchors.left: parent.left
        anchors.leftMargin: 12

        height: 1
        color: titleText.color
        opacity: 0.25
    }

    Text {
        id: detailsText

        width: parent.width - 5
        anchors.top: separator.bottom
        anchors.left: parent.left
        padding: 5
        clip: true

        font.pointSize: titleText.font.pointSize / 1.5
        font.weight: titleText.font.weight
        font.italic: titleText.font.italic
        font.family: titleText.font.family

        color: titleText.color
        opacity: 0.25

        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            parent.collapsed = !parent.collapsed
        }
    }
}
