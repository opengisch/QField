

# File QfMapCanvasPointHandler.qml

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qml**](dir_5e9804d2322dd99f421cb2c64b2c04c3.md) **>** [**QfMapCanvasPointHandler.qml**](QfMapCanvasPointHandler_8qml.md)

[Go to the documentation of this file](QfMapCanvasPointHandler_8qml.md)


```C++
import QtQuick
import org.qfield.core

Item {
  id: root

  // Priority enum for handler registration
  enum Priority {
    Low,
    Normal = 50,
    High = 100
  }

  property Item coordinateSpace

  function pointInItem(point, item) {
    if (!item || !item.visible) {
      return false;
    }
    const itemCoordinates = item.mapToItem(root.coordinateSpace, 0, 0);
    return point.x >= itemCoordinates.x && point.x <= itemCoordinates.x + item.width && point.y >= itemCoordinates.y && point.y <= itemCoordinates.y + item.height;
  }

  // Array to store registered handlers with priorities
  property var handlers: ([])

  // Register a new handler with priority (higher priority = executed first)
  function registerHandler(name, handler, priority = QfMapCanvasPointHandler.Priority.Normal) {
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


