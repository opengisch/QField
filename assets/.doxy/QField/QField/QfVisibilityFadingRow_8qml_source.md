

# File QfVisibilityFadingRow.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfVisibilityFadingRow.qml**](QfVisibilityFadingRow_8qml.md)

[Go to the documentation of this file](QfVisibilityFadingRow_8qml.md)


```C++
import QtQuick
import org.qfield

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


