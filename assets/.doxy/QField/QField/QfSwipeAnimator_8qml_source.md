

# File QfSwipeAnimator.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfSwipeAnimator.qml**](QfSwipeAnimator_8qml.md)

[Go to the documentation of this file](QfSwipeAnimator_8qml.md)


```C++
import QtQuick
import org.qfield

Flickable {
  id: flick
  boundsBehavior: Flickable.StopAtBounds
  flickableDirection: Flickable.HorizontalFlick
  clip: true

  required property real contentImplicitWidth
  default property alias contents: containerLayout.children
  property bool shouldAutoFlick: false
  property real duration: 3000

  SequentialAnimation {
    id: mainAnim
    loops: 10
    running: flick.shouldAutoFlick

    NumberAnimation {
      target: flick
      property: "contentX"
      to: -(flick.width - contentImplicitWidth)
      duration: flick.duration
    }

    PauseAnimation {
      duration: 1000
    }

    NumberAnimation {
      target: flick
      property: "contentX"
      to: 0
      duration: flick.duration
    }

    PauseAnimation {
      duration: 1000
    }
  }

  Item {
    id: containerLayout
  }

  // if containerLayout is TextArea or TextField this function could be used to
  // handle text selection and showing cursor rectangle
  function ensureCursorVisible(cursorRectangle) {
    if (contentX >= cursorRectangle.x) {
      contentX = cursorRectangle.x;
    } else if (contentX + width <= cursorRectangle.x + cursorRectangle.width) {
      contentX = cursorRectangle.x + cursorRectangle.width - width;
    }
  }
}
```


