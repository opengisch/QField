

# File MapCanvasPointHandler.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**MapCanvasPointHandler.qml**](MapCanvasPointHandler_8qml.md)

[Go to the documentation of this file](MapCanvasPointHandler_8qml.md)


```C++
import QtQuick

Item {
  id: root

  // Priority enum for handler registration
  enum Priority {
    Low,
    Normal = 50,
    High = 100
  }

  function pointInItem(point, item) {
    const itemCoordinates = item.mapToItem(mainWindow.contentItem, 0, 0);
    return point.x >= itemCoordinates.x && point.x <= itemCoordinates.x + item.width && point.y >= itemCoordinates.y && point.y <= itemCoordinates.y + item.height;
  }

  // Array to store registered handlers with priorities
  property var handlers: ([])

  // Register a new handler with priority (higher priority = executed first)
  function registerHandler(name, handler, priority = MapCanvasPointHandler.Priority.Normal) {
    const existingIndex = handlers.findIndex(h => h.name === name);
    if (existingIndex !== -1) {
      console.warn("Handler with name " + name + " already exists");
      return false;
    }
    handlers.push({
      "name": name,
      "handler": handler,
      "priority": priority
    });

    // Sort handlers by priority (descending - highest first)
    handlers.sort((a, b) => b.priority - a.priority);
    return true;
  }

  // Deregister a handler
  function deregisterHandler(name) {
    const index = handlers.findIndex(h => h.name === name);
    if (index === -1) {
      console.warn("Handler with name " + name + " does not exist");
      return false;
    }
    handlers.splice(index, 1);
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
    for (const handler of handlers) {
      if (handler.handler(point, type, interactionType)) {
        return true;
      }
    }
    return false;
  }
}
```


