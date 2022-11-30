import QtQuick 2.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import '..'


VisibilityFadingRow {
  id: vertexEditorToolbar

  signal finished()

  property FeatureModel featureModel
  property MapSettings mapSettings

  property bool screenHovering: false //<! if the stylus pen is used, one should not use the add button
  property bool vertexRubberbandVisible: true
  property int currentVertexId: -1
  property bool currentVertexModified: false

  readonly property bool blocking: featureModel.vertexModel.dirty

  spacing: 4

  function init(featureModel, mapSettings, editorRubberbandModel)
  {
    vertexEditorToolbar.featureModel = featureModel
    vertexEditorToolbar.mapSettings = mapSettings
  }

  function cancel()
  {
    featureModel.vertexModel.editingMode = VertexModel.NoEditing
    featureModel.vertexModel.reset()
  }

  function applyChanges( apply ) {
    if ( apply && featureModel.vertexModel.dirty ){
      featureModel.applyVertexModelToGeometry()

      if ( ! featureModel.save() ) {
        displayToast( qsTr( "Failed to save feature!" ), 'error' );
      }

      //set the vertexModel original geometry to the one of the updated feature
      featureModel.vertexModel.updateGeometry( featureModel.feature.geometry )
    }
  }

  function canvasClicked(point)
  {
    if ( featureModel.vertexModel.currentVertexIndex == -1 )
      featureModel.vertexModel.selectVertexAtPosition(point, 10)
    else
    {
      digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint)
      featureModel.vertexModel.currentVertexIndex = -1
      vertexEditorToolbar.currentVertexModified = false
    }

    return true // handled
  }

  QfToolButton {
    id: cancelButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    round: true
    visible: featureModel.vertexModel.dirty && !qfieldSettings.autoSave
    bgcolor: "#900000"
    onClicked: {
      digitizingLogger.clearCoordinates();
      cancel()
    }
  }

  QfToolButton {
    id: applyButton
    iconSource: Theme.getThemeIcon( "ic_check_white_48dp" )
    round: true
    visible: featureModel.vertexModel.dirty
    bgcolor: !qfieldSettings.autoSave ? Theme.mainColor : Theme.darkGray

    onClicked: {
      if (vertexEditorToolbar.currentVertexModified)
          digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint)

      digitizingLogger.writeCoordinates();
      applyChanges( true )
      if( !qfieldSettings.autoSave )
        finished()
    }
  }

  QfToolButton {
    id: removeVertexButton
    iconSource: Theme.getThemeIcon( "ic_remove_vertex_white_24dp" )
    round: true
    visible: featureModel.vertexModel.canRemoveVertex // for now, TODO multi geom
    bgcolor: Theme.darkGray

    onClicked: {
      if (featureModel.vertexModel.canRemoveVertex){
        featureModel.vertexModel.removeCurrentVertex()
        if (screenHovering)
          featureModel.vertexModel.currentVertexIndex = -1
      }
      //on remove we have to apply directly after the action
      applyChanges( qfieldSettings.autoSave )
    }
  }

  QfToolButton {
    id: addVertexButton
    iconSource: Theme.getThemeIcon( featureModel.vertexModel.editingMode === VertexModel.AddVertex ?
                                     "ic_my_location_white_24dp" : "ic_add_vertex_white_24dp" )
    round: true
    visible:  !screenHovering && featureModel.vertexModel.canAddVertex // for now, TODO multi geom
    bgcolor: Theme.darkGray

    onClicked: {
      applyChanges( qfieldSettings.autoSave )
      if (featureModel.vertexModel.editingMode === VertexModel.AddVertex)
        featureModel.vertexModel.editingMode = VertexModel.EditVertex
      else
        featureModel.vertexModel.editingMode = VertexModel.AddVertex
    }
  }

  QfToolButton {
    id: previousVertexButton
    iconSource: Theme.getThemeIcon( "ic_chevron_left_white_24dp" )
    round: true
    visible: (!screenHovering && featureModel.vertexModel && featureModel.vertexModel.canAddVertex) || featureModel.vertexModel.editingMode === VertexModel.AddVertex
    bgcolor: featureModel.vertexModel.canPreviousVertex ? Theme.darkGray : Theme.darkGraySemiOpaque

    onClicked: {
      if (vertexEditorToolbar.currentVertexModified)
          digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint)

      applyChanges( qfieldSettings.autoSave )
      featureModel.vertexModel.previous()
    }
  }

  QfToolButton {
    id: nextVertexButton
    iconSource: Theme.getThemeIcon( "ic_chevron_right_white_24dp" )
    round: true
    visible: (!screenHovering && featureModel.vertexModel && featureModel.vertexModel.canAddVertex) || featureModel.vertexModel.editingMode === VertexModel.AddVertex
    bgcolor: featureModel.vertexModel && featureModel.vertexModel.canNextVertex ? Theme.darkGray : Theme.darkGraySemiOpaque

    onClicked: {
      if (vertexEditorToolbar.currentVertexModified)
          digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint)

      applyChanges( qfieldSettings.autoSave )
      featureModel.vertexModel.next()
    }
  }

  DigitizingLogger {
    id: digitizingLogger
    type: 'edit_vertex'

    project: qgisProject
    mapSettings: mapSettings
    digitizingLayer: featureModel.currentLayer

    positionInformation: positionSource.positionInformation
    positionLocked: gnssLockButton.checked
    topSnappingResult: coordinateLocator.topSnappingResult
    cloudUserInformation: cloudConnection.userInformation
  }

  Connections {
    target: geometryEditingVertexModel

    function onCurrentVertexIndexChanged() {
      if ( featureModel.vertexModel.currentVertexIndex != -1
           && vertexEditorToolbar.currentVertexId !== featureModel.vertexModel.currentVertexIndex
           && !screenHovering
           && featureModel.vertexModel.editingMode !== VertexModel.NoEditing )
      {
        mapSettings.setCenter(featureModel.vertexModel.currentPoint)
        vertexEditorToolbar.currentVertexId = featureModel.vertexModel.currentVertexIndex
        vertexEditorToolbar.currentVertexModified = false
      }
    }

    function onCurrentPointChanged() {
        vertexEditorToolbar.currentVertexModified = true
    }
  }
}

