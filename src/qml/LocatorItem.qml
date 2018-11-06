import QtQuick 2.1
import QtQuick.Controls 2.0


Column {
  id: locatorItem

  TextField {
    id: searchField
    placeholderText: qsTr("Search…")
    onTextChanged: locator.performSearch(searchField.text)
    width: parent.width
  }

  Repeater {
    id: repeater
    model: locator.proxyModel()
    anchors.top: searchField.bottom
    width: parent.width

    delegate:
      Text {
        text: model.Text + model.ResultFilterGroupSorting
        property bool isGroup: model.ResultFilterGroupSorting === 0
        visible: model.ResultType !== 0 // remove filter name
        height: visible ? (isGroup ? 20 : 25 ) * dp : 0
        color: isGroup ? "red" : "blue"
        font.italic: isGroup ? true : false
        // other drawing code here.
      }
  }
}
