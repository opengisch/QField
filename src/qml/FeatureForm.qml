import QtQuick 2.6
import QtQuick.Controls 1.4 as Controls
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls.Private 1.0
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

  property AttributeFormModel model
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

        property var __currentTab

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

            padding: 8 * dp
            spacing: 8 * dp

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

                      label: Controls.Label {
                        font.bold: true
                        text: control.text
                      }
                    }
                  }

                  Rectangle {
                    color: tabBar.__currentTab === parent ? "orange" : "gray"

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
        onFeatureChanged: container.activate( tabBar.__currentTab )
      }

      /**
       * The main form content area
       */
      ListView {
        id: content
        anchors { top: tabBar.bottom; bottom: parent.bottom; left: parent.left; right: parent.right }
        clip: true
        section.property: "Group"
        section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
        section.delegate: Component {
          /* section header: group box name */
          Rectangle {
            width: parent.width
            height: section === "" ? 0 : 30 * dp
            color: "lightGray"

            Text {
              anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
              font.bold: true
              text: section
            }
          }
        }

        model: SubModel {
          id: contentModel
          model: form.model
        }

        delegate: fieldItem
      }

      Component.onCompleted: container.checkTabs()
    }

    function activate( tab ) {
      if ( !form.model.hasTabs )
      {
        tabBar.__currentTab = undefined
      }
      else
      {
        tabBar.__currentTab = tab

        contentModel.rootIndex = rootElement.modelIndex(tab.idx)
      }
    }

    function checkTabs()
    {
      if ( !form.model.hasTabs )
      {
        contentModel.rootIndex = undefined
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
      visible: Type === 'field'
      height: childrenRect.height

      anchors { left: parent.left; right: parent.right }

      Controls.Label {
        id: fieldLabel

        text: Name
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
          property var value: AttributeValue
          property var config: EditorWidgetConfig
          property var widget: EditorWidget
          property var field: Field

          active: widget !== 'Hidden'
          source: 'editorwidgets/' + widget + '.qml'

          onStatusChanged: {
            if ( attributeEditorLoader.status === Loader.Error )
            {
              console.warn( "Editor widget type '" + EditorWidget + "' not avaliable." )
              source = 'editorwidgets/TextEdit.qml'
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
            contentModel.setModelData( index, isNull ? undefined : value, AttributeFormModel.AttributeValue )
          }
        }
      }

      Controls.CheckBox {
        id: rememberCheckbox
        checkedState: RememberValue

        visible: form.state === "Add" && EditorWidget !== "Hidden"
        width: visible ? undefined : 0

        anchors { right: parent.right; top: fieldLabel.bottom }

        onCheckedChanged: {
          contentModel.setModelData( index, checkedState, AttributeFormModel.RememberValue )
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
      id: saveButton
      anchors.right: parent.right

      width: 48*dp
      height: 48*dp

      iconSource: Style.getThemeIcon( "ic_save_white_24dp" )

      onClicked: {
        save()
      }
    }

    Controls.Label {
      id: titleLabel
      anchors { right: saveButton.left; left: closeButton.right }
      height: closeButton.height
      text:
      {
        if ( form.state === 'Add' )
          qsTr( 'Add feature on <i>%1</i>' ).arg( model.featureModel.currentLayer.name )
        else if ( form.state === 'Edit' )
          qsTr( 'Edit feature on <i>%1</i>' ).arg( model.featureModel.currentLayer.name )
        else
          qsTr( 'View feature on <i>%1</i>' ).arg( model.featureModel.currentLayer.name )
      }
      font.bold: true
      horizontalAlignment: Text.AlignHCenter
    }

    Button {
      id: closeButton
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
