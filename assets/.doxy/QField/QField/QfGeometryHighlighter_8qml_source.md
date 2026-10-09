

# File QfGeometryHighlighter.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfGeometryHighlighter.qml**](QfGeometryHighlighter_8qml.md)

[Go to the documentation of this file](QfGeometryHighlighter_8qml.md)


```C++
import QtQuick
import org.qfield.core

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

  QfGeometryRenderer {
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


