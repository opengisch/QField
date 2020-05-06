import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2

import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import Theme 1.0

import org.qfield 1.0
import org.qgis 1.0

Rectangle{
    height: !readOnly ? referencingFeatureListView.height + itemHeight : Math.max( referencingFeatureListView.height, itemHeight) //because no additional addEntry item on readOnly
    property int itemHeight: 32 * dp

    border.color: 'lightgray'
    border.width: 1 * dp

    ReferencingFeatureListModel {
        //containing the current (parent) feature, the relation to the children
        //and the relation from the children to the other parent (if it's nm and cardinality is set)
        //if cardinality is not set, the nmRelationId is empty
        id: relationEditorModel
        relation: qgisProject.relationManager.relation(relationId)
        nmRelation: qgisProject.relationManager.relation(nmRelationId)
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
          visible: !readOnly

          Text {
              visible: !readOnly
              color: 'grey'
              text: !readOnly && !constraintsHardValid ? qsTr( 'Ensure contraints') : ''
              anchors { leftMargin: 10; left: parent.left; right: addButton.left; verticalCenter: parent.verticalCenter }
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
                      anchors.margins: 4 * dp
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
          anchors { left: parent.left; right: parent.right }

          focus: true

          height: Math.max( itemHeight, featureText.height )

          Text {
            id: featureText
            anchors { leftMargin: 10 * dp ; left: parent.left; right: deleteButtonRow.left; verticalCenter: parent.verticalCenter }
            font.bold: true
            color: readOnly ? 'grey' : 'black'
            text: { text: nmRelationId ? model.nmDisplayString : model.displayString }
          }

          MouseArea {
            anchors.fill: parent

            onClicked: {
                embeddedPopup.state = !readOnly ? 'Edit' : 'ReadOnly'
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
                visible: !readOnly

                contentItem: Rectangle {
                    anchors.fill: parent
                    color: nmRelationId ? 'blue' : '#900000'
                    Image {
                      anchors.fill: parent
                      anchors.margins: 4 * dp
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
    MessageDialog {
      id: deleteDialog

      property int referencingFeatureId
      property string referencingFeatureDisplayMessage
      property int nmReferencedFeatureId
      property string nmReferencedFeatureDisplayMessage

      visible: false

      title: nmRelationId ?
               qsTr( 'Unlink feature %1 (%2) of %3' ).arg( nmReferencedFeatureDisplayMessage ).arg( nmReferencedFeatureId ).arg( relationEditorModel.nmRelation.referencedLayer.name ) :
               qsTr( 'Delete feature %1 (%2) on %3' ).arg( referencingFeatureDisplayMessage ).arg( referencingFeatureId ).arg( relationEditorModel.relation.referencingLayer.name )
      text:  nmRelationId ?
               qsTr( 'Should the feature <b>%1 (%2)</b> of layer <b>%3</b> be unlinked?<br><i>(The connection will be deleted on layer <b>%4</b>)</i>').arg( nmReferencedFeatureDisplayMessage ).arg( nmReferencedFeatureId ).arg( relationEditorModel.nmRelation.referencedLayer.name ).arg( relationEditorModel.relation.referencingLayer.name ) :
               qsTr( 'Should the feature <b>%1 (%2)</b> on layer <b>%3</b> be deleted?').arg( referencingFeatureDisplayMessage ).arg( referencingFeatureId ).arg( relationEditorModel.relation.referencingLayer.name )
      standardButtons: StandardButton.Ok | StandardButton.Cancel
      onAccepted: {
        referencingFeatureListView.model.deleteFeature( referencingFeatureId )
        visible = false
      }
      onRejected: {
        visible = false
      }

    }

    BusyIndicator {
      id: busyIndicator
      anchors.centerIn: parent
      width: 36 * dp
      height: 36 * dp
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
