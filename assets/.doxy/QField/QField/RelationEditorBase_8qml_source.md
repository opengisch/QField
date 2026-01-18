

# File RelationEditorBase.qml

[**File List**](files.md) **>** [**editorwidgets**](dir_7621275d8427768f32eeef555af1c8d1.md) **>** [**RelationEditorBase.qml**](RelationEditorBase_8qml.md)

[Go to the documentation of this file](RelationEditorBase_8qml.md)


```C++
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
  property bool showSortButton: true

  signal toggleSortAction

  Component.onCompleted: {
    if (currentLayer && currentLayer.customProperty('QFieldSync/relationship_maximum_visible') !== undefined) {
      var value = JSON.parse(currentLayer.customProperty('QFieldSync/relationship_maximum_visible'))[relationId];
      maximumVisibleItems = value !== undefined ? value : 4;
    } else {
      maximumVisibleItems = 4;
    }
  }

  height: listView.height + headerEntry.height + 10
  enabled: true

  Rectangle {
    anchors.fill: parent
    anchors.bottomMargin: relationEditorBase.bottomMargin
    color: "transparent"
    border.color: Theme.controlBorderColor
    border.width: 1
    radius: 5
    clip: true

    Rectangle {
      id: headerEntry
      width: parent.width
      height: itemHeight
      color: Theme.controlBorderColor
      focus: true
      topLeftRadius: parent.radius
      topRightRadius: parent.radius
      bottomLeftRadius: listView.count === 0 ? parent.radius : 0
      bottomRightRadius: listView.count === 0 ? parent.radius : 0

      Text {
        text: qsTr("%n feature(s)", "", listView.count)
        anchors {
          leftMargin: 10
          left: parent.left
          right: addButtonRow.left
          verticalCenter: parent.verticalCenter
        }
        font: Theme.strongTipFont
        opacity: enabled ? 1 : 0.45
        color: Theme.mainTextColor
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
          enabled: isEnabled
          visible: enabled

          round: false
          iconSource: Theme.getThemeVectorIcon('ic_add_white_24dp')
          iconColor: enabled ? Theme.mainTextColor : 'grey'
          onClicked: {
            addingIndicator.running = true;
            addingTimer.restart();
          }
        }

        QfToolButton {
          id: sortButton
          width: parent.height
          height: parent.height
          visible: listView.count > 0 && relationEditorBase.showSortButton

          round: false
          iconSource: Theme.getThemeVectorIcon('ic_sort_white_24dp')
          iconColor: Theme.mainTextColor
          onClicked: {
            toggleSortAction();
          }
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
    }

    ListView {
      id: listView
      anchors.top: headerEntry.bottom
      width: parent.width
      height: !showAllItems && maximumVisibleItems > 0 ? Math.min(maximumVisibleItems * itemHeight, contentHeight) : contentHeight
      focus: true
      clip: true
      boundsBehavior: Flickable.StopAtBounds
      highlightRangeMode: ListView.ApplyRange
      ScrollBar.vertical: QfScrollBar {}
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
    title: nmRelationId ? qsTr('Unlink Feature') : qsTr('Delete Feature')

    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: nmRelationId ? qsTr('Should the feature <b>%1 (%2)</b> of layer <b>%3</b> be unlinked?<br><i>(The connection will be deleted on layer <b>%4</b>)</i>').arg(deleteDialog.nmReferencedFeatureDisplayMessage).arg(deleteDialog.nmReferencedFeatureId).arg(deleteDialog.nmReferencedLayerName).arg(deleteDialog.referencingLayerName) : qsTr('Should the feature <b>%1 (%2)</b> on layer <b>%3</b> be deleted?').arg(deleteDialog.referencingFeatureDisplayMessage).arg(deleteDialog.referencingFeatureId).arg(deleteDialog.referencingLayerName)
      font: Theme.defaultFont
    }

    onAccepted: {
      if (!relationEditorModel || !relationEditorModel.deleteFeature(referencingFeatureId)) {
        displayToast(qsTr("Failed to delete referencing feature"), 'error');
      }
      visible = false;
      form.model.applyRelationshipDefaultValues();
    }
    onRejected: {
      visible = false;
    }
  }

  property EmbeddedFeatureForm embeddedPopup: embeddedPopupLoader.item

  function ensureEmbeddedFormLoaded() {
    if (!embeddedPopupLoader.active) {
      embeddedPopupLoader.active = true;
    }
  }

  Loader {
    id: embeddedPopupLoader
    active: false

    sourceComponent: EmbeddedFeatureForm {
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
        form.model.applyRelationshipDefaultValues();
      }

      onOpened: {
        addingIndicator.running = false;
      }

      onRequestJumpToPoint: function (center, scale, handleMargins) {
        relationEditorBase.requestJumpToPoint(center, scale, handleMargins);
      }
    }
  }

  property Menu childMenu: QfMenu {
    id: childMenu
    title: qsTr("Child Menu")
    z: 10000 // 1000s are embedded feature forms, use a higher value to insure feature form popups always show above embedded feature formes

    property var entryReferencingFeature: undefined
    property string entryDisplayString: ""
    property var entryNmReferencedFeature: undefined
    property string entryNmReferencedFeatureDisplayMessage: ""

    onAboutToShow: {
      atlasMenuLoader.enabled = true;
    }

    topMargin: mainWindow.sceneTopMargin
    bottomMargin: mainWindow.sceneBottomMargin

    MenuItem {
      id: copyChildFeatureAttributes

      font: Theme.defaultFont
      width: parent.width
      height: 48
      leftPadding: Theme.menuItemLeftPadding
      icon.source: Theme.getThemeVectorIcon("ic_copy_black_24dp")

      text: qsTr("Copy Feature Attributes")
      onTriggered: {
        clipboardManager.copyFeatureToClipboard(childMenu.entryReferencingFeature, true);
      }
    }

    MenuItem {
      id: printAtlasChildFeature
      enabled: LayerUtils.isAtlasCoverageLayer(relationEditorModel.relation.referencingLayer)
      visible: enabled

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding
      icon.source: Theme.getThemeVectorIcon("ic_print_black_24dp")

      text: qsTr("Print Atlas Feature to PDF")
      onTriggered: {
        childMenu.close();
        showAtlasMenu();
      }
    }

    MenuItem {
      id: deleteChildFeature
      enabled: true
      visible: isActionEnabled('DeleteChildFeature')

      font: Theme.defaultFont
      width: parent.width
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding
      icon.source: Theme.getThemeVectorIcon("ic_delete_forever_white_24dp")

      text: qsTr("Delete Feature")
      onTriggered: {
        deleteDialog.referencingFeatureId = childMenu.entryReferencingFeature.id;
        deleteDialog.referencingFeatureDisplayMessage = childMenu.entryDisplayString;
        deleteDialog.nmReferencedFeatureId = childMenu.entryNmReferencedFeature !== undefined ? childMenu.entryNmReferencedFeature.id : 0;
        deleteDialog.nmReferencedFeatureDisplayMessage = childMenu.entryNmReferencedFeatureDisplayMessage;
        deleteDialog.visible = true;
      }
    }
  }

  // The loader is used to defer the cost involved in atlas-related item / model generation until the
  // menu is opened in order to avoid extra cost when opening a feature form
  Loader {
    id: atlasMenuLoader
    enabled: false
    sourceComponent: Component {
      QfMenu {
        id: atlasMenu

        property alias printInstantiator: atlasListInstantiator
        property alias printTimer: timer
        property alias printName: timer.printName

        title: qsTr("Print Atlas Feature(s)")

        signal enablePrintItem(int rows)

        topMargin: mainWindow.sceneTopMargin
        bottomMargin: mainWindow.sceneBottomMargin

        MenuItem {
          text: qsTr('Select template below')

          font: Theme.defaultFont
          height: 48
          leftPadding: Theme.menuItemLeftPadding

          enabled: false
        }

        Instantiator {
          id: atlasListInstantiator

          model: PrintLayoutListModel {
            project: qgisProject
            atlasCoverageLayer: relationEditorModel.relation.referencingLayer
          }

          MenuItem {
            text: Title

            font: Theme.defaultFont
            height: 48
            leftPadding: Theme.menuItemLeftPadding

            onTriggered: {
              displayToast(qsTr('Printing...'));
              atlasMenu.printName = Title;
              atlasMenu.printTimer.restart();
            }
          }
          onObjectAdded: (index, object) => {
            atlasMenu.insertItem(index + 1, object);
          }
          onObjectRemoved: (index, object) => {
            atlasMenu.removeItem(object);
          }
        }

        Timer {
          id: timer

          property string printName: ''

          interval: 500
          repeat: false
          onTriggered: {
            var ids = [childMenu.entryReferencingFeature.id];
            if (iface.printAtlasFeatures(printName, ids)) {
              displayToast(qsTr('Atlas feature(s) successfully printed and placed in your project folder'));
            }
          }
        }
      }
    }
  }

  function showAtlasMenu() {
    if (atlasMenuLoader.item.printInstantiator.count > 1) {
      atlasMenuLoader.item.popup(menuButton.x + menuButton.width - atlasMenuLoader.item.width, menuButton.y);
    } else {
      displayToast(qsTr('Printing...'));
      atlasMenuLoader.item.printName = atlasMenuLoader.item.printInstantiator.model.titleAt(0);
      atlasMenuLoader.item.printTimer.restart();
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
    ensureEmbeddedFormLoaded();
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
```


