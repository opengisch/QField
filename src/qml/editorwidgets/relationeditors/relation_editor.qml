import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qfield 1.0
import org.qgis 1.0
import Theme 1.0

import "../.."
import ".."

EditorWidgetBase {
    id: relationEditor

    property int itemHeight: 40
    property int bottomMargin: 10
    property int maximumVisibleItems: 4

    Component.onCompleted: {
      if ( currentLayer && currentLayer.customProperty('QFieldSync/relationship_maximum_visible') !== undefined ) {
        var value = JSON.parse(currentLayer.customProperty('QFieldSync/relationship_maximum_visible'))[relationId]
        maximumVisibleItems = value !== undefined ? value : 4
      } else {
        maximumVisibleItems = 4
      }
    }

    // because no additional addEntry item on readOnly (isEnabled false)
    height: (isEnabled
            ? referencingFeatureListView.height + itemHeight
            : Math.max( referencingFeatureListView.height, itemHeight)) + relationEditor.bottomMargin
    enabled: true

    ReferencingFeatureListModel {
      //containing the current (parent) feature, the relation to the children
      //and the relation from the children to the other parent (if it's nm and cardinality is set)
      //if cardinality is not set, the nmRelationId is empty
      id: relationEditorModel
      currentRelationId: relationId
      currentNmRelationId: nmRelationId
      feature: currentFeature

      property int featureFocus: -1
      onModelUpdated: {
        if (featureFocus > -1) {
          referencingFeatureListView.currentIndex = relationEditorModel.getFeatureIdRow(featureFocus)
          featureFocus = -1
        }
      }
    }

    Rectangle {
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.bottomMargin: relationEditor.bottomMargin
        width: parent.width
        color: "transparent"
        border.color: 'lightgray'
        border.width: 1
        clip: true

        //the list
        ListView {
          id: referencingFeatureListView
          model: relationEditorModel
          width: parent.width
          height: maximumVisibleItems > 0
                  ? Math.min(maximumVisibleItems * itemHeight,
                             referencingFeatureListView.count * itemHeight)
                    + (referencingFeatureListView.count > maximumVisibleItems ? itemHeight / 2 : 0)
                  : referencingFeatureListView.count * itemHeight
          delegate: referencingFeatureDelegate
          focus: true
          clip: true
          highlightRangeMode: ListView.ApplyRange

          ScrollBar.vertical: ScrollBar {
            width: 6
            policy: ScrollBar.AlwaysOn

            contentItem: Rectangle {
              implicitWidth: 6
              implicitHeight: itemHeight
              color: Theme.mainColor
            }
          }
        }

        //the add entry "last row"
        Item {
          id: addEntry
          anchors.bottom: parent.bottom
          height: itemHeight
          width: parent.width
          visible: isButtonEnabled('AddChildFeature')

          focus: true

          Rectangle {
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
                  if( relationEditorModel.parentPrimariesAvailable ) {
                    displayToast( qsTr( 'Adding child feature in layer %1' ).arg( relationEditorModel.relation.referencingLayer.name ) )
                    if ( relationEditorModel.relation.referencingLayer.geometryType() !== QgsWkbTypes.NullGeometry )
                    {
                      requestGeometry( relationEditor, relationEditorModel.relation.referencingLayer );
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
            anchors { top: parent.top; right: parent.right; rightMargin: 10 }
            height: listitem.height

            ToolButton {
                id: deleteButton
                width: parent.height
                height: parent.height
                visible: isEnabled && isButtonEnabled('DeleteChildFeature')

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
      running: relationEditorModel.isLoading
    }

    EmbeddedFeatureForm {
        id: embeddedPopup

        embeddedLevel: form.embeddedLevel + 1
        digitizingToolbar: form.digitizingToolbar
        barcodeReader: form.barcodeReader

        onFeatureCancelled: {
            if( autoSave )
                relationEditorModel.reload()
        }

        onFeatureSaved: {
            relationEditorModel.featureFocus = id
            relationEditorModel.reload()
        }
    }

    function isButtonEnabled(buttonType) {
      const buttons = relationEditorWidgetConfig.buttons
      if (buttons === undefined)
        return true

      if (buttons === 'NoButton')
        return false;
      if (buttons === 'AllButtons')
        return true
      if (buttons.split('|').indexOf(buttonType) >= 0)
        return true

      return false
    }

    function requestedGeometryReceived(geometry) {
        showAddFeaturePopup(geometry)
    }

    function showAddFeaturePopup(geometry) {
        embeddedPopup.state = 'Add'
        embeddedPopup.currentLayer = relationEditorModel.relation.referencingLayer
        embeddedPopup.linkedParentFeature = relationEditorModel.feature
        embeddedPopup.linkedRelation = relationEditorModel.relation
        if ( geometry !== undefined )
        {
            embeddedPopup.applyGeometry(geometry)
        }
        embeddedPopup.open()
    }
}
