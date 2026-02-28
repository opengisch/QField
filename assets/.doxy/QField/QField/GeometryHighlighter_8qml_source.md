

# File GeometryHighlighter.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**GeometryHighlighter.qml**](GeometryHighlighter_8qml.md)

[Go to the documentation of this file](GeometryHighlighter_8qml.md)


```C++
import QtQuick
import org.qfield

Item {
  id: geometryHighlighter

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
```


