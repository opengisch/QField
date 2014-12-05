import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
  id: toolBar

  property int currentIndex: 0
  property int count: 0
  property string currentName: ''
  property variant model

  signal gotoNext
  signal gotoPrevious
  signal statusIndicatorClicked

  anchors.top:parent.top
  anchors.left: parent.left
  anchors.right: parent.right
  height: 48*dp

  clip: true


  Button {
    id: nextButton

    anchors.right: parent.right

    width: 48*dp
    height: 48*dp

    iconSource: "/themes/holodark/next_item.png"

    onClicked: {
      toolBar.gotoNext()
    }

    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  Button {
    id: previousButton

    anchors.left: parent.left

    width: 48*dp
    height: 48*dp

    iconSource: "/themes/holodark/previous_item.png"

    onClicked: {
      toolBar.gotoPrevious
    }

    Behavior on width {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  Rectangle {
    id: navigationStatusIndicator
    anchors { left: previousButton.right; right: nextButton.left }
    height: 48*dp

    color: "#80CC28"

    clip: true

    focus: true

    Text {
      anchors.centerIn: parent

      text: currentIndex + '/' + count + ': ' + currentName
    }

    MouseArea {
      anchors.fill: parent

      onClicked: {
        toolBar.statusIndicatorClicked
      }
    }
  }

  function hideNavigationButtons() {
    previousButton.width = 0
    nextButton.width = 0
  }

  function showNavigationButtons() {
    previousButton.width = 48*dp
    nextButton.width = 48*dp
  }

  function onCurrentIndexChanged() {
    _updateInfo()
  }

  function onCurrentNameChanged() {
    _updateInfo()
  }

  function onCountChanged() {
    _updateInfo()
  }

  function _updateInfo() {

  }

  Connections {
    target: model

    onModelReset: {
      count = model.count()
    }
  }
}
