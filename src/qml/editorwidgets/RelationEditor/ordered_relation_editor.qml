import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQml.Models 2.15
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0

import "../.."
import ".."

EditorWidgetBase {
    id: relationEditor

    property int itemHeight: 40

    // because no additional addEntry item on readOnly (isEnabled false)
    height: isEnabled
            ? listView.height + itemHeight
            : Math.max( listView.height, itemHeight)
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
    }

    DelegateModel {
      id: visualModel

      model: orderedRelationModel
      delegate: dragDelegate
    }

    //the list
    ListView {
        id: listView
        model: visualModel
        width: parent.width
        height: listView.count * itemHeight
        focus: true
        clip: true
        boundsMovement: Flickable.StopAtBounds
        highlightRangeMode: ListView.StrictlyEnforceRange
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

          Row
          {
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
                    if ( orderedRelationModel.relation.referencingLayer.geometryType() !== QgsWkbTypes.NullGeometry )
                    {
                        requestGeometry( relationEditor, orderedRelationModel.relation.referencingLayer );
                        return;
                    }
                    showAddFeaturePopup()
                }
                else
                {
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

          color: dragArea.held
                   ? "coral"
                   : "transparent"
          Behavior on color { ColorAnimation { duration: 100 } }

          radius: 2
          Drag.active: dragArea.held
          Drag.source: dragArea
          Drag.hotSpot.x: width / 2
          Drag.hotSpot.y: height / 2

          states: State {
            when: dragArea.held

            // ParentChange { target: content; parent: root }
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
              id: handleImage
              source: Theme.getThemeVectorIcon("ic_drag_handle_48dp")
              width: parent.height
              height: parent.height
              opacity: !!isEnabled ? 1 : 0.5
            }

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
              height: 50
              padding: 4
              width: parent.width
                       - handleImage.width
                       - (featureImage.visible ? featureImage.width : 0)
                       - (deleteButton.visible ? deleteButton.width : 0)
            }

            ToolButton {
              id: deleteButton
              width: parent.height
              height: parent.height
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


//    //list components
//    Component {
//        id: referencingFeatureDelegate

//        Item {
//          id: listitem
//          anchors.left: parent ? parent.left : undefined
//          anchors.right: parent ? parent.right : undefined

//          focus: true

//          height: Math.max( itemHeight, featureText.height )

//          Text {
//            id: featureText
//            anchors { leftMargin: 10; left: parent.left; right: deleteButtonRow.left; verticalCenter: parent.verticalCenter }
//            font: Theme.defaultFont
//            color: !isEnabled ? 'grey' : 'black'
//            text: Description
//          }

//          MouseArea {
//            anchors.fill: parent

//            onClicked: {
//                embeddedPopup.state = isEnabled ? 'Edit' : 'ReadOnly'
//                embeddedPopup.currentLayer = nmRelationId ?  orderedRelationModel.nmRelation.referencedLayer : orderedRelationModel.relation.referencingLayer
//                embeddedPopup.linkedRelation = orderedRelationModel.relation
//                embeddedPopup.linkedParentFeature = orderedRelationModel.feature
//                embeddedPopup.feature = nmRelationId ? model.nmReferencedFeature : model.referencingFeature
//                embeddedPopup.open()
//            }
//          }

//          Row {
//            id: deleteButtonRow
//            anchors { top: parent.top; right: parent.right; rightMargin: 10 }
//            height: listitem.height

//            ToolButton {
//                id: deleteButton
//                width: parent.height
//                height: parent.height
//                visible: isEnabled

//                contentItem: Rectangle {
//                    anchors.fill: parent
//                    color: nmRelationId ? 'blue' : Theme.errorColor
//                    Image {
//                      anchors.fill: parent
//                      anchors.margins: 8
//                      fillMode: Image.PreserveAspectFit
//                      horizontalAlignment: Image.AlignHCenter
//                      verticalAlignment: Image.AlignVCenter
//                      source: Theme.getThemeIcon( 'ic_delete_forever_white_24dp' )
//                    }
//                }

//                onClicked: {
//                    deleteDialog.referencingFeatureId = model.referencingFeature.id
//                    deleteDialog.referencingFeatureDisplayMessage = model.displayString
//                    deleteDialog.nmReferencedFeatureId = nmRelationId ? model.model.nmReferencedFeature.id : ''
//                    deleteDialog.nmReferencedFeatureDisplayMessage = nmRelationId ? model.nmDisplayString : ''
//                    deleteDialog.visible = true
//                }
//            }
//          }

//          //bottom line
//          Rectangle {
//            id: bottomLine
//            anchors.bottom: parent.bottom
//            height: 1
//            color: 'lightGray'
//            width: parent.width
//          }
//        }
//    }

    //the delete entry stuff
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
        if ( ! referencingFeatureListView.model.deleteFeature( referencingFeatureId ) ) {
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

        onFeatureCancelled: {
            if( autoSave )
                orderedRelationModel.reload()
        }

        onFeatureSaved: {
            orderedRelationModel.featureFocus = id
            orderedRelationModel.reload()
        }
    }

    function requestedGeometry(geometry) {
        showAddFeaturePopup(geometry)
    }

    function showAddFeaturePopup(geometry) {
        embeddedPopup.state = 'Add'
        embeddedPopup.currentLayer = orderedRelationModel.relation.referencingLayer
        embeddedPopup.linkedParentFeature = orderedRelationModel.feature
        embeddedPopup.linkedRelation = orderedRelationModel.relation
        if ( geometry !== undefined )
        {
            embeddedPopup.applyGeometry(geometry)
        }
        embeddedPopup.open()
    }
}
