import QtQuick 2.11
import QtQuick.Controls 2.4 as Controls
import "js/style.js" as Style
import org.qgis 1.0
import org.qfield 1.0


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
    height: 40*dp
    anchors.right: parent.right
    visible: opacity > 0
    padding: 5*dp

    background: Rectangle {
      radius: 2*dp
      border.color: "#333"
      border.width: 1
    }

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
    height: Math.min( 200*dp, mainWindow.height - searchField.height )
    clip: true

    delegate: Rectangle {
      id: delegateRect
      anchors.margins: 10*dp
      height: visible ? (isGroup ? 25 : 40 ) * dp : 0
      width: parent.width
      visible: model.ResultType !== 0 // remove filter name
      property bool isGroup: model.ResultFilterGroupSorting === 0
      color: isGroup ? "#ffffff" : "#eeeeee"
      opacity: 0.95
      border.width: 1*dp
      border.color: "#bbbbbb"
      radius: 2*dp

      Text {
        text: model.Text
        anchors.verticalCenter: parent.verticalCenter
        leftPadding: 5*dp
        font.italic: delegateRect.isGroup ? true : false
      }

      Button {
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height
        visible: locator.contextMenuActionsModel( index ).rowCount() > 0
        text: "test"
      }

      MouseArea {
        anchors.fill: parent

        onClicked: {
          locator.triggerResultAtRow(index)
        }
      }
    }
  }
}

