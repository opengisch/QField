import QtQuick
import QtQuick.Controls

/**
 * \ingroup qml
 */
Pane {
  id: paneDrawer

  //! TRUE when the pane is laid out vertically (portrait or narrow), driving the drag axis and resize animation direction
  readonly property bool isVertical: parent.width < parent.height || parent.width < 300
  //! When TRUE the pane expands to fill the whole available area
  property bool isFullscreen: false
  //! TRUE while the header is being dragged to resize the pane
  property bool isDragging: false

  //! When TRUE the pane is laid out at its resting size, when FALSE it collapses to zero
  property bool contentVisible: false
  //! Content driven minimum height the pane snaps to at rest, clamped to half the available height
  property real minContentHeight: 0
  //! When TRUE the resting height ignores a minContentHeight and uses the default minimum
  property bool useDefaultMinHeight: false
  //! Height of the header, used to compute the collapse threshold
  property real headerHeight: 0
  //! Key passed to mapCanvasMap freeze/unfreeze so concurrent panes does not clear each other
  property string freezeKey
  //! Emitted when a drag releases below the minimum size without entering fullscreen
  signal collapsed

  //! Internal state driving the resize math, kept out of the public interface
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
