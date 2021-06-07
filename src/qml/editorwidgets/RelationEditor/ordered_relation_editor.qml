import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0

import ".."
import "."

EditorWidgetBase {
    id: relationEditor

    property int itemHeight: 40

    // because no additional addEntry item on readOnly (isEnabled false)
    height: isEnabled
            ? referencingFeatureListView.height + itemHeight
            : Math.max( referencingFeatureListView.height, itemHeight)
    enabled: true

    Rectangle {
        anchors.fill: parent
        color: "transparent"
        border.color: 'darkgray'
        border.width: 1
    }

    OrderedRelationModel {
        //containing the current (parent) feature, the relation to the children
        //and the relation from the children to the other parent (if it's nm and cardinality is set)
        id: orderedRelationModel
        currentRelationId: relationId
        feature: currentFeature
        orderingField: Config['ordering_field']
        imagePath: Config['image_path']
        description: Config['description']

        property int featureFocus: -1
        onModelUpdated: {
          if (featureFocus > -1) {
            referencingFeatureListView.currentIndex = orderedRelationModel.getFeatureIdRow(featureFocus)
            featureFocus = -1
          }
        }
    }

    DelegateModel {
      id: visualModel

      model: orderedRelationModel
      delegate: dragDelegate
    }

    //the list
    ListView {
        id: referencingFeatureListView
        model: visualModel
        width: parent.width
        height: Math.min(
                  4 * itemHeight,
                  referencingFeatureListView.count * itemHeight
                ) + (
                  referencingFeatureListView.count > 4
                    ? itemHeight / 2
                    : 0
                )
        focus: true
        clip: true
        highlightRangeMode: ListView.StrictlyEnforceRange

        ScrollBar.vertical: ScrollBar {
            width: 10
            policy: ScrollBar.AlwaysOn

            contentItem: Rectangle {
                implicitWidth: 10
                implicitHeight: itemHeight
                color: Theme.mainColor
            }
        }
    }

    //the add entry "last row"
    Item {
      id: addEntry
      anchors.top: referencingFeatureListView.bottom
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

            anchors { left: parent.left; right: parent.right }
            height: content.height

            drag.target: held ? content : undefined
            drag.axis: Drag.YAxis

            onPressAndHold: {
                if (orderedModel.layerEditingEnabled)
                {
                    held = true
                }
            }
            onReleased: {
               if (held === true)
               {
                   held = false
                   orderedModel.moveitems(indexFrom, indexTo)
               }
               else if (listView.currentIndex !== dragArea.DelegateModel.itemsIndex)
               {
                   listView.currentIndex = dragArea.DelegateModel.itemsIndex
                   orderedModel.onViewCurrentFeatureChanged(listView.currentIndex)
               }
            }

            Rectangle {
                id: content
                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                width: dragArea.width; height: row.implicitHeight + 4

                border.width: 1
                border.color: "lightsteelblue"

                color: dragArea.held ? "coral" : listView.currentIndex === dragArea.DelegateModel.itemsIndex ? "lightsteelblue" : "white"
                Behavior on color { ColorAnimation { duration: 100 } }

                radius: 2
                Drag.active: dragArea.held
                Drag.source: dragArea
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2

                states: State {
                    when: dragArea.held

                    ParentChange { target: content; parent: root }
                    AnchorChanges {
                        target: content
                        anchors { horizontalCenter: undefined; verticalCenter: undefined }
                    }
                }

                Row {
                    id: row
                    anchors { fill: parent; margins: 2 }
                    Image { source: ImagePath; width: 50; fillMode: Image.PreserveAspectFit; visible: ImagePath !== "" }
                    Text { text: Description; height: 40; verticalAlignment: Text.AlignVCenter; padding: 4 }
                }
            }
            DropArea {
                anchors { fill: parent; margins: 10 }

                onEntered: {
                    if (dragArea.indexFrom === -1) {
                      dragArea.indexFrom = drag.source.DelegateModel.itemsIndex
                    }

                    dragArea.indexTo = dragArea.DelegateModel.itemsIndex
                    visualModel.items.move(
                            drag.source.DelegateModel.itemsIndex,
                            dragArea.DelegateModel.itemsIndex)
                }
            }
        }
    }


    //list components
    Component {
        id: referencingFeatureDelegate

        Item {
          id: listitem
          anchors.left: parent ? parent.left : undefined
          anchors.right: parent ? parent.right : undefined

          focus: true

          height: Math.max( itemHeight, featureText.height )

          Text {
            id: featureText
            anchors { leftMargin: 10; left: parent.left; right: deleteButtonRow.left; verticalCenter: parent.verticalCenter }
            font: Theme.defaultFont
            color: !isEnabled ? 'grey' : 'black'
            text: { text: nmRelationId ? model.nmDisplayString : model.displayString }
          }

          MouseArea {
            anchors.fill: parent

            onClicked: {
                embeddedPopup.state = isEnabled ? 'Edit' : 'ReadOnly'
                embeddedPopup.currentLayer = nmRelationId ?  orderedRelationModel.nmRelation.referencedLayer : orderedRelationModel.relation.referencingLayer
                embeddedPopup.linkedRelation = orderedRelationModel.relation
                embeddedPopup.linkedParentFeature = orderedRelationModel.feature
                embeddedPopup.feature = nmRelationId ? model.nmReferencedFeature : model.referencingFeature
                embeddedPopup.open()
            }
          }

          Row
          {
            id: deleteButtonRow
            anchors { top: parent.top; right: parent.right; rightMargin: 10 }
            height: listitem.height

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

          //bottom line
          Rectangle {
            id: bottomLine
            anchors.bottom: parent.bottom
            height: 1
            color: 'lightGray'
            width: parent.width
          }
        }
    }

    //the delete entry stuff
    Dialog {
      id: deleteDialog
      parent: mainWindow.contentItem

      property int referencingFeatureId
      property string referencingFeatureDisplayMessage
      property string referencingLayerName: orderedRelationModel.relation.referencingLayer ? orderedRelationModel.relation.referencingLayer.name : ''
      property int nmReferencedFeatureId
      property string nmReferencedFeatureDisplayMessage
      property string nmReferencedLayerName: orderedRelationModel.nmRelation.referencedLayer ? orderedRelationModel.nmRelation.referencedLayer.name : ''
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
