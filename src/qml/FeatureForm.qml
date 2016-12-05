import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtQml.Models 2.2
import QtQml 2.2

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


    Flickable {
      id: flickable
      anchors { left: parent.left; right: parent.right }
      height: tabRow.height

      flickableDirection: Flickable.HorizontalFlick
      contentWidth: tabRow.width

      // Tabs
      TabBar {
        id: tabRow
        currentIndex: swipeView.currentIndex
        // visible: model.hasTabs

        Repeater {
          model: form.model

          TabButton {
            id: tabButton
            text: Name

            width: contentItem.width + leftPadding + rightPadding

            contentItem: Text {
              width: paintedWidth
              text: tabButton.text
              font: tabButton.font
              color: tabButton.down ? "#17a81a" : "#21be2b"
              horizontalAlignment: Text.AlignHCenter
              verticalAlignment: Text.AlignVCenter
            }
          }
        }
      }
    }

    SwipeView {
      id: swipeView
      currentIndex: tabRow.currentIndex
      anchors { top: flickable.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }

      Repeater {
        model: form.model

        Label {
          text: Name
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
      visible: Type === 'field'
      height: childrenRect.height

      anchors { left: parent.left; right: parent.right }

      Label {
        id: fieldLabel

        text: Name
        font.bold: true
        color: ConstraintValid ? "black" : "#c0392b"
      }

      Label {
        id: constraintDescriptionLabel
        anchors { left: parent.left; right: parent.right; top: fieldLabel.bottom }

        text: ConstraintDescription
        height: ConstraintValid ? 0 : undefined
        visible: !ConstraintValid

        color: "#e67e22"
      }

      Item {
        id: placeholder
        height: childrenRect.height
        anchors { left: parent.left; right: rememberCheckbox.left; top: constraintDescriptionLabel.bottom }

        Loader {
          id: attributeEditorLoader

          height: childrenRect.height
          anchors { left: parent.left; right: parent.right }

          enabled: form.state !== "ReadOnly" && AttributeEditable
          property var value: AttributeValue
          property var config: EditorWidgetConfig
          property var widget: EditorWidget
          property var field: Field
          property var constraintValid: ConstraintValid

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

      CheckBox {
        id: rememberCheckbox
        checkState: RememberValue

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

      contentItem: Image {
        source: Style.getThemeIcon( "ic_save_white_24dp" )
      }

      background: Rectangle {
        color: model.constraintsValid ? "#212121" : "#bdc3c7"
      }

      enabled: model.constraintsValid

      onClicked: {
        save()
      }
    }

    Label {
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

      contentItem: Image {
        source: Style.getThemeIcon( "ic_close_white_24dp" )
      }

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
