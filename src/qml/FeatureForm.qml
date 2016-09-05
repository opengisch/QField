import QtQuick 2.0
import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls.Styles 1.4
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
        height: model.hasTabs ? childrenRect.height : 0
        clip: true

        Flickable {
          flickableDirection: Flickable.HorizontalFlick

          anchors { left: parent.left; right: parent.right; top: parent.top }
          height: tabRow.height
          contentWidth: tabRow.width

          Row {
            id: tabRow
            height: childrenRect.height

            Repeater {
              model: DelegateModel {
                id: rootElement
                model: form.model

                delegate: Item {
                  height: childrenRect.height
                  width: childrenRect.width

                  Controls.Button {
                    id: button
                    text: Name

                    onClicked: {
                      activate( parent )
                    }

                    style: ButtonStyle {
                      background: Rectangle {
                        color: "white"
                      }
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
                      activate( this )
                  }

                  function activate( tab ) {
                    if ( !form.model.hasTabs )
                      return;
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
      }

      DropShadow {
        anchors.fill: tabBar
        horizontalOffset: 0
        verticalOffset: 2 * dp
        radius: 6.0 * dp
        samples: 17
        color: "#80000000"
        source: tabBar
      }

      Connections {
        target: tabBar
        onVisibleChanged: {
          if ( !tabBar.visible )
          {
            content.sourceComponent = undefined

            content.pRootIndex = undefined
            content.pType = 'tab'

            content.sourceComponent = element
          }
        }
      }

      Flickable {
        anchors { top: tabBar.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        clip: true

        flickableDirection: Flickable.VerticalFlick
        contentHeight: content.height

        Loader {
          id: content

          anchors { left: parent.left; right: parent.right }
          height: childrenRect.height

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
                id: content

                property var pRootIndex
                property var pType
                property var pName
                property var pEditorWidget
                property var pEditorWidgetConfig
                property var pField
                property var pAttributeValue
                property var pRememberValue

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

        Item {
          id: placeholder
          height: childrenRect.height
          anchors { left: parent.left; right: rememberCheckbox.left; top: fieldLabel.bottom }

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

            active: widget !== 'Hidden'
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
            onValueChanged: {

              // QML translates undefined to a NULL QVariant
              model.setData( pRootIndex, isNull ? undefined : value, AttributeFormModel.AttributeValue )
            }
          }
        }

        Controls.CheckBox {
          id: rememberCheckbox
          checkedState: pRememberValue

          visible: form.state === "Add" && pEditorWidget !== "Hidden"
          width: visible ? undefined : 0

          anchors { right: parent.right; top: fieldLabel.bottom }

          onCheckedChanged: {
            var idx = form.model.index(index, 0)
            form.model.setData(idx, checkedState, FeatureModel.RememberValue)
          }
        }
      }
    }
  }

  /** The title toolbar **/

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
