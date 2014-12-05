import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1

Rectangle {
  id: featureForm

  states: [
    State {
      name: "Hidden"
      StateChangeScript {
        script: hide()
      }
    },
    State {
      name: "FeatureList"
      PropertyChanges {
        target: globalFeaturesList
        shown: true
      }
      PropertyChanges {
        target: featureListToolBar
        showNavigationButtons: false
      }
      StateChangeScript {
        script: show()
      }
    },
    State {
      name: "FeatureForm"
      PropertyChanges {
        target: globalFeaturesList
        shown: false
      }
      PropertyChanges {
        target: featureListToolBar
        showNavigationButtons: true
      }
    }
  ]
  state: "Hidden"

  focus: ( state != "Hidden" )

  clip: true

  ListView {
    id: globalFeaturesList

    anchors.top: featureListToolBar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    height: parent.height - featureListToolBar.height

    property bool shown: false

    clip: true

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
          height: 48*dp
          color: "#80A0EE"

          Text {
            id: layerNameText
            anchors { centerIn: parent; leftMargin: 5 }
            text: "<b><i>" + display + "</i></b>"
          }

          MouseArea {
            anchors.fill: parent
            onClicked: {
              featuresList.toggleVisible()
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
                anchors { left: parent.left; right: parent.right }

                focus: true

                height: 48*dp
                Text {
                  id: featureText
                  anchors { leftMargin: 10; left: parent.left; right: parent.right; verticalCenter: parent.verticalCenter }
                  text: "<b>" + display + "</b>"
                }

                /* bottom border */
                Rectangle {
                  anchors.bottom: parent.bottom
                  height: 1
                  color: "lightGray"
                  width: parent.width
                }

                MouseArea {
                  anchors.fill: parent

                  onClicked: {
                    iface.showFeatureForm( feature )
                    globalFeaturesList.height = 0
                  }

                  onPressAndHold:
                  {

                  }
                }
              }
            }
          }

          function toggleVisible() {
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

    /* bottom border */
    Rectangle {
      anchors.bottom: parent.bottom
      height: 1
      color: "lightGray"
      width: parent.width
    }

    onShownChanged: {
      if ( shown )
      {
        height = parent.height - featureListToolBar.height
      }
      else
      {
        height = 0
      }
    }

    Behavior on height {
      PropertyAnimation {
        easing.type: Easing.InQuart
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

    visible: (!globalFeaturesList.shown)

    delegate: Item {
      height: 30
      anchors.left: parent.left
      anchors.right: parent.right

      Rectangle {
        anchors.fill: parent

        Row {
          Text {
            anchors.leftMargin: 5
            width: featureFormList.width / 3
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

  NavigationBar {
    id: featureListToolBar
    model: featureListModel

    Behavior on opacity {
      PropertyAnimation {
        easing.type: Easing.InQuart
      }
    }
  }

  Keys.onReleased: {
    if (event.key === Qt.Key_Back) {
      featureForm.hide()
      event.accepted = true
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
      state = "FeatureList"
    }
  }

  Connections {
    target: featureModel

    onModelReset: {
      state = "FeatureForm"
    }
  }

  Connections {
    target: featureListToolBar

    onStatusIndicatorClicked: {
      state = "FeatureList"
    }
  }

  function show()
  {
    var widthDenominator = settings.value( "featureForm/widthDenominator", 3 );
    width = parent.width / widthDenominator
    // Focus to retrieve back button events
    focus = true
  }

  function hide()
  {
    width = 0
    focus = false
  }
}
