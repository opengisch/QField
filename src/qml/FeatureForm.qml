import QtQuick 2.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.1
import org.qgis 1.0
import org.qfield 1.0
import "js/style.js" as Style

Rectangle {
  signal saved
  signal cancelled
  signal aboutToSave

  property FeatureModel feature
  property alias toolbarVisible: toolbar.visible
  property alias model: formElement.model

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

  Component {
    id: formElement

    property AttributeFormModel model

    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.top: toolbar.bottom
  }

  Controls.TabView
  {
    focus: true

    Repeater {
      id: repeater;

      model: attributeFormModel

      Controls.Tab { title: DisplayRole }
    }
  }

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
      columns: 2
      rowSpacing: 5
      columnSpacing: 5
      anchors.margins: 5

      Repeater {
        model: form.model

        ColumnLayout
        {
          Layout.row: index
          Layout.column: 0
          Layout.fillWidth: true
          Layout.fillHeight: true
          Layout.preferredHeight: childrenRect.height

          visible: EditorWidget !== "Hidden"

          Controls.Label {
            text: AttributeName
            font.bold: true
          }

          Item {
            id: placeholder
            height: childrenRect.height
            anchors.left: parent.left
            anchors.right: parent.right

            Connections {
              target: form
              onAboutToSave: {
                try {
                  attributeEditorLoader.item.pushChanges()
                }
                catch ( err )
                {}
              }
            }

            Loader {
              id: attributeEditorLoader
              anchors { left: parent.left; right: parent.right }

              enabled: form.state !== "ReadOnly"
              property var value: AttributeValue
              property var config: EditorWidgetConfig
              property var widget: EditorWidget
              property var field: Field

              source: 'editorwidgets/' + widget + '.qml'

              onStatusChanged: {
                if ( attributeEditorLoader.status === Loader.Error )
                {
                  console.warn( "Editor widget type '" + EditorWidget + "' not avaliable." )
                  widget = 'TextEdit'
                }
              }
            }

            Connections {
              target: attributeEditorLoader.item
              onValueChanged: form.model.setAttribute( index, value, isNull )
            }
          }
        }
      }

      Repeater {
        model: form.model

        Controls.CheckBox {
          Layout.row: index
          Layout.column: 1
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
        save()
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

  function save() {
    parent.focus = true
    aboutToSave()

    if ( form.state === "Add" ) {
      model.create()
      state = "Edit"
    }
    else
    {
      model.save()
    }

    saved()
  }

  Connections {
    target: Qt.inputMethod
    onVisibleChanged: {
      Qt.inputMethod.commit()
    }
  }
}
