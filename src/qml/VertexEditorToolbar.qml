import QtQuick 2.12
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

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
    featureModel.vertexModel.reset()
  }

  Button {
    id: cancelButton
    iconSource: Theme.getThemeIcon( "ic_clear_white_24dp" )
    round: true
    visible: featureModel.vertexModel.dirty
    bgcolor: "#900000"
    onClicked: {
      cancel()
    }
  }

  Button {
    id: applyButton
    iconSource: Theme.getThemeIcon( "ic_check_white_48dp" )
    round: true
    bgcolor: featureModel.vertexModel.dirty ? Theme.mainColor : "#616161"

    onClicked: {
      if (featureModel.vertexModel.dirty){
        featureModel.applyVertexModelToGeometry()
        featureModel.save()
      }
      finished()
    }
  }


  Button {
    id: previousVertexButton
    iconSource: Theme.getThemeIcon( "ic_chevron_left_white_24dp" )
    round: true
    visible: featureModel.vertexModel.canAddVertex // for now, TODO multi geom
    bgcolor: featureModel.vertexModel.canPreviousVertex ? "#FFD600" : "#616161"

    onClicked: {
      featureModel.vertexModel.previous()
    }
  }

  Button {
    id: removeVertexButton
    iconSource: Theme.getThemeIcon( "ic_remove_white_24dp" )
    round: true
    visible: featureModel.vertexModel.canAddVertex // for now, TODO multi geom
    bgcolor: featureModel.vertexModel.canRemoveVertex ? "#FFD600" : "#616161"

    onClicked: {
      if (featureModel.vertexModel.canRemoveVertex){
        featureModel.vertexModel.removeCurrentVertex()
      }
    }
  }

  Button {
    id: addVertexButton
    iconSource: Theme.getThemeIcon( featureModel.vertexModel.editingMode === VertexModel.AddVertex ?
                                     "ic_my_location_white_24dp.png" : "ic_add_white_24dp" )
    visible: featureModel.vertexModel.canAddVertex // for now, TODO multi geom
    round: true
    bgcolor: "#FFD600"

    onClicked: {
      if (featureModel.vertexModel.editingMode === VertexModel.AddVertex)
        featureModel.vertexModel.editingMode = VertexModel.EditVertex
      else
        featureModel.vertexModel.editingMode = VertexModel.AddVertex
    }
  }


  Button {
    id: nextVertexButton
    iconSource: Theme.getThemeIcon( "ic_chevron_right_white_24dp" )
    round: true
    visible: featureModel.vertexModel.canAddVertex // for now, TODO multi geom
    bgcolor: featureModel.vertexModel.canNextVertex ? "#FFD600" : "#616161"

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

