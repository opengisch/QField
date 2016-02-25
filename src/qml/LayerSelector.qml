import QtQuick 2.3
import QtQuick.Controls 1.4
import org.qgis 1.0

ComboBox {
  property VectorLayer currentLayer

  model: MapLayerModel {}
  textRole: "display"

  onCurrentIndexChanged: __updateCurrentLayer()
  onModelChanged: __updateCurrentLayer()

  Connections {
    target: model

    onRowsInserted: __updateCurrentLayer()
    onRowsRemoved: __updateCurrentLayer()
  }

  function __updateCurrentLayer() {
    var lyr = modelHelpers.data( model, currentIndex, 0, "layer" )
    if ( lyr === undefined )
      currentLayer = null
    else
      currentLayer = lyr
  }
}
