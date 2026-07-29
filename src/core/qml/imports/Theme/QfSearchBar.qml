import QtQuick
import QtQuick.Controls
import org.qfield

/**
 * \ingroup qml
 */
Item {
  id: searchBar

  default property alias contents: filterContainer.children

  readonly property alias searchTerm: searchField.text
  readonly property real searchHeight: searchField.height
  property string placeHolderText: qsTr("Search")

  property bool enableFilterButton: false
  property bool filterActive: false
  property var parameterKeys: []

  signal filterClicked
  signal searchTermEdited
  signal searchTriggered
  signal cleared

  height: searchBarContainer.height
  clip: true

  function highlightedText(raw) {
    if (!raw) {
      return "";
    }

    // Encode HTML < and > characters to avoid text going missing
    raw = raw.replace('<', '&lt;').replace('>', '&gt;');
    if (parameterKeys.length == 0) {
      return raw;
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
    id: searchBarContainer
    width: parent.width
    height: searchField.height + (filterActive ? filterContainer.childrenRect.height + filterContainer.anchors.margins + 8 : 0)
    radius: 6
    color: Theme.mainBackgroundColor
    border.color: filterActive || searchField.activeFocus ? Theme.mainColor : "transparent"
    border.width: 2

    Behavior on height {
      PropertyAnimation {
        duration: 150
        easing.type: Easing.InOutQuad
      }
    }

    QfToolButton {
      id: clearButton
      anchors.right: filterButton.visible ? filterButton.left : parent.right
      width: 40
      height: searchField.height
      iconSource: Theme.getThemeVectorIcon('ic_clear_white_24dp')
      iconColor: Theme.mainTextColor
      bgcolor: "transparent"
      visible: searchField.text !== ""
      onClicked: {
        searchBar.clear();
      }
    }

    QfToolButton {
      id: filterButton
      anchors.right: parent.right
      width: 40
      height: searchField.height
      iconSource: Theme.getThemeVectorIcon("ic_tune_white_24dp")
      iconColor: searchBar.filterActive ? Theme.mainColor : Theme.mainTextColor
      bgcolor: "transparent"
      visible: searchBar.enableFilterButton
      onClicked: searchBar.filterClicked()
    }

    QfToolButton {
      id: searchButton
      anchors.left: parent.left
      width: 40
      height: searchField.height
      bgcolor: "transparent"
      iconSource: Theme.getThemeVectorIcon("ic_baseline_search_white")
      iconColor: Theme.mainTextColor
      onClicked: {
        if (searchField.text !== "") {
          searchBar.searchTriggered();
        } else {
          searchField.forceActiveFocus();
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
      height: Material.textFieldHeight
      selectByMouse: true
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase | Qt.ImhSensitiveData
      placeholderText: (!searchField.activeFocus && text === "" && displayText === "") ? searchBar.placeHolderText : ""
      font: Theme.defaultFont
      color: highlightOverlay.visible ? "transparent" : Theme.mainTextColor

      background: Item {}

      onTextEdited: {
        searchBar.searchTermEdited();
      }

      Keys.onPressed: event => {
        if (event.key === Qt.Key_Return || event.key === Qt.Key_Enter) {
          searchBar.searchTriggered();
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

    Column {
      id: filterContainer
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.top: searchField.bottom
      anchors.margins: 10
    }

    Rectangle {
      anchors.top: searchField.bottom
      anchors.topMargin: -1
      anchors.horizontalCenter: parent.horizontalCenter
      width: parent.width - 70
      height: 1
      color: searchField.placeholderTextColor
      visible: filterActive
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
    cleared();
  }
}
