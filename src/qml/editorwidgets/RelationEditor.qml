import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2

import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

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
              text: !readOnly && !constraintsValid ? qsTr( 'Ensure contraints') : ''
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
                enabled: constraintsValid

                contentItem: Rectangle {
                    anchors.fill: parent
                    color: parent.enabled ? nmRelationId === '' ? 'black' : 'blue' : 'grey'
                    Image {
                      anchors.fill: parent
                      anchors.margins: 4 * dp
                      fillMode: Image.PreserveAspectFit
                      horizontalAlignment: Image.AlignHCente
                      verticalAlignment: Image.AlignVCenter
                      source: Style.getThemeIcon( 'ic_add_white_24dp' )
                    }
                }

                onClicked: {
                  if( buffer() ) {
                      //this has to be checked after buffering because the primary could be a value that has been created on creating featurer (e.g. fid)
                      if( relationEditorModel.parentPrimariesAvailable ) {
                          embeddedFeatureForm.state = 'Add'
                          embeddedAttributeFormModel.featureModel.linkedParentFeature = relationEditorModel.feature
                          embeddedAttributeFormModel.featureModel.linkedRelation = relationEditorModel.relation
                          embeddedAttributeFormModel.featureModel.resetAttributes()
                          embeddedFeatureForm.active = true
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
            text: { text: nmRelationId === '' ? model.displayString : model.nmDisplayString }
          }

          MouseArea {
            anchors.fill: parent

            onClicked: {
                embeddedFeatureForm.state = !readOnly ? 'Edit' : 'ReadOnly'
                embeddedAttributeFormModel.featureModel.linkedParentFeature = relationEditorModel.feature
                embeddedAttributeFormModel.featureModel.linkedRelation = relationEditorModel.relation
                embeddedAttributeFormModel.featureModel.feature = nmRelationId === '' ? model.referencingFeature : model.nmReferencedFeature
                embeddedFeatureForm.active = true
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
                    color: nmRelationId === '' ? 'black' : 'blue'
                    Image {
                      anchors.fill: parent
                      anchors.margins: 4 * dp
                      fillMode: Image.PreserveAspectFit
                      horizontalAlignment: Image.AlignHCenter
                      verticalAlignment: Image.AlignVCenter
                      source: Style.getThemeIcon( 'ic_delete_forever_white_24dp' )
                    }
                }

                onClicked: {
                    deleteDialog.referencingFeatureId = model.referencingFeature.id
                    deleteDialog.referencingFeatureDisplayMessage = model.displayString
                    deleteDialog.layerName = relationEditorModel.relation.referencingLayer.name
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
      property string layerName

      visible: false

      title: qsTr( '%1 feature %2 (%3) on layer %4' ).arg( nmRelationId ?  'Unlink connection to ' : 'Delete ').arg( referencingFeatureDisplayMessage ).arg(referencingFeatureId).arg(layerName)
      text: qsTr( 'Should the feature %1 (%2) on layer %3 be %4').arg( referencingFeatureDisplayMessage ).arg(referencingFeatureId).arg( layerName ).arg( nmRelationId ? 'unlinked?' : 'deleted?' )
      standardButtons: StandardButton.Ok | StandardButton.Cancel
      onAccepted: {
        referencingFeatureListView.model.deleteFeature( referencingFeatureId )
        console.log('delete feature '+referencingFeatureId)
        visible = false
      }
      onRejected: {
        visible = false
      }
    }

    //the add entry stuff
    AttributeFormModel {
      id: embeddedAttributeFormModel

      featureModel: FeatureModel {
        currentLayer: nmRelationId ? relationEditorModel.relation.referencingLayer : relationEditorModel.associatedRelation.referencedLayer
      }
    }

    Loader {
      id: embeddedFeatureForm

      property var state

      sourceComponent: embeddedFeatureFormComponent
      active: false
      onLoaded: {
        item.open()
      }
    }

    Component {
      id: embeddedFeatureFormComponent

      Popup {
        id: popup
        parent: ApplicationWindow.overlay

        x: 24 * dp
        y: 24 * dp
        width: parent.width - 48 * dp
        height: parent.height - 48 * dp
        modal: true
        closePolicy: Popup.CloseOnEscape

        FeatureForm {
            model: embeddedAttributeFormModel

            focus: true

            embedded: true
            toolbarVisible: true

            anchors.fill: parent

            state: embeddedFeatureForm.state

            onSaved: {
                popup.close()
            }

            onCancelled: {
                popup.close()
            }
        }

        onClosed: {
          embeddedFeatureForm.active = false
          relationEditorModel.reload()
        }
      }
    }

    BusyIndicator {
      id: busyIndicator
      anchors.centerIn: parent
      width: 36 * dp
      height: 36 * dp
      running: relationEditorModel.isLoading
    }
}
