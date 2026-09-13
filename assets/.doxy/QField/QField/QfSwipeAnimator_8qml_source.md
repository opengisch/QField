

# File QfSwipeAnimator.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfSwipeAnimator.qml**](QfSwipeAnimator_8qml.md)

[Go to the documentation of this file](QfSwipeAnimator_8qml.md)


```C++
import QtQuick
import org.qfield.core

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

    onStopped: {
      flick.contentX = 0;
    }

    PauseAnimation {
      duration: 2000
    }

    NumberAnimation {
      target: flick
      property: "contentX"
      to: -(flick.width - contentImplicitWidth)
      duration: flick.duration
    }

    PauseAnimation {
      duration: 2000
    }

    NumberAnimation {
      target: flick
      property: "contentX"
      to: 0
      duration: flick.duration
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


