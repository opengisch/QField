

# File QfEgenioussDeviceChooser.qml

[**File List**](files.md) **>** [**gui**](dir_99d0482cf009f9d97a0877749b817f19.md) **>** [**qml**](dir_fe94622d8d68495e133d6eeeba479fc2.md) **>** [**QfEgenioussDeviceChooser.qml**](QfEgenioussDeviceChooser_8qml.md)

[Go to the documentation of this file](QfEgenioussDeviceChooser_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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


