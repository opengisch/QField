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

  readonly property bool blocking: featureModel ? featureModel.vertexModel.dirty : false

  spacing: 4

  function init(featureModel, mapSettings, editorRubberbandModel, editorRenderer)
  {
    featureModel.vertexModel.currentVertexIndex = -1
    vertexEditorToolbar.featureModel = featureModel
    vertexEditorToolbar.mapSettings = mapSettings
    digitizingLogger.digitizingLayer = featureModel.currentLayer
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

  function canvasClicked(point, type)
  {
    if (type === "stylus") {
      if ( featureModel.vertexModel.currentVertexIndex == -1 )
        featureModel.vertexModel.selectVertexAtPosition(point, 14)
      else
      {
        digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint)
        featureModel.vertexModel.currentVertexIndex = -1
        vertexEditorToolbar.currentVertexModified = false
      }
    } else {
      featureModel.vertexModel.selectVertexAtPosition(point, 14, false)
    }

    return true
  }

  QfToolButton {
    id: undoButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    round: true
    visible: featureModel && featureModel.vertexModel.canUndo
    bgcolor: "#900000"
    onClicked: {
      featureModel.vertexModel.undoHistory()
      mapSettings.setCenter(featureModel.vertexModel.currentPoint)
    }
  }

  QfToolButton {
    id: cancelButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    round: true
    visible: featureModel && featureModel.vertexModel.dirty && !qfieldSettings.autoSave
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
    visible: featureModel && featureModel.vertexModel.dirty
    bgcolor: !qfieldSettings.autoSave ? Theme.mainColor : Theme.darkGray

    onClicked: {
      if (vertexEditorToolbar.currentVertexModified)
          digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint)

      digitizingLogger.writeCoordinates();
      applyChanges(true)
      finished()
    }
  }

  QfToolButton {
    id: removeVertexButton
    iconSource: Theme.getThemeIcon( "ic_remove_vertex_white_24dp" )
    round: true
    visible: featureModel && featureModel.vertexModel.canRemoveVertex
    bgcolor: Theme.darkGray

    onClicked: {
      if (featureModel.vertexModel.canRemoveVertex){
        featureModel.vertexModel.removeCurrentVertex()
        if (screenHovering) {
          featureModel.vertexModel.currentVertexIndex = -1
        }
      }

      applyChanges(qfieldSettings.autoSave)
    }
  }

  QfToolButton {
    id: addVertexButton
    iconSource: Theme.getThemeIcon("ic_add_vertex_white_24dp")
    round: true
    visible:  !screenHovering && featureModel && featureModel.vertexModel.canAddVertex && featureModel.vertexModel.editingMode !== VertexModel.AddVertex
    bgcolor: Theme.darkGray

    onClicked: {
      applyChanges(qfieldSettings.autoSave)
      if (featureModel.vertexModel.currentVertexIndex != -1) {
        if (featureModel.vertexModel.editingMode === VertexModel.AddVertex) {
          featureModel.vertexModel.editingMode = VertexModel.EditVertex
        } else {
          featureModel.vertexModel.editingMode = VertexModel.AddVertex
        }
      } else {
        featureModel.vertexModel.addVertexNearestToPosition(coordinateLocator.currentCoordinate)
        applyChanges(qfieldSettings.autoSave)
      }
    }
  }

  QfToolButton {
    id: previousVertexButton
    iconSource: Theme.getThemeIcon( "ic_chevron_left_white_24dp" )
    round: true
    visible: featureModel && ((!screenHovering && featureModel.vertexModel.canAddVertex) || featureModel.vertexModel.editingMode === VertexModel.AddVertex)
    bgcolor: featureModel && featureModel.vertexModel.canPreviousVertex ? Theme.darkGray : Theme.darkGraySemiOpaque

    onClicked: {
      if (vertexEditorToolbar.currentVertexModified)
      {
        digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint)
      }

      applyChanges(qfieldSettings.autoSave)
      featureModel.vertexModel.previous()
    }
  }

  QfToolButton {
    id: nextVertexButton
    iconSource: Theme.getThemeIcon( "ic_chevron_right_white_24dp" )
    round: true
    visible: featureModel && ((!screenHovering && featureModel.vertexModel.canAddVertex) || featureModel.vertexModel.editingMode === VertexModel.AddVertex)
    bgcolor: featureModel && featureModel.vertexModel.canNextVertex ? Theme.darkGray : Theme.darkGraySemiOpaque

    onClicked: {
      if (vertexEditorToolbar.currentVertexModified)
      {
        digitizingLogger.addCoordinate(featureModel.vertexModel.currentPoint)
      }

      applyChanges(qfieldSettings.autoSave)
      featureModel.vertexModel.next()
    }
  }

  DigitizingLogger {
    id: digitizingLogger
    type: 'edit_vertex'

    project: qgisProject
    mapSettings: mapSettings

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

