

# File QfCogoOperationSettings.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfCogoOperationSettings.qml**](QfCogoOperationSettings_8qml.md)

[Go to the documentation of this file](QfCogoOperationSettings_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import org.qgis
import org.qfield.core
import org.qfield.gui

QfOverlayContainer {
  id: cogoOperationSettings

  property string name: ""
  property var parameters: []
  property var parameterValues: ({})

  property var positionInformations: ({})
  property gnssPositionInformation positionInformation: QfPositioningUtils.createEmptyGnssPositionInformation()

  property MapSettings mapSettings

  signal requestJumpToPoint(var center, real scale, bool handleMargins)
  signal requestPosition(var item, bool fromCoordinateLocator)

  width: parent.width
  height: childrenRect.height

  Rectangle {
    width: parent.width
    height: cogoOperationSettingsView.height
    color: QfTheme.mainBackgroundColorSemiOpaque

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
          source: {
            const type = modelData.type;
            return "cogoparameterwidgets/QfCogoParameter" + type.charAt(0).toUpperCase() + type.slice(1) + ".qml";
          }
        }

        Connections {
          target: parameter.item

          function onValueChangeRequested(value) {
            let pv = cogoOperationSettings.parameterValues;
            pv[parameter.parameterName] = value;
            cogoOperationSettings.parameterValues = pv;
            cogoOperationSettings.parameterValuesChanged();
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
      cogoOperationSettings.positionInformation = QfPositioningUtils.averagedPositionInformation(pis);
    } else {
      cogoOperationSettings.positionInformation = QfPositioningUtils.createEmptyGnssPositionInformation();
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


