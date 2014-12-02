import QtQuick 2.0

import QtQuick.Controls 1.2

Rectangle {
  id: featureForm

  color: "white"

  Rectangle {
    id: featureListToolBar

    anchors.top:parent.top
    anchors.left: parent.left
    anchors.right: parent.right

    height: 48*dp + 2*5

    opacity: 0

    color: "#80CC28"

    Item {
      anchors.margins: 5

      anchors.fill: parent

      Button {
        id: nextButton

        anchors.right: parent.right

        width: 48*dp
        height: 48*dp

        iconSource: "/themes/holodark/next_item.png"

        onClicked: {
          featureForm.width = 0
        }
      }

      Button {
        id: previousButton

        anchors.left: parent.left

        width: 48*dp
        height: 48*dp

        iconSource: "/themes/holodark/previous_item.png"

        onClicked: {
          featureForm.width = 0
        }
      }
    }
  }

  ListView {
    id: featureFormList

    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom
    anchors.margins: 10

    model: featureModel

    focus: true

    delegate: Rectangle {
      height: 20

      border.color: "lightGray"
      border.width: 1

      Row {
        Text {
          width: 60
          text: "<b>" + attributeName + "</b>"
          clip: true
        }

        TextEdit {
          text: attributeValue
          /* onTextChanged: { feature[index].value = text; } */
        }
      }
    }
  }

  Behavior on width {
    PropertyAnimation {
      easing.type: Easing.InQuart
    }
  }


  Connections {
    target: featureModel

    onModelReset: {
      featureForm.width = featureForm.parent.width / 3

      console.info( featureFormList.count )
    }
  }

  onWidthChanged: {
    if ( width < parent.width / 3 )
      featureListToolBar.opacity = 0
    else
      featureListToolBar.opacity = 100
  }
}
