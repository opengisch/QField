import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

Item {
  id: searchBar

  property alias searchTerm: searchField.displayText
  property string placeHolderText: qsTr("Search")

  signal returnPressed

  Rectangle {
    width: parent.width
    height: 40
    radius: 6
    border.width: 1
    color: Theme.mainBackgroundColor
    border.color: searchField.activeFocus ? Theme.mainColor : "transparent"

    QfToolButton {
      id: clearButton
      anchors.right: parent.right
      width: 40
      height: 40
      iconSource: Theme.getThemeVectorIcon('ic_clear_white_24dp')
      iconColor: Theme.mainTextColor
      bgcolor: "transparent"
      visible: searchField.text !== ""
      onClicked: {
        clear();
      }
    }

    QfToolButton {
      id: searchButton
      width: 40
      height: 40
      anchors.left: parent.left
      bgcolor: "transparent"
      iconSource: Theme.getThemeVectorIcon("ic_baseline_search_white")
      iconColor: Theme.mainTextColor
      onClicked: {
        searchField.focus = true;
      }
    }

    TextField {
      id: searchField
      rightPadding: 7
      anchors.left: searchButton.right
      anchors.right: clearButton.left
      anchors.leftMargin: -16
      anchors.rightMargin: 4
      height: 40
      selectByMouse: true
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhSensitiveData
      placeholderText: (!searchField.activeFocus && text === "" && displayText === "") ? searchBar.placeHolderText : ""
      background: Item {
      }

      Keys.onPressed: event => {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
          searchBar.returnPressed();
        }
      }
    }
  }

  function focusOnTextField() {
    searchField.forceActiveFocus();
  }

  function setSearchTerm(term) {
    searchField.text = term;
  }

  function clear() {
    searchField.text = '';
  }
}
