import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQml.Models 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0

import "../.."
import ".."

EditorWidgetBase {
  id: relationEditor

  property int itemHeight: 40

  // because no additional addEntry item on readOnly (isEnabled false)
  height: listView.contentHeight
            + (isEnabled ? addEntry.height : 0)
  enabled: true

  Rectangle {
    anchors.fill: parent
    color: "transparent"
    border.color: 'lightgray'
    border.width: 1
  }

  OrderedRelationModel {
    //containing the current (parent) feature, the relation to the children
    //and the relation from the children to the other parent (if it's nm and cardinality is set)
    id: orderedRelationModel
    currentRelationId: relationId
    feature: currentFeature
    orderingField: relationEditorWidgetConfig['ordering_field']
    imagePath: relationEditorWidgetConfig['image_path']
    description: relationEditorWidgetConfig['description']

    property int featureFocus: -1

    onFailedReorder: {
      displayToast( "Failed to reorder features.", "error" )
    }
  }

  DelegateModel {
    id: visualModel

    model: orderedRelationModel
    delegate: dragDelegate
  }

  ListView {
    id: listView
    model: visualModel
    width: parent.width
    height: contentHeight
    focus: true
    clip: true
    boundsMovement: Flickable.StopAtBounds
    highlightRangeMode: ListView.ApplyRange
  }


  //the add entry "last row"
  Item {
    id: addEntry
    anchors.top: listView.bottom
    height: itemHeight
    width: parent.width

    focus: true

    Rectangle{
      anchors.fill: parent
      color: 'lightgrey'
      visible: isEnabled

      Text {
        visible: isEnabled
        color: 'grey'
        text: isEnabled && !constraintsHardValid ? qsTr( 'Ensure contraints') : ''
        anchors { leftMargin: 10; left: parent.left; right: addButtonRow.left; verticalCenter: parent.verticalCenter }
        font.bold: true
        font.italic: true
        font.pointSize: Theme.tipFont.pointSize
      }

      Row {
        id: addButtonRow
        anchors { top: parent.top; right: parent.right; rightMargin: 10 }
        height: parent.height

        ToolButton {
            id: addButton
            width: parent.height
            height: parent.height
            enabled: constraintsHardValid

            contentItem: Rectangle {
                anchors.fill: parent
                color: parent.enabled ? nmRelationId ? 'blue' : 'black' : 'grey'
                Image {
                  anchors.fill: parent
                  anchors.margins: 8
                  fillMode: Image.PreserveAspectFit
                  horizontalAlignment: Image.AlignHCenter
                  verticalAlignment: Image.AlignVCenter
                  source: Theme.getThemeIcon( 'ic_add_white_24dp' )
                }
            }
        }
      }

      MouseArea {
        anchors.fill: parent
        onClicked: {
          if( save() ) {
            //this has to be checked after buffering because the primary could be a value that has been created on creating featurer (e.g. fid)
            if( orderedRelationModel.parentPrimariesAvailable ) {
              displayToast( qsTr( 'Adding child feature in layer %1' ).arg( orderedRelationModel.relation.referencingLayer.name ) )
              if ( orderedRelationModel.relation.referencingLayer.geometryType() !== QgsWkbTypes.NullGeometry ) {
                requestGeometry( relationEditor, orderedRelationModel.relation.referencingLayer );
                return;
              }
              showAddFeaturePopup()
            } else {
              displayToast (qsTr( 'Cannot add child feature: parent primary keys are not available' ), 'warning' )
            }
          }
        }
      }
    }
  }


  Component {
    id: dragDelegate

    MouseArea {
      id: dragArea
      pressAndHoldInterval: 130

      property bool held: false

      property int indexFrom: -1
      property int indexTo: -1

      anchors.left: parent ? parent.left : undefined
      anchors.right: parent ? parent.right : undefined
      height: content.height

      drag.target: held ? content : undefined
      drag.axis: Drag.YAxis

      onPressAndHold: {
        if (isEnabled) {
          held = true
        }
      }
      onReleased: {
        if (held === true) {
          held = false
          orderedRelationModel.moveItems(indexFrom, indexTo)
        } else if (listView.currentIndex !== dragArea.DelegateModel.itemsIndex) {
          listView.currentIndex = dragArea.DelegateModel.itemsIndex
          orderedRelationModel.onViewCurrentFeatureChanged(listView.currentIndex)
        }
      }

      onClicked: {
        embeddedPopup.state = isEnabled ? 'Edit' : 'ReadOnly'
        embeddedPopup.currentLayer = orderedRelationModel.relation.referencingLayer
        embeddedPopup.linkedRelation = orderedRelationModel.relation
        embeddedPopup.linkedRelationOrderingField = orderedRelationModel.orderingField
        embeddedPopup.linkedParentFeature = orderedRelationModel.feature
        embeddedPopup.feature = model.referencingFeature
        embeddedPopup.open()
      }

      Rectangle {
        id: content
        anchors {
          horizontalCenter: parent.horizontalCenter
          verticalCenter: parent.verticalCenter
        }
        width: dragArea.width
        height: row.implicitHeight + 4

        color: dragArea.held ? Theme.lightGray : "transparent"
        Behavior on color { ColorAnimation { duration: 100 } }

        radius: 2
        Drag.active: dragArea.held
        Drag.source: dragArea
        Drag.hotSpot.x: width / 2
        Drag.hotSpot.y: height / 2

        states: State {
          when: dragArea.held

          AnchorChanges {
            target: content
            anchors.horizontalCenter: undefined
            anchors.verticalCenter: undefined
          }
        }

        Row {
          id: row
          anchors.fill: parent
          anchors.margins: 2

          height: Math.max( itemHeight, featureText.height )

          Image {
            id: featureImage
            source: ImagePath
                      ? ('file://' + ImagePath)
                      : Theme.getThemeIcon("ic_photo_notavailable_black_24dp")
            width: parent.height
            height: parent.height
            fillMode: Image.PreserveAspectFit
            visible: !!ImagePath
          }

          Text {
            id: featureText
            font: Theme.defaultFont
            color: !isEnabled ? 'grey' : 'black'
            text: Description || model.displayString
            verticalAlignment: Text.AlignVCenter
            padding: 4
            elide: Text.ElideRight
            width: parent.width
                     - 8
                     - (featureImage.visible ? featureImage.width : 0)
                     - (moveDownButton.visible ? moveDownButton.width : 0)
                     - (moveUpButton.visible ? moveUpButton.width : 0)
                     - (deleteButton.visible ? deleteButton.width : 0)
          }

          ToolButton {
            id: moveDownButton
            width: itemHeight
            height: itemHeight
            visible: isEnabled
            opacity: (index === listView.count - 1) ? 0.3 : 1

            contentItem: Rectangle {
              anchors.fill: parent
              color: 'transparent'
              Image {
                anchors.fill: parent
                anchors.margins: 8
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter
                source: Theme.getThemeVectorIcon( 'ic_chevron_down' )
              }
            }

            onClicked: {
              if (index === listView.count - 1) {
                return
              }

              orderedRelationModel.moveItems(
                index,
                index + 1
              )
            }
          }

          ToolButton {
            id: moveUpButton
            width: itemHeight
            height: itemHeight
            visible: isEnabled
            opacity: (index === 0) ? 0.3 : 1

            contentItem: Rectangle {
              anchors.fill: parent
              color: 'transparent'
              Image {
                anchors.fill: parent
                anchors.margins: 8
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter
                source: Theme.getThemeVectorIcon( 'ic_chevron_up' )
              }
            }

            onClicked: {
              if (index === 0) {
                return
              }

              orderedRelationModel.moveItems(
                index,
                index - 1
              )
            }
          }

          ToolButton {
            id: deleteButton
            width: itemHeight
            height: itemHeight
            visible: isEnabled

            contentItem: Rectangle {
              anchors.fill: parent
              color: nmRelationId ? 'blue' : Theme.errorColor
              Image {
                anchors.fill: parent
                anchors.margins: 8
                fillMode: Image.PreserveAspectFit
                horizontalAlignment: Image.AlignHCenter
                verticalAlignment: Image.AlignVCenter
                source: Theme.getThemeIcon( 'ic_delete_forever_white_24dp' )
              }
            }

            onClicked: {
              deleteDialog.referencingFeatureId = model.referencingFeature.id
              deleteDialog.referencingFeatureDisplayMessage = model.displayString
              deleteDialog.nmReferencedFeatureId = nmRelationId ? model.model.nmReferencedFeature.id : ''
              deleteDialog.nmReferencedFeatureDisplayMessage = nmRelationId ? model.nmDisplayString : ''
              deleteDialog.visible = true
            }
          }
        }

        Rectangle {
          id: bottomLine
          anchors.bottom: parent.bottom
          height: 1
          color: 'lightGray'
          width: parent.width
        }
      }

      DropArea {
        anchors.fill: parent
        anchors.margins: 10

        onEntered: {
          if (dragArea.indexFrom === -1) {
            dragArea.indexFrom = drag.source.DelegateModel.itemsIndex
          }

          dragArea.indexTo = dragArea.DelegateModel.itemsIndex
          visualModel.items.move(
            drag.source.DelegateModel.itemsIndex,
            dragArea.DelegateModel.itemsIndex
          )
        }
      }
    }
  }

  Dialog {
    id: deleteDialog
    parent: mainWindow.contentItem

    property int referencingFeatureId
    property string referencingFeatureDisplayMessage
    property string referencingLayerName: relationEditorModel.relation.referencingLayer ? relationEditorModel.relation.referencingLayer.name : ''
    property int nmReferencedFeatureId
    property string nmReferencedFeatureDisplayMessage
    property string nmReferencedLayerName: relationEditorModel.nmRelation.referencedLayer ? relationEditorModel.nmRelation.referencedLayer.name : ''
    property string nmReferencingLayerName

    visible: false
    modal: true
    z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature forms
    font: Theme.defaultFont

    title: nmRelationId
            ? qsTr( 'Unlink feature %1 (%2) of %3' )
              .arg( nmReferencedFeatureDisplayMessage )
              .arg( nmReferencedFeatureId )
              .arg( nmReferencedLayerName )
            : qsTr( 'Delete feature %1 (%2) on %3' )
              .arg( referencingFeatureDisplayMessage )
              .arg( referencingFeatureId )
              .arg( referencingLayerName )
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text:  nmRelationId
              ? qsTr( 'Should the feature <b>%1 (%2)</b> of layer <b>%3</b> be unlinked?<br><i>(The connection will be deleted on layer <b>%4</b>)</i>')
                .arg( nmReferencedFeatureDisplayMessage )
                .arg( deleteDialog.nmReferencedFeatureId )
                .arg( deleteDialog.nmReferencedLayerName )
                .arg( deleteDialog.referencingLayerName )
              : qsTr( 'Should the feature <b>%1 (%2)</b> on layer <b>%3</b> be deleted?')
                .arg( deleteDialog.referencingFeatureDisplayMessage )
                .arg( deleteDialog.referencingFeatureId )
                .arg( deleteDialog.referencingLayerName )
    }

    standardButtons: Dialog.Ok | Dialog.Cancel
    onAccepted: {
      if ( ! orderedRelationModel.deleteFeature( referencingFeatureId ) ) {
        displayToast( qsTr( "Failed to delete referencing feature" ), 'error' )
      }

      visible = false
    }
    onRejected: {
      visible = false
    }

  }

  BusyIndicator {
    id: busyIndicator
    anchors.centerIn: parent
    width: 36
    height: 36
    running: orderedRelationModel.isLoading
  }

  EmbeddedFeatureForm {
    id: embeddedPopup

    embeddedLevel: form.embeddedLevel + 1
    digitizingToolbar: form.digitizingToolbar
    barcodeReader: form.barcodeReader

    onFeatureCancelled: {
      if( autoSave ) {
        orderedRelationModel.reload()
      }
    }

    onFeatureSaved: {
      orderedRelationModel.featureFocus = id
      orderedRelationModel.reload()
    }
  }

  function requestedGeometryReceived(geometry) {
    showAddFeaturePopup(geometry)
  }

  function showAddFeaturePopup(geometry) {
    embeddedPopup.state = 'Add'
    embeddedPopup.currentLayer = orderedRelationModel.relation.referencingLayer
    embeddedPopup.linkedParentFeature = orderedRelationModel.feature
    embeddedPopup.linkedRelation = orderedRelationModel.relation
    embeddedPopup.linkedRelationOrderingField = orderedRelationModel.orderingField
    if ( geometry !== undefined ) {
      embeddedPopup.applyGeometry(geometry)
    }
    embeddedPopup.open()
  }
}
