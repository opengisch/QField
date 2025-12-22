import QtQuick
import QtQuick.Controls
import Theme
import org.qgis
import org.qfield

QfOverlayContainer {
  id: cogoSettings

  property var parameters: [{}]

  signal requestJumpToPoint(var center, real scale, bool handleMargins)
  signal requestPosition(var item)

  width: parent.width
  height: childrenRect.height

  Rectangle {
    width: parent.width
    height: cogoSettingsView.height
    color: Theme.mainBackgroundColorSemiOpaque

    ListView {
      id: cogoSettingsView
      anchors {
        left: parent.left
        right: parent.right
      }
      ScrollBar.vertical: QfScrollBar {
      }
      model: cogoSettings.parameters
      height: Math.min(mainWindow.height / 2, contentHeight + 10)
      topMargin: 5
      leftMargin: 10
      rightMargin: 10
      bottomMargin: 5
      clip: true

      delegate: Item {
        width: parent.width
        height: childrenRect.height + 4

        Loader {
          id: parameter

          property string label: parameterLabel !== undefined ? parameterLabel : ""
          property string name: parameterName

          y: 2
          width: parent.width
          source: "cogoparameterwidgets/" + parameterType + ".qml"
        }

        Connections {
          target: parameter.item

          function onRequestPosition(item) {
            cogoSettings.requestPosition(item);
          }
        }
      }
    }
  }
}
