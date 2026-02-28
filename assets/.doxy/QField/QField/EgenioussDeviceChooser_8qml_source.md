

# File EgenioussDeviceChooser.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**EgenioussDeviceChooser.qml**](EgenioussDeviceChooser_8qml.md)

[Go to the documentation of this file](EgenioussDeviceChooser_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

Item {
  width: parent.width
  height: childrenRect.height

  property string deviceAddress
  property string devicePort

  function generateName() {
    return "Egeniouss";
  }

  function setSettings(settings) {
    loader.item.setSettings(settings);
  }

  function getSettings() {
    return loader.item.getSettings();
  }

  function setupLoader(uiComponent) {
    loader.sourceComponent = uiComponent;
  }

  Loader {
    id: loader
    width: parent.width
  }
}
```


