import QtQuick 2.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.1
import org.qgis 1.0
import "js/style.js" as Style

Rectangle {
  signal saved
  signal cancelled

  property FeatureModel model
  property alias toolbarVisible: toolbar.visible

  id: form

  states: [
    State {
      name: "ReadOnly"
    },
    State {
      name: "Edit"
    },
    State {
      name: "Add"
    }
  ]

  Flickable {
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.top: toolbar.bottom

    contentHeight: grid.height

    flickableDirection: Flickable.VerticalFlick

    GridLayout {
      id: grid
      anchors.left: parent.left
      anchors.right: parent.right
      columns: 3
      rowSpacing: 5
      columnSpacing: 5
      anchors.margins: 5

      Repeater {
        model: form.model

        Controls.Label {
          Layout.row: index
          Layout.column: 0
          text: AttributeName
          visible: EditorWidget !== "Hidden"
        }
      }

      Repeater {
        model: form.model

        Item {
          id: placeholder
          Layout.row: index
          Layout.column: 1
          Layout.fillWidth: true
          Layout.fillHeight: true
          Layout.preferredHeight: childrenRect.height

          Loader {
            id: attributeEditorLoader
            anchors { left: parent.left; right: parent.right }

            enabled: form.state !== "ReadOnly"
            property var value: AttributeValue
            property var config: EditorWidgetConfig
            property var widget: EditorWidget

            source: 'editorwidgets/' + widget + '.qml'

            onStatusChanged: {
              if (attributeEditorLoader.status === Loader.Error )
              {
                console.warn( "Editor widget type '" + EditorWidget + "' not avaliable." )
                widget = 'TextEdit'
              }
            }
          }

          Connections {
            target: attributeEditorLoader.item
            onValueChanged: form.model.setAttribute( index, value, FeatureModel.AttributeValue )
          }
        }
      }

      Repeater {
        model: form.model

        Controls.CheckBox {
          Layout.row: index
          Layout.column: 2
          Layout.fillWidth: false
          Layout.fillHeight: false
          checkedState: RememberValue

          visible: form.state === "Add" && EditorWidget !== "Hidden"

          anchors.right: parent.right

          onCheckedChanged: {
            var idx = form.model.index(index, 0)
            form.model.setData(idx, checkedState, FeatureModel.RememberValue)
          }
        }
      }
    }
  }


  Rectangle {
    id: toolbar

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top

    height: visible ? 48*dp : 0

    Button {
      anchors.right: parent.right

      width: 48*dp
      height: 48*dp

      iconSource: Style.getThemeIcon( "ic_save_white_24dp" )

      onClicked: {
        Qt.inputMethod.hide()

        if ( form.state === "Add" ) {
          model.create()
          state = "Edit"
        }
        model.save()

        saved()
      }
    }

    Button {
      anchors.left: parent.left

      width: 48*dp
      height: 48*dp

      iconSource: Style.getThemeIcon( "ic_close_white_24dp" )

      onClicked: {
        Qt.inputMethod.hide()

        cancelled()
      }
    }
  }
}
