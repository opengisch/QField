import QtQuick
import org.qfield

Item {
  id: plugin

  property var mainWindow: iface.mainWindow()
  property var pointHandler: iface.findItemByObjectName("pointHandler")
  property string handlerName: "canvas_interaction_blocker"

  Component.onCompleted: {
    pointHandler.registerHandler(plugin.handlerName, (point, type, interactionType) => {
      return true;
    });
  }

  Component.onDestruction: {
    if (pointHandler) {
      pointHandler.deregisterHandler(plugin.handlerName);
    }
  }
}
