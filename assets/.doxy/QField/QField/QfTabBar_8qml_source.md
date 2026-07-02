

# File QfTabBar.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfTabBar.qml**](QfTabBar_8qml.md)

[Go to the documentation of this file](QfTabBar_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield

ListView {
  id: tabRow

  signal clicked

  orientation: Qt.Horizontal
  highlightFollowsCurrentItem: true
  currentIndex: 0

  highlight: Item {
    Rectangle {
      height: 2
      color: Theme.mainColor
      radius: 4
      width: parent.width
      anchors.bottom: parent.bottom
    }
  }

  delegate: TabButton {
    text: modelData
    height: tabRow.defaultHeight
    width: tabRow.parent.width / tabRow.count
    font: Theme.defaultFont
    checked: tabRow.currentIndex === index
    onClicked: {
      tabRow.currentIndex = index;
      tabRow.clicked();
    }
  }

  onCurrentIndexChanged: {
    tabRow.positionViewAtIndex(currentIndex, ListView.Contain);
  }

  property real defaultHeight: 48
}
```


