

# File CogoOperationSettings.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**CogoOperationSettings.qml**](CogoOperationSettings_8qml.md)

[Go to the documentation of this file](CogoOperationSettings_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield
import Theme

QfOverlayContainer {
  id: cogoOperationSettings

  property string name: ""
  property var parameters: []
  property var parameterValues: ({})

  property var positionInformations: ({})
  property GnssPositionInformation positionInformation: PositioningUtils.createEmptyGnssPositionInformation()

  property MapSettings mapSettings

  signal requestJumpToPoint(var center, real scale, bool handleMargins)
  signal requestPosition(var item, bool fromCoordinateLocator)

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
      ScrollBar.vertical: QfScrollBar {}
      model: cogoOperationSettings.parameters
      height: Math.min(mainWindow.height * 0.5, contentHeight + 10)
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
          property var parameterConfiguration: modelData !== undefined ? modelData.configuration : []
          property var destinationCrs: mapSettings.destinationCrs

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

          function onRequestPosition(item, fromCoordinateLocator) {
            cogoOperationSettings.requestPosition(item, fromCoordinateLocator);
          }
        }
      }
    }
  }

  onPositionInformationsChanged: {
    let pis = [];
    let canAverage = true;
    for (const parameter of parameters) {
      if (parameter.type === "point") {
        if (positionInformations[parameter.name] !== undefined) {
          pis.push(positionInformations[parameter.name]);
        } else {
          canAverage = false;
          break;
        }
      }
    }
    if (canAverage && pis.length > 0) {
      cogoOperationSettings.positionInformation = PositioningUtils.averagedPositionInformation(pis);
    } else {
      cogoOperationSettings.positionInformation = PositioningUtils.createEmptyGnssPositionInformation();
    }
  }

  function requestedPositionReceived(name, position, positionInformation) {
    let pis = cogoOperationSettings.positionInformations;
    pis[name] = positionInformation;
    cogoOperationSettings.positionInformations = pis;
  }

  function clear() {
    cogoOperationSettings.parameterValues = {};
    cogoOperationSettings.positionInformations = {};
    cogoOperationSettingsView.model = [];
    cogoOperationSettingsView.model = cogoOperationSettings.parameters;
  }
}
```


