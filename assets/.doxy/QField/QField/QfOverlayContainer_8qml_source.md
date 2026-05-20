

# File QfOverlayContainer.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfOverlayContainer.qml**](QfOverlayContainer_8qml.md)

[Go to the documentation of this file](QfOverlayContainer_8qml.md)


```C++
import QtQuick
import org.qfield

Rectangle {
  id: container

  default property alias contents: containerLayout.children
  property alias layout: containerLayout
  property alias title: title.text

  width: parent.width
  height: childrenRect.height
  color: Theme.mainBackgroundColorSemiOpaque
  radius: 8
  clip: true

  Column {
    id: containerLayout
    width: parent.width - 10
    anchors.horizontalCenter: parent.horizontalCenter
    topPadding: 5
    bottomPadding: 5
    spacing: 4

    Text {
      id: title
      text: ""
      font: Theme.strongTipFont
      color: Theme.mainTextColor
      leftPadding: 6
    }
  }
}
```


