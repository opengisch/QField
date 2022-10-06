import QtQuick 2.14
import QtQuick.Controls 2.14

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Item {
  id: locatorItem

  property bool searchFieldVisible: searchField.visible
  property alias locatorModelSuperBridge: locator

  /* Emitted when the search term typed into the locator bar has changed. If
   * the searchTermHandled boolean property is set to true while the signal
   * is propagated, the locator will consider the search term as having been
   * handled and will skip the default behavior.
   */
  signal searchTermChanged(var searchTerm)
  property bool searchTermHandled: false

  state: "off"

  width: searchFieldRect.width
  height: childrenRect.height

  states: [
      State {
          name: "on"
          PropertyChanges { target: searchFieldRect; visible: true; }
          PropertyChanges { target: searchFieldRect; width: mainWindow.width - 62 }
          PropertyChanges { target: barcodeReaderButtonRect; visible: true; }
          PropertyChanges { target: clearButtonRect; visible: true; }
          PropertyChanges { target: busyIndicator; visible: true; }
      },
      State {
        name: "off"
        PropertyChanges { target: busyIndicator; visible: false; }
        PropertyChanges { target: clearButtonRect; visible: false; }
        PropertyChanges { target: barcodeReaderButtonRect; visible: false; }
        PropertyChanges { target: searchFieldRect; width: 48 }
        PropertyChanges { target: searchFieldRect; visible: false; }
      }
  ]

  transitions: [
      Transition {
        from: "off"
        to: "on"
        SequentialAnimation {
          PropertyAnimation { target: searchFieldRect; property: "visible"; duration: 0 }
          NumberAnimation { target: searchFieldRect; easing.type: Easing.InOutQuad; properties: "width"; duration: 250 }
          PropertyAnimation { target: barcodeReaderButtonRect; property: "visible"; duration: 0 }
          PropertyAnimation { target: clearButtonRect; property: "visible"; duration: 0 }
          PropertyAnimation { target: busyIndicator; property: "visible"; duration: 0 }
        }
      },
      Transition {
        from: "on"
        to: "off"
        SequentialAnimation {
          PropertyAnimation { target: busyIndicator; property: "visible"; duration: 0 }
          PropertyAnimation { target: clearButtonRect; property: "visible"; duration: 0 }
          PropertyAnimation { target: barcodeReaderButtonRect; property: "visible"; duration: 0 }
          NumberAnimation { target: searchFieldRect; easing.type: Easing.InOutQuad; properties: "width"; duration: 150 }
          PropertyAnimation { target: searchFieldRect; property: "visible"; duration: 0 }
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

  Connections {
    target: iface

    function onLoadProjectEnded(path,name) {
      if (searchField.text.length > 0) {
        // Any pre-existing results would most likely be invalid in a new project context, clear
        searchField.text = '';
        locatorItem.state = "off"
      }
    }
  }

  Connections {
    id: barcodeReaderConnection
    target: barcodeReader
    enabled: false

    function onDecoded(string) {
      searchField.text = string;
    }

    function onVisibleChanged() {
      if (!visible) {
        enabled = false;
      }
    }
  }

  Rectangle {
    id: searchFieldRect
    z: 10
    anchors.top: parent.top
    anchors.right: parent.right
    width: 48
    height: 48
    radius: 24
    color: "white"
    visible: false

    TextField {
      id: searchField
      focus: locatorItem.state == "on" ? true : false
      placeholderText: qsTr("Search…")
      placeholderTextColor: Theme.mainColor
      width: parent.width - busyIndicator.width - 76
      height: 48
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.verticalCenter: searchFieldRect.verticalCenter
      topPadding: 0
      leftPadding: 24
      rightPadding: 0
      bottomPadding: 0
      font: Theme.defaultFont
      selectByMouse: true
      verticalAlignment: TextInput.AlignVCenter
      background: Rectangle {
        height: 48
        radius: 24
        color: "transparent"
      }
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
      onDisplayTextChanged: {
        searchTermHandled = false
        searchTermChanged(searchField.displayText)
        if (!searchTermHandled) {
          locator.performSearch(searchField.displayText)
        }
      }
    }
  }

  BusyIndicator {
    id: busyIndicator
    z: 11
    running: locator.isRunning
    anchors.right: barcodeReaderButtonRect.left
    anchors.rightMargin: -15
    anchors.verticalCenter: barcodeReaderButtonRect.verticalCenter
    height: searchFieldRect.height - 10
    visible: false
  }

  Rectangle {
    id: clearButtonRect
    width: 40
    height: 40
    z: 12
    color: "transparent"
    anchors.centerIn: busyIndicator
    visible: false

    Image {
      id: clearButton
      z: 12
      width: 20
      height: 20
      source: Theme.getThemeIcon("ic_clear_black_18dp")
      sourceSize.width: 20 * screen.devicePixelRatio
      sourceSize.height: 20 * screen.devicePixelRatio
      fillMode: Image.PreserveAspectFit
      anchors.centerIn: clearButtonRect
      opacity: searchField.displayText.length > 0 ? 1 : 0.25
    }

    MouseArea {
      anchors.fill: parent
      onClicked: {
        if (searchField.displayText.length > 0) {
          searchButton.forceActiveFocus();
          searchField.text = '';
          searchField.forceActiveFocus();
        } else {
          locatorItem.state = "off"
        }
      }
    }
  }

  Rectangle {
    id: barcodeReaderButtonRect
    width: 40
    height: 40
    z: 12
    color: "transparent"
    anchors.right: searchButton.left
    anchors.rightMargin: 5
    anchors.verticalCenter: searchFieldRect.verticalCenter
    visible: false

    Image {
      id: barcodeReaderButton
      z: 12
      width: 24
      height: 24
      source: Theme.getThemeVectorIcon("ic_qrcode_black_24dp")
      sourceSize.width: 24 * screen.devicePixelRatio
      sourceSize.height: 24 * screen.devicePixelRatio
      fillMode: Image.PreserveAspectFit
      anchors.centerIn: barcodeReaderButtonRect
      opacity: 1
    }

    MouseArea {
      anchors.fill: parent
      onClicked: {
        Qt.inputMethod.hide();

        barcodeReader.open();
        barcodeReaderConnection.enabled = true;
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
      locatorItem.state = locatorItem.state =="off" ? "on" : "off"
    }

    onPressAndHold: {
      locatorSettings.open();
      locatorSettings.focus = true;
    }
  }

  Rectangle {
    id: resultsBox
    z: 1
    width: searchFieldRect.width - 24
    height: searchFieldRect.visible && resultsList.count > 0 ? resultsList.height + 24 : 0
    anchors.top: searchFieldRect.top
    anchors.left: searchFieldRect.left
    anchors.topMargin: 24
    color: "white"
    visible: searchFieldRect.visible && resultsList.count > 0
    clip: true

    Behavior on height {
      NumberAnimation { duration: 150; easing.type: Easing.InOutQuad }
    }

    ListView {
      id: resultsList
      z: 2
      anchors.top: resultsBox.top
      anchors.topMargin: 24
      model: locator.proxyModel()
      width: parent.width
      height: resultsList.count > 0 ? Math.min( childrenRect.height, mainWindow.height / 2 - searchFieldRect.height - 10 ) : 0
      clip: true

      delegate: Rectangle {
        id: delegateRect

        property bool isGroup: model.ResultFilterGroupSorting === 0
        property bool isFilterName: model.ResultType === 0
        property int resultIndex: index

        anchors.margins: 10
        height: isFilterName || isGroup ? 30 : 40
        width: resultsList.width
        color: isFilterName ? Theme.mainColor : isGroup ? Theme.lightGray : "white"
        opacity: 0.95

        Text {
          id: textCell
          text: isFilterName ? model.ResultFilterName : model.Text.trim()
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          anchors.right: actionsRow.left
          leftPadding: 5
          font.bold: false
          font.pointSize: Theme.resultFont.pointSize
          color: isFilterName ? "white" : "black"
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
                locatorItem.state = "off"
              }
            }
          }
        }

        /* bottom border */
        Rectangle {
          anchors.bottom: parent.bottom
          height: isFilterName || isGroup ? 0 : 1
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
