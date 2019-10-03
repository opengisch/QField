import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as Controls

import org.qfield 1.0
import Theme 1.0

Controls.TableView {
  id: variableEditor
  anchors.fill: parent

  function reset() {
    Qt.inputMethod.hide()
    variableEditor.model.reloadVariables()
  }

  function apply() {
    variableEditor.model.save()
  }

  model: ExpressionVariableModel {}

  /* The column for the variable name */
  Controls.TableViewColumn {
    id : variableNameColumn
    role: "VariableName"
    title: "Name"
    width: 200 * dp

    delegate: Item {
      anchors.fill: parent
      anchors.leftMargin: 8 * dp

      TextField {
        id: nameEditor
        anchors.fill: parent
        anchors.margins: 4*dp
        text: styleData.value
        visible: variableEditor.model.isEditable( styleData.row ) && styleData.selected
        font: Theme.defaultFont

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
        font.pointSize: Theme.defaultFont.pointSize
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

      TextField {
        id: varEditor
        anchors.fill: parent
        anchors.margins: 4*dp
        text: styleData.value
        visible: variableEditor.model.isEditable( styleData.row ) && styleData.selected
        font: Theme.defaultFont

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
        font.pointSize: Theme.defaultFont.pointSize
      }

      Text {
        anchors.fill: parent
        text: styleData.value
        visible: !varEditor.visible
        color: variableEditor.model.isEditable( styleData.row ) ? "black" : "#7f8c8d"
      }

      Button {
        width: 48 * dp
        height: 48 * dp
        contentItem: Image {
          fillMode: Image.Pad
          horizontalAlignment: Image.AlignHCenter
          verticalAlignment: Image.AlignVCenter
          source: Theme.getThemeIcon( 'ic_delete_forever_white_24dp' )
        }
        anchors.right: parent.right
        visible: styleData.value !== '' && variableEditor.model.isEditable( styleData.row )

        onClicked: {
          variableEditor.model.removeCustomVariable( styleData.row );
        }
      }
    }
  }

  rowDelegate: Rectangle {
     height: 48 * dp
     SystemPalette {
        id: myPalette;
        colorGroup: SystemPalette.Active
     }
     color: {
       var baseColor = styleData.alternate ? myPalette.alternateBase : myPalette.base
       // Using the selection color renders the text input almost unusable
       // return styleData.selected ? myPalette.highlight : baseColor
       return baseColor
     }
  }
}
