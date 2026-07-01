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
        const minContentHeight = bookmarkListToolBar.height + (bookmarksList.contentHeight + bookmarksList.anchors.bottomMargin) + 25;
        const newHeight = Math.min(minContentHeight, defaultMin);
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

  Rectangle {
    id: bookmarkListToolBar

    anchors.top: parent.top
    anchors.topMargin: topMargin
    anchors.left: parent.left
    anchors.right: parent.right
    height: topMargin + 58
    color: Theme.mainBackgroundColor
    clip: true

    property double topMargin: bookmarkList.y == 0 ? mainWindow.sceneTopMargin : 0.0
    property double leftMargin: bookmarkList.x == 0 ? mainWindow.sceneLeftMargin : 0.0
    property double rightMargin: mainWindow.sceneRightMargin

    Rectangle {
      width: parent.width * 0.3
      height: 5
      radius: 10

      anchors.horizontalCenter: parent.horizontalCenter
      anchors.top: parent.top
      anchors.topMargin: bookmarkListToolBar.topMargin + 1

      color: Theme.controlBorderColor
    }

    Item {
      anchors {
        fill: parent
        topMargin: bookmarkListToolBar.topMargin + 5
      }
      clip: true

      Text {
        property double balancedMargin: Math.max(backButton.width + selectionCount.width, menuButton.width)
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: balancedMargin + bookmarkListToolBar.leftMargin
        anchors.rightMargin: balancedMargin + bookmarkListToolBar.rightMargin
        height: 48

        font: Theme.strongFont
        color: Theme.mainTextColor
        text: qsTr("Bookmarks")
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        fontSizeMode: Text.Fit
        wrapMode: Text.Wrap
        elide: Text.ElideRight

        DragHandler {
          enabled: true
          target: null
          acceptedButtons: Qt.LeftButton
          grabPermissions: PointerHandler.CanTakeOverFromAnything
          dragThreshold: 5

          property var oldPos

          onActiveChanged: {
            if (active) {
              bookmarkList.isDragging = true;
              oldPos = centroid.scenePosition;
            } else {
              bookmarkList.statusIndicatorDragReleased();
            }
          }

          onCentroidChanged: {
            if (active) {
              const dx = centroid.scenePosition.x - oldPos.x;
              const dy = centroid.scenePosition.y - oldPos.y;
              if (dx !== 0 || dy !== 0) {
                bookmarkList.statusIndicatorDragged(dx, dy);
                oldPos = centroid.scenePosition;
              }
            }
          }
        }
      }
    }

    QfToolButton {
      id: backButton
      anchors.left: parent.left
      anchors.leftMargin: bookmarkListToolBar.leftMargin
      anchors.top: parent.top
      anchors.topMargin: bookmarkListToolBar.topMargin + 5
      width: 48
      height: 48
      clip: true
      iconSource: bookmarkList.multiSelection ? Theme.getThemeVectorIcon("ic_clear_white_24dp") : Theme.getThemeVectorIcon("ic_arrow_left_white_24dp")
      iconColor: Theme.mainTextColor

      onClicked: {
        if (bookmarkList.multiSelection) {
          bookmarkList.model.clearSelection();
          bookmarkList.setMultiSelection(false);
        } else {
          bookmarkList.hide();
        }
      }
    }

    Text {
      id: selectionCount

      anchors.left: backButton.right
      anchors.top: parent.top
      anchors.topMargin: bookmarkListToolBar.topMargin + 5

      width: (bookmarkList.multiSelection && bookmarkList.model ? 48 : 0)
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

    QfToolButton {
      id: menuButton
      anchors.right: parent.right
      anchors.rightMargin: bookmarkListToolBar.rightMargin
      anchors.top: parent.top
      anchors.topMargin: bookmarkListToolBar.topMargin + 5
      width: 48
      height: 48
      clip: true

      iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
      iconColor: Theme.mainTextColor

      onClicked: {
        bookmarkListMenu.popup(menuButton.x + menuButton.width - bookmarkListMenu.width, menuButton.y);
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
        bookmarkList.setMultiSelection(!bookmarkList.multiSelection);
        if (!bookmarkList.multiSelection) {
          bookmarkList.model.clearSelection();
        }
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      id: deleteSelectedBookmarksBtn
      text: qsTr('Delete Selected Bookmark(s)')
      icon.source: Theme.getThemeVectorIcon("ic_delete_forever_white_24dp")
      enabled: bookmarkList.multiSelection && bookmarkList.model && bookmarkList.model.selectedCount > 0

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        bookmarkListMenu.close();
        deleteBookmarksDialog.selectedCount = bookmarkList.model ? bookmarkList.model.selectedCount : 0;
        deleteBookmarksDialog.open();
      }
    }
  }

  QfDialog {
    id: deleteBookmarksDialog
    parent: mainWindow.contentItem

    property int selectedCount: 0

    title: qsTr("Delete bookmark(s)")
    Label {
      width: mainWindow.width - 60
      wrapMode: Text.WordWrap
      font: Theme.defaultFont
      color: Theme.mainTextColor
      text: qsTr("Should the %n selected bookmark(s) really be deleted?", "", deleteBookmarksDialog.selectedCount)
    }

    onAccepted: {
      if (bookmarkList.model) {
        const deleted = bookmarkList.model.deleteSelected();
        displayToast(qsTr("Deleted %n bookmark(s)", "", deleted));
      }
      bookmarkList.setMultiSelection(false);
      bookmarkList.focus = true;
    }

    onRejected: {
      bookmarkList.focus = true;
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
    height: parent.height - bookmarkListToolBar.height
    ScrollBar.vertical: QfScrollBar {}

    section.property: "BookmarkGroup"
    section.labelPositioning: ViewSection.InlineLabels
    section.delegate: Component {
      Rectangle {
        width: parent.width
        height: 30
        color: Theme.controlBorderColor

        Text {
          anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
          }
          font.bold: true
          font.pointSize: Theme.resultFont.pointSize
          color: Theme.mainTextColor
          text: {
            switch (section) {
            case "orange":
              return qsTr("Orange");
            case "red":
              return qsTr("Red");
            case "blue":
              return qsTr("Blue");
            }
            return qsTr("Green");
          }
        }
      }
    }

    delegate: Rectangle {
      id: itemBackground
      anchors {
        left: parent ? parent.left : undefined
        right: parent ? parent.right : undefined
      }
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

      CheckBox {
        id: selectionCheckBox
        anchors {
          leftMargin: 5
          left: parent.left
          verticalCenter: parent.verticalCenter
        }
        checked: BookmarkSelected
        visible: bookmarkList.multiSelection && BookmarkUser

        onClicked: {
          bookmarkList.model.toggleSelected(BookmarkId);
        }
      }

      Text {
        id: bookmarkLabel
        anchors {
          leftMargin: bookmarkList.multiSelection ? 50 : 10
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
          if (!BookmarkUser) {
            return;
          }
          bookmarkList.setMultiSelection(true);
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

  function setMultiSelection(active) {
    multiSelection = active;
    if (model) {
      model.hideProjectBookmarks = active;
    }
  }

  function statusIndicatorDragged(deltaX, deltaY) {
    fullScreenView = false;
    if (isVertical) {
      dragHeightAdjustment += deltaY;
    } else {
      dragWidthAdjustment += deltaX;
    }
  }

  function statusIndicatorDragReleased() {
    isDragging = false;
    if (isVertical) {
      const minContentHeight = bookmarkListToolBar.height + 48 + 30;
      if (bookmarkList.height < minContentHeight) {
        if (fullScreenView) {
          fullScreenView = false;
        } else {
          bookmarkList.hide();
        }
      } else if (dragHeightAdjustment < -parent.height * 0.2) {
        fullScreenView = true;
      }
    } else {
      if (bookmarkList.width < bookmarkList.parent.width * 0.3) {
        if (fullScreenView) {
          fullScreenView = false;
        } else {
          bookmarkList.hide();
        }
      } else if (dragWidthAdjustment < -parent.width * 0.2) {
        fullScreenView = true;
      }
    }
    dragHeightAdjustment = 0;
    dragWidthAdjustment = 0;
  }

  Keys.onReleased: event => {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      if (Overlay.overlay && Overlay.overlay.visibleChildren.length > 1 || (Overlay.overlay.visibleChildren.length === 1 && !toast.visible)) {
        return;
      }
      if (bookmarkList.multiSelection) {
        bookmarkList.model.clearSelection();
        bookmarkList.setMultiSelection(false);
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
        if (running)
          mapCanvasMap.freeze('bookmarkresize');
        else
          mapCanvasMap.unfreeze('bookmarkresize');
      }
    }
  }

  Behavior on height {
    enabled: !isDragging
    PropertyAnimation {
      duration: parent.width < parent.height ? 250 : 0
      easing.type: Easing.OutQuart

      onRunningChanged: {
        if (running)
          mapCanvasMap.freeze('bookmarkresize');
        else
          mapCanvasMap.unfreeze('bookmarkresize');
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
    fullScreenView = false;
    bookmarkList.setMultiSelection(false);
    if (bookmarkList.model) {
      bookmarkList.model.clearSelection();
    }
  }
}
