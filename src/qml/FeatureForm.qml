import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as Controls
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import QtQml.Models 2.11
import QtQml 2.3

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import QtQuick.Controls.Styles 1.4
import "."

Page {
  signal saved
  signal cancelled
  signal aboutToSave

  property AttributeFormModel model
  property alias toolbarVisible: toolbar.visible
  //! if embedded form called by RelationEditor or RelationReferenceWidget
  property bool embedded: false

  function reset() {
    master.reset()
  }

  id: form

  states: [
    State {
      name: 'ReadOnly'
    },
    State {
      name: 'Edit'
    },
    State {
      name: 'Add'
    }
  ]

  /**
   * a substate used under 'Add' (not yet under 'Edit' but possibly in future)
   * in case that the feature needs to be stored "meanwhile"
   * e.g. on relation editor widget when adding childs to a not yet stored parent
   */
  property bool buffered: false

  /**
   * This is a relay to forward private signals to internal components.
   */
  QtObject {
    id: master

    /**
     * This signal is emitted whenever the state of Flickables and TabBars should
     * be restored.
     */
    signal reset
  }

  Item {
    id: container

    clip: true

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
        visible: model.hasTabs
        height: 48 * dp

        Connections {
          target: master
          onReset: tabRow.currentIndex = 0
        }

        Connections {
          target: swipeView
          onCurrentIndexChanged: tabRow.currentIndex = swipeView.currentIndex
        }

        Repeater {
          model: form.model && form.model.hasTabs ? form.model : 0

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
              // color: tabButton.down ? '#17a81a' : '#21be2b'
              color: !tabButton.enabled ? '999999' : tabButton.down ||
                                        tabButton.checked ? '#1B5E20' : '#4CAF50'
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
            anchors.fill: formPage
            color: "white"
          }

          /**
           * The main form content area
           */
          ListView {
            id: content
            anchors.fill: parent
            clip: true
            section.property: 'Group'
            section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
            section.delegate: Component {
              // section header: group box name
              Rectangle {
                width: parent.width
                height: section === "" ? 0 : 30 * dp
                color: 'lightGray'

                Text {
                  anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
                  width: parent.width
                  font.bold: true
                  text: section
                  wrapMode: Text.WordWrap
                }
              }
            }

            Connections {
              target: master
              onReset: content.contentY = 0
            }

            model: SubModel {
              id: contentModel
              model: form.model
              rootIndex: form.model && form.model.hasTabs ? form.model.index(currentIndex, 0) : undefined
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
      visible: Type === 'field' || Type === 'relation'
      height: childrenRect.height

      anchors {
        left: parent.left
        right: parent.right
        leftMargin: 12 * dp
      }

      Controls.Label {
        id: fieldLabel
        width: parent.width
        text: Name || ''
        wrapMode: Text.WordWrap
        font.bold: true
        color: ConstraintValid ? form.state === 'ReadOnly' || embedded && EditorWidget === 'RelationEditor' ? 'grey' : 'black' : '#c0392b'
      }

      Controls.Label {
        id: constraintDescriptionLabel
        anchors {
          left: parent.left
          right: parent.right
          top: fieldLabel.bottom
        }

        text: ConstraintDescription || ''
        height: ConstraintValid ? 0 : undefined
        visible: !ConstraintValid

        color: "#e67e22"
      }

      Item {
        id: placeholder
        height: childrenRect.height
        anchors { left: parent.left; right: rememberCheckbox.left; top: constraintDescriptionLabel.bottom; rightMargin: 10 * dp  }

        Loader {
          id: attributeEditorLoader

          height: childrenRect.height
          anchors { left: parent.left; right: parent.right }

          //disable widget if the form is in ReadOnly mode, or if it's an RelationEditor widget in an embedded form
          enabled: (form.state !== 'ReadOnly' || EditorWidget === 'RelationEditor' || EditorWidget === 'ValueRelation' ) && !!AttributeEditable
          property bool readOnly: form.state === 'ReadOnly' || embedded && EditorWidget === 'RelationEditor'
          property var value: AttributeValue
          property var config: ( EditorWidgetConfig || {} )
          property var widget: EditorWidget
          property var field: Field
          property var relationId: RelationId
          property var nmRelationId: NmRelationId
          property var constraintValid: ConstraintValid
          property bool constraintsValid: form.model.constraintsValid
          property var currentFeature: form.model.featureModel.feature

          active: widget !== 'Hidden'
          source: 'editorwidgets/' + ( widget || 'TextEdit' ) + '.qml'

          onStatusChanged: {
            if ( attributeEditorLoader.status === Loader.Error )
            {
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

      CheckBox {
        id: rememberCheckbox
        checked: RememberValue ? true : false 
        visible: form.state === "Add" && EditorWidget !== "Hidden" && EditorWidget !== 'RelationEditor'
        width: visible ? undefined : 0

        anchors { right: parent.right; top: fieldLabel.bottom }

        onCheckedChanged: {
          RememberValue = checked
        }

        indicator.height: 16 * dp
        indicator.width: 16 * dp
        icon.height: 16 * dp
        icon.width: 16 * dp
      }
    }
  }

  function save() {
    //if this is for some reason not handled before (like when tiping on a map while editing)
    if( !model.constraintsValid ) {
        displayToast( qsTr( 'Constraints not valid - cancel editing') )
        cancel()
        return
    }

    parent.focus = true
    aboutToSave()

    if ( form.state === 'Add' ) {
      if( !buffered )
      {
        model.create()
      }
      else
      {
        model.save()
        buffered = false
      }
      state = 'Edit'
    }
    else
    {
      model.save()
    }
    saved()
  }

  function buffer() {
      if( !model.constraintsValid ) {
          displayToast( qsTr('Constraints not valid - cannot buffer') )
          return false
      }

      aboutToSave() //used the same way like on save

      if ( form.state === 'Add' ) {
        if( !buffered )
        {
          model.create()
          buffered = true
        }
        else
        {
          model.save()
        }
      }
      else{
        model.save()
      }

      return true
  }

  function cancel() {
    if( buffered )
      model.deleteFeature()
    buffered = false
    cancelled()
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

    background: Rectangle {
      //testwise have special color for buffered
      color: model.constraintsValid ?  Theme.mainColor : 'orange'
    }

    RowLayout {
      anchors.fill: parent
      Layout.margins: 0

      Button {
        id: saveButton

        Layout.alignment: Qt.AlignTop | Qt.AlignLeft

        visible: form.state === 'Add' || form.state === 'Edit'
        width: 48*dp
        height: 48*dp
        clip: true
        bgcolor: Theme.darkGray

        iconSource: Theme.getThemeIcon( 'ic_check_white_48dp' )

        onClicked: {
          if( model.constraintsValid ) {
            save()
          } else {
            displayToast( qsTr('Constraints not valid') )
          }
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
            qsTr( 'Add feature on %1' ).arg(layerName )
          else if ( form.state === 'Edit' )
            qsTr( 'Edit feature on %1' ).arg(layerName)
          else
            qsTr( 'View feature on %1' ).arg(layerName)
        }
        font: Theme.strongFont
        color: "#FFFFFF"
        elide: Label.ElideRight
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
        Layout.fillWidth: true
      }

      Button {
        id: closeButton

        Layout.alignment: Qt.AlignTop | Qt.AlignRight

        width: 49*dp
        height: 48*dp
        clip: true
        bgcolor: form.state === 'Add' ? "#900000" : Theme.darkGray

        iconSource: form.state === 'Add' ? Theme.getThemeIcon( 'ic_delete_forever_white_24dp' ) : Theme.getThemeIcon( 'ic_close_white_24dp' )

        onClicked: {
          Qt.inputMethod.hide()
          cancel()
        }
      }
    }
  }
}
