import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3

import org.qfield 1.0
import Theme 1.0

ColumnLayout {
    anchors.fill: parent
    Layout.fillWidth: true
    Layout.fillHeight: true
    spacing: 4

    Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        color: "white"
        border.color: "lightgray"
        border.width: 1

        ListView {
            id: table
            model: ExpressionVariableModel {}
            flickableDirection: Flickable.VerticalFlick
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            spacing: 1

            anchors.fill: parent
            anchors.margins: 3 * dp

            function reset() {
                Qt.inputMethod.hide()
                table.model.reloadVariables()
            }

            function apply() {
                table.model.save()
            }

            delegate: Rectangle {
                property var itemRow: index
                property bool canDelete: table.model.isEditable( index )

                id: rectangle
                width: parent.width
                height: line.height
                color: "#ffffff"

                Row {
                    id: line
                    leftPadding: 4 * dp
                    spacing: 5 * dp

                    TextField {
                        id: variableNameText
                        width: 0.35 * table.width - 10 * dp
                        topPadding: 10 * dp
                        bottomPadding: 10 * dp
                        leftPadding: 5 * dp
                        rightPadding: 5 * dp
                        text: VariableName
                        enabled: table.model.isEditable(index)
                        font: Theme.tipFont
                        horizontalAlignment: TextInput.AlignLeft
                        placeholderText: qsTr( "Enter name" )

                        background: Rectangle {
                            y: variableNameText.height - height - variableNameText.bottomPadding / 2
                            implicitWidth: 120 * dp
                            height: variableNameText.activeFocus ? 2 * dp : variableNameText.enabled ? 1 * dp : 0
                            color: variableNameText.activeFocus ? "#4CAF50" : "#C8E6C9"
                        }

                        onTextChanged: {
                            table.model.setName( index, text )
                        }
                    }

                    TextField {
                        id: variableValueText
                        width: 0.65 * table.width - 10 * dp - (canDelete ? 48 * dp : 0)
                        topPadding: 10 * dp
                        bottomPadding: 10 * dp
                        leftPadding: 5 * dp
                        rightPadding: 5 * dp
                        text: VariableValue
                        enabled: table.model.isEditable(index)
                        font: Theme.tipFont
                        horizontalAlignment: TextInput.AlignLeft
                        placeholderText: qsTr( "Enter value" )

                        background: Rectangle {
                            y: variableValueText.height - height - variableValueText.bottomPadding / 2
                            implicitWidth: 120 * dp
                            height: variableValueText.activeFocus ? 2 * dp : variableNameText.enabled ? 1 * dp : 0
                            color: variableValueText.activeFocus ? "#4CAF50" : "#C8E6C9"
                        }

                        onTextChanged: {
                            table.model.setValue( index, text )
                        }
                    }

                    Button {
                        id: deleteVariableButton
                        width: 48 * dp
                        height: 48 * dp
                        contentItem: Image {
                            fillMode: Image.Pad
                            horizontalAlignment: Image.AlignHCenter
                            verticalAlignment: Image.AlignVCenter
                            source: Theme.getThemeIcon( 'ic_delete_forever_white_24dp' )
                        }
                        visible: canDelete

                        onClicked: {
                            table.model.removeCustomVariable( index );
                        }
                    }
                }
            }
        }
    }

    QfButton {
        id: addCustomVariableButton
        Layout.fillWidth: true
        text: qsTr( "Add a new variable" )

        onClicked: {
            table.model.addCustomVariable( "new_variable" , "" );
            table.positionViewAtIndex( table.count - 1, ListView.visible );
            // TODO: Use Qt 5.13 itemAtIndex( index )
            table.children[0].children[table.count].children[0].children[0].forceActiveFocus();
        }
    }
}
