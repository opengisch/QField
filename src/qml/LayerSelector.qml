import QtQuick 2.3
import QtQuick.Controls 2.0
import org.qgis 1.0

Item {
  property VectorLayer currentLayer
  height: comboBox.height

  Rectangle {
    anchors.fill: parent
    color: "white"
  }

  ComboBox {
    id: comboBox
    anchors { left: parent.left; right: parent.right }
    model: modelHelper.model
    textRole: "display"

    onCurrentIndexChanged: __updateCurrentLayer()
    onModelChanged: __updateCurrentLayer()

    Connections {
      target: comboBox.model

      onRowsInserted: comboBox.__updateCurrentLayer()
      onRowsRemoved: comboBox.__updateCurrentLayer()
    }

    function __updateCurrentLayer() {
      var lyr = modelHelper.data( currentIndex, 0, "layer" )
      if ( lyr === undefined )
        currentLayer = null
      else
        currentLayer = lyr
    }
  }

  ModelHelper {
    id: modelHelper
    model: MapLayerModel {
      filters: MapLayerModel.VectorLayer | MapLayerModel.WritableLayer
    }
  }
}
