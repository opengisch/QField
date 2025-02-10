import QtQuick
import QtQuick.Controls
import QtQml.Models
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

  // because no additional addEntry item on readOnly (isEnabled false)
  height: listView.contentHeight + (isEnabled ? addEntry.height : 0)
  enabled: true

  Rectangle {
    anchors.fill: parent
    color: "transparent"
    border.color: Theme.controlBorderColor
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
      displayToast("Failed to reorder features.", "error");
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

  Item {
    id: addEntry
    anchors.top: listView.bottom
    height: itemHeight
    width: parent.width

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
              displayToast(qsTr('Cannot add child feature: insure the parent feature meets all constraints and can be saved'), 'warning');
              return;
            }
          }

          //this has to be checked after buffering because the primary could be a value that has been created on creating featurer (e.g. fid)
          if (orderedRelationModel.parentPrimariesAvailable) {
            displayToast(qsTr('Adding child feature in layer %1').arg(orderedRelationModel.relation.referencingLayer.name));
            if (orderedRelationModel.relation.referencingLayer.geometryType() !== Qgis.GeometryType.Null) {
              requestGeometry(relationEditor, orderedRelationModel.relation.referencingLayer);
              return;
            }
            showAddFeaturePopup();
          } else {
            displayToast(qsTr('Cannot add child feature: attribute value linking parent and children is not set'), 'warning');
          }
        }
      }

      MouseArea {
        anchors.fill: parent
        onClicked: {
          addingTimer.restart();
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
          held = true;
        }
      }
      onReleased: {
        if (held === true) {
          held = false;
          orderedRelationModel.moveItems(indexFrom, indexTo);
        } else if (listView.currentIndex !== dragArea.DelegateModel.itemsIndex) {
          listView.currentIndex = dragArea.DelegateModel.itemsIndex;
          orderedRelationModel.triggerViewCurrentFeatureChange(listView.currentIndex);
        }
      }

      onClicked: {
        if (orderedRelationModel.relation.referencingLayer !== undefined) {
          geometryHighlighter.geometryWrapper.qgsGeometry = nmRelationId ? model.nmReferencingFeature.geometry : model.referencingFeature.geometry;
          geometryHighlighter.geometryWrapper.crs = orderedRelationModel.relation.referencingLayer.crs;
          mapCanvas.mapSettings.extent = FeatureUtils.extent(mapCanvas.mapSettings, orderedRelationModel.relation.referencingLayer, nmRelationId ? model.nmReferencingFeature : model.referencingFeature);
        }
      }

      Rectangle {
        id: content
        anchors {
          horizontalCenter: parent.horizontalCenter
          verticalCenter: parent.verticalCenter
        }
        width: dragArea.width
        height: row.implicitHeight + 4

        Ripple {
          clip: true
          width: parent.width
          height: parent.height
          pressed: dragArea.pressed
          anchor: content
          active: dragArea.pressed
          color: Material.rippleColor
        }

        color: "transparent"

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

          height: Math.max(itemHeight, featureText.height)

          Image {
            id: featureImage
            source: ImagePath ? UrlUtils.fromString(ImagePath) : Theme.getThemeVectorIcon("ic_photo_notavailable_black_24dp")
            anchors.verticalCenter: parent.verticalCenter
            width: 48
            height: 48
            fillMode: Image.PreserveAspectFit
            visible: !!ImagePath
          }

          Text {
            id: featureText
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width - 8 - (featureImage.visible ? featureImage.width : 0) - viewButton.width - moveDownButton.width - moveUpButton.width - deleteButton.width
            topPadding: 5
            bottomPadding: 5
            font: Theme.defaultFont
            color: !isEnabled ? Theme.mainTextDisabledColor : Theme.mainTextColor
            text: Description || model.displayString
            elide: Text.ElideRight
            wrapMode: Text.WordWrap
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
              embeddedPopup.currentLayer = orderedRelationModel.relation.referencingLayer;
              embeddedPopup.linkedRelation = orderedRelationModel.relation;
              embeddedPopup.linkedRelationOrderingField = orderedRelationModel.orderingField;
              embeddedPopup.linkedParentFeature = orderedRelationModel.feature;
              embeddedPopup.feature = model.referencingFeature;
              embeddedPopup.open();
            }
          }

          QfToolButton {
            id: moveDownButton
            anchors.verticalCenter: parent.verticalCenter
            visible: isEnabled
            width: visible ? 48 : 0
            height: 48
            opacity: (index === listView.count - 1) ? 0.3 : 1

            round: false
            iconSource: Theme.getThemeVectorIcon('ic_chevron_down')
            iconColor: Theme.mainTextColor
            bgcolor: 'transparent'

            onClicked: {
              if (index === listView.count - 1) {
                return;
              }
              orderedRelationModel.moveItems(index, index + 1);
            }
          }

          QfToolButton {
            id: moveUpButton
            anchors.verticalCenter: parent.verticalCenter
            visible: isEnabled
            width: visible ? 48 : 0
            height: 48
            opacity: (index === 0) ? 0.3 : 1

            round: false
            iconSource: Theme.getThemeVectorIcon('ic_chevron_up')
            iconColor: Theme.mainTextColor
            bgcolor: 'transparent'

            onClicked: {
              if (index === 0) {
                return;
              }
              orderedRelationModel.moveItems(index, index - 1);
            }
          }

          QfToolButton {
            id: deleteButton
            anchors.verticalCenter: parent.verticalCenter
            visible: isEnabled
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

        Rectangle {
          id: bottomLine
          anchors.bottom: parent.bottom
          height: 1
          color: Theme.controlBorderColor
          width: parent.width
        }
      }

      DropArea {
        anchors.fill: parent
        anchors.margins: 10

        onEntered: {
          if (dragArea.indexFrom === -1) {
            dragArea.indexFrom = drag.source.DelegateModel.itemsIndex;
          }
          dragArea.indexTo = dragArea.DelegateModel.itemsIndex;
          visualModel.items.move(drag.source.DelegateModel.itemsIndex, dragArea.DelegateModel.itemsIndex);
        }
      }
    }
  }

  BusyIndicator {
    id: busyIndicator
    anchors.centerIn: parent
    width: 36
    height: 36
    running: orderedRelationModel.isLoading
  }

  QfDialog {
    id: deleteDialog
    parent: mainWindow.contentItem

    property int referencingFeatureId
    property string referencingFeatureDisplayMessage
    property string referencingLayerName: orderedRelationModel.relation.referencingLayer ? orderedRelationModel.relation.referencingLayer.name : ''
    property int nmReferencedFeatureId
    property string nmReferencedFeatureDisplayMessage
    property string nmReferencedLayerName: orderedRelationModel.nmRelation.referencedLayer ? orderedRelationModel.nmRelation.referencedLayer.name : ''
    property string nmReferencingLayerName

    z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature forms
    title: nmRelationId ? qsTr('Unlink feature %1 (%2) of %3').arg(nmReferencedFeatureDisplayMessage).arg(nmReferencedFeatureId).arg(nmReferencedLayerName) : qsTr('Delete feature %1 (%2) on %3').arg(referencingFeatureDisplayMessage).arg(referencingFeatureId).arg(referencingLayerName)
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: nmRelationId ? qsTr('Should the feature <b>%1 (%2)</b> of layer <b>%3</b> be unlinked?<br><i>(The connection will be deleted on layer <b>%4</b>)</i>').arg(deleteDialog.nmReferencedFeatureDisplayMessage).arg(deleteDialog.nmReferencedFeatureId).arg(deleteDialog.nmReferencedLayerName).arg(deleteDialog.referencingLayerName) : qsTr('Should the feature <b>%1 (%2)</b> on layer <b>%3</b> be deleted?').arg(deleteDialog.referencingFeatureDisplayMessage).arg(deleteDialog.referencingFeatureId).arg(deleteDialog.referencingLayerName)
    }
    onAccepted: {
      if (!orderedRelationModel.deleteFeature(referencingFeatureId)) {
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
      if (autoSave) {
        orderedRelationModel.reload();
      }
    }

    onFeatureSaved: id => {
      orderedRelationModel.featureFocus = id;
      orderedRelationModel.reload();
    }

    onOpened: {
      addingIndicator.running = false;
    }
  }

  function requestedGeometryReceived(geometry) {
    showAddFeaturePopup(geometry);
  }

  function showAddFeaturePopup(geometry) {
    embeddedPopup.state = 'Add';
    embeddedPopup.currentLayer = orderedRelationModel.relation.referencingLayer;
    embeddedPopup.linkedParentFeature = orderedRelationModel.feature;
    embeddedPopup.linkedRelation = orderedRelationModel.relation;
    embeddedPopup.linkedRelationOrderingField = orderedRelationModel.orderingField;
    if (geometry !== undefined) {
      embeddedPopup.applyGeometry(geometry);
    }
    embeddedPopup.open();
    embeddedPopup.attributeFormModel.applyParentDefaultValues();
  }
}
