import QtQuick 2.0
import QtQuick.Controls 1.4 as Controls

import org.qfield 1.0
import "js/style.js" as Style

Item {
  height: variableEditor.contentItem.height

  function reset() {
    Qt.inputMethod.hide()
    variableEditor.model.reloadVariables()
  }

  function apply() {
    variableEditor.model.save()
  }

  Controls.TableView {
    id: variableEditor
    anchors.fill: parent

    /* The column for the variable name */
    Controls.TableViewColumn {
      id : variableNameColumn
      role: "VariableName"
      title: "Name"
      width: 200 * dp

      delegate: Item {
        anchors.fill: parent
        anchors.leftMargin: 8 * dp

        Controls.TextField {
          id: nameEditor
          text: styleData.value
          visible: variableEditor.model.isEditable( styleData.row ) && styleData.selected

          onTextChanged: {
            variableEditor.model.setName( styleData.row, text )
          }
        }

        Text {
          anchors.fill: parent
          text: qsTr( "[New variable name]" )
          visible: styleData.value === '' && variableEditor.model.isEditable( styleData.row ) && !styleData.selected
          color: "#7f8c8d"
          font.italic: true
        }

        Text {
          anchors.fill: parent
          text: styleData.value
          visible: !nameEditor.visible
          color: variableEditor.model.isEditable( styleData.row ) ? "black" : "#7f8c8d"
        }
      }
    }

    /* The column for the variable value */

    Controls.TableViewColumn {
      id : variableValueColumn
      role: "VariableValue"
      title: "Value"

      width: variableEditor.width - variableNameColumn.width

      delegate: Item {
        anchors.fill: parent
        anchors.leftMargin: 8 * dp

        Controls.TextField {
          id: varEditor
          text: styleData.value
          visible: variableEditor.model.isEditable( styleData.row ) && styleData.selected

          onTextChanged: {
            variableEditor.model.setValue( styleData.row, text )
          }
        }

        Text {
          anchors.fill: parent
          text: qsTr( "[New variable value]" )
          visible: styleData.value === '' && variableEditor.model.isEditable( styleData.row ) && !styleData.selected
          color: "#7f8c8d"
          font.italic: true
        }

        Text {
          anchors.fill: parent
          text: styleData.value
          visible: !varEditor.visible
          color: variableEditor.model.isEditable( styleData.row ) ? "black" : "#7f8c8d"
        }

        Controls.Button {
          iconSource: Style.getThemeIcon( 'ic_delete_forever_white_24dp' )
          anchors.right: parent.right
          visible: styleData.value !== '' && variableEditor.model.isEditable( styleData.row )

          onClicked: {
            variableEditor.model.removeCustomVariable( styleData.row );
          }
        }
      }
    }

    model: ExpressionVariableModel {}
  }
}
