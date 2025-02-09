import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qfield
import org.qgis
import Theme
import "../.."
import ".."

EditorWidgetBase {
  id: relationEditor

  property int itemHeight: 48
  property int bottomMargin: 10
  property int maximumVisibleItems: 4

  Component.onCompleted: {
    if (currentLayer && currentLayer.customProperty('QFieldSync/relationship_maximum_visible') !== undefined) {
      var value = JSON.parse(currentLayer.customProperty('QFieldSync/relationship_maximum_visible'))[relationId];
      maximumVisibleItems = value !== undefined ? value : 4;
    } else {
      maximumVisibleItems = 4;
    }
  }

  // because no additional addEntry item on readOnly (isEnabled false)
  height: (isEnabled ? referencingFeatureListView.height + itemHeight : Math.max(referencingFeatureListView.height, itemHeight)) + relationEditor.bottomMargin
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
        referencingFeatureListView.currentIndex = relationEditorModel.getFeatureIdRow(featureFocus);
        featureFocus = -1;
      }
    }
  }

  Rectangle {
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.bottomMargin: relationEditor.bottomMargin
    width: parent.width
    color: "transparent"
    border.color: Theme.controlBorderColor
    border.width: 1
    clip: true

    ListView {
      id: referencingFeatureListView
      model: relationEditorModel
      width: parent.width
      height: maximumVisibleItems > 0 ? Math.min(maximumVisibleItems * itemHeight, referencingFeatureListView.count * itemHeight) + (referencingFeatureListView.count > maximumVisibleItems ? itemHeight / 2 : 0) : referencingFeatureListView.count * itemHeight
      delegate: referencingFeatureDelegate
      focus: true
      clip: true
      highlightRangeMode: ListView.ApplyRange
      ScrollBar.vertical: QfScrollBar {
      }
    }

    Item {
      id: addEntry
      anchors.bottom: parent.bottom
      height: itemHeight
      width: parent.width
      visible: isButtonEnabled('AddChildFeature')

      focus: true

      Rectangle {
        anchors.fill: parent
        color: Theme.controlBorderColor
        visible: isEnabled

        Text {
          visible: isEnabled
          color: Theme.secondaryTextColor
          text: isEnabled && !constraintsHardValid ? qsTr('Ensure contraints are met') : ''
          anchors {
            leftMargin: 10
            left: parent.left
            right: addButtonRow.left
            verticalCenter: parent.verticalCenter
          }
          font: Theme.tipFont
        }

        Row {
          id: addButtonRow
          anchors {
            top: parent.top
            right: parent.right
            rightMargin: 10
          }
          height: parent.height

          QfToolButton {
            id: addButton
            width: parent.height
            height: parent.height
            enabled: constraintsHardValid

            round: false
            iconSource: Theme.getThemeVectorIcon('ic_add_white_24dp')
            bgcolor: parent.enabled ? 'black' : 'grey'
          }
        }

        BusyIndicator {
          id: addingIndicator
          anchors {
            top: parent.top
            right: parent.right
            rightMargin: 10
          }
          width: parent.height
          height: parent.height
          running: false
        }

        Timer {
          id: addingTimer

          property string printName: ''

          interval: 50
          repeat: false

          onTriggered: {
            let saved = form.state === 'Add' ? !form.setupOnly && save() : true;
            if (ProjectUtils.transactionMode(qgisProject) !== Qgis.TransactionMode.Disabled) {
              // When a transaction mode is enabled, we must fallback to saving the parent feature to have provider-side issues
              if (!saved) {
                addingIndicator.running = false;
                displayToast(qsTr('Cannot add child feature: insure the parent feature meets all constraints and can be saved'), 'warning');
                return;
              }
            }

            //this has to be checked after buffering because the primary could be a value that has been created on creating featurer (e.g. fid)
            if (relationEditorModel.parentPrimariesAvailable) {
              displayToast(qsTr('Adding child feature in layer %1').arg(relationEditorModel.relation.referencingLayer.name));
              if (relationEditorModel.relation.referencingLayer.geometryType() !== Qgis.GeometryType.Null) {
                requestGeometry(relationEditor, relationEditorModel.relation.referencingLayer);
                return;
              }
              showAddFeaturePopup();
            } else {
              addingIndicator.running = false;
              displayToast(qsTr('Cannot add child feature: attribute value linking parent and children is not set'), 'warning');
            }
          }
        }

        MouseArea {
          anchors.fill: parent
          onClicked: {
            addingIndicator.running = true;
            addingTimer.restart();
          }
        }
      }
    }

    BusyIndicator {
      id: busyIndicator
      anchors.centerIn: parent
      width: 36
      height: 36
      running: relationEditorModel.isLoading
    }
  }

  Component {
    id: referencingFeatureDelegate

    Item {
      id: listitem
      anchors.left: parent ? parent.left : undefined
      anchors.right: parent ? parent.right : undefined

      focus: true

      height: Math.max(itemHeight, featureText.height)

      Ripple {
        clip: true
        width: parent.width
        height: parent.height
        pressed: mouseArea.pressed
        anchor: listitem
        active: mouseArea.pressed
        color: Material.rippleColor
      }

      MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
          if (relationEditorModel.relation.referencingLayer !== undefined) {
            geometryHighlighter.geometryWrapper.qgsGeometry = nmRelationId ? model.nmReferencingFeature.geometry : model.referencingFeature.geometry;
            geometryHighlighter.geometryWrapper.crs = relationEditorModel.relation.referencingLayer.crs;
            mapCanvas.mapSettings.extent = FeatureUtils.extent(mapCanvas.mapSettings, relationEditorModel.relation.referencingLayer, nmRelationId ? model.nmReferencingFeature : model.referencingFeature);
          }
        }
      }

      Row {
        id: itemRow
        anchors.fill: parent
        anchors.rightMargin: 10
        anchors.leftMargin: 10
        height: listitem.height

        Text {
          id: featureText
          anchors.verticalCenter: parent.verticalCenter
          width: parent.width - viewButton.width - deleteButton.width
          topPadding: 5
          bottomPadding: 5
          font: Theme.defaultFont
          color: !isEnabled ? Theme.mainTextDisabledColor : Theme.mainTextColor
          elide: Text.ElideRight
          wrapMode: Text.WordWrap
          text: nmRelationId ? model.nmDisplayString : model.displayString
        }

        QfToolButton {
          id: viewButton
          anchors.verticalCenter: parent.verticalCenter
          width: 48
          height: 48

          round: false
          iconSource: isEnabled ? Theme.getThemeVectorIcon('ic_edit_attributes_white_24dp') : Theme.getThemeVectorIcon('ic_baseline-list_white_24dp')
          iconColor: Theme.mainTextColor
          bgcolor: 'transparent'

          onClicked: {
            embeddedPopup.state = isEnabled ? 'Edit' : 'ReadOnly';
            embeddedPopup.currentLayer = nmRelationId ? relationEditorModel.nmRelation.referencedLayer : relationEditorModel.relation.referencingLayer;
            embeddedPopup.linkedRelation = relationEditorModel.relation;
            embeddedPopup.linkedParentFeature = relationEditorModel.feature;
            embeddedPopup.feature = nmRelationId ? model.nmReferencedFeature : model.referencingFeature;
            embeddedPopup.open();
          }
        }

        QfToolButton {
          id: deleteButton
          anchors.verticalCenter: parent.verticalCenter
          visible: isEnabled && isButtonEnabled('DeleteChildFeature')
          width: visible ? 48 : 0
          height: 48

          round: false
          iconSource: Theme.getThemeVectorIcon('ic_delete_forever_white_24dp')
          iconColor: Theme.mainTextColor
          bgcolor: 'transparent'

          onClicked: {
            deleteDialog.referencingFeatureId = model.referencingFeature.id;
            deleteDialog.referencingFeatureDisplayMessage = model.displayString;
            deleteDialog.nmReferencedFeatureId = nmRelationId ? model.model.nmReferencedFeature.id : 0;
            deleteDialog.nmReferencedFeatureDisplayMessage = nmRelationId ? model.nmDisplayString : '';
            deleteDialog.visible = true;
          }
        }
      }

      //bottom line
      Rectangle {
        id: bottomLine
        anchors.bottom: parent.bottom
        height: 1
        color: Theme.controlBorderColor
        width: parent.width
      }
    }
  }

  QfDialog {
    id: deleteDialog
    parent: mainWindow.contentItem

    property int referencingFeatureId
    property string referencingFeatureDisplayMessage
    property string referencingLayerName: relationEditorModel.relation.referencingLayer ? relationEditorModel.relation.referencingLayer.name : ''
    property int nmReferencedFeatureId
    property string nmReferencedFeatureDisplayMessage
    property string nmReferencedLayerName: relationEditorModel.nmRelation.referencedLayer ? relationEditorModel.nmRelation.referencedLayer.name : ''
    property string nmReferencingLayerName

    z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature forms
    title: nmRelationId ? qsTr('Unlink feature %1 (%2) of %3').arg(nmReferencedFeatureDisplayMessage).arg(nmReferencedFeatureId).arg(nmReferencedLayerName) : qsTr('Delete feature %1 (%2) on %3').arg(referencingFeatureDisplayMessage).arg(referencingFeatureId).arg(referencingLayerName)
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: nmRelationId ? qsTr('Should the feature <b>%1 (%2)</b> of layer <b>%3</b> be unlinked?<br><i>(The connection will be deleted on layer <b>%4</b>)</i>').arg(deleteDialog.nmReferencedFeatureDisplayMessage).arg(deleteDialog.nmReferencedFeatureId).arg(deleteDialog.nmReferencedLayerName).arg(deleteDialog.referencingLayerName) : qsTr('Should the feature <b>%1 (%2)</b> on layer <b>%3</b> be deleted?').arg(deleteDialog.referencingFeatureDisplayMessage).arg(deleteDialog.referencingFeatureId).arg(deleteDialog.referencingLayerName)
    }
    onAccepted: {
      if (!referencingFeatureListView.model.deleteFeature(referencingFeatureId)) {
        displayToast(qsTr("Failed to delete referencing feature"), 'error');
      }
      visible = false;
    }
    onRejected: {
      visible = false;
    }
  }

  EmbeddedFeatureForm {
    id: embeddedPopup

    embeddedLevel: form.embeddedLevel + 1
    digitizingToolbar: form.digitizingToolbar
    codeReader: form.codeReader

    onFeatureCancelled: {
      if (autoSave)
        relationEditorModel.reload();
    }

    onFeatureSaved: id => {
      relationEditorModel.featureFocus = id;
      relationEditorModel.reload();
    }

    onOpened: {
      addingIndicator.running = false;
    }
  }

  function isButtonEnabled(buttonType) {
    const buttons = relationEditorWidgetConfig.buttons;
    if (buttons === undefined)
      return true;
    if (buttons === 'NoButton')
      return false;
    if (buttons === 'AllButtons')
      return true;
    if (buttons.split('|').indexOf(buttonType) >= 0)
      return true;
    return false;
  }

  function requestedGeometryReceived(geometry) {
    showAddFeaturePopup(geometry);
  }

  function showAddFeaturePopup(geometry) {
    embeddedPopup.state = 'Add';
    embeddedPopup.currentLayer = relationEditorModel.relation.referencingLayer;
    embeddedPopup.linkedParentFeature = relationEditorModel.feature;
    embeddedPopup.linkedRelation = relationEditorModel.relation;
    if (geometry !== undefined) {
      embeddedPopup.applyGeometry(geometry);
    }
    embeddedPopup.open();
    embeddedPopup.attributeFormModel.applyParentDefaultValues();
  }
}
