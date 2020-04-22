import QtQuick 2.12
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import ".."


VisibilityFadingRow {
  id: vertexEditorToolbar

  signal finished()

  property FeatureModel featureModel
  property MapSettings mapSettings
  readonly property bool blocking: featureModel.vertexModel.dirty

  spacing: 4 * dp

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
      featureModel.save()
      //set the vertexModel original geometry to the one of the updated feature
      featureModel.vertexModel.updateGeometry( featureModel.feature.geometry)
    }
  }

  Button {
    id: cancelButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    round: true
    visible: featureModel.vertexModel.dirty && !qfieldSettings.autoSave
    bgcolor: "#900000"
    onClicked: {
      cancel()
    }
  }

  Button {
    id: applyButton
    iconSource: Theme.getThemeIcon( "ic_check_white_48dp" )
    round: true
    visible: featureModel.vertexModel.dirty
    bgcolor: !qfieldSettings.autoSave ? Theme.mainColor : Theme.darkGray

    onClicked: {
      applyChanges( true )
      if( !qfieldSettings.autoSave )
        finished()
    }
  }

  Button {
    id: removeVertexButton
    iconSource: Theme.getThemeIcon( "ic_remove_white_24dp" )
    round: true
    visible: featureModel.vertexModel.canRemoveVertex // for now, TODO multi geom
    bgcolor: Theme.darkGray

    onClicked: {
      if (featureModel.vertexModel.canRemoveVertex){
        featureModel.vertexModel.removeCurrentVertex()
      }
      //on remove we have to apply directly after the action
      applyChanges( qfieldSettings.autoSave )
    }
  }

  Button {
    id: addVertexButton
    iconSource: Theme.getThemeIcon( featureModel.vertexModel.editingMode === VertexModel.AddVertex ?
                                     "ic_my_location_white_24dp" : "ic_add_white_24dp" )
    round: true
    visible: featureModel.vertexModel.canAddVertex // for now, TODO multi geom
    bgcolor: Theme.darkGray

    onClicked: {
      applyChanges( qfieldSettings.autoSave )
      if (featureModel.vertexModel.editingMode === VertexModel.AddVertex)
        featureModel.vertexModel.editingMode = VertexModel.EditVertex
      else
        featureModel.vertexModel.editingMode = VertexModel.AddVertex
    }
  }

  Button {
    id: previousVertexButton
    iconSource: Theme.getThemeIcon( "ic_chevron_left_white_24dp" )
    round: true
    visible: featureModel.vertexModel.canAddVertex // for now, TODO multi geom
    bgcolor: featureModel.vertexModel.canPreviousVertex ? Theme.darkGray : Theme.darkGraySemiOpaque

    onClicked: {
      applyChanges( qfieldSettings.autoSave )
      featureModel.vertexModel.previous()
    }
  }

  Button {
    id: nextVertexButton
    iconSource: Theme.getThemeIcon( "ic_chevron_right_white_24dp" )
    round: true
    visible: featureModel.vertexModel && featureModel.vertexModel.canAddVertex // for now, TODO multi geom
    bgcolor: featureModel.vertexModel && featureModel.vertexModel.canNextVertex ? Theme.darkGray : Theme.darkGraySemiOpaque

    onClicked: {
      applyChanges( qfieldSettings.autoSave )
      featureModel.vertexModel.next()
    }
  }

  Connections {
    target: vertexModel
    onCurrentPointChanged:
    {
      if (featureModel.vertexModel.editingMode === VertexModel.EditVertex  ||
          featureModel.vertexModel.editingMode === VertexModel.AddVertex)
        mapSettings.setCenter(featureModel.vertexModel.currentPoint)
    }
  }
}

