import QtQuick 2.0
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.12

ApplicationWindow
{
    id: mainWindow
    visible: true
    title: "System Loader"

    ColumnLayout {
        id: welcomeGrid
        anchors.fill: parent

        Image {
            source: "qrc:/images/qfield_logo.svg"

            sourceSize.width: 200
            sourceSize.height: 200

            fillMode: Image.PreserveAspectFit

            Layout.alignment: Qt.AlignCenter
        }

        Timer {
            interval: 3000
            repeat: false
            running: true
            onTriggered: {
                statusLabel.opacity = 1
                loaderProgress.opacity = 1
            }
        }

        ProgressBar {
            id: loaderProgress
            opacity: 0
            from: 0
            to: 100
            indeterminate: feedback.progress == -1
            value: feedback.progress
            width: parent.width * 0.6

            Layout.alignment: Qt.AlignCenter

            Behavior on opacity {
                NumberAnimation {
                    easing.type: Easing.InOutQuad
                    duration: 1000
                }
            }
        }

        Label {
            id: statusLabel
            opacity: 0
            font.pointSize: 16
            text: qsTr( "Preparing QField, this is a one time operation that might take some time" )
            wrapMode: Label.WordWrap
            horizontalAlignment: Label.AlignHCenter

            Layout.leftMargin: 6
            Layout.rightMargin: 6
            Layout.topMargin: 2
            Layout.bottomMargin: 2
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            Layout.fillWidth: true

            Behavior on opacity {
                NumberAnimation {
                    easing.type: Easing.InOutQuad
                    duration: 1000
                }
            }
        }
    }
}
