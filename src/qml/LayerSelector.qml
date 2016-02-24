import QtQuick 2.3
import QtQuick.Controls 1.4
import org.qgis 1.0

ComboBox {
  property VectorLayer currentLayer

  model: MapLayerModel {}
  textRole: "display"

  onCurrentIndexChanged: currentLayer = modelHelpers.data( model, currentIndex, 0, "layer" )
  onModelChanged: currentLayer = modelHelpers.data( model, currentIndex, 0, "layer" )

  Connections {
    target: model

    onRowsInserted: currentLayer = modelHelpers.data( model, currentIndex, 0, "layer" )
    onRowsRemoved: currentLayer = modelHelpers.data( model, currentIndex, 0, "layer" )
  }
}
