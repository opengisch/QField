import QtQuick
import QtQuick.Controls
import Theme
import org.qgis
import org.qfield

QfOverlayContainer {
  id: cogoOperationSettings

  property string name: ""
  property var parameters: []
  property var parameterValues: ({})

  signal requestJumpToPoint(var center, real scale, bool handleMargins)
  signal requestPosition(var item)

  width: parent.width
  height: childrenRect.height

  Rectangle {
    width: parent.width
    height: cogoOperationSettingsView.height
    color: Theme.mainBackgroundColorSemiOpaque

    ListView {
      id: cogoOperationSettingsView
      anchors {
        left: parent.left
        right: parent.right
      }
      ScrollBar.vertical: QfScrollBar {
      }
      model: cogoOperationSettings.parameters
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

          property string parameterName: modelData !== undefined ? modelData.name : ""
          property string parameterLabel: modelData !== undefined ? modelData.label : ""
          property color parameterColor: modelData !== undefined ? modelData.color : "#00000000"
          property var parameterConfiguration: modelData !== undefined ? modelData.config : []

          y: 2
          width: parent.width
          source: "cogoparameterwidgets/" + modelData.type + ".qml"
        }

        Connections {
          target: parameter.item

          function onValueChangeRequested(value) {
            let pv = cogoOperationSettings.parameterValues;
            pv[parameter.parameterName] = value;
            cogoOperationSettings.parameterValues = pv;
          }

          function onRequestPosition(item) {
            cogoOperationSettings.requestPosition(item);
          }
        }
      }
    }
  }
}
