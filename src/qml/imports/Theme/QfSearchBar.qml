import QtQuick
import QtQuick.Controls
import org.qfield

/**
 * \ingroup qml
 */
Item {
  id: searchBar

  property alias searchTerm: searchField.displayText
  property string placeHolderText: qsTr("Search")
  property bool enableFilterButton: false
  property bool filterActive: false
  property var parameterKeys: ["owner", "include"]
  property var parameterFlags: ["public"]

  signal returnPressed
  signal filterClicked
  signal searchTriggered

  height: childrenRect.height

  function highlightedText(raw) {
    if (!raw) {
      return "";
    }
    let html = "";
    const parts = raw.split(/(\s+)/);
    for (const part of parts) {
      const colonIdx = part.indexOf(":");
      if (colonIdx > 0 && parameterKeys.indexOf(part.slice(0, colonIdx)) !== -1) {
        const key = part.slice(0, colonIdx + 1);
        const val = part.slice(colonIdx + 1);
        html += '<span style="color:' + Theme.mainColor + '">' + key + '</span>' + val;
      } else {
        html += part;
      }
    }
    return html;
  }

  Rectangle {
    width: parent.width
    height: 40
    radius: 6
    border.width: 1
    color: Theme.mainBackgroundColor
    border.color: searchField.activeFocus ? Theme.mainColor : "transparent"

    QfToolButton {
      id: clearButton
      anchors.right: filterButton.visible ? filterButton.left : parent.right
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
      id: filterButton
      anchors.right: parent.right
      width: 40
      height: 40
      iconSource: Theme.getThemeVectorIcon("ic_tune_white_24dp")
      iconColor: searchBar.filterActive ? Theme.mainColor : Theme.mainTextColor
      bgcolor: "transparent"
      visible: searchBar.enableFilterButton
      onClicked: searchBar.filterClicked()
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
        if (searchField.text !== "") {
          searchBar.searchTriggered();
        } else {
          searchField.focus = true;
        }
      }
    }

    TextField {
      id: searchField
      rightPadding: 7
      anchors.left: searchButton.right
      anchors.right: clearButton.visible ? clearButton.left : (filterButton.visible ? filterButton.left : parent.right)
      anchors.leftMargin: -16
      anchors.rightMargin: 4
      height: 40
      selectByMouse: true
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhSensitiveData
      placeholderText: (!searchField.activeFocus && text === "" && displayText === "") ? searchBar.placeHolderText : ""
      background: Item {}
      font: Theme.defaultFont
      color: highlightOverlay.visible ? "transparent" : Theme.mainTextColor
      Keys.onPressed: event => {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
          searchBar.returnPressed();
        }
      }
    }

    Text {
      id: highlightOverlay
      anchors.fill: searchField
      anchors.leftMargin: searchField.leftPadding
      anchors.rightMargin: searchField.rightPadding
      verticalAlignment: Text.AlignVCenter
      visible: !searchField.activeFocus && searchField.text !== ""
      textFormat: Text.RichText
      font: searchField.font
      color: Theme.mainTextColor
      elide: Text.ElideRight
      clip: true
      text: searchBar.highlightedText(searchField.text)
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
