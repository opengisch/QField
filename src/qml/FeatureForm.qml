import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

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

    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    height: parent.height - featureListToolBar.height

    /* using a VisualDataModel to work on a tree structured model */
    model: VisualDataModel {
      id: visualLayerModel
      model: featureListModel

      /* Delegate for layer title + features */
      delegate: Item {
        anchors { left: parent.left; right: parent.right; leftMargin: 1 }
        height: layerNameBg.height + featuresList.height

        /* Layer title */
        Rectangle {
          id: layerNameBg
          anchors { left: parent.left; right:parent.right }
          height: layerNameText.height
          color: "#80A0EE"

          Text {
            id: layerNameText
            anchors { centerIn: parent; leftMargin: 5 }
            text: "<b><i>" + display + "</i></b>"
          }

          MouseArea {
            anchors.fill: parent
            onClicked: {
              console.info( "Clicked" )
              featuresList.toggleShown()
            }
          }
        }

        /* List of features within a layer */
        ColumnLayout {
          id: featuresList
          anchors { top:layerNameBg.bottom; left: parent.left; right: parent.right; leftMargin: 10 }
          clip: true

          Repeater {
            model: VisualDataModel {
              model: featureListModel
              rootIndex: visualLayerModel.modelIndex( index )

              delegate: Item {
                height: featureText.height + 1
                Text {
                  id: featureText
                  anchors { leftMargin: 10; left: parent.left; right: parent.right }
                  text: "<b>" + display + "</b>"

                  Rectangle {
                    height: 1
                    color: "lightGray"
                    width: parent.width
                    anchors.bottom: parent.bottom
                  }
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


          Behavior on height {
            PropertyAnimation {
              easing.type: Easing.InQuart
            }
          }
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

    model: featureModel

    focus: true

    visible: false

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

        /* Bottom border */
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

    clip: true

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
