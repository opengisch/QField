import QtQuick 2.3
import QtQuick.Controls 1.4
import org.qgis 1.0

ComboBox {
  property VectorLayer currentLayer

  model: modelHelper.model
  textRole: "display"

  onCurrentIndexChanged: __updateCurrentLayer()
  onModelChanged: __updateCurrentLayer()

  ModelHelper {
    id: modelHelper
    model: MapLayerModel {
      filters: MapLayerModel.PointLayer
    }
  }

  Connections {
    target: model

    onRowsInserted: __updateCurrentLayer()
    onRowsRemoved: __updateCurrentLayer()
  }

  function __updateCurrentLayer() {
    var lyr = modelHelper.data( currentIndex, 0, "layer" )
    if ( lyr === undefined )
      currentLayer = null
    else
      currentLayer = lyr
  }
}
