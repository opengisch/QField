import QtQuick.Controls 1.4
import org.qgis 1.0

ComboBox {
  property VectorLayer currentLayer

  model: MapLayerModel {}
  textRole: "display"

  onCurrentIndexChanged: currentLayer = modelHelpers.data( model, currentIndex, 0, "layer" )
}
