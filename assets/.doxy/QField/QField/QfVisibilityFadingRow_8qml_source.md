

# File QfVisibilityFadingRow.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfVisibilityFadingRow.qml**](QfVisibilityFadingRow_8qml.md)

[Go to the documentation of this file](QfVisibilityFadingRow_8qml.md)


```C++
import QtQuick
import org.qfield.core

Row {
  id: visibilityFadingRow
  property bool stateVisible: false
  visible: opacity > 0
  opacity: 0
  spacing: 4

  states: [
    State {
      when: visibilityFadingRow.stateVisible
      PropertyChanges {
        target: visibilityFadingRow
        opacity: 1.0
      }
    },
    State {
      when: !visibilityFadingRow.stateVisible
      PropertyChanges {
        target: visibilityFadingRow
        opacity: 0.0
      }
    }
  ]

  transitions: Transition {
    SequentialAnimation {
      NumberAnimation {
        target: visibilityFadingRow
        property: "opacity"
        duration: 250
      }
    }
  }
}
```


