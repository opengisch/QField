import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

Item {
  property alias searchTerm: searchBar.text

  Rectangle {
    width: parent.width
    height: 40
    radius: 6
    border.width: 1
    color: Theme.mainBackgroundColor
    border.color: searchBar.activeFocus ? Theme.mainColor : "transparent"

    QfToolButton {
      id: clearButton
      anchors.right: parent.right
      width: 40
      height: 40
      iconSource: Theme.getThemeIcon('ic_close_black_24dp')
      iconColor: Theme.mainTextColor
      bgcolor: "transparent"
      visible: searchBar.text !== ""
      onClicked: {
        searchBar.text = '';
      }
    }

    QfToolButton {
      id: searchButton
      width: 40
      height: 40
      anchors.left: parent.left
      bgcolor: "transparent"
      iconSource: Theme.getThemeIcon("ic_baseline_search_black")
      iconColor: Theme.mainTextColor
      onClicked: {
        searchBar.focus = true;
      }
    }

    TextField {
      id: searchBar
      rightPadding: 7
      anchors.left: searchButton.right
      anchors.right: clearButton.left
      anchors.leftMargin: -16
      anchors.rightMargin: 4
      height: 40
      selectByMouse: true
      placeholderText: (!searchBar.activeFocus && text === "" && displayText === "") ? qsTr("Search for project") : ""
      background: Item {
      }
    }
  }
}
