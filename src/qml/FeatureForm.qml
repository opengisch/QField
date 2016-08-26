import QtQuick 2.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtQml.Models 2.2

import org.qgis 1.0
import org.qfield 1.0
import "js/style.js" as Style

Rectangle {
  signal saved
  signal cancelled
  signal aboutToSave

  property FeatureModel feature
  property AttributeFormModel model
  property alias toolbarVisible: toolbar.visible

  property var __currentTab

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

  Item {
    anchors.bottom: parent.bottom
    anchors.right: parent.right
    anchors.left: parent.left
    anchors.top: toolbar.bottom

    id: container

    // Tabs
    Item {
      anchors.fill: parent

      Rectangle {
        id: tabBar

        anchors { left: parent.left; right:parent.right; top: parent.top }
        height: childrenRect.height

        Row {
          Repeater {
            model: DelegateModel {
              id: rootElement
              model: form.model

              delegate: Item {
                height: childrenRect.height
                width: childrenRect.width

                Button {
                  id: button
                  text: Name

                  bgcolor: "white"

                  onClicked: {
                    activate(parent)
                  }
                }

                Rectangle {
                  color: __currentTab === parent ? "orange" : "gray"

                  height: 2 * dp
                  anchors.right: parent.right
                  anchors.left: parent.left
                  anchors.bottom: button.bottom

                  Behavior on color {
                    ColorAnimation {
                      easing.type: Easing.InOutQuad

                    }
                  }
                }

                Component.onCompleted: {
                  if ( index === 0 )
                    activate(this)
                }

                function activate(tab) {
                  __currentTab = tab

                  content.sourceComponent = undefined

                  content.pRootIndex = rootElement.modelIndex(index)
                  content.pType = Type
                  content.pName = ""
                  content.pEditorWidget = EditorWidget
                  content.pEditorWidgetConfig = EditorWidgetConfig
                  content.pAttributeValue = AttributeValue
                  content.pField = Field
                  content.pRememberValue = RememberValue

                  content.sourceComponent = element
                }
              }
            }
          }
        }
      }

      DropShadow {
        anchors.fill: tabBar
        horizontalOffset: 0
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: tabBar
      }

      Flickable {
        anchors { top: tabBar.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        clip: true

        flickableDirection: Flickable.VerticalFlick

        Loader {
          id: content

          anchors { left: parent.left; right: parent.right }

          property var pRootIndex
          property var pType
          property var pName
          property var pEditorWidget
          property var pEditorWidgetConfig
          property var pField
          property var pAttributeValue
          property var pRememberValue
        }
      }
    }
  }

  Component {
    id: element

    Item {
      height: childrenRect.height
      anchors { left: parent.left; right: parent.right }

      Controls.GroupBox {
        visible: pType != 'field'
        title: pName
        anchors { left: parent.left; right: parent.right }

        Column {
          anchors { left: parent.left; right: parent.right }

          Repeater {
            model: DelegateModel {
              id: delegateModel

              model: form.model
              rootIndex: pRootIndex
              delegate: Loader {
                property var pRootIndex
                property var pType
                property var pName
                property var pEditorWidget
                property var pEditorWidgetConfig
                property var pField
                property var pAttributeValue
                property var pRememberValue

                id: content

                anchors { left: parent.left; right: parent.right }

                Component.onCompleted: {
                  content.sourceComponent = undefined

                  content.pRootIndex = delegateModel.modelIndex(index)
                  content.pType = Type
                  content.pName = Name
                  content.pEditorWidget = EditorWidget
                  content.pEditorWidgetConfig = EditorWidgetConfig
                  content.pAttributeValue = AttributeValue
                  content.pField = Field
                  content.pRememberValue = RememberValue

                  content.sourceComponent = element
                }
              }
            }
          }
        }
      }

      Item {
        id: fieldContainer
        visible: pType == 'field'
        height: childrenRect.height
        anchors { left: parent.left; right: parent.right }

        Controls.Label {
          id: fieldLabel

          text: pName
          font.bold: true
        }

        Controls.TextField {
          text: "XXY"
          anchors { top: fieldLabel.bottom }
          anchors { left: parent.left; right: parent.right }
        }
/*
        Item {
          id: placeholder
          height: childrenRect.height
          anchors { left: parent.left; right: parent.right; top: fieldLabel.bottom }

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

            height: childrenRect.height
            anchors { left: parent.left; right: parent.right }

            enabled: form.state !== "ReadOnly"
            property var value: pAttributeValue
            property var config: pEditorWidgetConfig
            property var widget: pEditorWidget
            property var field: pField

            source: 'editorwidgets/' + widget + '.qml'

            onStatusChanged: {
              if ( attributeEditorLoader.status === Loader.Error )
              {
                console.warn( "Editor widget type '" + pEditorWidget + "' not avaliable." )
                widget = 'TextEdit'
              }
            }
          }

          Connections {
            target: attributeEditorLoader.item
            onValueChanged: form.model.setAttribute( index, value, isNull )
          }
        }

        Controls.CheckBox {
          checkedState: pRememberValue

          visible: form.state === "Add" && pEditorWidget !== "Hidden"

          anchors.right: parent.right

          onCheckedChanged: {
            var idx = form.model.index(index, 0)
            form.model.setData(idx, checkedState, FeatureModel.RememberValue)
          }
        }
      */
      }
    }
  }














/*
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
*/

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
