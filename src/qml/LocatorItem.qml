import QtQuick 2.11
import QtQuick.Controls 2.4 as Controls
import "js/style.js" as Style



Item {
  id: locatorItem
  property bool searching: false

  states: [
      State { when: locatorItem.searching;
              PropertyChanges { target: searchButton; opacity: 0.0 }
              PropertyChanges { target: searchField; opacity: 1.0 }},
      State { when: !locatorItem.searching;
              PropertyChanges { target: searchButton; opacity: 1.0 }
              PropertyChanges { target: searchField; opacity: 0.0 }}
  ]

  Controls.TextField {
    id: searchField
    placeholderText: qsTr("Search…")
    onTextChanged: locator.performSearch(searchField.text)
    width: parent.width
    anchors.right: parent.right
    visible: opacity > 0

    transitions: Transition {
      SequentialAnimation {
        NumberAnimation {
          target: searchField
          property: "opacity"
          duration: 400
        }
      }
    }
  }

  Item {
    // Button in controls 1 has no opacity
    id: searchButton
    anchors.right: parent.right

    Button {
      anchors { right: parent.right; top: parent.top; rightMargin: 4*dp; topMargin: 4*dp }

      iconSource: Style.getThemeIcon( "ic_baseline_search_white" )
      round: true
      bgcolor: "#80CC28"

      onClicked: {
        locatorItem.searching = true
        searchField.focus = true
      }
    }

    visible: opacity > 0
    transitions: Transition {
      SequentialAnimation {
        NumberAnimation {
          target: searchButton
          property: "opacity"
          duration: 400
        }
      }
    }
  }

  ListView {
    id: resultsList
    anchors.top: searchField.bottom
    model: locator.proxyModel()
    width: parent.width
    height: mainWindow.height - searchField.height

    delegate: Rectangle {
      id: delegateRect
      anchors.margins: 10*dp
      height: visible ? 25 * dp : 0
      width: parent.width
      visible: model.ResultType !== 0 // remove filter name
      property bool isGroup: model.ResultFilterGroupSorting === 0
      color: isGroup ? "#ffffff" : "#eeeeee"
      opacity: 0.95
      border.width: 1*dp
      border.color: "#bbbbbb"

      Text {
        text: model.Text
        anchors.fill: parent
        padding: 5*dp
        font.italic: delegateRect.isGroup ? true : false
        // other drawing code here.
      }

      MouseArea {
        anchors.fill: parent

        onClicked: {
          console.log( index )
          locator.triggerResultAtRow(index)
        }
      }
    }
  }
}

