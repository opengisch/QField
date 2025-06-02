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
  function clicked(point, type) {
    return processInteraction(point, type, "clicked");
  }

  // Process a canvas double tap
  function doubleClicked(point, type) {
    return processInteraction(point, type, "doubleClicked");
  }

  // Process a canvas press and hold
  function pressAndHold(point, type) {
    return processInteraction(point, type, "pressedAndHold");
  }

  // Helper function to process any type of interaction
  function processInteraction(point, type, interactionType) {
    // Check if any registered handler wants to handle this interaction
    for (var name in handlers) {
      if (handlers[name](point, type, interactionType)) {
        return true;
      }
    }
    return false;
  }
}
