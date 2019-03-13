import QtQuick 2.11
import org.qfield 1.0

Item {
  id: geometryHighlighter
  property alias geometryWrapper: geometryRenderer.geometryWrapper
  property int duration: 3000

  Connections {
    target: geometryRenderer
    onQgsGeometryChanged: {
      timer.restart()
    }
  }

  GeometryRenderer {
    id: geometryRenderer
  }

  Timer {
    id: timer
    interval: geometryHighlighter.duration
    running: false
    repeat: false
    onTriggered: geometryHighlighter.geometry = null
  }
}
