import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Pane {
  id: bookmarkList
  property alias model: bookmarksList.model

  property bool multiSelection: false
  property bool fullScreenView: false
  property bool isVertical: parent.width < parent.height || parent.width < 300

  property bool isDragging: false
  property real dragHeightAdjustment: 0
  property real dragWidthAdjustment: 0

  signal requestJumpToPoint(var center, real scale, bool handleMargins)

  property real lastWidth

  width: {
    if (props.isVisible) {
      if (dragWidthAdjustment != 0) {
        return lastWidth - dragWidthAdjustment;
      } else if (fullScreenView || parent.width <= parent.height || width >= 0.95 * parent.width) {
        lastWidth = parent.width;
        return parent.width;
      } else {
        const newWidth = Math.min(Math.max(200, parent.width / 2.25), parent.width);
        lastWidth = newWidth;
        return newWidth;
      }
    } else {
      lastWidth = 0;
      return 0;
    }
  }
  property real lastHeight

  height: {
    if (props.isVisible) {
      if (dragHeightAdjustment != 0) {
        return Math.min(lastHeight - dragHeightAdjustment, parent.height - mainWindow.sceneTopMargin);
      } else if (fullScreenView || parent.width > parent.height || height >= 0.95 * parent.height) {
        lastHeight = parent.height;
        return parent.height;
      } else {
        const defaultMin = Math.min(Math.max(200, parent.height / 2), parent.height);
        const contentHeight = bookmarkListToolBar.height + (bookmarksList.contentHeight + bookmarksList.anchors.bottomMargin) + 25;
        const newHeight = Math.max(Math.min(contentHeight, defaultMin), Math.min(defaultMin, bookmarkListToolBar.height + 120));
        lastHeight = newHeight;
        return newHeight;
      }
    } else {
      lastHeight = 0;
      return 0;
    }
  }

  topPadding: 0
  leftPadding: 0
  rightPadding: 0
  bottomPadding: 0

  visible: props.isVisible
  clip: true

  WheelHandler {
    acceptedDevices: PointerDevice.AllDevices
    onWheel: {}
  }

  QtObject {
    id: props

    property bool isVisible: false
  }

  ToolBar {
    id: bookmarkListToolBar

    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    height: 48 + mainWindow.sceneTopMargin

    background: Rectangle {
      color: Theme.mainBackgroundColor
    }

    Item {
      anchors.fill: parent
      anchors.topMargin: mainWindow.sceneTopMargin
      anchors.leftMargin: bookmarkList.x == 0 ? mainWindow.sceneLeftMargin : 0
      anchors.rightMargin: mainWindow.sceneRightMargin

      QfToolButton {
        id: backButton
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        width: 48
        height: 48
        iconSource: bookmarkList.multiSelection ? Theme.getThemeVectorIcon("ic_clear_white_24dp") : Theme.getThemeVectorIcon("ic_arrow_left_white_24dp")
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"

        onClicked: {
          if (bookmarkList.multiSelection) {
            bookmarkList.model.clearSelection();
            bookmarkList.multiSelection = false;
          } else {
            bookmarkList.hide();
          }
        }
      }

      Text {
        id: selectionCount
        anchors.left: backButton.right
        anchors.verticalCenter: parent.verticalCenter
        width: bookmarkList.multiSelection ? 48 : 0
        visible: width > 0
        height: 48
        verticalAlignment: Text.AlignVCenter
        font: Theme.strongFont
        color: Theme.mainTextColor
        text: {
          const count = bookmarkList.model ? bookmarkList.model.selectedCount : 0;
          return count < 100 ? count : '99+';
        }
      }

      Text {
        property double balancedMargin: Math.max(backButton.width + selectionCount.width, menuButton.width)
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: balancedMargin
        anchors.rightMargin: balancedMargin
        height: 48

        font: Theme.strongFont
        color: Theme.mainTextColor
        text: qsTr("Bookmarks")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        fontSizeMode: Text.Fit
        wrapMode: Text.Wrap
        elide: Text.ElideRight
      }

      QfToolButton {
        id: menuButton
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        width: 48
        height: 48
        iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
        iconColor: Theme.mainTextColor
        bgcolor: "transparent"

        onClicked: {
          bookmarkListMenu.popup(menuButton.x + menuButton.width - bookmarkListMenu.width, menuButton.y);
        }
      }
    }
  }

  QfMenu {
    id: bookmarkListMenu
    title: qsTr("Bookmark List Menu")

    topMargin: mainWindow.sceneTopMargin
    bottomMargin: mainWindow.sceneBottomMargin

    MenuItem {
      text: qsTr('Toggle Bookmark Selection')

      checkable: true
      checked: bookmarkList.multiSelection

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding

      onTriggered: {
        bookmarkList.multiSelection = !bookmarkList.multiSelection;
        if (!bookmarkList.multiSelection) {
          bookmarkList.model.clearSelection();
        }
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: moveSelectedBookmarksBtn
      text: qsTr('Move Selected Bookmark(s)')
      icon.source: Theme.getThemeVectorIcon("ic_move_white_24dp")
      enabled: bookmarkList.multiSelection && bookmarkList.model && bookmarkList.model.selectedCount > 0

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {}
    }

    MenuItem {
      id: deleteSelectedBookmarksBtn
      text: qsTr('Delete Selected Bookmark(s)')
      icon.source: Theme.getThemeVectorIcon("ic_delete_forever_white_24dp")
      enabled: bookmarkList.multiSelection && bookmarkList.model && bookmarkList.model.selectedCount > 0

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {}
    }

    MenuItem {
      id: groupBookmarksBtn
      text: qsTr('Group Bookmarks')
      checkable: true
      checked: bookmarkList.model ? bookmarkList.model.groupByColor : false
      enabled: bookmarksList.count > 0

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemCheckLeftPadding

      onTriggered: {
        if (bookmarkList.model) {
          bookmarkList.model.groupByColor = !bookmarkList.model.groupByColor;
        }
      }
    }
  }

  ListView {
    id: bookmarksList

    clip: true
    anchors.top: bookmarkListToolBar.bottom
    anchors.left: parent.left
    anchors.leftMargin: bookmarkList.x == 0 ? mainWindow.sceneLeftMargin : 0
    anchors.right: parent.right
    anchors.rightMargin: mainWindow.sceneRightMargin
    anchors.bottom: parent.bottom
    anchors.bottomMargin: mainWindow.sceneBottomMargin

    ScrollBar.vertical: QfScrollBar {}

    delegate: Rectangle {
      id: itemBackground
      anchors.left: parent ? parent.left : undefined
      anchors.right: parent ? parent.right : undefined
      height: Math.max(48, bookmarkLabel.height)
      color: "transparent"

      Ripple {
        clip: true
        width: parent.width
        height: parent.height
        pressed: mouseArea.pressed
        anchor: itemBackground
        active: mouseArea.pressed
        color: Material.rippleColor
      }

      Rectangle {
        id: groupAccent
        anchors.left: parent.left
        height: parent.height
        width: 6
        color: {
          switch (BookmarkGroup) {
          case "orange":
            return Theme.bookmarkOrange;
          case "red":
            return Theme.bookmarkRed;
          case "blue":
            return Theme.bookmarkBlue;
          }
          return Theme.bookmarkDefault;
        }
      }

      CheckBox {
        id: selectionCheckBox
        anchors {
          leftMargin: 11
          left: parent.left
          verticalCenter: parent.verticalCenter
        }
        checked: BookmarkSelected
        visible: bookmarkList.multiSelection

        onClicked: {
          bookmarkList.model.toggleSelected(BookmarkId);
        }
      }

      Label {
        id: bookmarkLabel
        anchors {
          leftMargin: bookmarkList.multiSelection ? 56 : 16
          rightMargin: 10
          left: parent.left
          right: parent.right
          verticalCenter: parent.verticalCenter
        }
        font.bold: true
        font.pointSize: Theme.resultFont.pointSize
        color: Theme.mainTextColor
        text: BookmarkName !== '' ? BookmarkName : qsTr("Untitled bookmark")
        wrapMode: Text.WordWrap
      }

      MouseArea {
        id: mouseArea
        anchors.fill: parent

        onClicked: {
          if (bookmarkList.multiSelection) {
            bookmarkList.model.toggleSelected(BookmarkId);
          } else {
            bookmarkList.model.setExtentFromBookmark(bookmarkList.model.index(index, 0));
            bookmarkList.hide();
          }
        }

        onPressAndHold: {
          bookmarkList.multiSelection = true;
          bookmarkList.model.toggleSelected(BookmarkId);
        }
      }

      Rectangle {
        anchors.bottom: parent.bottom
        height: 1
        color: Theme.controlBorderColor
        width: parent.width
      }
    }

    Label {
      anchors.centerIn: parent
      width: parent.width - 40
      visible: bookmarksList.count === 0
      horizontalAlignment: Text.AlignHCenter
      wrapMode: Text.WordWrap
      font: Theme.defaultFont
      color: Theme.secondaryTextColor
      text: qsTr("No bookmarks yet")
    }

    Behavior on height {
      PropertyAnimation {
        easing.type: Easing.OutQuart
      }
    }
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      if (Overlay.overlay && Overlay.overlay.visibleChildren.length > 1 || (Overlay.overlay.visibleChildren.length === 1 && !toast.visible)) {
        return;
      }
      if (bookmarkList.multiSelection) {
        bookmarkList.model.clearSelection();
        bookmarkList.multiSelection = false;
      } else {
        bookmarkList.hide();
      }
      event.accepted = true;
    }
  }

  Behavior on width {
    enabled: !isDragging
    PropertyAnimation {
      duration: parent.width > parent.height ? 250 : 0
      easing.type: Easing.OutQuart

      onRunningChanged: {
        if (running) {
          mapCanvasMap.freeze('bookmarkresize');
        } else {
          mapCanvasMap.unfreeze('bookmarkresize');
        }
      }
    }
  }

  Behavior on height {
    enabled: !isDragging
    PropertyAnimation {
      duration: parent.width < parent.height ? 250 : 0
      easing.type: Easing.OutQuart

      onRunningChanged: {
        if (running) {
          mapCanvasMap.freeze('bookmarkresize');
        } else {
          mapCanvasMap.unfreeze('bookmarkresize');
        }
      }
    }
  }

  function show() {
    props.isVisible = true;
    focus = true;
  }

  function hide() {
    props.isVisible = false;
    focus = false;
    bookmarkList.multiSelection = false;
    if (bookmarkList.model) {
      bookmarkList.model.clearSelection();
      bookmarkList.model.showProjectOnly = false;
      bookmarkList.model.groupByColor = false;
    }
  }
}
