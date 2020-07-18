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

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

Rectangle {
  id: featureForm

  property FeatureListModelSelection selection
  property MapSettings mapSettings
  property color selectionColor
  property alias model: globalFeaturesList.model
  property alias extentController: featureListToolBar.extentController
  property bool allowEdit
  property bool allowDelete

  signal showMessage(string message)
  signal editGeometry

  width: {
      if (props.isVisible) {
          if (qfieldSettings.fullScreenIdentifyView || parent.width < parent.height || parent.width < 300) {
              parent.width
          } else {
              Math.min(Math.max( 200, parent.width / 2.6), parent.width)
          }
      } else { 0 }
  }
  height: {
     if (props.isVisible) {
         if (qfieldSettings.fullScreenIdentifyView || parent.width > parent.height) {
             parent.height
         } else {
             Math.min(Math.max( 200, parent.height / 2 ), parent.height)
         }
     } else { 0 }
  }

  states: [
    State {
      name: "Hidden"
      StateChangeScript {
        script: {
          hide()
          if( featureFormList.state === "Edit" ){
            //e.g. tip on the canvas during an edit
            featureFormList.confirm()
          }
        }
      }
    },
    /* Shows a list of features */
    State {
      name: "FeatureList"
      PropertyChanges {
        target: globalFeaturesList
        shown: true

      }
      PropertyChanges {
        target: featureListToolBar
        state: "Indication"
      }
      StateChangeScript {
        script: {
          show()
          locatorItem.searching = false
          if( featureFormList.state === "Edit" ){
            ///e.g. tip on the canvas during an edit
            featureFormList.confirm()
          }
        }
      }
    },
    /* Shows the form for the currently selected feature */
    State {
      name: "FeatureForm"
      PropertyChanges {
        target: globalFeaturesList
        shown: false
      }
      PropertyChanges {
        target: featureListToolBar
        state: "Navigation"
      }
      PropertyChanges {
        target: featureFormList
        state: "ReadOnly"

      }
    },
    /* Shows an editable form for the currently selected feature */
    State {
      name: "FeatureFormEdit"
      PropertyChanges {
        target: featureListToolBar
        state: "Edit"
      }
      PropertyChanges {
        target: featureFormList
        state: "Edit"
      }
    }

  ]
  state: "Hidden"

  clip: true

  QtObject {
    id: props

    property bool isVisible: false
  }

  ListView {
    id: globalFeaturesList

    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom

    property bool shown: false

    clip: true

    section.property: "layerName"
    section.labelPositioning: ViewSection.CurrentLabelAtStart | ViewSection.InlineLabels
    section.delegate: Component {
      /* section header: layer name */
      Rectangle {
        width: parent.width
        height: 30
        color: "lightGray"

        Text {
          anchors { horizontalCenter: parent.horizontalCenter; verticalCenter: parent.verticalCenter }
          font.bold: true
          text: section
        }
      }
    }

    delegate: Item {
      anchors { left: parent.left; right: parent.right }

      focus: true

      height: Math.max( 48, featureText.height )

      CheckBox {
          anchors { leftMargin: 5; left: parent.left; verticalCenter: parent.verticalCenter }
          checked: featureSelected
          visible: featureForm.selection.model.selectedCount > 0
      }

      Text {
        id: featureText
        anchors { leftMargin: featureForm.selection.model.selectedCount > 0 ? 50 : 10; left: parent.left; right: editRow.left; verticalCenter: parent.verticalCenter }
        font.bold: true
        text: display
      }

      Rectangle {
        anchors.left: parent.left
        height: parent.height
        width: 6
        color: featureForm.selectionColor
        opacity: index == featureForm.selection.focusedItem ? 1 : 0
        Behavior on opacity {
          PropertyAnimation {
            easing.type: Easing.InQuart
          }
        }
      }

      MouseArea {
        anchors.fill: parent

        onClicked: {
          if ( featureForm.selection.model.selectedCount == 0 ) {
            featureFormList.model.featureModel.modelMode = FeatureModel.SingleFeatureModel
            featureForm.selection.focusedItem = index
            featureForm.state = "FeatureForm"
          } else {
            featureForm.selection.toggleSelectedItem( index );
            featureForm.selection.focusedItem = featureForm.selection.model.selectedCount > 0 ? index : -1;
          }
        }

        onPressAndHold:
        {
          featureFormList.model.featureModel.modelMode = FeatureModel.MultiFeatureModel
          featureForm.selection.focusedItem = index
          featureForm.selection.toggleSelectedItem( index );
        }
      }

      /* bottom border */
      Rectangle {
        anchors.bottom: parent.bottom
        height: 1
        color: "lightGray"
        width: parent.width
      }
    }

    /* bottom border */
    Rectangle {
      anchors.bottom: parent.bottom
      height: 1
      color: "lightGray"
      width: parent.width
    }

    onShownChanged: {
      if ( shown )
      {
        height = parent.height - featureListToolBar.height
      }
      else
      {
        height = 0
      }
    }

    Behavior on height {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  FeatureForm {
    id: featureFormList

    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    height: parent.height - globalFeaturesList.height

    model: AttributeFormModel {
      featureModel: FeatureModel {
        currentLayer: featureForm.selection.focusedLayer
        feature: featureForm.selection.focusedFeature
        features: featureForm.selection.model.selectedFeatures
      }
    }

    focus: true

    visible: !globalFeaturesList.shown

  }

  NavigationBar {
    id: featureListToolBar
    allowDelete: allowDelete
    model: globalFeaturesList.model
    selection: featureForm.selection
    extentController: FeaturelistExtentController {
      model: globalFeaturesList.model
      selection: featureForm.selection
      mapSettings: featureForm.mapSettings

      onFeatureFormStateRequested: {
        featureForm.state = "FeatureForm"
      }
    }

    onStatusIndicatorClicked: {
      featureForm.state = "FeatureList"
    }

    onEditAttributesButtonClicked: {
        if( trackingModel.featureInTracking(selection.focusedLayer, selection.focusedFeature.id) )
        {
            displayToast( qsTr( "Stop tracking this feature to edit attributes" ) )
        }
        else
        {
            featureForm.state = "FeatureFormEdit"
        }
    }

    onEditGeometryButtonClicked: {
        if( trackingModel.featureInTracking(selection.focusedLayer, selection.focusedFeature.id) )
        {
            displayToast( qsTr( "Stop tracking this feature to edit geometry" ) )
        }
        else
        {
            editGeometry()
        }
    }

    onSave: {
      featureFormList.confirm()
      featureForm.state = featureForm.selection.model.selectedCount > 0 ? "FeatureList" : "FeatureForm"
      displayToast( qsTr( "Changes saved" ) )
    }

    onCancel: {
      featureFormList.model.featureModel.reset()
      featureForm.state = featureForm.selection.model.selectedCount > 0 ? "FeatureList" : "FeatureForm"
      displayToast( qsTr( "Last changes discarded" ) )
    }

    onMultiEditClicked: {
        if (featureForm.selection.focusedItem == -1) {
          // focus on the first selected item to grab its layer
          featureForm.selection.focusedItem = 0;
        }
        featureForm.state = "FeatureFormEdit"
    }

    onMultiDeleteClicked: {
        if( trackingModel.featureInTracking(featureForm.selection.focusedLayer, featureForm.selection.model.selectedFeatures) )
        {
          displayToast( qsTr( "A number of features are being tracked, stop tracking to delete those" ) )
        }
        else
        {
          deleteDialog.show()
        }
    }
  }

  Keys.onReleased: {
    if ( event.key === Qt.Key_Back ||
        event.key === Qt.Key_Escape ) {
      if( state != "FeatureList" ) {
        if( featureListToolBar.state === "Edit"){
          if( featureFormList.model.constraintsHardValid ) {
            featureListToolBar.save()
          } else {
            displayToast( "Constraints not valid" )
            if( qfieldSettings.autoSave ){
               featureListToolBar.cancel()
            }
          }
        }else{
          state = "FeatureList"
        }
      }else{
        state = "Hidden"
      }
      event.accepted = true
    }
  }

  Behavior on width {
    PropertyAnimation {
      duration: parent.width > parent.height ? 250 : 0
      easing.type: Easing.InQuart

      onRunningChanged: {
        if ( running )
          mapCanvasMap.freeze('formresize')
        else
          mapCanvasMap.unfreeze('formresize')
      }
    }
  }

  Behavior on height {
    PropertyAnimation {
      duration: parent.width < parent.height ? 250 : 0
      easing.type: Easing.InQuart

      onRunningChanged: {
        if ( running )
          mapCanvasMap.freeze('formresize')
        else
          mapCanvasMap.unfreeze('formresize')
      }
    }
  }

  Connections {
    target: globalFeaturesList.model

    onRowsInserted: {
      if ( model.rowCount() > 0 ) {
        state = "FeatureList"
      } else {
        showMessage( qsTr('No feature at this position') )
        state = "Hidden"
      }
    }

    onModelReset: {
      if ( model.rowCount() > 0 ) {
        state = "FeatureList"
      } else {
        state = "Hidden"
      }
    }
  }

  function show()
  {
    props.isVisible = true
    focus = true
  }

  function hide()
  {
    props.isVisible = false
    focus = false

    featureForm.selection.clear()

    model.clear()
  }

  MessageDialog {
    id: deleteDialog

    property int selectedCount
    property bool isDeleted

    visible: false

    title: qsTr( "Delete feature(s)" )
    text: qsTr( "Should the %n feature(s) selected really be deleted?", "0", selectedCount )
    standardButtons: StandardButton.Ok | StandardButton.Cancel
    onAccepted: {
      if ( isDeleted ) {
        return;
      }

      isDeleted = featureForm.model.deleteSelection()

      if ( isDeleted ) {
        displayToast( qsTr( "Successfully deleted %n feature(s)", "", selectedCount ) );
      } else {
        displayToast( qsTr( "Failed to delete %n feature(s)", "", selectedCount ) );
      }

      visible = false
    }
    onRejected: {
      visible = false
    }

    function show() {
        this.isDeleted = false;
        this.selectedCount = featureForm.model.selectedCount;
        this.open();
    }
  }

  //if project changed we should hide drawer in case it's still open with old values
  //it pedals back, "simulates" a cancel without touching anything, but does not reset the model
  Connections {
    target: qgisProject

    onLayersWillBeRemoved: {
        if( state != "FeatureList" ) {
          if( featureListToolBar.state === "Edit"){
              featureForm.state = "FeatureForm"
              displayToast( qsTr( "Changes discarded" ) )
          }
          state = "FeatureList"
        }
        state = "Hidden"
    }
  }

}
