import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2

import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

Frame{
    height: 200
    property int itemHeight: 24 * dp

    //the model
    ReferencingFeatureListModel {
        id: relationEditorModel
        relation: qgisProject.relationManager.relation(relationId)
        associatedRelation: qgisProject.relationManager.relation(associatedRelationId)
        feature: currentFeature
    }

    //the list
    ListView {
        id: referencingFeatureListView
        width: parent.width
        height: parent.height - itemHeight
        model: relationEditorModel
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

      Row
      {
        id: editRow
        anchors { top: parent.top; right: parent.right }
        height: parent.height

        ToolButton {
            id: addButton
            width: parent.height
            height: parent.height
            visible: !readOnly

            contentItem: Rectangle {
                anchors.fill: parent
                color: "black"
                Image {
                  anchors.fill: parent
                  anchors.margins: 4 * dp
                  fillMode: Image.PreserveAspectFit
                  horizontalAlignment: Image.AlignHCenter
                  verticalAlignment: Image.AlignVCenter
                  source: Style.getThemeIcon( 'ic_add_white_24dp' )
                }
            }

            onClicked: {
              embeddedFeatureForm.state = "Add"
              embeddedFeatureForm.relatedLayer = relationEditorModel.relation.referencingLayer
              embeddedFeatureForm.active = true
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
            anchors { leftMargin: 10; left: parent.left; right: deleteButton.left; verticalCenter: parent.verticalCenter }
            font.bold: true
            color: readOnly ? "grey" : "black"
            text: { text: model.displayString }
          }

          MouseArea {
            anchors.fill: parent

            onClicked: {
                embeddedFeatureForm.state = !readOnly ? "Edit" : "ReadOnly"
                embeddedFeatureForm.relatedFeature = model.referencingFeature //nm not yet activated: associatedRelationId === "" ? model.referencingFeature : model.associatedReferencedFeature
                embeddedFeatureForm.relatedLayer = relationEditorModel.relation.referencingLayer //nm not yet activated: associatedRelationId === "" ? relationEditorModel.relation.referencingLayer : relationEditorModel.associatedRelation.referencedLayer
                embeddedFeatureForm.active = true
            }
          }

          Row
          {
            id: editRow
            anchors { top: parent.top; right: parent.right }
            height: listitem.height

            ToolButton {
                id: deleteButton
                width: parent.height
                height: parent.height
                visible: !readOnly

                contentItem: Rectangle {
                    anchors.fill: parent
                    color: "black"
                    Image {
                      anchors.fill: parent
                      anchors.margins: 4 * px
                      fillMode: Image.PreserveAspectFit
                      horizontalAlignment: Image.AlignHCenter
                      verticalAlignment: Image.AlignVCenter
                      source: Style.getThemeIcon( 'ic_delete_forever_white_24dp' )
                    }
                }

                onClicked: {
                    deleteDialog.referencingFeatureId = model.referencingFeature.id
                    deleteDialog.visible = true
                }
            }
          }

          //bottom line
          Rectangle {
            id: bottomLine
            anchors.bottom: parent.bottom
            height: 1
            color: "lightGray"
            width: parent.width
          }
        }
    }

    //the delete entry stuff
    MessageDialog {
      id: deleteDialog

      property int referencingFeatureId
      property var layerName

      visible: false

      title: qsTr( "Delete feature %1 on layer %2" ).arg(referencingFeatureId).arg(layerName)
      text: qsTr( "Should the feature %1 on layer %2").arg(referencingFeatureId).arg( layerName)
      standardButtons: StandardButton.Ok | StandardButton.Cancel
      onAccepted: {
        referencingFeatureListView.model.deleteFeature( referencingFeatureId )
        console.log("delete feature "+referencingFeatureId)
        visible = false
      }
      onRejected: {
        visible = false
      }
    }

    //the add entry stuff
    Loader {
      id: embeddedFeatureForm

      property var state
      property var relatedFeature
      property var relatedLayer

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
        height: parent.width - 48 * dp
        modal: true
        focus: true
        closePolicy: Popup.CloseOnEscape

        FeatureForm {
            model: AttributeFormModel {
            id: attributeFormModel

            featureModel: FeatureModel {
              currentLayer: relatedLayer
              feature: state != "Add" ? embeddedFeatureForm.relatedFeature : undefined
              linkedParentFeature: relationEditorModel.feature
              linkedRelation: relationEditorModel.relation
            }
          }

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
}
