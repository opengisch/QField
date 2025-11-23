import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Window
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Drawer {
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

  edge: parent.width < parent.height ? Qt.BottomEdge : Qt.RightEdge
  closePolicy: Popup.NoAutoClose // prevent accidental feature addition when clicking outside of the popup drawer
  focus: visible

  property real lastWidth

  width: {
    if (dragWidthAdjustment != 0) {
      return lastWidth - dragWidthAdjustment;
    } else if (overlayFeatureFormDrawer.fullScreenView || parent.width < parent.height || parent.width < 300 || width >= 0.95 * parent.width) {
      lastWidth = parent.width;
      return lastWidth;
    } else {
      lastWidth = Math.min(Math.max(200, parent.width / 2.25), parent.width);
      return lastWidth;
    }
  }

  property real lastHeight

  height: {
    if (dragHeightAdjustment != 0) {
      return Math.min(lastHeight - dragHeightAdjustment, parent.height - mainWindow.sceneTopMargin);
    } else if (overlayFeatureFormDrawer.fullScreenView || parent.width >= parent.height || height >= 0.95 * parent.height) {
      lastHeight = parent.height;
      return lastHeight;
    } else {
      lastHeight = Math.min(Math.max(200, parent.height / 2), parent.height);
      return lastHeight;
    }
  }

  topPadding: 0
  leftPadding: 0
  rightPadding: 0
  bottomPadding: 0

  interactive: false
  dragMargin: 0

  /**
   * If the save/cancel was initiated by button, the drawer needs to be closed in the end
   * If the drawer is closed by back key or integrated functionality (by Drawer) it has to save in the end
   * To make a difference between these scenarios we need position of the drawer and the isSaved flag of the FeatureForm
   */
  onOpened: {
    isAdding = true;
  }

  onClosed: {
    if (!digitizingToolbar.geometryRequested) {
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
        overlayFeatureFormDrawer.close(); // note: the digitizing toolbar will re-open the drawer to avoid panel stacking issues
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
    isVertical: overlayFeatureFormDrawer.isVertical

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
      if (overlayFeatureFormDrawer.position > 0) {
        overlayFeatureForm.isSaved = true; //because just saved
        overlayFeatureFormDrawer.close();
      } else {
        overlayFeatureForm.isSaved = false; //reset
      }
      digitizingToolbar.digitizingLogger.writeCoordinates();
      resetTabs();
    }

    onCancelled: {
      displayToast(qsTr("Changes discarded"));
      //close drawer if still open
      if (overlayFeatureFormDrawer.position > 0) {
        overlayFeatureForm.isSaved = true; //because never changed
        overlayFeatureFormDrawer.close();
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
            overlayFeatureFormDrawer.close();
          }
        } else if (dragHeightAdjustment < -parent.height * 0.2) {
          fullScreenView = true;
        }
      } else {
        if (overlayFeatureFormDrawer.width < overlayFeatureFormDrawer.parent.width * 0.3) {
          if (fullScreenView) {
            fullScreenView = false;
          } else {
            overlayFeatureFormDrawer.close();
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
          overlayFeatureFormDrawer.close();
        } else {
          //block closing to fix constraints or cancel with button
          displayToast(qsTr("Constraints not valid"), 'warning');
        }
        event.accepted = true;
      }
    }
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

  Component.onCompleted: {
    if (Material.roundedScale) {
      Material.roundedScale = Material.NotRounded;
    }
    close();
  }
}
