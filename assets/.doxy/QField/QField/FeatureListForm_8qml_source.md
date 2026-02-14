

# File FeatureListForm.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**FeatureListForm.qml**](FeatureListForm_8qml.md)

[Go to the documentation of this file](FeatureListForm_8qml.md)


```C++
/***************************************************************************
                            FeatureListForm.qml
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
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qgis
import org.qfield
import Theme

Pane {
  id: featureFormList

  property ProcessingAlgorithm algorithm: processingAlgorithm

  property FeatureListModelSelection selection
  property MapSettings mapSettings
  property DigitizingToolbar digitizingToolbar
  property ConfirmationToolbar moveFeaturesToolbar
  property ConfirmationToolbar rotateFeaturesToolbar
  property CodeReader codeReader

  property color selectionColor
  property alias model: globalFeaturesList.model
  property alias extentController: featureListToolBar.extentController

  property bool allowEdit
  property bool allowDelete

  property bool multiSelection: false
  property bool fullScreenView: qfieldSettings.fullScreenIdentifyView
  property bool isVertical: parent.width < parent.height || parent.width < 300

  property bool isDragging: false
  property real dragHeightAdjustment: 0
  property real dragWidthAdjustment: 0

  property bool canvasOperationRequested: digitizingToolbar.geometryRequested || moveFeaturesToolbar.moveFeaturesRequested || rotateFeaturesToolbar.rotateFeaturesRequested

  signal showMessage(string message)
  signal editGeometry
  signal requestJumpToPoint(var center, real scale, bool handleMargins)

  function requestCancel() {
    featureForm.requestCancel();
  }

  property real lastWidth

  width: {
    if (props.isVisible || featureFormList.canvasOperationRequested) {
      if (dragWidthAdjustment != 0) {
        return lastWidth - dragWidthAdjustment;
      } else if (fullScreenView || parent.width <= parent.height || width >= 0.95 * parent.width) {
        lastWidth = parent.width;
        return parent.width;
      } else {
        const newWidth = Math.min(Math.max(200, parent.width / 2.25), parent.width);
        lastWidth = newWidth;
        return newWidth;
      }
    } else {
      lastWidth = 0;
      return 0;
    }
  }
  property real lastHeight

  height: {
    if (props.isVisible || featureFormList.canvasOperationRequested) {
      if (dragHeightAdjustment != 0) {
        return Math.min(lastHeight - dragHeightAdjustment, parent.height - mainWindow.sceneTopMargin);
      } else if (fullScreenView || parent.width > parent.height || height >= 0.95 * parent.height) {
        lastHeight = parent.height;
        return parent.height;
      } else {
        const defaultMin = Math.min(Math.max(200, parent.height / 2), parent.height);
        var minContentHeight = featureForm.visible ? defaultMin : featureListToolBar.height + (globalFeaturesList.contentHeight + globalFeaturesList.anchors.bottomMargin) + 25;
        const newHeight = Math.min(minContentHeight, defaultMin);
        lastHeight = newHeight;
        return newHeight;
      }
    } else {
      lastHeight = 0;
      return 0;
    }
  }

  anchors.bottomMargin: featureFormList.canvasOperationRequested ? featureFormList.height : 0
  anchors.rightMargin: featureFormList.canvasOperationRequested ? -featureFormList.width : 0
  opacity: featureFormList.canvasOperationRequested ? 0.5 : 1

  topPadding: 0
  leftPadding: 0
  rightPadding: 0
  bottomPadding: 0

  enabled: !featureFormList.canvasOperationRequested
  visible: props.isVisible

  states: [
    State {
      name: "Hidden"
      StateChangeScript {
        script: {
          hide();
          if (featureForm.state === "Edit") {
            //e.g. tip on the canvas during an edit
            featureForm.confirm();
          }
        }
      }
    },
    /* Shows a list of features */
    State {
      name: "FeatureList"
      PropertyChanges {
        target: globalFeaturesList
        visible: true
      }
      PropertyChanges {
        target: featureForm
        visible: false
      }
      PropertyChanges {
        target: processingAlgorithmsList
        visible: false
      }
      PropertyChanges {
        target: processingAlgorithmForm
        visible: false
      }
      PropertyChanges {
        target: featureListToolBar
        state: "Indication"
      }
      StateChangeScript {
        script: {
          show();
          locatorItem.state = "off";
          if (featureForm.state === "Edit") {
            featureForm.confirm();
          }
          featureListToolBar.title = qsTr('Features');
        }
      }
    },
    /* Shows the form for the currently selected feature */
    State {
      name: "FeatureForm"
      PropertyChanges {
        target: globalFeaturesList
        visible: false
      }
      PropertyChanges {
        target: featureForm
        visible: true
      }
      PropertyChanges {
        target: featureListToolBar
        state: "Navigation"
      }
      PropertyChanges {
        target: featureForm
        state: "ReadOnly"
      }
    },
    /* Shows an editable form for the currently selected feature */
    State {
      name: "FeatureFormEdit"
      PropertyChanges {
        target: globalFeaturesList
        visible: false
      }
      PropertyChanges {
        target: featureForm
        visible: true
      }
      PropertyChanges {
        target: featureListToolBar
        state: "Edit"
      }
      PropertyChanges {
        target: featureForm
        state: "Edit"
      }
    },
    /* Show a list of processing algorithms compatible with the selected feature(s) */
    State {
      name: "ProcessingAlgorithmsList"
      PropertyChanges {
        target: processingAlgorithmsList
        visible: true
      }
      PropertyChanges {
        target: processingAlgorithmForm
        visible: false
      }
      PropertyChanges {
        target: globalFeaturesList
        visible: false
      }
      PropertyChanges {
        target: featureListToolBar
        state: "Processing"
      }
      StateChangeScript {
        script: {
          featureListToolBar.title = qsTr('Processing Algorithms');
        }
      }
    },
    State {
      name: "ProcessingAlgorithmForm"
      PropertyChanges {
        target: processingAlgorithmsList
        visible: false
      }
      PropertyChanges {
        target: processingAlgorithmForm
        visible: true
      }
      PropertyChanges {
        target: featureListToolBar
        state: "ProcessingLaunch"
      }
      StateChangeScript {
        script: {
          featureListToolBar.title = processingAlgorithmForm.algorithmDisplayName;
          featureListToolBar.title = processingAlgorithmForm.algorithmDisplayName;
        }
      }
    }
  ]
  state: "Hidden"
  clip: true

  WheelHandler {
    acceptedDevices: PointerDevice.AllDevices
    onWheel: {}
  }

  QtObject {
    id: props

    property bool isVisible: false
  }

  ListView {
    id: globalFeaturesList

    clip: true
    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.leftMargin: featureFormList.x == 0 ? mainWindow.sceneLeftMargin : 0
    anchors.right: parent.right
    anchors.rightMargin: mainWindow.sceneRightMargin
    anchors.bottom: parent.bottom
    anchors.bottomMargin: mainWindow.sceneBottomMargin
    height: parent.height - featureListToolBar.height
    visible: false
    ScrollBar.vertical: QfScrollBar {}

    section.property: "layerName"
    section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
    section.delegate: Component {
      /* section header: layer name */
      Rectangle {
        width: parent.width
        height: 30
        color: Theme.controlBorderColor

        Text {
          anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
          }
          font.bold: true
          font.pointSize: Theme.resultFont.pointSize
          color: Theme.mainTextColor
          text: section
        }
      }
    }

    delegate: Rectangle {
      id: itemBackground
      anchors {
        left: parent ? parent.left : undefined
        right: parent ? parent.right : undefined
      }
      height: Math.max(48, featureText.height)
      color: "transparent"

      Ripple {
        clip: true
        width: parent.width
        height: parent.height
        pressed: mouseArea.pressed
        anchor: itemBackground
        active: mouseArea.pressed
        color: Material.rippleColor
      }

      CheckBox {
        anchors {
          leftMargin: 5
          left: parent.left
          verticalCenter: parent.verticalCenter
        }
        checked: featureSelected
        visible: featureFormList.multiSelection
      }

      Text {
        id: featureText
        anchors {
          leftMargin: featureFormList.multiSelection ? 50 : 10
          rightMargin: 10
          left: parent.left
          right: parent.right
          verticalCenter: parent.verticalCenter
        }
        font.bold: true
        font.pointSize: Theme.resultFont.pointSize
        color: Theme.mainTextColor
        text: display
        wrapMode: Text.WordWrap
      }

      Rectangle {
        anchors.left: parent.left
        height: parent.height
        width: 6
        color: featureFormList.selectionColor
        opacity: index == featureFormList.selection.focusedItem && featureFormList.selection.model.selectedCount == 0 ? 1 : 0
        Behavior on opacity {
          PropertyAnimation {
            easing.type: Easing.OutQuart
          }
        }
      }

      MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
          if (featureFormList.multiSelection) {
            featureFormList.selection.toggleSelectedItem(index);
            if (featureFormList.selection.model.selectedCount == 0) {
              featureForm.model.featureModel.modelMode = FeatureModel.SingleFeatureModel;
              featureFormList.multiSelection = false;
            }
            featureFormList.selection.focusedItem = featureFormList.selection.model.selectedCount > 0 ? index : -1;
          } else {
            featureForm.model.featureModel.modelMode = FeatureModel.SingleFeatureModel;
            featureFormList.state = "FeatureForm";
            featureFormList.selection.focusedItem = index;
            featureFormList.multiSelection = false;
          }
          featureForm.model.applyFeatureModel();
        }

        onPressAndHold: {
          featureForm.model.featureModel.modelMode = FeatureModel.MultiFeatureModel;
          featureFormList.selection.focusedItem = index;
          featureFormList.selection.toggleSelectedItem(index);
          featureFormList.multiSelection = true;
        }
      }

      /* bottom border */
      Rectangle {
        anchors.bottom: parent.bottom
        height: 1
        color: Theme.controlBorderColor
        width: parent.width
      }
    }

    function sectionCount() {
      let sections = {};
      for (let i = 0; i < globalFeaturesList.model.count; ++i) {
        const idx = globalFeaturesList.model.index(i, 0);
        let sectionVal = globalFeaturesList.model.data(idx, MultiFeatureListModel.LayerNameRole);
        sections[sectionVal] = true;
      }
      return Object.keys(sections).length;
    }

    /* bottom border */
    Rectangle {
      anchors.bottom: parent.bottom
      height: 1
      color: Theme.controlBorderColor
      width: parent.width
    }

    Behavior on height {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }
  }

  FeatureForm {
    id: featureForm

    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    leftMargin: featureFormList.x == 0 ? mainWindow.sceneLeftMargin : 0
    rightMargin: mainWindow.sceneRightMargin
    bottomMargin: mainWindow.sceneBottomMargin
    height: parent.height - globalFeaturesList.height
    visible: false
    isDraggable: true

    digitizingToolbar: featureFormList.digitizingToolbar
    codeReader: featureFormList.codeReader

    model: AttributeFormModel {
      featureModel: FeatureModel {
        project: qgisProject
        currentLayer: featureFormList.selection.focusedLayer
        feature: featureFormList.selection.focusedFeature
        features: featureFormList.selection.model.selectedFeatures
        appExpressionContextScopesGenerator: appScopesGenerator
      }
    }

    onRequestJumpToPoint: function (center, scale, handleMargins) {
      featureFormList.requestJumpToPoint(center, scale, handleMargins);
    }

    onCancelled: {
      featureFormList.selection.focusedItemChanged();
      featureForm.model.featureModel.reset();
      featureFormList.state = featureFormList.selection.model.selectedCount > 0 ? "FeatureList" : "FeatureForm";
      if (!qfieldSettings.autoSave) {
        displayToast(qsTr("Changes discarded"), 'warning');
      }
    }
  }

  ProcessingAlgorithmsList {
    id: processingAlgorithmsList

    inPlaceLayer: featureFormList.selection.model.selectedLayer

    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.bottomMargin: mainWindow.sceneBottomMargin
    visible: false

    onAlgorithmSelected: id => {
      processingAlgorithm.id = id;
      featureFormList.state = "ProcessingAlgorithmForm";
    }
  }

  ProcessingAlgorithmForm {
    id: processingAlgorithmForm

    inPlaceLayer: processingAlgorithm.inPlaceLayer
    algorithmId: processingAlgorithm.id

    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.bottomMargin: mainWindow.sceneBottomMargin
    visible: false
  }

  ProcessingAlgorithm {
    id: processingAlgorithm

    parameters: processingAlgorithmForm.algorithmParametersModel.parameters

    inPlaceLayer: featureFormList.selection.model.selectedLayer
    inPlaceFeatures: featureFormList.selection.model.selectedFeatures

    preview: featureFormList.state == "ProcessingAlgorithmForm"
  }

  NavigationBar {
    id: featureListToolBar

    topMargin: featureFormList.y == 0 ? mainWindow.sceneTopMargin : 0.0
    leftMargin: featureFormList.x == 0 ? mainWindow.sceneLeftMargin : 0.0
    rightMargin: mainWindow.sceneRightMargin

    allowDelete: allowDelete
    model: globalFeaturesList.model
    selection: featureFormList.selection
    multiSelection: featureFormList.multiSelection
    isVertical: featureListForm.isVertical
    extentController: FeaturelistExtentController {
      model: globalFeaturesList.model
      selection: featureFormList.selection
      mapSettings: featureFormList.mapSettings
      autoZoom: qfieldSettings.autoZoomToIdentifiedFeature

      onFeatureFormStateRequested: {
        featureFormList.state = "FeatureForm";
      }

      onRequestJumpToPoint: function (center, scale, handleMargins) {
        featureFormList.requestJumpToPoint(center, scale, handleMargins);
      }
    }

    onBackClicked: {
      featureFormList.focus = true;
      if (featureFormList.state !== "FeatureList") {
        featureFormList.state = "FeatureList";
      } else {
        featureFormList.state = "Hidden";
      }
    }

    onStatusIndicatorClicked: {
      featureFormList.selection.focusedItem = -1;
      featureFormList.state = "FeatureList";
    }

    onStatusIndicatorDragged: function (deltaX, deltaY) {
      fullScreenView = false;
      if (isVertical) {
        dragHeightAdjustment += deltaY;
      } else {
        dragWidthAdjustment += deltaX;
      }
    }

    onStatusIndicatorDragAcquired: {
      isDragging = true;
    }

    onStatusIndicatorDragReleased: {
      isDragging = false;
      if (isVertical) {
        const minContentHeight = featureListToolBar.height + 48 + 30;
        if (featureFormList.height < minContentHeight) {
          if (fullScreenView) {
            fullScreenView = false;
          } else {
            if (featureFormList.state !== 'FeatureFormEdit') {
              featureFormList.state = 'Hidden';
            }
          }
        } else if (dragHeightAdjustment < -parent.height * 0.2) {
          fullScreenView = true;
        }
      } else {
        if (featureFormList.width < featureFormList.parent.width * 0.3) {
          if (fullScreenView) {
            fullScreenView = false;
          } else {
            if (featureFormList.state != 'FeatureFormEdit') {
              featureFormList.state = 'Hidden';
            }
          }
        } else if (dragWidthAdjustment < -parent.width * 0.2) {
          fullScreenView = true;
        }
      }
      dragHeightAdjustment = 0;
      dragWidthAdjustment = 0;
    }

    onEditAttributesButtonClicked: {
      if (trackingModel.featureInTracking(selection.focusedLayer, selection.focusedFeature.id)) {
        displayToast(qsTr("Stop tracking this feature to edit attributes"));
      } else {
        featureFormList.state = "FeatureFormEdit";
      }
    }

    onEditGeometryButtonClicked: {
      if (trackingModel.featureInTracking(selection.focusedLayer, selection.focusedFeature.id)) {
        displayToast(qsTr("Stop tracking this feature to edit geometry"));
      } else {
        editGeometry();
      }
    }

    onSave: {
      featureForm.confirm();
      featureFormList.state = featureFormList.selection.model.selectedCount > 0 ? "FeatureList" : "FeatureForm";
      displayToast(qsTr("Changes saved"));
    }

    onCancel: {
      featureFormList.requestCancel();
    }

    onDestinationClicked: {
      navigation.setDestinationFeature(featureFormList.selection.focusedFeature, featureFormList.selection.focusedLayer);
      featureFormList.state = "Hidden";
    }

    onMoveClicked: {
      if (featureFormList.selection.focusedItem !== -1) {
        featureFormList.state = "FeatureList";
        featureFormList.multiSelection = true;
        featureFormList.selection.model.toggleSelectedItem(featureFormList.selection.focusedItem);
        moveFeaturesToolbar.initializeMoveFeatures();
      }
    }

    onDuplicateClicked: {
      if (featureFormList.selection.model.duplicateFeature(featureFormList.selection.focusedLayer, featureFormList.selection.focusedFeature)) {
        displayToast(qsTr("Successfully duplicated feature"));
        featureFormList.selection.focusedItem = -1;
        featureFormList.state = "FeatureList";
        featureFormList.multiSelection = true;
        moveFeaturesToolbar.initializeMoveFeatures();
      }
    }

    onRotateClicked: {
      if (featureFormList.selection.focusedItem !== -1) {
        featureFormList.state = "FeatureList";
        featureFormList.multiSelection = true;
        featureFormList.selection.model.toggleSelectedItem(featureFormList.selection.focusedItem);
        rotateFeaturesToolbar.initializeRotateFeatures();
      }
    }

    onTransferClicked: {
      transferDialog.show();
    }

    onDeleteClicked: {
      var selectedFeatures = featureFormList.selection.model.selectedFeatures;
      var selectedFeature = selectedFeatures && selectedFeatures.length > 0 ? selectedFeatures[0] : null;
      if (selectedFeature && featureFormList.selection.focusedLayer && trackingModel.featureInTracking(featureFormList.selection.focusedLayer, selectedFeature)) {
        displayToast(qsTr("A number of features are being tracked, stop tracking to delete those"));
      } else {
        deleteDialog.show();
      }
    }

    onToggleMultiSelection: {
      featureFormList.selection.focusedItem = -1;
      if (featureFormList.multiSelection) {
        if (featureFormList.state == "ProcessingAlgorithmsList" || featureFormList.state == "ProcessingAlgorithmForm") {
          featureFormList.state = "FeatureList";
        }
        featureForm.model.featureModel.modelMode = FeatureModel.SingleFeatureModel;
        featureForm.model.applyFeatureModel();
        featureFormList.selection.model.clearSelection();
      } else {
        featureForm.model.featureModel.modelMode = FeatureModel.MultiFeatureModel;
      }
      featureFormList.multiSelection = !featureFormList.multiSelection;
      featureFormList.focus = true;
    }

    onMultiEditClicked: {
      if (featureFormList.selection.focusedItem == -1) {
        // focus on the first selected item to grab its layer
        featureFormList.selection.focusedItem = 0;
      }
      featureFormList.state = "FeatureFormEdit";
    }

    onMultiMergeClicked: {
      if (trackingModel.featureInTracking(featureFormList.selection.focusedLayer, featureFormList.selection.model.selectedFeatures)) {
        displayToast(qsTr("A number of features are being tracked, stop tracking to merge those"));
      } else {
        mergeDialog.show();
      }
    }

    onMultiMoveClicked: {
      moveFeaturesToolbar.initializeMoveFeatures();
    }

    onMultiProcessingClicked: {
      featureFormList.state = "ProcessingAlgorithmsList";
    }

    onProcessingRunClicked: {
      processingAlgorithm.run();
      if (globalFeaturesList.model.count > 0) {
        featureFormList.state = "FeatureList";
      } else {
        featureFormList.state = "Hidden";
      }
    }

    onProcessingFeatureClicked: {
      if (featureFormList.selection.focusedItem !== -1) {
        featureFormList.state = "FeatureList";
        featureFormList.multiSelection = true;
        featureFormList.selection.model.toggleSelectedItem(featureFormList.selection.focusedItem);
        featureFormList.state = "ProcessingAlgorithmsList";
      }
    }

    CoordinateTransformer {
      id: moveFeaturesTransformer
      sourceCrs: mapCanvas.mapSettings.destinationCrs
      destinationCrs: featureFormList.selection.model.selectedLayer ? featureFormList.selection.model.selectedLayer.crs : mapCanvas.mapSettings.destinationCrs
    }

    Connections {
      target: moveFeaturesToolbar

      function onMoveConfirmed() {
        moveFeaturesTransformer.sourcePosition = moveFeaturesToolbar.endPoint;
        var translateX = moveFeaturesTransformer.projectedPosition.x;
        var translateY = moveFeaturesTransformer.projectedPosition.y;
        moveFeaturesTransformer.sourcePosition = moveFeaturesToolbar.startPoint;
        translateX -= moveFeaturesTransformer.projectedPosition.x;
        translateY -= moveFeaturesTransformer.projectedPosition.y;
        featureFormList.model.moveSelection(translateX, translateY);
        moveFeaturesToolbar.startPoint = undefined;
        moveFeaturesToolbar.endPoint = undefined;
      }
    }

    Connections {
      target: rotateFeaturesToolbar

      function onRotateConfirmed() {
        featureFormList.model.rotateSelection(rotateFeaturesToolbar.angle);
      }
    }

    onMultiDuplicateClicked: {
      if (featureFormList.multiSelection) {
        if (featureFormList.model.duplicateSelection()) {
          displayToast(qsTr("Successfully duplicated selected features, list updated to show newly-created features"));
          moveFeaturesToolbar.initializeMoveFeatures();
        }
      }
    }

    onMultiDeleteClicked: {
      if (trackingModel.featuresInTracking(featureFormList.selection.focusedLayer, featureFormList.selection.model.selectedFeatures)) {
        displayToast(qsTr("A number of features are being tracked, stop tracking to delete those"));
      } else {
        deleteDialog.show();
      }
    }
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      // if visible overlays (such as embedded feature forms) are present, don't take over
      if (Overlay.overlay && Overlay.overlay.visibleChildren.length > 1 || (Overlay.overlay.visibleChildren.length === 1 && !toast.visible)) {
        return;
      }
      if (state != "FeatureList") {
        if (featureListToolBar.state === "Edit") {
          featureForm.requestCancel();
        } else {
          state = "FeatureList";
        }
      } else {
        if (featureFormList.multiSelection) {
          featureFormList.selection.model.clearSelection();
          featureFormList.selection.focusedItem = -1;
          featureFormList.multiSelection = false;
        } else {
          state = "Hidden";
        }
      }
      event.accepted = true;
    }
  }

  Behavior on width {
    enabled: !isDragging
    PropertyAnimation {
      duration: parent.width > parent.height ? 250 : 0
      easing.type: Easing.OutQuart

      onRunningChanged: {
        if (running)
          mapCanvasMap.freeze('formresize');
        else
          mapCanvasMap.unfreeze('formresize');
      }
    }
  }

  Behavior on height {
    enabled: !isDragging
    PropertyAnimation {
      duration: parent.width < parent.height ? 250 : 0
      easing.type: Easing.OutQuart

      onRunningChanged: {
        if (running)
          mapCanvasMap.freeze('formresize');
        else
          mapCanvasMap.unfreeze('formresize');
      }
    }
  }

  Behavior on anchors.rightMargin {
    PropertyAnimation {
      duration: 250
      easing.type: Easing.OutQuart

      onRunningChanged: {
        if (running)
          mapCanvasMap.freeze('formresize');
        else
          mapCanvasMap.unfreeze('formresize');
      }
    }
  }

  Behavior on anchors.bottomMargin {
    PropertyAnimation {
      duration: 250
      easing.type: Easing.OutQuart

      onRunningChanged: {
        if (running)
          mapCanvasMap.freeze('formresize');
        else
          mapCanvasMap.unfreeze('formresize');
      }
    }
  }

  Connections {
    target: globalFeaturesList.model

    function onRowsInserted(parent, first, VectorLayerStatic) {
      if (model.rowCount() > 0) {
        state = "FeatureList";
      } else {
        showMessage(qsTr('No feature at this position'));
        state = "Hidden";
      }
    }

    function onCountChanged() {
      if (model.rowCount() === 0) {
        state = "Hidden";
      }
    }

    function onModelReset() {
      if (model.rowCount() > 0) {
        state = "FeatureList";
      } else {
        state = "Hidden";
      }
    }
  }

  function show() {
    props.isVisible = true;
    focus = true;
  }

  function hide() {
    props.isVisible = false;
    focus = false;
    fullScreenView = qfieldSettings.fullScreenIdentifyView;
    if (!featureFormList.canvasOperationRequested) {
      featureFormList.multiSelection = false;
      featureForm.model.featureModel.modelMode = FeatureModel.SingleFeatureModel;
      featureFormList.selection.clear();
      if (featureFormList.selection.model) {
        featureFormList.selection.model.clearSelection();
      }
      model.clear();
    }
  }

  QfDialog {
    id: mergeDialog
    parent: mainWindow.contentItem

    property int selectedCount: 0
    property string featureDisplayName: ''
    property bool isMerged: false

    title: qsTr("Merge feature(s)")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("Should the %n feature(s) selected really be merge?\n\nThe features geometries will be combined into feature '%1', which will keep its attributes.", "0", mergeDialog.selectedCount).arg(mergeDialog.featureDisplayName)
    }

    onAccepted: {
      if (isMerged) {
        return;
      }
      isMerged = featureFormList.model.mergeSelection();
      if (isMerged) {
        displayToast(qsTr("Successfully merged %n feature(s)", "", selectedCount));
      } else {
        displayToast(qsTr("Failed to merge %n feature(s)", "", selectedCount), 'warning');
      }
      visible = false;
      featureFormList.focus = true;
    }
    onRejected: {
      visible = false;
      featureFormList.focus = true;
    }

    function show() {
      this.isMerged = false;
      this.selectedCount = featureFormList.model.selectedCount;
      this.featureDisplayName = FeatureUtils.displayName(featureFormList.selection.focusedLayer, featureFormList.model.selectedFeatures[0]);
      this.open();
    }
  }

  QfDialog {
    id: transferDialog
    parent: mainWindow.contentItem
    title: qsTr("Transfer Feature Attributes")

    Column {
      width: childrenRect.width
      height: childrenRect.height
      spacing: 10

      TextMetrics {
        id: transferLabelMetrics
        font: transferLabel.font
        text: transferLabel.text
      }

      Label {
        id: transferLabel
        width: mainWindow.width - 60 < transferLabelMetrics.width ? mainWindow.width - 60 : transferLabelMetrics.width
        text: qsTr("Select a feature below from which attributes will be transfered onto the currently opened feature.")
        wrapMode: Text.WordWrap
        font: Theme.defaultFont
        color: Theme.mainTextColor
      }

      QfComboBox {
        id: transferComboBox
        width: transferLabel.width

        model: FeatureListModel {
          id: transferFeatureListModel
        }

        textRole: "displayString"
        valueRole: "featureId"
      }
    }

    onAccepted: {
      let feature = transferFeatureListModel.getFeatureById(transferComboBox.currentValue);
      if (featureForm.model.featureModel.updateAttributesFromFeature(feature)) {
        featureForm.model.featureModel.save();
        mainWindow.displayToast(qsTr('Feature attributes transferred'));
      } else {
        mainWindow.displayToast(qsTr('No feature attributes were transferred'));
      }
      transferFeatureListModel.currentLayer = null;
    }

    onRejected: {
      transferFeatureListModel.currentLayer = null;
    }

    function show() {
      transferFeatureListModel.displayValueField = featureForm.model.featureModel.currentLayer.displayExpression;
      transferFeatureListModel.currentLayer = featureForm.model.featureModel.currentLayer;
      this.open();
    }
  }

  QfDialog {
    id: deleteDialog
    parent: mainWindow.contentItem

    property int selectedCount: 0
    property bool isDeleted: false

    title: qsTr("Delete feature(s)")
    Label {
      width: parent.width
      wrapMode: Text.WordWrap
      text: qsTr("Should the %n feature(s) selected really be deleted?", "0", deleteDialog.selectedCount)
    }
    onAccepted: {
      if (isDeleted) {
        return;
      }
      if (featureFormList.multiSelection) {
        isDeleted = featureFormList.model.deleteSelection();
      } else {
        isDeleted = featureFormList.selection.model.deleteFeature(featureFormList.selection.focusedLayer, featureFormList.selection.focusedFeature.id);
      }
      if (isDeleted) {
        displayToast(qsTr("Successfully deleted %n feature(s)", "", selectedCount));
        if (!featureFormList.multiSelection) {
          featureFormList.selection.focusedItem = -1;
          featureFormList.state = "FeatureList";
        }
        if (featureFormList.selection.model.count === 0)
          featureFormList.state = "Hidden";
      } else {
        displayToast(qsTr("Failed to delete %n feature(s)", "", selectedCount), 'error');
      }
      visible = false;
      featureFormList.focus = true;
    }
    onRejected: {
      visible = false;
      featureFormList.focus = true;
    }

    function show() {
      this.isDeleted = false;
      this.selectedCount = featureFormList.multiSelection ? featureFormList.model.selectedCount : 1;
      this.open();
    }
  }

  //if project changed we should hide drawer in case it's still open with old values
  //it pedals back, "simulates" a cancel without touching anything, but does not reset the model
  Connections {
    target: qgisProject

    function onLayersWillBeRemoved(layerIds) {
      if (state != "FeatureList") {
        if (featureListToolBar.state === "Edit") {
          featureFormList.state = "FeatureForm";
          displayToast(qsTr("Changes discarded"), 'warning');
        }
        state = "FeatureList";
      }
      state = "Hidden";
    }
  }
}
```


