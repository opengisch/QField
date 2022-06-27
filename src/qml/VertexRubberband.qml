import QtQuick 2.14
import QtQml 2.14

import org.qgis 1.0
import org.qfield 1.0

Repeater {
    id: vertexRubberband
    property MapSettings mapSettings
    property bool isVisible: true

    delegate: Rectangle {
      MapToScreen {
          id: mapToScreen
          mapSettings: vertexRubberband.mapSettings
          mapPoint: Point
      }

      visible: vertexRubberband.isVisible

      x: mapToScreen.screenPoint.x - width/2
      y: mapToScreen.screenPoint.y - width/2

      width: ExistingVertex ? 20 : 10
      height: ExistingVertex ? 20 : 10
      radius: ExistingVertex ? width / 2 : 0

      border.color: if (CurrentVertex) {'red'} else {'blue'}
      border.width: VertexModel.ExistingVertex ? 2 : 1
    }
}

