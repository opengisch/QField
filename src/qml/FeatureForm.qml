import QtQuick 2.0
import QtQuick.Controls 1.2

Rectangle {
  id: featureForm

  color: "white"

  focus: true // important - otherwise we'll get no key events

  Keys.onReleased: {
    if (event.key === Qt.Key_Back) {
      featureForm.hide()
      event.accepted = true
    }
  }

  ListView {
    id: globalFeaturesList

    focus: true

    model: VisualDataModel {
      id: visualLayerModel
      model: featureListModel

      delegate: Item {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 1

        Rectangle {
          id: layerName
          height: 30
          anchors.top: parent.top
          anchors.left: parent.left
          anchors.right: parent.right

          Text {
            anchors.leftMargin: 5
            anchors.fill: parent
            text: "<b><i>" + display + "</i></b>"
          }

          MouseArea {
            anchors.fill: parent
            onClicked: {
              console.info( "Clicked" )
              featuresList.toggleShown()
            }
          }

          Column {
            id: featuresList
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom
            anchors.top: layerName.bottom
            anchors.left: parent.left
            anchors.right: parent.right

            Repeater {
              model: VisualDataModel {
                model: featureListModel
                rootIndex: visualLayerModel.modelIndex( index )
                delegate: Item {
                  height: 30

                  anchors.left: parent.left
                  anchors.right: parent.right

                  Text {
                    anchors.leftMargin: 10
                    anchors.left: parent.left
                    text: "<b>" + display + "</b>"
                    clip: true
                  }
                }
              }
            }

            function toggleShown() {
              console.info( featuresList.height )
              if ( featuresList.height == 0 )
              {
                featuresList.height = undefined
              }
              else
              {
                featuresList.height = 0
              }
            }
          }

          Rectangle {
            height: 1
            color: "lightGray"
            width: parent.width
            anchors.bottom: parent.bottom
          }
        }
      }

    }

    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom

  }

  ListView {
    id: featureFormList

    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom

    model: featureModel

    focus: true

    delegate: Item {
      height: 30
      anchors.left: parent.left
      anchors.right: parent.right

      Rectangle {
        anchors.fill: parent

        Row {
          Text {
            anchors.leftMargin: 5
            anchors.right: featureFormList.left + featureFormList.width / 3
            text: "<b>" + attributeName + "</b>"
            clip: true
          }

          TextEdit {
            anchors.rightMargin: 5
            text: attributeValue
            /* onTextChanged: { feature[index].value = text; } */
          }
        }

        Rectangle {
          height: 1
          color: "lightGray"
          width: parent.width
          anchors.bottom: parent.bottom
        }
      }
    }
  }

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
          featureForm.hide()
        }
      }

      Button {
        id: previousButton

        anchors.left: parent.left

        width: 48*dp
        height: 48*dp

        iconSource: "/themes/holodark/previous_item.png"

        onClicked: {
          featureForm.hide()
        }
      }
    }

    Behavior on opacity {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }


  Behavior on width {
    PropertyAnimation {
      easing.type: Easing.InQuart
    }
  }


  Connections {
    target: featureListModel

    onModelReset: {
      featureForm.show()
    }
  }

  function show()
  {
    featureForm.width = featureForm.parent.width / 3
    featureForm.focus = true
    featureListToolBar.opacity = 100
  }

  function hide()
  {
    featureForm.width = 0
    featureForm.focus = false
    featureListToolBar.opacity = 0
  }
}
