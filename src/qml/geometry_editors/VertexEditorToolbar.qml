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

  function canvasClicked(point)
  {
    featureModel.vertexModel.selectVertexAtPosition(point, 10*dp)
    return true // handled
  }

  Button {
    id: cancelButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    round: true
    visible: featureModel.vertexModel.dirty
    bgcolor: featureModel.vertexModel.dirty ? Theme.darkRed : Theme.darkGray
    onClicked: {
      cancel()
    }
  }

  Button {
    id: applyButton
    iconSource: Theme.getThemeIcon( "ic_check_white_48dp" )
    round: true
    visible: featureModel.vertexModel.dirty
    bgcolor: Theme.mainColor

    onClicked: {
      featureModel.applyVertexModelToGeometry()
      featureModel.save()
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

