import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQml.Models 2.2
import QtQml 2.2

import org.qgis 1.0
import org.qfield 1.0
import "js/style.js" as Style

Page {
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
    id: container

    anchors.fill: parent

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

        Item {
          id: formPage
          property int currentIndex: index

          Rectangle {
            anchors.fill: swipeView
            color: "white"
          }

          /**
           * The main form content area
           */
          ListView {
            id: content
            anchors.fill: parent
            clip: true
            section.property: "Group"
            section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
            section.delegate: Component {
              // section header: group box name
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
              rootIndex: form.model.index(currentIndex, 0)
            }

            delegate: fieldItem
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

  /** The title toolbar **/
  header: ToolBar {
    id: toolbar
    RowLayout {
      spacing: 20
      anchors.fill: parent

      ToolButton {
        id: saveButton

        contentItem: Image {
          fillMode: Image.Pad
          horizontalAlignment: Image.AlignHCenter
          verticalAlignment: Image.AlignVCenter
          source: Style.getThemeIcon( "ic_save_white_24dp" )
        }

        background: model.constraintsValid ? "#212121" : "#bdc3c7"

        enabled: model.constraintsValid

        onClicked: {
          save()
        }
      }

      Label {
        id: titleLabel

        text:
        {
          var currentLayer = model.featureModel.currentLayer
          if (!currentLayer)
            ''
          if ( form.state === 'Add' )
            qsTr( 'Add feature on <i>%1</i>' ).arg( currentLayer.name )
          else if ( form.state === 'Edit' )
            qsTr( 'Edit feature on <i>%1</i>' ).arg(currentLayer.name )
          else
            qsTr( 'View feature on <i>%1</i>' ).arg( currentLayer.name )
        }
        font.bold: true
        font.pixelSize: 20
        elide: Label.ElideRight
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        Layout.fillWidth: true
      }

      ToolButton {
        id: closeButton
        anchors.right: parent.right

        contentItem: Image {
          fillMode: Image.Pad
          horizontalAlignment: Image.AlignHCenter
          verticalAlignment: Image.AlignVCenter
          source: Style.getThemeIcon( "ic_close_white_24dp" )
        }

        onClicked: {
          Qt.inputMethod.hide()

          cancelled()
        }
      }
    }
  }
}
