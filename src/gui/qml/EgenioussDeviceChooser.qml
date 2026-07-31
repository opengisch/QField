import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme

/**
 * \ingroup qml
 */
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
