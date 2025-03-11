/***************************************************************************
                            NavigationBar.qml
                              -------------------
              begin                : 10.12.2014
              copyright            : (C) 2014 by Matthias Kuhn
              email                : matthias (at) opengis.ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Rectangle {
  id: toolBar

  property string title: qsTr('Features')

  property bool multiSelection
  property bool allowDelete

  property MultiFeatureListModel model
  property FeatureListModelSelection selection
  property FeaturelistExtentController extentController

  property double topMargin: 0.0

  signal backClicked
  signal statusIndicatorClicked
  signal statusIndicatorSwiped(var direction)
  signal editAttributesButtonClicked
  signal editGeometryButtonClicked
  signal save
  signal cancel

  signal destinationClicked
  signal moveClicked
  signal duplicateClicked
  signal rotateClicked
  signal transferClicked
  signal deleteClicked

  signal toggleMultiSelection
  signal multiEditClicked
  signal multiMergeClicked
  signal multiMoveClicked
  signal multiDuplicateClicked
  signal multiDeleteClicked
  signal multiProcessingClicked

  signal processingRunClicked

  anchors.top: parent.top
  anchors.left: parent.left
  anchors.right: parent.right
  height: toolBar.topMargin + 48

  clip: true

  states: [
    State {
      name: "Navigation"
    },
    State {
      name: "Indication"
    },
    State {
      name: "Edit"
    }
  ]

  state: "Indication"

  Rectangle {
    id: navigationStatusIndicator
    anchors.fill: parent

    height: toolBar.topMargin + 48

    color: (featureForm.model.constraintsHardValid && featureForm.model.constraintsSoftValid) || toolBar.state !== "Edit" ? Theme.mainColor : !featureForm.model.constraintsHardValid ? Theme.errorColor : Theme.warningColor

    clip: true
    focus: true

    Text {
      // Insure that the text is always visually centered by using the same left and right margi
      property double balancedMargin: Math.max((saveButton.visible ? saveButton.width : 0) + (previousButton.visible ? previousButton.width : 0) + (nextButton.visible ? nextButton.width : 0) + (multiClearButton.visible ? multiClearButton.width : 0), (cancelButton.visible ? cancelButton.width : 0) + (editButton.visible ? editButton.width : 0) + (editGeomButton.visible ? editGeomButton.width : 0) + (multiEditButton.visible ? multiEditButton.width : 0) + (menuButton.visible ? menuButton.width : 0))
      font: Theme.strongFont
      color: Theme.mainOverlayColor
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.top: parent.top
      anchors.leftMargin: 0 + balancedMargin
      anchors.rightMargin: 0 + balancedMargin
      anchors.topMargin: toolBar.topMargin
      height: parent.height - toolBar.topMargin

      text: {
        if (model && selection && selection.focusedItem > -1 && (toolBar.state === 'Navigation' || toolBar.state === 'Edit')) {
          var featurePosition = model.count > 1 ? ((selection.focusedItem + 1) + '/' + model.count + ': ') : '';
          return featurePosition + FeatureUtils.displayName(selection.focusedLayer, selection.focusedFeature);
        } else {
          return toolBar.title;
        }
      }

      horizontalAlignment: Text.AlignHCenter
      verticalAlignment: Text.AlignVCenter
      fontSizeMode: Text.Fit
      wrapMode: Text.Wrap
      elide: Text.ElideRight

      MouseArea {
        anchors.fill: parent

        property real velocity: 0.0
        property int startX: 0
        property int startY: 0
        property int lastX: 0
        property int lastY: 0
        property int distance: 0
        property bool isTracing: false

        onPressed: mouse => {
          startX = mouse.x;
          startY = mouse.y;
          lastX = mouse.x;
          lastY = mouse.y;
          velocity = 0;
          distance = 0;
          isTracing = true;
        }
        onPositionChanged: mouse => {
          if (!isTracing)
            return;
          var currentVelocity = Math.abs(mouse.y - lastY);
          lastX = mouse.x;
          lastY = mouse.y;
          velocity = (velocity + currentVelocity) / 2.0;
          distance = Math.abs(mouse.y - startY);
          isTracing = velocity > 15 && distance > parent.height;
        }
        onReleased: {
          if (!isTracing) {
            toolBar.statusIndicatorSwiped(getDirection());
          } else {
            toolBar.statusIndicatorClicked();
          }
        }

        function getDirection() {
          var diffX = lastX - startX;
          var diffY = lastY - startY;
          if (Math.abs(diffX) > Math.abs(diffY)) {
            return lastX < startX ? 'left' : 'right';
          }
          return lastY < startY ? 'up' : 'down';
        }
      }
    }
  }

  QfToolButton {
    id: nextButton

    anchors.left: previousButton.right
    anchors.top: parent.top
    anchors.topMargin: toolBar.topMargin

    visible: toolBar.state == "Navigation"
    width: visible ? 48 : 0
    height: 48
    clip: true

    iconSource: Theme.getThemeVectorIcon("ic_chevron_right_white_24dp")
    iconColor: Theme.mainOverlayColor

    enabled: (toolBar.state == "Navigation")

    onClicked: {
      if (toolBar.model && (selection.focusedItem + 1) < toolBar.model.count) {
        selection.focusedItem = selection.focusedItem + 1;
      } else {
        selection.focusedItem = -1;
        backClicked();
      }
    }

    Behavior on width  {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }
  }

  QfToolButton {
    id: previousButton

    anchors.left: parent.left
    anchors.top: parent.top
    anchors.topMargin: toolBar.topMargin

    visible: toolBar.state != "Edit" && !toolBar.multiSelection
    width: visible ? 48 : 0
    height: 48
    clip: true

    iconSource: toolBar.state == "Navigation" ? Theme.getThemeVectorIcon("ic_chevron_left_white_24dp") : Theme.getThemeVectorIcon("ic_arrow_left_white_24dp")
    iconColor: Theme.mainOverlayColor

    enabled: toolBar.state != "Edit" && !toolBar.multiSelection

    onClicked: {
      if (toolBar.model && (selection.focusedItem > 0)) {
        selection.focusedItem = selection.focusedItem - 1;
      } else {
        selection.focusedItem = -1;
        backClicked();
      }
    }

    Behavior on width  {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }
  }

  QfToolButton {
    id: saveButton

    anchors.left: parent.left
    anchors.top: parent.top
    anchors.topMargin: toolBar.topMargin

    visible: toolBar.state == "Edit" || toolBar.state == "ProcessingLaunch"
    width: visible ? 48 : 0
    height: 48
    clip: true

    iconSource: Theme.getThemeVectorIcon("ic_check_white_24dp")
    iconColor: Theme.mainOverlayColor

    opacity: featureForm.model.constraintsHardValid ? 1.0 : 0.3
    onClicked: {
      if (toolBar.state == "ProcessingLaunch") {
        processingRunClicked();
      } else {
        if (featureForm.model.constraintsHardValid) {
          toolBar.save();
        } else {
          displayToast("Constraints not valid", 'warning');
        }
      }
    }
    Behavior on width  {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }
  }

  QfToolButton {
    id: cancelButton

    anchors.right: parent.right
    anchors.top: parent.top
    anchors.topMargin: toolBar.topMargin

    visible: !qfieldSettings.autoSave && toolBar.state == "Edit"
    width: visible ? 48 : 0
    height: 48
    clip: true

    iconSource: Theme.getThemeVectorIcon("ic_clear_white_24dp")
    iconColor: Theme.mainOverlayColor

    onClicked: {
      toolBar.cancel();
    }

    Behavior on width  {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }
  }

  QfToolButton {
    id: editGeomButton

    property bool readOnly: false

    visible: stateMachine.state === "digitize" && !selection.focusedGeometry.isNull && !featureForm.model.featureModel.geometryLocked && (projectInfo.editRights || editButton.isCreatedCloudFeature) && toolBar.state == "Navigation" && !readOnly && projectInfo.editRights

    anchors.right: editButton.left
    anchors.top: parent.top
    anchors.topMargin: toolBar.topMargin

    iconSource: Theme.getThemeVectorIcon("ic_edit_geometry_white_24dp")
    iconColor: Theme.mainOverlayColor

    width: visible ? 48 : 0
    height: 48
    clip: true

    onClicked: {
      extentController.zoomToSelected(true);
      toolBar.editGeometryButtonClicked();
    }

    Behavior on width  {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }

    Connections {
      target: selection

      function onFocusedItemChanged() {
        editGeomButton.readOnly = selection.focusedLayer && selection.focusedLayer.readOnly;
      }
    }
  }

  QfToolButton {
    id: editButton

    property bool supportsEditing: false
    property bool isCreatedCloudFeature: false

    anchors.right: menuButton.left
    anchors.top: parent.top
    anchors.topMargin: toolBar.topMargin

    visible: toolBar.state == "Navigation" && supportsEditing && (projectInfo.editRights || isCreatedCloudFeature)
    width: visible ? 48 : 0
    height: 48
    clip: true

    iconSource: Theme.getThemeVectorIcon("ic_edit_attributes_white_24dp")
    iconColor: Theme.mainOverlayColor

    onClicked: {
      toolBar.editAttributesButtonClicked();
    }

    Behavior on width  {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }

    Connections {
      target: selection

      function onFocusedItemChanged() {
        editButton.supportsEditing = selection.focusedLayer && selection.focusedLayer.supportsEditing;
      }
      function onFocusedFeatureChanged() {
        if (QFieldCloudUtils.getProjectId(qgisProject.fileName) != '') {
          editButton.isCreatedCloudFeature = cloudProjectsModel.layerObserver.deltaFileWrapper.isCreatedFeature(selection.focusedLayer, selection.focusedFeature);
        } else {
          editButton.isCreatedCloudFeature = false;
        }
      }
    }
  }

  QfToolButton {
    id: menuButton

    anchors.right: parent.right
    anchors.top: parent.top
    anchors.topMargin: toolBar.topMargin

    visible: toolBar.state != "Edit" && toolBar.state != "Processing" && toolBar.state != "ProcessingLaunch"
    width: visible ? 48 : 0
    height: 48
    clip: true

    iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
    iconColor: Theme.mainOverlayColor

    onClicked: {
      if (toolBar.state == "Indication") {
        featureListMenu.popup(menuButton.x + menuButton.width - featureListMenu.width, menuButton.y);
      } else if (toolBar.state == "Navigation") {
        featureMenu.popup(menuButton.x + menuButton.width - featureMenu.width, menuButton.y);
      }
    }

    Behavior on width  {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }
  }

  QfToolButton {
    id: multiClearButton

    anchors.left: saveButton.right
    anchors.top: parent.top
    anchors.topMargin: toolBar.topMargin

    visible: (toolBar.state == "Processing" || toolBar.state == "ProcessingLaunch" || toolBar.state == "Indication") && toolBar.multiSelection && toolBar.model
    width: visible ? 48 : 0
    height: 48
    clip: true

    iconSource: Theme.getThemeVectorIcon("ic_clear_white_24dp")
    iconColor: Theme.mainOverlayColor

    enabled: (toolBar.multiSelection && toolBar.model)

    onClicked: toggleMultiSelection()

    Behavior on width  {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }
  }

  Text {
    id: multiSelectCount

    anchors.left: multiClearButton.right
    anchors.top: parent.top
    anchors.topMargin: toolBar.topMargin

    width: (toolBar.state == "Indication" && toolBar.multiSelection && toolBar.model ? 48 : 0)
    visible: width > 0
    height: 48
    verticalAlignment: Text.AlignVCenter
    font: Theme.strongFont
    color: Theme.mainOverlayColor

    text: model.selectedFeatures.length < 100 ? model.selectedFeatures.length : '99+'

    enabled: multiClearButton.enabled
  }

  QfToolButton {
    id: multiEditButton

    anchors.right: menuButton.left
    anchors.top: parent.top
    anchors.topMargin: toolBar.topMargin

    visible: toolBar.state == "Indication" && toolBar.model && toolBar.model.canEditAttributesSelection && toolBar.model.selectedCount > 1 && projectInfo.editRights
    width: visible ? 48 : 0
    height: 48
    clip: true

    iconSource: Theme.getThemeVectorIcon("ic_edit_attributes_white_24dp")
    iconColor: Theme.mainOverlayColor

    enabled: toolBar.model && toolBar.model.canEditAttributesSelection && toolBar.model.selectedCount > 1 && projectInfo.editRights

    onClicked: {
      multiEditClicked();
    }

    Behavior on width  {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }
  }

  QfMenu {
    id: featureListMenu
    title: qsTr("Feature List Menu")

    topMargin: mainWindow.sceneTopMargin
    bottomMargin: mainWindow.sceneBottomMargin

    MenuItem {
      text: qsTr('Toggle Feature Selection')

      checkable: true
      checked: toolBar.multiSelection

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding

      onTriggered: {
        toggleMultiSelection();
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr('Print Atlas Feature(s) to PDF')
      icon.source: Theme.getThemeVectorIcon("ic_print_black_24dp")
      enabled: toolBar.model && toolBar.model.selectedCount > 0 && LayerUtils.isAtlasCoverageLayer(toolBar.model.selectedLayer)

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        featureListMenu.close();
        showAtlasMenu();
      }
    }

    MenuSeparator {
      enabled: mergeSelectedFeaturesBtn.visible || moveSelectedFeaturesBtn.visible || duplicateSelectedFeaturesBtn.visible || deleteSelectedFeaturesBtn.visible
      visible: enabled
      width: parent.width
      height: enabled ? undefined : 0
    }

    MenuItem {
      id: mergeSelectedFeaturesBtn
      text: qsTr('Merge Selected Features')
      height: 48
      icon.source: Theme.getThemeVectorIcon("ic_merge_features_white_24dp")
      enabled: toolBar.model && toolBar.model.canMergeSelection && toolBar.model.selectedCount > 1 && projectInfo.editRights

      font: Theme.defaultFont
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: multiMergeClicked()
    }

    MenuItem {
      id: moveSelectedFeaturesBtn
      text: qsTr('Move Selected Feature(s)')
      height: 48
      icon.source: Theme.getThemeVectorIcon("ic_move_white_24dp")
      enabled: toolBar.model && toolBar.model.canMoveSelection && projectInfo.editRights

      font: Theme.defaultFont
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: multiMoveClicked()
    }

    MenuItem {
      id: duplicateSelectedFeaturesBtn
      text: qsTr('Duplicate Selected Feature(s)')
      height: 48
      icon.source: Theme.getThemeVectorIcon("ic_duplicate_black_24dp")
      enabled: toolBar.model && toolBar.model.canDuplicateSelection && projectInfo.insertRights

      font: Theme.defaultFont
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: multiDuplicateClicked()
    }

    MenuItem {
      id: deleteSelectedFeaturesBtn
      text: qsTr('Delete Selected Feature(s)')
      icon.source: Theme.getThemeVectorIcon("ic_delete_forever_white_24dp")
      enabled: toolBar.model && toolBar.model.canDeleteSelection && projectInfo.editRights
      visible: enabled
      height: enabled ? 48 : 0

      font: Theme.defaultFont
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: multiDeleteClicked()
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: processingSelectedFeaturesBtn
      text: qsTr('Process Selected Feature(s)')
      height: 48
      icon.source: Theme.getThemeVectorIcon("ic_processing_black_24dp")
      enabled: toolBar.model && toolBar.model.canProcessSelection && projectInfo.editRights

      font: Theme.defaultFont
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: multiProcessingClicked()
    }
  }

  QfMenu {
    id: featureMenu
    title: qsTr("Feature Menu")

    topMargin: mainWindow.sceneTopMargin
    bottomMargin: mainWindow.sceneBottomMargin

    Row {
      id: featureMenuActionsToolbar
      leftPadding: 2
      rightPadding: 2
      spacing: 2
      height: 48
      clip: true

      property color hoveredColor: Qt.hsla(Theme.mainTextColor.hslHue, Theme.mainTextColor.hslSaturation, Theme.mainTextColor.hslLightness, 0.2)

      QfToolButton {
        anchors.verticalCenter: parent.verticalCenter
        height: 48
        width: 48
        round: true
        iconSource: Theme.getThemeVectorIcon("ic_copy_black_24dp")
        iconColor: enabled ? Theme.mainTextColor : Theme.mainTextDisabledColor
        bgcolor: enabled && hovered ? parent.hoveredColor : "#00ffffff"

        onClicked: {
          clipboardManager.copyFeatureToClipboard(featureForm.model.featureModel.feature, true);
          mainWindow.displayToast(qsTr('Feature attributes copied to clipboard'));
        }
      }

      QfToolButton {
        anchors.verticalCenter: parent.verticalCenter
        height: 48
        width: 48
        round: true
        iconSource: Theme.getThemeVectorIcon("ic_paste_black_24dp")
        iconColor: enabled ? Theme.mainTextColor : Theme.mainTextDisabledColor
        bgcolor: enabled && hovered ? parent.hoveredColor : "#00ffffff"
        enabled: clipboardManager && clipboardManager.holdsFeature

        onClicked: {
          var feature = clipboardManager.pasteFeatureFromClipboard();
          if (featureForm.model.featureModel.updateAttributesFromFeature(feature)) {
            featureForm.model.featureModel.save();
            mainWindow.displayToast(qsTr('Feature attributes updated from clipboard'));
          } else {
            mainWindow.displayToast(qsTr('No feature attributes were updated from clipboard'));
          }
        }
      }

      QfToolButton {
        anchors.verticalCenter: parent.verticalCenter
        height: 48
        width: 48
        round: true
        iconSource: Theme.getThemeVectorIcon("ic_print_black_24dp")
        iconColor: enabled ? Theme.mainTextColor : Theme.mainTextDisabledColor
        bgcolor: enabled && hovered ? parent.hoveredColor : "#00ffffff"
        enabled: LayerUtils.isAtlasCoverageLayer(selection.focusedLayer)

        onClicked: {
          featureMenu.close();
          showAtlasMenu();
        }
      }

      QfToolButton {
        anchors.verticalCenter: parent.verticalCenter
        height: 48
        width: 48
        round: true
        iconSource: Theme.getThemeVectorIcon("ic_navigation_flag_purple_24dp")
        iconColor: enabled ? Theme.mainTextColor : Theme.mainTextDisabledColor
        bgcolor: enabled && hovered ? parent.hoveredColor : "#00ffffff"

        onClicked: {
          featureMenu.close();
          destinationClicked();
          mainWindow.displayToast(qsTr('Feature set as navigation destination'));
        }
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr('Zoom to Feature')
      icon.source: Theme.getThemeVectorIcon("ic_fullscreen_white_24dp")

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: extentController.zoomToSelected()
    }

    MenuItem {
      text: qsTr('Auto-Zoom to Feature')

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding

      checkable: true
      checked: extentController.autoZoom

      onTriggered: extentController.autoZoom = !extentController.autoZoom
    }

    MenuSeparator {
      visible: moveFeatureBtn.visible || duplicateFeatureBtn.visible || deleteFeatureBtn.visible
      width: parent.width
    }

    MenuItem {
      id: moveFeatureBtn
      text: qsTr('Move Feature')
      icon.source: Theme.getThemeVectorIcon("ic_move_white_24dp")
      enabled: ((projectInfo.editRights || editButton.isCreatedCloudFeature) && (!selection.focusedLayer || !featureForm.model.featureModel.geometryLocked))
      visible: enabled

      font: Theme.defaultFont
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: moveClicked()
    }

    MenuItem {
      id: duplicateFeatureBtn
      text: qsTr('Duplicate Feature')
      icon.source: Theme.getThemeVectorIcon("ic_duplicate_black_24dp")
      enabled: (projectInfo.insertRights && (!selection.focusedLayer || !selection.focusedLayer.customProperty("QFieldSync/is_geometry_locked", false)))
      visible: enabled

      font: Theme.defaultFont
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: duplicateClicked()
    }

    MenuItem {
      id: rotateFeatureBtn
      text: qsTr('Rotate Feature')
      icon.source: Theme.getThemeVectorIcon("ic_rotate_white_24dp")
      // allow only rotation for line or polygon or multipoint
      enabled: ((projectInfo.editRights || editButton.isCreatedCloudFeature) && (!selection.focusedLayer || !featureForm.model.featureModel.geometryLocked)) && (selection.focusedLayer !== null && (selection.focusedLayer.geometryType() === 0 || selection.focusedLayer.geometryType() === 1 || selection.focusedLayer.geometryType() === 2))
      visible: enabled

      font: Theme.defaultFont
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: rotateClicked()
    }

    MenuItem {
      id: transferFeatureAttributesBtn
      text: qsTr('Update Attributes From Feature')
      icon.source: Theme.getThemeVectorIcon("ic_transfer_into_black_24dp")
      enabled: (projectInfo.insertRights && (!selection.focusedLayer || !selection.focusedLayer.customProperty("QFieldSync/is_geometry_locked", false)))
      visible: enabled

      font: Theme.defaultFont
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: transferClicked()
    }

    MenuItem {
      id: deleteFeatureBtn
      text: qsTr('Delete Feature')
      icon.source: Theme.getThemeVectorIcon("ic_delete_forever_white_24dp")
      enabled: ((projectInfo.editRights || editButton.isCreatedCloudFeature) && (!selection.focusedLayer || !selection.focusedLayer.customProperty("QFieldSync/is_geometry_locked", false)))
      visible: enabled

      font: Theme.defaultFont
      height: visible ? 48 : 0
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: deleteClicked()
    }
  }

  QfMenu {
    id: atlasMenu

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
        atlasCoverageLayer: toolBar.state === "Indication" ? model.selectedLayer : selection.focusedLayer
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
        var ids = [];
        if (toolBar.state === "Indication") {
          for (var i = 0; i < model.selectedFeatures.length; i++) {
            ids.push(model.selectedFeatures[i].id);
          }
        } else {
          ids.push(selection.focusedFeature.id);
        }
        if (iface.printAtlasFeatures(printName, ids)) {
          displayToast(qsTr('Atlas feature(s) successfully printed and placed in your project folder'));
        }
      }
    }
  }

  function showAtlasMenu() {
    if (atlasListInstantiator.model.rowCount() > 1) {
      atlasMenu.popup(menuButton.x + menuButton.width - atlasMenu.width, menuButton.y);
    } else {
      displayToast(qsTr('Printing...'));
      atlasMenu.printName = atlasListInstantiator.model.titleAt(0);
      atlasMenu.printTimer.restart();
    }
  }
}
