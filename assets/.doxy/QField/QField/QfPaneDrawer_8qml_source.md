

# File QfPaneDrawer.qml

[**File List**](files.md) **>** [**imports**](dir_3be62dd4600925273911e91e0c7964f3.md) **>** [**Theme**](dir_1633596792308d5fdfbf00fb99c556ce.md) **>** [**QfPaneDrawer.qml**](QfPaneDrawer_8qml.md)

[Go to the documentation of this file](QfPaneDrawer_8qml.md)


```C++
import QtQuick
import QtQuick.Controls

Pane {
  id: paneDrawer

  readonly property bool isVertical: parent.width < parent.height || parent.width < 300
  property bool isFullscreen: false
  property bool isDragging: false

  property bool contentVisible: false
  property real minContentHeight: 0
  property bool useDefaultMinHeight: false
  property real headerHeight: 0
  property string freezeKey
  signal collapsed

  QtObject {
    id: props

    property real dragHeightAdjustment: 0
    property real dragWidthAdjustment: 0
    property real lastWidth
    property real lastHeight
  }

  width: {
    if (contentVisible) {
      if (props.dragWidthAdjustment != 0) {
        return props.lastWidth - props.dragWidthAdjustment;
      } else if (isFullscreen || parent.width <= parent.height || width >= 0.95 * parent.width) {
        props.lastWidth = parent.width;
        return parent.width;
      } else {
        const newWidth = Math.min(Math.max(200, parent.width / 2.25), parent.width);
        props.lastWidth = newWidth;
        return newWidth;
      }
    } else {
      props.lastWidth = 0;
      return 0;
    }
  }

  height: {
    if (contentVisible) {
      if (props.dragHeightAdjustment != 0) {
        return Math.min(props.lastHeight - props.dragHeightAdjustment, parent.height - mainWindow.sceneTopMargin);
      } else if (isFullscreen || parent.width > parent.height || height >= 0.95 * parent.height) {
        props.lastHeight = parent.height;
        return parent.height;
      } else {
        const defaultMin = Math.min(Math.max(200, parent.height / 2), parent.height);
        const newHeight = useDefaultMinHeight ? defaultMin : Math.min(minContentHeight, defaultMin);
        props.lastHeight = newHeight;
        return newHeight;
      }
    } else {
      props.lastHeight = 0;
      return 0;
    }
  }

  topPadding: 0
  leftPadding: 0
  rightPadding: 0
  bottomPadding: 0

  visible: contentVisible
  clip: true

  WheelHandler {
    acceptedDevices: PointerDevice.AllDevices
    onWheel: {}
  }

  function statusIndicatorDragged(deltaX, deltaY) {
    isFullscreen = false;
    if (isVertical) {
      props.dragHeightAdjustment += deltaY;
    } else {
      props.dragWidthAdjustment += deltaX;
    }
  }

  function statusIndicatorDragReleased() {
    isDragging = false;
    if (isVertical) {
      const minHeight = headerHeight + 48 + 30;
      if (paneDrawer.height < minHeight) {
        if (isFullscreen) {
          isFullscreen = false;
        } else {
          paneDrawer.collapsed();
        }
      } else if (props.dragHeightAdjustment < -parent.height * 0.2) {
        isFullscreen = true;
      }
    } else {
      if (paneDrawer.width < paneDrawer.parent.width * 0.3) {
        if (isFullscreen) {
          isFullscreen = false;
        } else {
          paneDrawer.collapsed();
        }
      } else if (props.dragWidthAdjustment < -parent.width * 0.2) {
        isFullscreen = true;
      }
    }
    props.dragHeightAdjustment = 0;
    props.dragWidthAdjustment = 0;
  }

  Behavior on width {
    enabled: !isDragging
    PropertyAnimation {
      duration: paneDrawer.parent.width > paneDrawer.parent.height ? 250 : 0
      easing.type: Easing.OutQuart

      onRunningChanged: {
        if (running) {
          mapCanvasMap.freeze(paneDrawer.freezeKey);
        } else {
          mapCanvasMap.unfreeze(paneDrawer.freezeKey);
        }
      }
    }
  }

  Behavior on height {
    enabled: !isDragging
    PropertyAnimation {
      duration: paneDrawer.parent.width < paneDrawer.parent.height ? 250 : 0
      easing.type: Easing.OutQuart

      onRunningChanged: {
        if (running) {
          mapCanvasMap.freeze(paneDrawer.freezeKey);
        } else {
          mapCanvasMap.unfreeze(paneDrawer.freezeKey);
        }
      }
    }
  }
}
```


