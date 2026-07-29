import QtQuick
import org.qfield

/**
 * \ingroup qml
 */
Item {
  id: geometryHighlighter

  /// type:QgsGeometryWrapper
  property alias geometryWrapper: geometryRenderer.geometryWrapper
  property int duration: 3000

  SequentialAnimation {
    id: timer
    running: false
    loops: 3
    alwaysRunToEnd: true

    OpacityAnimator {
      target: geometryHighlighter
      from: 1
      to: 0
      duration: 500
    }
    OpacityAnimator {
      target: geometryHighlighter
      from: 0
      to: 1
      duration: 500
    }

    onFinished: {
      geometryHighlighter.geometryWrapper.clear();
    }
  }

  GeometryRenderer {
    id: geometryRenderer
    mapSettings: mapCanvas.mapSettings
  }

  Connections {
    target: geometryRenderer.geometryWrapper

    function onQgsGeometryChanged() {
      timer.restart();
    }
  }
}
