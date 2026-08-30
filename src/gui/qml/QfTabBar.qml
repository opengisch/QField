import QtQuick
import QtQuick.Controls
import org.qfield.core
import org.qfield.gui

/**
 * \ingroup org.qfield.gui
 */
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
