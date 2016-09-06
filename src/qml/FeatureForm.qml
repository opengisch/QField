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

                  property int idx

                  Controls.Button {
                    id: button
                    text: Name

                    onClicked: {
                      container.activate( parent )
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
                    idx = index
                    if ( index === 0 )
                      container.activate( this )
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
        target: form.model
        onHasTabsChanged: container.checkTabs()
      }

      Connections {
        target: form.model
        onFeatureChanged: container.activate( __currentTab )
      }

      /**
       * The tab bar
       */
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

      Component.onCompleted: container.checkTabs()
    }

    function activate( tab ) {
      if ( !form.model.hasTabs )
        return;
      __currentTab = tab

      if ( !container.visible )
        content.sourceComponent = undefined

      content.pRootIndex = rootElement.modelIndex(tab.idx)
      content.pType = 'tab'
      content.pName = ''
    }

    function checkTabs()
    {
      if ( !form.model.hasTabs )
      {
        if ( !container.visible )
          content.sourceComponent = undefined

        content.pRootIndex = undefined
        content.pType = 'tab'
      }
    }

    Connections {
      target: container
      onVisibleChanged: {
        if ( container.visible )
          content.sourceComponent = element
      }
    }
  }

  /**
   * An item inside the form.
   * Can be either a group box or a field editor.
   * There is also a container groupbox without title constructed for tabs.
   */
  Component {
    id: element

    Loader {
      sourceComponent: pType === 'field' ? ( pEditorWidget === 'Hidden' ? undefined : fieldItem ): groupBoxItem

      // anchors { left: parent.left; right: parent.right }
      height: childrenRect.height

      property var xRootIndex: pRootIndex
      property var type: pType
      property var name: pName
      property var editorWidget: pEditorWidget
      property var editorWidgetConfig: pEditorWidgetConfig
      property var xField: pField
      property var attributeValue: pAttributeValue
      property var rememberValue: pRememberValue
    }
  }

  /**
   * A group box, only visible if the item type is tab or groupbox ( != field )
   */
  Component {
    id: groupBoxItem

    Controls.GroupBox {
      title: name
      height: fieldsColumn.height
      // anchors { left: parent.left; right: parent.right }

      Column {
        id: fieldsColumn
        anchors { left: parent.left; right: parent.right }
        height: childrenRect.height

        Repeater {
          model: DelegateModel {
            id: delegateModel

            model: form.model
            rootIndex: xRootIndex

            delegate: Loader {
              height: childrenRect.height

              property var pRootIndex: delegateModel.modelIndex( index )
              property string pType: Type
              property string pName: Name
              property var pEditorWidget: EditorWidget
              property var pEditorWidgetConfig: EditorWidgetConfig
              property var pField: Field
              property var pAttributeValue: AttributeValue
              property var pRememberValue: RememberValue

              sourceComponent: element

              anchors { left: parent.left; right: parent.right }
            }
          }
        }
      }
    }
  }

  /**
   * A field editor
   */
  Component {
    id: fieldItem

    Item {
      id: fieldContainer
      visible: type === 'field'
      height: childrenRect.height

      Controls.Label {
        id: fieldLabel

        text: name
        font.bold: true
      }

      Item {
        id: placeholder
        height: childrenRect.height
        anchors { left: parent.left; right: rememberCheckbox.left; top: fieldLabel.bottom }

        Loader {
          id: attributeEditorLoader

          height: childrenRect.height
          anchors { left: parent.left; right: parent.right }

          enabled: form.state !== "ReadOnly"
          property var value: attributeValue
          property var config: editorWidgetConfig
          property var widget: editorWidget
          property var field: xField

          active: widget !== 'Hidden'
          source: 'editorwidgets/' + widget + '.qml'

          onStatusChanged: {
            if ( attributeEditorLoader.status === Loader.Error )
            {
              console.warn( "Editor widget type '" + editorWidget + "' not avaliable." )
              widget = 'TextEdit'
            }
          }
        }

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

        Connections {
          target: attributeEditorLoader.item
          onValueChanged: {

            // QML translates undefined to a NULL QVariant
            model.setData( xRootIndex, isNull ? undefined : value, AttributeFormModel.AttributeValue )
          }
        }
      }

      Controls.CheckBox {
        id: rememberCheckbox
        checkedState: rememberValue

        visible: form.state === "Add" && editorWidget !== "Hidden"
        width: visible ? undefined : 0

        anchors { right: parent.right; top: fieldLabel.bottom }

        onCheckedChanged: {
          model.setData( xRootIndex, checkedState, AttributeFormModel.RememberValue )
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
