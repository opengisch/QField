import QtQuick 2.0
import QtQuick.Controls 1.4
import org.qgis 1.0

Rectangle {
  signal saved
  signal cancelled

  property alias model: featureFormList.model
  property alias toolbarVisible: toolbar.visible

  id: form

  states: [
    State {
      name: "ReadOnly"
    },
    State {
      name: "Edit"
    },
    State {
      name: "Add"
    }
  ]

  Rectangle {
    id: toolbar

    anchors.left: parent.left
    anchors.right: parent.right
    anchors.top: parent.top

    height: visible ? 48*dp : 0

    Button {
      anchors.right: parent.right

      width: 48*dp
      height: 48*dp

      iconSource: "/themes/holodark/accept.png"

      onClicked: {
        Qt.inputMethod.hide()

        if ( form.state === "Add" ) {
          featureFormList.model.create()
          state = "Edit"
        }
        featureFormList.model.save()

        saved()
      }
    }

    Button {
      anchors.left: parent.left

      width: 48*dp
      height: 48*dp

      iconSource: "/themes/holodark/cancel.png"

      onClicked: {
        Qt.inputMethod.hide()

        cancelled()
      }
    }
  }

  ListView {
    id: featureFormList

    anchors.top: toolbar.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.bottom: parent.bottom

    delegate: Item {
      anchors.left: parent.left
      anchors.right: parent.right
      height: childrenRect.height

      Rectangle {
        anchors {
          right: parent.right;
          left: parent.left
        }

        height: Math.max( 30*dp, childrenRect.height )

        Item {
          anchors {
            verticalCenter: parent.verticalCenter;
            right: parent.right;
            left:parent.left
          }

          height: childrenRect.height

          /* attribute name */
          Text {
            id: txtAttributeName
            anchors.leftMargin: 5
            width: featureFormList.width / 3
            font.bold: true
            text: attributeName
            clip: true
          }

          Item {
            anchors { leftMargin: 5; right: parent.right; left: txtAttributeName.right }
            height: childrenRect.height

            /* attribute value */
            Loader {
              id: attributeEditorLoader
              anchors { left: parent.left; right: parent.right }

              visible: form.state != "ReadOnly"
              property var value: attributeValue
              property var config: editorWidgetConfig

              source: 'editorwidgets/' + editorWidget + '.qml'

              onStatusChanged: {
                if (attributeEditorLoader.status == Loader.Error )
                  attributeEditorLoader.source = 'editorwidgets/TextEdit.qml'
              }
            }

            Connections {
              target: attributeEditorLoader.item
              onValueChanged: featureFormList.model.setAttribute( index, value, FeatureModel.AttributeValue )
            }

            Text {
              visible: form.state == "ReadOnly"
              text: attributeValue
            }
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
}
