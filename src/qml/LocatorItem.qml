import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.12

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

Item {
  id: locatorItem

  property bool searchFieldVisible: searchField.visible

  state: "off"

  width: searchField.width
  height: childrenRect.height

  states: [
      State {
          name: "on"
          PropertyChanges { target: searchField; visible: true; }
          PropertyChanges { target: searchField; width: mainWindow.width - 62 }
          PropertyChanges { target: clearButton; visible: true; }
      },
      State {
        name: "off"
        PropertyChanges { target: clearButton; visible: false; }
        PropertyChanges { target: searchField; width: 48 }
        PropertyChanges { target: searchField; visible: false; }
      }
  ]

  transitions: [
      Transition {
        from: "off"
        to: "on"
        SequentialAnimation {
          PropertyAnimation { target: searchField; property: "visible"; duration: 0 }
          NumberAnimation { target: searchField; easing.type: Easing.InOutQuad; properties: "width"; duration: 250 }
          PropertyAnimation { target: clearButton; property: "visible"; duration: 0 }
        }
      },
      Transition {
        from: "on"
        to: "off"
        SequentialAnimation {
          PropertyAnimation { target: clearButton; property: "visible"; duration: 0 }
          NumberAnimation { target: searchField; easing.type: Easing.InOutQuad; properties: "width"; duration: 150 }
          PropertyAnimation { target: searchField; property: "visible"; duration: 0 }
        }
      }
  ]

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

  TextField {
    id: searchField
    z: 10
    focus: locatorItem.state == "on" ? true : false
    placeholderText: qsTr("Search…")
    placeholderTextColor: Theme.mainColor
    width: 48
    height: 48
    anchors.top: parent.top
    anchors.right: parent.right
    visible: false
    topPadding: 0
    leftPadding: 24
    rightPadding: 24
    bottomPadding: 0
    font: Theme.defaultFont
    selectByMouse: true
    verticalAlignment: TextInput.AlignVCenter

    background: Rectangle {
      height: 48
      radius: 24
    }

    inputMethodHints: Qt.ImhNoPredictiveText
    onTextChanged: locator.performSearch(searchField.text)
  }

  BusyIndicator {
    id: busyIndicator
    z: 11
    running: locator.isRunning
    anchors.right: searchButton.left
    anchors.verticalCenter: searchField.verticalCenter
    height: searchField.height - 10
  }

  Image {
    id: clearButton
    z: 12
    width: 20
    height: 20
    source: Theme.getThemeIcon("ic_clear_black_18dp")
    sourceSize.width: 20 * screen.devicePixelRatio
    sourceSize.height: 20 * screen.devicePixelRatio
    fillMode: Image.PreserveAspectFit
    anchors.centerIn: busyIndicator
    opacity: searchField.text.length > 0 ? 1 : 0.25
    visible: false

    MouseArea {
      anchors.fill: parent
      onClicked: {
        if (searchField.text.length > 0) {
          searchField.text = '';
        } else {
          locatorItem.state = "off"
        }
      }
    }
  }

  QfToolButton {
    id: searchButton
    z:20
    anchors { right: parent.right; top: parent.top; }

    iconSource: Theme.getThemeIcon( "ic_baseline_search_white" )
    round: true
    bgcolor: Theme.mainColor

    onClicked: {
      locatorItem.state = "on"
    }
  }

  Rectangle {
    id: resultsBox
    z: 1
    width: searchField.width - 24
    height: searchField.visible && resultsList.count > 0 ? Math.max( 200, mainWindow.height / 2 - 48) : 0
    anchors.top: searchField.top
    anchors.left: searchField.left
    anchors.topMargin: 24
    color: "white"
    visible: searchField.visible && resultsList.count > 0

    Behavior on height {
      NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
    }

    ListView {
      id: resultsList
      z: 2
      anchors.top: resultsBox.top
      model: locator.proxyModel()
      width: parent.width
      height: parent.height
      clip: true

      delegate: Rectangle {
        id: delegateRect

        property bool isGroup: model.ResultFilterGroupSorting === 0
        property int resultIndex: index

        anchors.margins: 10
        height: isGroup ? 25 : 40
        width: parent.width
        visible: model.ResultType !== 0 // remove filter name
        color: isGroup ? Theme.lightGray : "white"
        opacity: 0.95

        Text {
          id: textCell
          text: model.Text.trim()
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          anchors.right: actionsRow.left
          leftPadding: 5
          font.bold: delegateRect.isGroup ? true : false
          font.pointSize: Theme.resultFont.pointSize
          elide: Text.ElideRight
          horizontalAlignment: isGroup ? Text.AlignHCenter : Text.AlignLeft
        }

        Row {
          id: actionsRow
          anchors.right: parent.right
          anchors.top: parent.top
          height: parent.height
          anchors.rightMargin: 1

          Repeater {
            model: locator.contextMenuActionsModel( index )
            QfToolButton {
              anchors.verticalCenter: parent.verticalCenter
              height: parent.height
              width:  36
              bgcolor: "transparent"

              iconSource: Theme.getThemeIcon( model.iconPath )

              onClicked: {
                locator.triggerResultAtRow(delegateRect.resultIndex, model.id)
              }
            }
          }
        }

        /* bottom border */
        Rectangle {
          anchors.bottom: parent.bottom
          height: isGroup ? 0 : 1
          color: "lightGray"
          width: parent.width
        }

        MouseArea {
          anchors.left: parent.left
          anchors.top: parent.top
          anchors.bottom: parent.bottom
          anchors.right: actionsRow.left

          onClicked: {
            locator.triggerResultAtRow(index)
            locatorItem.state = "off"
          }
        }
      }
    }
  }
}
