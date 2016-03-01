import QtQuick 2.0
import org.qgis 1.0

ListView {
  id: featureFormList

  states: [
    State {
      name: "ReadOnly"
    },
    State {
      name: "Edit"
    }
  ]

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

            visible: featureFormList.state == "Edit"
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
            visible: featureFormList.state != "Edit"
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
