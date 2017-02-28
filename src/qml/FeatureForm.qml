import QtQuick 2.6
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4 as Controls
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

    anchors {
      top: toolbar.bottom
      bottom: parent.bottom
      left: parent.left
      right: parent.right
    }

    Flickable {
      id: flickable
      anchors {
        left: parent.left
        right: parent.right
      }
      height: tabRow.height

      flickableDirection: Flickable.HorizontalFlick
      contentWidth: tabRow.width

      // Tabs
      TabBar {

        id: tabRow
        currentIndex: swipeView.currentIndex
        visible: model.hasTabs
        height: 48 * dp

        Repeater {
          model: form.model

          TabButton {
            id: tabButton
            text: Name
            leftPadding: 8 * dp
            rightPadding: 8 * dp

            width: contentItem.width + leftPadding + rightPadding
            height: 48 * dp

            contentItem: Text {
              // Make sure the width is derived from the text so we can get wider
              // than the parent item and the Flickable is useful
              width: paintedWidth
              text: tabButton.text
              // color: tabButton.down ? "#17a81a" : "#21be2b"
              color: !tabButton.enabled ? "#999999" : tabButton.down ||
                                        tabButton.checked ? "#1B5E20" : "#4CAF50"
              font.weight: tabButton.checked ? Font.DemiBold : Font.Normal

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
      anchors {
        top: flickable.bottom
        left: parent.left
        right: parent.right
        bottom: parent.bottom
      }

      Repeater {
        // One page per tab in tabbed forms, 1 page in auto forms
        model: form.model.hasTabs ? form.model : 1

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
              rootIndex: form.model.hasTabs ? form.model.index(currentIndex, 0) : undefined
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

      anchors {
        left: parent.left
        right: parent.right
        leftMargin: 12 * dp
      }

      Controls.Label {
        id: fieldLabel

        text: Name || ''
        font.bold: true
        color: ConstraintValid ? "black" : "#c0392b"
      }

      Controls.Label {
        id: constraintDescriptionLabel
        anchors {
          left: parent.left
          right: parent.right
          top: fieldLabel.bottom
        }

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

          enabled: form.state !== "ReadOnly" && !!AttributeEditable
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
            AttributeValue = isNull ? undefined : value
          }
        }
      }

      Controls.CheckBox {
        id: rememberCheckbox
        checked: RememberValue ? true : false

        visible: form.state === "Add" && EditorWidget !== "Hidden"
        width: visible ? undefined : 0

        anchors { right: parent.right; top: fieldLabel.bottom }

        onCheckedChanged: {
          RememberValue = checked
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
  ToolBar {
    id: toolbar
    height: visible ? 48 * dp : 0
    visible: form.state === 'Add'
    anchors {
      top: parent.top
      left: parent.left
      right: parent.right
    }

    RowLayout {
      anchors.fill: parent
      Layout.margins: 0

      ToolButton {
        id: saveButton

        contentItem: Image {
          fillMode: Image.Pad
          horizontalAlignment: Image.AlignHCenter
          verticalAlignment: Image.AlignVCenter
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

        text:
        {
          var currentLayer = model.featureModel.currentLayer
          var layerName = 'N/A'
          if (currentLayer !== null)
            layerName = currentLayer.name

          if ( form.state === 'Add' )
            qsTr( 'Add feature on <i>%1</i>' ).arg(layerName )
          else if ( form.state === 'Edit' )
            qsTr( 'Edit feature on <i>%1</i>' ).arg(layerName)
          else
            qsTr( 'View feature on <i>%1</i>' ).arg(layerName)
        }
        font.bold: true
        font.pointSize: 16
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
