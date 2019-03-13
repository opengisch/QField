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
    locatorHighlightGeometry: locatorHighlightItem.geometryWrapper
    keepScale: qfieldSettings.locatorKeepScale

    featureListController: featureForm.extentController

    onMessageEmitted: {
      displayToast(text)
    }
  }

  Controls.TextField {
    id: searchField
    placeholderText: qsTr("Search…")
    onTextEdited: locator.performSearch(searchField.text)
    width: parent.width
    height: 40*dp
    anchors.right: parent.right
    visible: locatorItem.searching
    padding: 5*dp
    inputMethodHints: Qt.ImhNoPredictiveText  // see https://forum.qt.io/topic/12147/solved-textfield-textinput-do-not-emit-textchanged-signal
    font.pointSize: 16
    selectByMouse: true

    background: Rectangle {
      radius: 2*dp
      border.color: "#333"
      border.width: 1*dp
    }

    Keys.onReleased: {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        locatorItem.searching = false
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

  Button {
    id: searchButton
    anchors { right: parent.right; top: parent.top; }
    visible: !locatorItem.searching

    iconSource: Style.getThemeIcon( "ic_baseline_search_white" )
    round: true
    bgcolor: "#80CC28"

    onClicked: {
      locatorItem.searching = true
      searchField.focus = true
    }
  }

  Rectangle {
    id: resultsBox
    width: parent.width
    height: childrenRect.height+2*border.width
    border.color: "darkslategray"
    border.width: resultsList.count ? 1*dp : 0
    radius: 2*dp
    anchors.top: searchField.bottom
    color: "white"
    visible: locatorItem.searching

  ListView {
    id: resultsList
    anchors.centerIn: parent
    model: locator.proxyModel()
    width: parent.width-2*resultsBox.border.width
    height: Math.min( childrenRect.height, 200*dp, mainWindow.height - searchField.height )
    clip: true

    delegate: Rectangle {
      id: delegateRect
      anchors.margins: 10*dp
      height: if (visible) { if(isGroup){ 25*dp } else { Math.max(childrenRect.height+8*dp, 40*dp) }} else { 0 }
      width: parent.width
      visible: model.ResultType !== 0 // remove filter name
      property bool isGroup: model.ResultFilterGroupSorting === 0
      property int resultIndex: index
      color: isGroup ? "#eee" : "#fff"
      opacity: 0.95
      border.width: 1*dp
      border.color: "#bbb"

      Text {
        id: textCell
        text: model.Text
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.right: actionsRow.left
        leftPadding: 5*dp
        font.italic: delegateRect.isGroup ? true : false
        font.pointSize: 16
        wrapMode: Text.Wrap
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
            height: parent.height
            width:  36*dp
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
}

