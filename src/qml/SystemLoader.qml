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

        ProgressBar {
            id: loaderProgress
            from: 0
            to: 100
            value: feedback.progress
            width: parent.width * 0.6

            Layout.alignment: Qt.AlignCenter
        }

        Label {
            font.pointSize: 16
            text: qsTr( "Preparing QField ..." )
            Layout.alignment: Qt.AlignCenter
        }
    }
}
