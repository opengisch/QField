

# File QfTabBar.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfTabBar.qml**](QfTabBar_8qml.md)

[Go to the documentation of this file](QfTabBar_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qfield.core
import org.qfield.gui

ListView {
  id: tabRow

  signal clicked

  orientation: Qt.Horizontal
  highlightFollowsCurrentItem: true
  highlightResizeDuration: 0
  currentIndex: 0

  highlight: Item {
    Rectangle {
      height: 2
      color: QfTheme.mainColor
      radius: 4
      width: parent.width
      anchors.bottom: parent.bottom
    }
  }

  delegate: TabButton {
    text: modelData
    height: tabRow.defaultHeight
    width: tabRow.width / tabRow.count
    font: QfTheme.defaultFont
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


