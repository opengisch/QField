import QtQuick 2.0

import QtQuick.Controls 2.0

ListView {
  id: listView
  model: layerTree

  delegate: ItemDelegate {
    width: parent.width
      highlighted: ListView.isCurrentItem
      onClicked: listView.currentIndex = index
  }
}
