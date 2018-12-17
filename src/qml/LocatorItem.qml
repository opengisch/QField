import QtQuick 2.11
import QtQuick.Controls 2.4 as Controls
import "js/style.js" as Style
import org.qgis 1.0
import org.qfield 1.0


Item {
  id: locatorItem
  property bool searching: false
  height: childrenRect.height

  LocatorModelSuperBridge {
    id: locator
    mapSettings: mapCanvas.mapSettings
    locatorHighlight: locatorHighlightItem
    featureListController: featureForm.extentController

    onMessageEmitted: {
      displayToast(text)
    }
  }

  states: [
      State { when: locatorItem.searching;
              PropertyChanges { target: searchButton; opacity: 0.0 }
              PropertyChanges { target: searchField; opacity: 1.0 }
              PropertyChanges { target: resultsList; opacity: 1.0 }},
      State { when: !locatorItem.searching;
              PropertyChanges { target: searchButton; opacity: 1.0 }
              PropertyChanges { target: searchField; opacity: 0.0 }
              PropertyChanges { target: resultsList; opacity: 0.0 }}
  ]

  Controls.TextField {
    id: searchField
    placeholderText: qsTr("Search…")
    onTextEdited: locator.performSearch(searchField.text)
    width: parent.width
    height: 40*dp
    anchors.right: parent.right
    visible: opacity > 0
    padding: 5*dp
    inputMethodHints: Qt.ImhNoPredictiveText  // see https://forum.qt.io/topic/12147/solved-textfield-textinput-do-not-emit-textchanged-signal
    font.pointSize: 16

    background: Rectangle {
      radius: 2*dp
      border.color: "#333"
      border.width: 1
    }

    Keys.onReleased: {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        locatorItem.searching = false
      }
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

  Controls.BusyIndicator {
    id: busyIndicator
    running: locator.isRunning
    anchors.right: searchField.right
    anchors.top: searchField.top
    anchors.margins: 4 * dp
    height: searchField.height - 8 * dp
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
      property int resultIndex: index
      color: isGroup ? "#eee" : "#fff"
      opacity: 0.95
      border.width: 1*dp
      border.color: "#bbb"

      Text {
        text: model.Text
        anchors.verticalCenter: parent.verticalCenter
        leftPadding: 5*dp
        font.italic: delegateRect.isGroup ? true : false
      }

      Row {
        id: actionsRow
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height
        anchors.rightMargin: 1*dp

        Repeater {
          model: locator.contextMenuActionsModel( index )
          Button {
            anchors.verticalCenter: parent.verticalCenter
            height: parent.height -2*dp
            width: parent.height -2*dp
            bgcolor: "#80cc28"
            Image {
              anchors.fill: parent
              source: Style.getThemeIcon( model.iconPath )
              fillMode: Image.Pad
            }
            MouseArea {
              anchors.fill: parent
              onClicked: {
                locator.triggerResultAtRow(delegateRect.resultIndex, model.id)
              }
            }
          }
        }
      }

      MouseArea {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.right: actionsRow.left

        onClicked: {
          locator.triggerResultAtRow(index)
        }
      }
    }
  }
}

