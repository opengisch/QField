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
QfPaneDrawer {
  id: bookmarkList
  property alias model: bookmarksList.model
  property bool multiSelection: false

  signal requestBookmarkProperties(string bookmarkId, string bookmarkName, string bookmarkGroup)

  contentVisible: props.isVisible
  freezeKey: 'bookmarkresize'
  headerHeight: bookmarkListToolBar.height
  minContentHeight: bookmarkListToolBar.height + (bookmarksList.contentHeight + bookmarksList.anchors.bottomMargin) + 25

  onCollapsed: bookmarkList.hide()

  states: [
    State {
      name: "Hidden"
    },
    State {
      name: "Visible"
    }
  ]
  state: "Hidden"

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
      text: bookmarkList.multiSelection ? qsTr('Export Selected Bookmark(s)') : qsTr('Export All Bookmarks')
      icon.source: Theme.getThemeVectorIcon("ic_download_white_24dp")
      enabled: !bookmarkList.multiSelection || (bookmarkList.model && bookmarkList.model.selectedCount > 0)

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        bookmarkListMenu.close();
        if (bookmarkList.model.exportBookmarks(bookmarkList.multiSelection)) {
          displayToast(qsTr("Bookmarks exported"));
        }
      }
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

  QfMenu {
    id: itemMenu
    title: qsTr("Bookmark Actions")

    property string itemId: ''
    property string itemName: ''
    property string itemGroup: ''

    topMargin: mainWindow.sceneTopMargin
    bottomMargin: mainWindow.sceneBottomMargin

    MenuItem {
      text: qsTr("Edit Bookmark")
      icon.source: Theme.getThemeVectorIcon("ic_edit_attributes_white_24dp")

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        bookmarkList.requestBookmarkProperties(itemMenu.itemId, itemMenu.itemName, itemMenu.itemGroup);
      }
    }

    MenuItem {
      text: qsTr("Copy Bookmark Details")
      icon.source: Theme.getThemeVectorIcon("ic_copy_black_24dp")

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        const point = bookmarkList.model.getBookmarkPoint(itemMenu.itemId);
        const crs = bookmarkList.model.getBookmarkCrs(itemMenu.itemId);
        const coordinates = StringUtils.pointInformation(point, crs);
        platformUtilities.copyTextToClipboard(itemMenu.itemName + '\n' + coordinates);
        displayToast(qsTr('Bookmark details copied to clipboard'));
      }
    }

    MenuSeparator {
      width: parent.width
    }

    MenuItem {
      text: qsTr("Delete Bookmark")
      icon.source: Theme.getThemeVectorIcon("ic_delete_forever_white_24dp")

      font: Theme.defaultFont
      height: 48
      leftPadding: Theme.menuItemLeftPadding

      onTriggered: {
        removeBookmarkDialog.open();
      }
    }
  }

  QfDialog {
    id: removeBookmarkDialog
    parent: mainWindow.contentItem

    title: qsTr("Remove bookmark")
    Label {
      width: mainWindow.width - 60
      wrapMode: Text.WordWrap
      font: Theme.defaultFont
      color: Theme.mainTextColor
      text: qsTr("You are about to remove a bookmark, proceed?")
    }

    onAccepted: {
      bookmarkList.model.removeBookmark(itemMenu.itemId);
      bookmarkList.focus = true;
    }

    onRejected: {
      bookmarkList.focus = true;
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

    section.property: "BookmarkSection"
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
            case "project":
              return qsTr("Project bookmarks");
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

      QfToolButton {
        id: itemMenuButton
        anchors {
          right: parent.right
          rightMargin: 5
          verticalCenter: parent.verticalCenter
        }
        width: 48
        height: 48
        visible: !bookmarkList.multiSelection && BookmarkUser
        round: true
        opacity: 0.5
        bgcolor: "transparent"
        iconSource: Theme.getThemeVectorIcon("ic_dot_menu_black_24dp")
        iconColor: Theme.mainTextColor

        onClicked: {
          const gc = mapToItem(bookmarkList, 0, 0);
          itemMenu.itemId = BookmarkId;
          itemMenu.itemName = BookmarkName;
          // BookmarkGroup surfaces empty groups as "green"; restore empty so edits don't persist a color onto an ungrouped bookmark.
          itemMenu.itemGroup = BookmarkGroup === 'green' ? '' : BookmarkGroup;
          itemMenu.popup(gc.x + width - itemMenu.width, gc.y - height);
        }
      }

      MouseArea {
        id: mouseArea
        anchors.fill: parent
        anchors.rightMargin: itemMenuButton.visible ? itemMenuButton.width : 0

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

  function show() {
    props.isVisible = true;
    focus = true;
    state = "Visible";
  }

  function setMultiSelection(active) {
    multiSelection = active;
    if (model) {
      model.hideProjectBookmarks = active;
    }
  }

  function hide() {
    props.isVisible = false;
    focus = false;
    isFullscreen = false;
    bookmarkList.setMultiSelection(false);
    if (bookmarkList.model) {
      bookmarkList.model.clearSelection();
    }
    state = "Hidden";
  }
}
