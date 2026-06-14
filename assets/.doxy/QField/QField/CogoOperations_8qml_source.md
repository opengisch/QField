

# File CogoOperations.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**CogoOperations.qml**](CogoOperations_8qml.md)

[Go to the documentation of this file](CogoOperations_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQml.Models
import org.qgis
import org.qfield
import Theme

Item {
  id: cogoOperations

  property FeatureModel featureModel //<! the feature which has its geometry being edited
  property MapSettings mapSettings
  property RubberbandModel rubberbandModel //<! the rubberband model attached to the ferature model to build its geometry

  property var cogoOperationSettings: undefined

  signal requestJumpToPoint(var center, real scale, bool handleMargins)
  signal requestPosition(var item, bool fromCoordinateLocator)

  // returns true if handled
  function canvasClicked(point, type) {
    if (cogoOperationLoader.item) {
      return cogoOperationLoader.item.canvasClicked(point, type);
    } else {
      return false;
    }
  }

  // returns true if handled
  function canvasLongPressed(point, type) {
    if (cogoOperationLoader.item) {
      return cogoOperationLoader.item.canvasLongPressed(point, type);
    } else {
      return false;
    }
  }

  width: content.contentWidth + cogoOperationsContainer.spacing * 2
  height: 48
  enabled: false
  visible: enabled

  onEnabledChanged: {
    if (enabled) {
      if (cogoOperationSettings.name === "") {
        const lastUsed = settings.value("/QField/CogoOperationLastUsed", "point_at_xyz");
        const operationData = cogoOperationsModel.get(lastUsed);
        cogoOperationSettings.name = operationData.Name;
        cogoOperationSettings.title = operationData.DisplayName;
      }
    }
  }

  Container {
    id: cogoOperationsContainer
    anchors.fill: parent
    spacing: 4
    clip: true
    focusPolicy: Qt.NoFocus

    contentItem: Rectangle {
      radius: 48 / 2
      color: Theme.toolButtonBackgroundSemiOpaqueColor
      clip: true

      ListView {
        id: content
        width: content.contentWidth
        height: 46
        x: 4
        y: 4
        model: cogoOperationsContainer.contentModel
        snapMode: ListView.SnapToItem
        orientation: ListView.Horizontal
        spacing: 4
      }
    }

    Repeater {
      model: CogoOperationsModel {
        id: cogoOperationsModel
      }
      delegate: QfToolButton {
        width: 40
        height: 40
        padding: 2
        round: true
        bgcolor: cogoOperationSettings && cogoOperationSettings.name === Name ? Theme.toolButtonBackgroundColor : Theme.toolButtonBackgroundSemiOpaqueColor
        iconSource: Theme.getThemeVectorIcon(Icon)
        iconColor: cogoOperationSettings && cogoOperationSettings.name === Name ? Theme.mainColor : Theme.toolButtonColor

        onClicked: {
          Qt.inputMethod.hide();
          settings.setValue("/QField/CogoOperationLastUsed", Name);
          cogoOperationSettings.name = Name;
          cogoOperationSettings.title = DisplayName;
          displayToast(DisplayName);
        }
      }
    }
  }

  Connections {
    target: cogoOperationSettings

    function onRequestPosition(item, fromCoordinateLocator) {
      cogoOperations.requestPosition(item, fromCoordinateLocator);
    }
  }
}
```


