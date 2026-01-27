

# File LocatorItem.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**LocatorItem.qml**](LocatorItem_8qml.md)

[Go to the documentation of this file](LocatorItem_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qgis
import org.qfield
import Theme

Item {
  id: locatorItem

  property LocatorModelSuperBridge locatorBridge

  property bool searchFieldVisible: searchField.visible
  property alias locatorFiltersModel: locatorFilters

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
      PropertyChanges {
        target: searchFieldRect
        visible: true
      }
      PropertyChanges {
        target: searchFieldRect
        width: mainWindow.width - mainWindow.sceneLeftMargin - mainWindow.sceneRightMargin - 62
      }
      PropertyChanges {
        target: codeReaderButton
        visible: true
      }
      PropertyChanges {
        target: clearButton
        visible: true
      }
      PropertyChanges {
        target: busyIndicator
        visible: true
      }
    },
    State {
      name: "off"
      PropertyChanges {
        target: busyIndicator
        visible: false
      }
      PropertyChanges {
        target: clearButton
        visible: false
      }
      PropertyChanges {
        target: codeReaderButton
        visible: false
      }
      PropertyChanges {
        target: searchFieldRect
        width: 48
      }
      PropertyChanges {
        target: searchFieldRect
        visible: false
      }
    }
  ]

  transitions: [
    Transition {
      from: "off"
      to: "on"
      SequentialAnimation {
        PropertyAnimation {
          target: searchFieldRect
          property: "visible"
          duration: 0
        }
        NumberAnimation {
          target: searchFieldRect
          easing.type: Easing.InOutQuad
          properties: "width"
          duration: 250
        }
        PropertyAnimation {
          target: codeReaderButton
          property: "visible"
          duration: 0
        }
        PropertyAnimation {
          target: clearButton
          property: "visible"
          duration: 0
        }
        PropertyAnimation {
          target: busyIndicator
          property: "visible"
          duration: 0
        }
      }
    },
    Transition {
      from: "on"
      to: "off"
      SequentialAnimation {
        PropertyAnimation {
          target: busyIndicator
          property: "visible"
          duration: 0
        }
        PropertyAnimation {
          target: clearButton
          property: "visible"
          duration: 0
        }
        PropertyAnimation {
          target: codeReaderButton
          property: "visible"
          duration: 0
        }
        NumberAnimation {
          target: searchFieldRect
          easing.type: Easing.InOutQuad
          properties: "width"
          duration: 150
        }
        PropertyAnimation {
          target: searchFieldRect
          property: "visible"
          duration: 0
        }
      }
    }
  ]

  LocatorFiltersModel {
    id: locatorFilters
    locatorModelSuperBridge: locatorBridge
  }

  Connections {
    target: locatorBridge

    function onSearchRequested(text) {
      locatorItem.state = "on";
      searchField.text = text;
    }

    function onSearchTextChangeRequested(text) {
      searchField.text = text;
    }
  }

  Connections {
    target: iface

    function onLoadProjectEnded(path, name) {
      if (searchField.text.length > 0) {
        // Any pre-existing results would most likely be invalid in a new project context, clear
        searchField.text = '';
        locatorItem.state = "off";
      }
    }
  }

  Connections {
    id: codeReaderConnection
    target: codeReader
    enabled: false

    function onAccepted(string) {
      var prefix = locatorBridge.getPrefixFromSearchString(searchField.text);
      searchField.text = prefix !== '' ? prefix + ' ' + string : string;
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
    color: Theme.controlBackgroundColor
    visible: false

    TextField {
      id: searchField
      focus: locatorItem.state == "on" ? true : false
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
      color: Theme.mainTextColor
      selectByMouse: true
      verticalAlignment: TextInput.AlignVCenter
      background: Rectangle {
        height: 48
        radius: 24
        color: "transparent"
      }
      inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhNoAutoUppercase
      onDisplayTextChanged: {
        locatorItem.state = "on";
        searchTermHandled = false;
        searchTermChanged(searchField.displayText);
        if (!searchTermHandled) {
          locatorBridge.performSearch(searchField.displayText);
        }
        if (searchField.displayText === 'f ' && dashBoard.activeLayer == undefined) {
          displayToast(qsTr('To search features within the active layer, select a vector layer through the legend.'));
        }
      }
    }
  }

  BusyIndicator {
    id: busyIndicator
    z: 11
    running: locatorBridge.isRunning
    anchors.right: codeReaderButton.left
    anchors.rightMargin: -15
    anchors.verticalCenter: codeReaderButton.verticalCenter
    height: searchFieldRect.height - 10
    visible: false
  }

  QfToolButton {
    id: clearButton
    anchors.centerIn: busyIndicator
    visible: false

    width: 40
    height: 40
    padding: 2
    z: 12

    iconSource: Theme.getThemeVectorIcon("ic_clear_black_18dp")
    iconColor: Theme.mainTextColor
    bgcolor: "transparent"
    opacity: searchField.displayText.length > 0 ? 1 : 0.25

    onClicked: {
      if (searchField.displayText.length > 0) {
        searchButton.forceActiveFocus();
        searchField.text = '';
        searchField.forceActiveFocus();
      } else {
        locatorItem.state = "off";
      }
    }
  }

  QfToolButton {
    id: codeReaderButton
    width: 40
    height: 40
    padding: 2
    z: 12
    anchors.right: searchButton.left
    anchors.rightMargin: 5
    anchors.verticalCenter: searchFieldRect.verticalCenter
    visible: false

    iconSource: withNfc ? Theme.getThemeVectorIcon("ic_qr_nfc_code_black_24dp") : Theme.getThemeVectorIcon("ic_qr_code_black_24dp")
    iconColor: Theme.darkTheme ? Theme.mainTextColor : "transparent"
    bgcolor: "transparent"

    onClicked: {
      Qt.inputMethod.hide();
      codeReader.open();
      codeReaderConnection.enabled = true;
    }
  }

  QfToolButton {
    id: searchButton
    z: 20
    anchors {
      right: parent.right
      top: parent.top
    }

    iconSource: Theme.getThemeVectorIcon("ic_baseline_search_white")
    iconColor: Theme.mainOverlayColor
    bgcolor: Theme.mainColor
    round: true

    onClicked: {
      locatorItem.state = locatorItem.state == "off" ? "on" : "off";
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
    color: Theme.mainBackgroundColor
    visible: searchFieldRect.visible && resultsList.count > 0
    clip: true

    Behavior on height {
      NumberAnimation {
        duration: 150
        easing.type: Easing.InOutQuad
      }
    }

    ListView {
      id: resultsList
      z: 2
      anchors.top: resultsBox.top
      anchors.topMargin: 24
      model: searchField.displayText !== '' ? locatorBridge.proxyModel() : locatorFilters
      width: parent.width
      height: resultsList.count > 0 ? Math.min(contentHeight, mainWindow.height / 2 - searchFieldRect.height - 10) : 0
      clip: true

      ScrollBar.vertical: QfScrollBar {}

      delegate: searchField.displayText !== '' ? resultsComponent : filtersComponent
    }

    Component {
      id: filtersComponent

      Rectangle {
        id: delegateRect

        anchors.margins: 10
        width: resultsList.width
        height: textArea.childrenRect.height + textArea.topPadding + textArea.bottomPadding
        color: "transparent"
        opacity: (Prefix === 'f' && dashBoard.activeLayer == undefined) ? 0.35 : 0.95

        Ripple {
          clip: true
          width: parent.width
          height: parent.height
          pressed: mouseArea.pressed
          anchor: delegateRect
          active: mouseArea.pressed
          color: Material.rippleColor
        }

        Column {
          id: textArea
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          anchors.right: parent.right
          topPadding: 8
          bottomPadding: 8
          spacing: 2

          Text {
            id: nameCell
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            text: Name + ' (' + Prefix + ')' + (Prefix === 'f' && dashBoard.activeLayer ? ' — ' + dashBoard.activeLayer.name : '')
            font.bold: false
            font.pointSize: Theme.resultFont.pointSize
            color: Theme.mainTextColor
            elide: Text.ElideRight
            horizontalAlignment: Text.AlignLeft
          }

          Text {
            id: descriptionCell
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            text: Description || ''
            font.bold: false
            font.pointSize: Theme.resultFont.pointSize
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
          }
        }

        /* bottom border */
        Rectangle {
          anchors.bottom: parent.bottom
          height: 1
          color: Theme.controlBorderColor
          width: parent.width
        }

        MouseArea {
          id: mouseArea
          anchors.fill: parent

          onClicked: {
            if (Prefix === 'f' && dashBoard.activeLayer == undefined) {
              displayToast(qsTr('Activate a vector layer in the legend first to use this functionality'), 'warning');
            } else {
              searchField.text = Prefix + ' ';
            }
          }
        }
      }
    }

    Component {
      id: resultsComponent

      Rectangle {
        id: delegateRect

        property bool isGroup: ResultType !== undefined && ResultType === 1
        property bool isFilterName: ResultType !== undefined && ResultType === 0
        property int resultIndex: index

        anchors.margins: 10
        height: isFilterName || isGroup ? 30 : Math.max(actionsRow.childrenRect.height, textArea.childrenRect.height + textArea.topPadding + textArea.bottomPadding)
        width: resultsList.width
        color: isFilterName ? Theme.mainColor : isGroup ? Theme.controlBorderColor : "transparent"
        opacity: 0.95

        Ripple {
          clip: true
          width: parent.width
          height: parent.height
          pressed: mouseArea.pressed
          anchor: delegateRect
          active: mouseArea.pressed
          color: Material.rippleColor
        }

        Column {
          id: textArea
          anchors.verticalCenter: parent.verticalCenter
          anchors.left: parent.left
          anchors.right: actionsRow.left
          topPadding: 8
          bottomPadding: 8
          spacing: 2

          Text {
            id: nameCell
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            text: isFilterName ? ResultFilterName : typeof (model.Text) == 'string' ? model.Text.trim() : ''
            font.bold: false
            font.pointSize: Theme.resultFont.pointSize
            color: isFilterName ? Theme.mainOverlayColor : Theme.mainTextColor
            elide: Text.ElideRight
            horizontalAlignment: isGroup ? Text.AlignHCenter : Text.AlignLeft
          }

          Text {
            id: descriptionCell
            visible: !isFilterName && !isGroup && text !== ''
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            text: locatorBridge.getLocatorModelDescription(index)
            font.bold: false
            font.pointSize: Theme.resultFont.pointSize
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignLeft
          }
        }

        Row {
          id: actionsRow
          anchors.right: parent.right
          anchors.verticalCenter: parent.verticalCenter
          height: 32
          anchors.rightMargin: 5

          Repeater {
            model: locatorBridge.contextMenuActionsModel(index)

            QfToolButton {
              anchors.verticalCenter: parent.verticalCenter
              width: 32
              height: 32
              padding: 0
              bgcolor: "transparent"

              iconSource: IconPath
              iconColor: {
                const colorMatch = IconPath.match(/(?:\?|&)color=([^\s&]+)(?:&|$)/);
                return colorMatch !== null ? Theme[colorMatch[1]] : "transparent";
              }

              onClicked: {
                locatorItem.state = "off";
                locatorBridge.triggerResultAtRow(delegateRect.resultIndex, Id);
              }
            }
          }
        }

        /* bottom border */
        Rectangle {
          anchors.bottom: parent.bottom
          height: isFilterName || isGroup ? 0 : 1
          color: Theme.controlBorderColor
          width: parent.width
        }

        MouseArea {
          id: mouseArea
          anchors.left: parent.left
          anchors.top: parent.top
          anchors.bottom: parent.bottom
          anchors.right: actionsRow.left

          onClicked: {
            if (!isFilterName && !isGroup && nameCell.text !== '') {
              locatorBridge.triggerResultAtRow(index);
              locatorItem.state = "off";
            }
          }
        }
      }
    }
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      event.accepted = true;
      state = "off";
    }
  }
}
```


