import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0
import Theme 1.0

ColumnLayout {
    function reset() {
        Qt.inputMethod.hide()
        table.model.reloadVariables()
    }

    function apply() {
        table.model.save()
    }

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
            anchors.margins: 3

            delegate: Rectangle {
                property var itemRow: index
                property bool canDelete: table.model.isEditable( index )

                id: rectangle
                width: parent.width
                height: line.height
                color: "#ffffff"

                Row {
                    id: line
                    leftPadding: 4
                    spacing: 5

                    TextField {
                        id: variableNameText
                        width: 0.35 * table.width - 10
                        topPadding: 10
                        bottomPadding: 10
                        leftPadding: 5
                        rightPadding: 5
                        text: VariableName
                        enabled: table.model.isEditable(index)
                        font: Theme.tipFont
                        horizontalAlignment: TextInput.AlignLeft
                        placeholderText: qsTr( "Enter name" )

                        background: Rectangle {
                            y: variableNameText.height - height - variableNameText.bottomPadding / 2
                            implicitWidth: 120
                            height: variableNameText.activeFocus ? 2: variableNameText.enabled ? 1: 0
                            color: variableNameText.activeFocus ? Theme.accentColor : Theme.accentLightColor
                        }

                        onTextChanged: {
                            table.model.setName( index, text )
                        }
                    }

                    TextField {
                        id: variableValueText
                        width: 0.65 * table.width - 10- (canDelete ? deleteVariableButton.width: 0)
                        topPadding: 10
                        bottomPadding: 10
                        leftPadding: 5
                        rightPadding: 5
                        text: VariableValue
                        enabled: table.model.isEditable(index)
                        font: Theme.tipFont
                        horizontalAlignment: TextInput.AlignLeft
                        placeholderText: qsTr( "Enter value" )

                        background: Rectangle {
                            y: variableValueText.height - height - variableValueText.bottomPadding / 2
                            implicitWidth: 120
                            height: variableValueText.activeFocus ? 2: variableNameText.enabled ? 1: 0
                            color: variableValueText.activeFocus ? Theme.accentColor : Theme.accentLightColor
                        }

                        onTextChanged: {
                            table.model.setValue( index, text )
                        }
                    }

                    ToolButton {
                        id: deleteVariableButton
                        width: 36
                        height: 36
                        anchors.verticalCenter: parent.verticalCenter
                        visible: canDelete

                        contentItem: Rectangle {
                            anchors.fill: parent
                            color: Theme.darkGray
                            Image {
                                anchors.fill: parent
                                anchors.margins: 4
                                fillMode: Image.PreserveAspectFit
                                horizontalAlignment: Image.AlignHCenter
                                verticalAlignment: Image.AlignVCenter
                                source: Theme.getThemeIcon( 'ic_delete_forever_white_24dp' )
                            }
                        }
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
