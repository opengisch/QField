import QtQuick
import QtQuick.Controls
import org.qfield
import org.qgis
import Theme
import "../"

EditorWidgetBase {
  id: relationEditorBase

  property var relationEditorModel: undefined
  property alias listView: listView

  property int itemHeight: 48
  property int bottomMargin: 10
  property int maximumVisibleItems: 4
  property bool showAllItems: false

  Component.onCompleted: {
    if (currentLayer && currentLayer.customProperty('QFieldSync/relationship_maximum_visible') !== undefined) {
      var value = JSON.parse(currentLayer.customProperty('QFieldSync/relationship_maximum_visible'))[relationId];
      maximumVisibleItems = value !== undefined ? value : 4;
    } else {
      maximumVisibleItems = 4;
    }
  }

  // because no additional addEntry item on readOnly (isEnabled false)
  height: listView.contentHeight + (isEnabled ? addEntry.height : 0) + 10
  enabled: true

  Rectangle {
    anchors.top: parent.top
    anchors.bottom: parent.bottom
    anchors.bottomMargin: relationEditorBase.bottomMargin
    width: parent.width
    color: "transparent"
    border.color: Theme.controlBorderColor
    border.width: 1
    clip: true

    ListView {
      id: listView
      width: parent.width
      height: !showAllItems && maximumVisibleItems > 0 ? Math.min(maximumVisibleItems * itemHeight, listView.count * itemHeight) + (listView.count > maximumVisibleItems ? itemHeight / 2 : 0) : listView.count * itemHeight
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
      visible: isActionEnabled('AddChildFeature')

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

  property QfDialog deleteDialog: QfDialog {
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
      if (!relationEditorModel || !relationEditorModel.deleteFeature(referencingFeatureId)) {
        displayToast(qsTr("Failed to delete referencing feature"), 'error');
      }
      visible = false;
    }
    onRejected: {
      visible = false;
    }
  }

  property EmbeddedFeatureForm embeddedPopup: EmbeddedFeatureForm {
    embeddedLevel: form.embeddedLevel + 1
    digitizingToolbar: form.digitizingToolbar
    codeReader: form.codeReader

    onFeatureCancelled: {
      if (autoSave) {
        relationEditorModel.reload();
      }
    }

    onFeatureSaved: id => {
      relationEditorModel.featureFocus = id;
      relationEditorModel.reload();
    }

    onOpened: {
      addingIndicator.running = false;
    }
  }

  property Menu childMenu: Menu {
    id: childMenu
    title: qsTr("Child Menu")
    z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes

    property int entryReferencingFeatureId: -1
    property string entryDisplayString: ""
    property int entryNmReferencedFeatureId: -1
    property string entryNmReferencedFeatureDisplayMessage: ""

    width: {
      let result = 50;
      let padding = 0;
      for (var i = 0; i < count; ++i) {
        let item = itemAt(i);
        result = Math.max(item.contentItem.implicitWidth, result);
        padding = Math.max(item.leftPadding + item.rightPadding, padding);
      }
      return mainWindow.width > 0 ? Math.min(result + padding, mainWindow.width - 20) : result + padding;
    }

    topMargin: mainWindow.sceneTopMargin
    bottomMargin: mainWindow.sceneBottomMargin

    MenuItem {
      id: deleteChildItem
      enabled: isEnabled
      visible: isActionEnabled('DeleteChildFeature')

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding
      icon.source: Theme.getThemeVectorIcon("ic_delete_forever_white_24dp")

      text: qsTr("Delete Child Feature")
      onTriggered: {
        deleteDialog.referencingFeatureId = childMenu.entryReferencingFeatureId;
        deleteDialog.referencingFeatureDisplayMessage = childMenu.entryDisplayString;
        deleteDialog.nmReferencedFeatureId = childMenu.entryNmReferencedFeatureId;
        deleteDialog.nmReferencedFeatureDisplayMessage = childMenu.entryNmReferencedFeatureDisplayMessage;
        deleteDialog.visible = true;
      }
    }
  }

  function isActionEnabled(buttonType) {
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
    if (relationEditorModel.orderingField) {
      embeddedPopup.linkedRelationOrderingField = relationEditorModel.orderingField;
    }
    if (geometry !== undefined) {
      embeddedPopup.applyGeometry(geometry);
    }
    embeddedPopup.open();
    embeddedPopup.attributeFormModel.applyParentDefaultValues();
  }
}
