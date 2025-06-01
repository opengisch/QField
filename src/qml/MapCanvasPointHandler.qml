import QtQuick

Item {
  id: root

  // Dictionary to store registered handlers
  property var handlers: ({})

  // Register a new handler
  function registerHandler(name, handler) {
    if (handlers[name]) {
      console.warn("Handler with name " + name + " already exists");
      return false;
    }
    handlers[name] = handler;
    return true;
  }

  // Deregister a handler
  function deregisterHandler(name) {
    if (!handlers[name]) {
      console.warn("Handler with name " + name + " does not exist");
      return false;
    }
    delete handlers[name];
    return true;
  }

  // Process a canvas click
  function canvasClicked(point, type) {
    // Check if any registered handler wants to handle this click
    for (var name in handlers) {
      if (handlers[name](point, type)) {
        return true;
      }
    }
    return false;
  }
}
