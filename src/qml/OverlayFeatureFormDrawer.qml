import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Window
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Pane {
  id: overlayFeatureFormDrawer

  property bool fullScreenView: qfieldSettings.fullScreenIdentifyView
  property bool isVertical: parent.width < parent.height || parent.width < 300

  property bool isDragging: false
  property real dragHeightAdjustment: 0
  property real dragWidthAdjustment: 0

  property alias featureModel: attributeFormModel.featureModel
  property alias state: overlayFeatureForm.state
  property alias featureForm: overlayFeatureForm
  property alias digitizingToolbar: overlayFeatureForm.digitizingToolbar
  property alias codeReader: overlayFeatureForm.codeReader
  property bool isAdding: false

  signal requestJumpToPoint(var center, real scale, bool handleMargins)

  focus: visible

  property int lastWidth

  width: {
    if (props.isVisible) {
      if (dragWidthAdjustment != 0) {
        return lastWidth - dragWidthAdjustment;
      } else if (fullScreenView || parent.width <= parent.height || width >= 0.95 * parent.width) {
        lastWidth = parent.width;
        return lastWidth;
      } else {
        lastWidth = Math.min(Math.max(200, parent.width / 2.25), parent.width);
        return lastWidth;
      }
    } else {
      lastWidth = 0;
      return 0;
    }
  }
  
  property int lastHeight

  height: {
    if (props.isVisible) {
      if (dragHeightAdjustment != 0) {
        return Math.min(lastHeight - dragHeightAdjustment, parent.height - mainWindow.sceneTopMargin);
      } else if (fullScreenView || parent.width > parent.height || height >= 0.95 * parent.height) {
        lastHeight = parent.height - mainWindow.sceneTopMargin;
        return lastHeight;
      } else {
        lastHeight = Math.min(Math.max(200, parent.height / 2), parent.height - mainWindow.sceneTopMargin);
        return lastHeight;
      }
    } else {
      lastHeight = 0;
      return 0;
    }
  }

  topPadding: 0
  leftPadding: 0
  rightPadding: 0
  bottomPadding: 0

  enabled: true
  visible: props.isVisible

  QtObject {
    id: props

    property bool isVisible: false
  }

  /**
   * When the form becomes visible, mark as adding
   */
  onVisibleChanged: {
    if (visible) {
      isAdding = true;
    } else {
      onClosed();
    }
  }

  function onClosed() {
    if (digitizingToolbar && !digitizingToolbar.geometryRequested) {
      if (!overlayFeatureForm.isSaved) {
        overlayFeatureForm.confirm();
      } else {
        overlayFeatureForm.isSaved = false; //reset
      }
      digitizingRubberband.model.reset();
      featureModel.resetFeature();
      isAdding = false;
    }
  }

  Connections {
    target: digitizingToolbar

    function onGeometryRequestedChanged() {
      if (digitizingToolbar.geometryRequested && overlayFeatureFormDrawer.isAdding) {
        hide(); // note: the digitizing toolbar will re-open the drawer to avoid panel stacking issues
      }
    }
  }

  FeatureForm {
    id: overlayFeatureForm
    anchors.fill: parent
    visible: true

    topMargin: overlayFeatureFormDrawer.y === 0 ? mainWindow.sceneTopMargin : 0.0
    leftMargin: overlayFeatureFormDrawer.x === 0 ? mainWindow.sceneLeftMargin : 0.0
    rightMargin: mainWindow.sceneRightMargin
    bottomMargin: mainWindow.sceneBottomMargin

    property bool isSaved: false

    model: AttributeFormModel {
      id: attributeFormModel
      featureModel: FeatureModel {
        project: qgisProject
        appExpressionContextScopesGenerator: appScopesGenerator
        topSnappingResult: coordinateLocator.topSnappingResult
      }
    }

    state: "Add"

    onConfirmed: {
      displayToast(qsTr("Changes saved"));
      //close drawer if still open
      if (props.isVisible) {
        overlayFeatureForm.isSaved = true; //because just saved
        hide();
      } else {
        overlayFeatureForm.isSaved = false; //reset
      }
      digitizingToolbar.digitizingLogger.writeCoordinates();
      resetTabs();
    }

    onCancelled: {
      displayToast(qsTr("Changes discarded"));
      //close drawer if still open
      if (props.isVisible) {
        overlayFeatureForm.isSaved = true; //because never changed
        hide();
      } else {
        overlayFeatureForm.isSaved = false; //reset
      }
      digitizingToolbar.digitizingLogger.clearCoordinates();
      resetTabs();
    }

    onRequestJumpToPoint: function (center, scale, handleMargins) {
      overlayFeatureFormDrawer.requestJumpToPoint(center, scale, handleMargins);
    }

    onToolbarDragged: function (deltaX, deltaY) {
      fullScreenView = false;
      if (isVertical) {
        dragHeightAdjustment += deltaY;
      } else {
        dragWidthAdjustment += deltaX;
      }
    }

    onToolbarDragAcquired: {
      isDragging = true;
    }

    onToolbarDragReleased: {
      isDragging = false;
      if (isVertical) {
        if (overlayFeatureFormDrawer.height < overlayFeatureFormDrawer.parent.height * 0.3) {
          if (fullScreenView) {
            fullScreenView = false;
          } else {
            hide();
          }
        } else if (dragHeightAdjustment < -parent.height * 0.2) {
          fullScreenView = true;
        }
      } else {
        if (overlayFeatureFormDrawer.width < overlayFeatureFormDrawer.parent.width * 0.3) {
          if (fullScreenView) {
            fullScreenView = false;
          } else {
            hide();
          }
        } else if (dragWidthAdjustment < -parent.width * 0.2) {
          fullScreenView = true;
        }
      }
      dragHeightAdjustment = 0;
      dragWidthAdjustment = 0;
    }

    Keys.onReleased: event => {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        if (overlayFeatureForm.model.constraintsHardValid || qfieldSettings.autoSave) {
          hide();
        } else {
          //block closing to fix constraints or cancel with button
          displayToast(qsTr("Constraints not valid"), 'warning');
        }
        event.accepted = true;
      }
    }
  }

  Rectangle {
    width: parent.width * 0.3
    height: 5
    radius: 10

    anchors.horizontalCenter: parent.horizontalCenter
    anchors.top: parent.top
    anchors.topMargin: overlayFeatureForm.topMargin + 2

    color: Theme.controlBorderColor
  }

  function show() {
    props.isVisible = true;
  }

  function hide() {
    props.isVisible = false;
  }

  Behavior on width  {
    PropertyAnimation {
      duration: parent.width > parent.height && !isDragging ? 250 : 0
      easing.type: Easing.OutQuart
    }
  }

  Behavior on height  {
    PropertyAnimation {
      duration: parent.width < parent.height && !isDragging ? 250 : 0
      easing.type: Easing.OutQuart
    }
  }
}
