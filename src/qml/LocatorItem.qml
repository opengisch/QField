import QtQuick 2.1
import QtQuick.Controls 2.0

Column {
  id: locatorItem

  TextField {
    id: searchField
    placeholderText: qsTr("Search…")
    onTextChanged: locator.performSearch(searchField.text)
  }

  Repeater {
    model: locator.proxyModel()
    anchors.top: searchField.bottom

    // https://stackoverflow.com/questions/26717209/filtering-sorting-a-qabstractlistmodel-in-qml
    delegate:
      Text {
        text: model.Text + model.ResultType
        visible: model.ResultType !== 0 // remove filter name
        height: visible ? 30 * dp : 0
        // other drawing code here.
      }
  }
}
