import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.12

import Theme 1.0
import ".."

import org.qfield 1.0
import org.qgis 1.0

Rectangle{
    height: isEnabled ? referencingFeatureListView.height + itemHeight : Math.max( referencingFeatureListView.height, itemHeight) //because no additional addEntry item on readOnly (isEnabled false)
    property int itemHeight: 32
    enabled: true

    border.color: 'lightgray'
    border.width: 1

    ReferencingFeatureListModel {
        //containing the current (parent) feature, the relation to the children
        //and the relation from the children to the other parent (if it's nm and cardinality is set)
        //if cardinality is not set, the nmRelationId is empty
        id: relationEditorModel
        currentRelationId: relationId
        currentNmRelationId: nmRelationId
        feature: currentFeature
    }

    //the list
    ListView {
        id: referencingFeatureListView
        model: relationEditorModel
        width: parent.width
        height: Math.min( 5 * itemHeight, referencingFeatureListView.count * itemHeight )
        delegate: referencingFeatureDelegate
        focus: true
        clip: true
        highlightRangeMode: ListView.StrictlyEnforceRange
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
          }

          Row
          {
            id: addButtonRow
            anchors { top: parent.top; right: parent.right }
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
                      anchors.margins: 4
                      fillMode: Image.PreserveAspectFit
                      horizontalAlignment: Image.AlignHCenter
                      verticalAlignment: Image.AlignVCenter
                      source: Theme.getThemeIcon( 'ic_add_white_24dp' )
                    }
                }

                onClicked: {
                  if( save() ) {
                      //this has to be checked after buffering because the primary could be a value that has been created on creating featurer (e.g. fid)
                      if( relationEditorModel.parentPrimariesAvailable ) {
                          embeddedPopup.state = 'Add'
                          embeddedPopup.currentLayer = relationEditorModel.relation.referencingLayer
                          embeddedPopup.linkedParentFeature = relationEditorModel.feature
                          embeddedPopup.linkedRelation = relationEditorModel.relation
                          embeddedPopup.open()
                      }
                      else
                      {
                          displayToast(qsTr( 'Cannot add child. Parent primary keys are not available.' ) )
                      }
                  }
              }
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
            font.bold: true
            color: !isEnabled ? 'grey' : 'black'
            text: { text: nmRelationId ? model.nmDisplayString : model.displayString }
          }

          MouseArea {
            anchors.fill: parent

            onClicked: {
                embeddedPopup.state = isEnabled ? 'Edit' : 'ReadOnly'
                embeddedPopup.currentLayer = nmRelationId ?  relationEditorModel.nmRelation.referencedLayer : relationEditorModel.relation.referencingLayer
                embeddedPopup.linkedRelation = relationEditorModel.relation
                embeddedPopup.linkedParentFeature = relationEditorModel.feature
                embeddedPopup.feature = nmRelationId ? model.nmReferencedFeature : model.referencingFeature
                embeddedPopup.open()
            }
          }

          Row
          {
            id: deleteButtonRow
            anchors { top: parent.top; right: parent.right }
            height: listitem.height

            ToolButton {
                id: deleteButton
                width: parent.height
                height: parent.height
                visible: isEnabled

                contentItem: Rectangle {
                    anchors.fill: parent
                    color: nmRelationId ? 'blue' : '#900000'
                    Image {
                      anchors.fill: parent
                      anchors.margins: 4
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
      property string referencingLayerName: relationEditorModel.relation.referencingLayer ? relationEditorModel.relation.referencingLayer.name : ''
      property int nmReferencedFeatureId
      property string nmReferencedFeatureDisplayMessage
      property string nmReferencedLayerName: relationEditorModel.nmRelation.referencedLayer ? relationEditorModel.nmRelation.referencedLayer.name : ''
      property string nmReferencingLayerName

      visible: false
      modal: true

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
                  .arg( parent.nmReferencedFeatureDisplayMessage )
                  .arg( parent.nmReferencedFeatureId )
                  .arg( parent.nmReferencedLayerName )
                  .arg( parent.referencingLayerName )
                : qsTr( 'Should the feature <b>%1 (%2)</b> on layer <b>%3</b> be deleted?')
                  .arg( parent.referencingFeatureDisplayMessage )
                  .arg( parent.referencingFeatureId )
                  .arg( parent.referencingLayerName )
      }

      standardButtons: Dialog.Ok | Dialog.Cancel
      onAccepted: {
        if ( ! referencingFeatureListView.model.deleteFeature( referencingFeatureId ) ) {
          displayToast( qsTr( "Failed to delete referencing feature" ) )
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
      running: relationEditorModel.isLoading
    }

    EmbeddedFeatureForm{
        id: embeddedPopup

        onFeatureCancelled: {
            if( autoSave )
                relationEditorModel.reload()
        }

        onFeatureSaved: {
            relationEditorModel.reload()
        }
    }
}
