import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Theme

Row {
  id: control
  property int currentPage: 1
  property int allItemsCount: 0
  property int itemPerPage: 20
  property int buttonsCount: 5
  property int pagesCount: allItemsCount > 0 ? Math.ceil(allItemsCount / itemPerPage) : 0
  property int etcPosition: Math.floor(buttonsCount / 2) + 1

  signal changePage(var newPageIndex, var count)

  height: 30
  spacing: 8
  padding: 8

  QfButton {
    text: qsTr("Previous")
    height: 30
    radius: 8
    visible: pagesCount > 1
    enabled: currentPage >= 2
    bgcolor: "#00000000"
    borderColor: Theme.lightGray
    color: enabled ? Theme.mainColor : Theme.mainTextDisabledColor
    onClicked: {
      _changePage(currentPage - 1);
    }
  }

  Row {
    spacing: 5

    QfButton {
      text: "1"
      height: width
      width: 30
      radius: 8
      visible: pagesCount > 0
      checked: currentPage == 1
      bgcolor: checked ? Theme.mainColor : "#00000000"
      color: Theme.mainTextColor
      borderColor: checked ? Theme.mainColor : Theme.lightGray
      onClicked: {
        _changePage(1);
      }
    }

    Text {
      anchors.verticalCenter: parent.verticalCenter
      visible: pagesCount > buttonsCount && currentPage > etcPosition
      text: "..."
      color: Theme.mainTextColor
    }

    Repeater {
      id: pageRepeater
      model: pagesCount < 2 ? 0 : pagesCount >= buttonsCount ? buttonsCount - 2 : pagesCount - 2
      delegate: QfButton {
        property int page: currentPage <= etcPosition ? 2 + index : pagesCount - currentPage <= buttonsCount - etcPosition ? pagesCount - pageRepeater.count + index : currentPage + 2 + index - etcPosition
        text: page
        height: width
        width: 30
        radius: 8
        bgcolor: checked ? Theme.mainColor : "#00000000"
        color: Theme.mainTextColor
        borderColor: checked ? Theme.mainColor : Theme.lightGray
        checked: currentPage === page
        onClicked: {
          _changePage(page);
        }
      }
    }

    Text {
      anchors.verticalCenter: parent.verticalCenter
      visible: pagesCount > buttonsCount && pagesCount - currentPage > buttonsCount - etcPosition
      text: "..."
      color: Theme.mainTextColor
    }

    QfButton {
      text: pagesCount
      height: width
      width: 30
      radius: 8
      visible: pagesCount > 1
      checked: currentPage === pagesCount
      bgcolor: checked ? Theme.mainColor : "#00000000"
      color: Theme.mainTextColor
      borderColor: checked ? Theme.mainColor : Theme.lightGray
      onClicked: {
        _changePage(pagesCount);
      }
    }
  }

  QfButton {
    text: "Next"
    height: 30
    radius: 8
    visible: pagesCount > 1
    enabled: currentPage < pagesCount
    bgcolor: "#00000000"
    borderColor: Theme.lightGray
    color: enabled ? Theme.mainColor : Theme.mainTextDisabledColor
    onClicked: {
      _changePage(currentPage + 1);
    }
  }

  function _changePage(newPage) {
    if (newPage < 1 || newPage > pagesCount || newPage === currentPage)
      return;
    currentPage = newPage;
    changePage(currentPage, itemPerPage);
  }
}
