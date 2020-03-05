import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12



Popup {
    id: questionDialog
    parent: ApplicationWindow.overlay

    property alias questionText: label.text
    property alias button1Text: button1.text
    property alias button2Text: button2.text
    property alias button3Text: button3.text
    property int nButtons: 3

    signal button1Clicked()
    signal button2Clicked()
    signal button3Clicked()

    anchors.centerIn: parent

    ColumnLayout {
        id: column
        Label {
            id: label
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }
        RowLayout {
            Layout.alignment: Qt.AlignRight
            Button{
                id: button1
                onClicked: {
                    button1Clicked();
                    close();
                }
            }
            Button{
                id: button2
                visible: nButtons >= 2
                onClicked: {
                    button2Clicked();
                    close();
                }
            }
            Button{
                id: button3
                visible: nButtons >= 3
                onClicked: {
                    button3Clicked();
                    close();
                }
            }
        }
    }
}
