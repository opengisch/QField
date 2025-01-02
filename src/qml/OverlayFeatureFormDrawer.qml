import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Window
import org.qfield

/**
 * \ingroup qml
 */
Drawer {
  id: overlayFeatureFormDrawer

  property alias featureModel: attributeFormModel.featureModel
  property alias state: overlayFeatureForm.state
  property alias featureForm: overlayFeatureForm
  property alias digitizingToolbar: overlayFeatureForm.digitizingToolbar
  property alias codeReader: overlayFeatureForm.codeReader
  property bool isAdding: false

  edge: parent.width < parent.height ? Qt.BottomEdge : Qt.RightEdge
  closePolicy: Popup.NoAutoClose // prevent accidental feature addition when clicking outside of the popup drawer
  focus: visible

  width: {
    if (qfieldSettings.fullScreenIdentifyView || parent.width < parent.height || parent.width < 300) {
      parent.width;
    } else {
      Math.min(Math.max(200, parent.width / 2.25), parent.width);
    }
  }
  height: {
    if (qfieldSettings.fullScreenIdentifyView || parent.width > parent.height) {
      parent.height;
    } else {
      Math.min(Math.max(200, parent.height / 2), parent.height);
    }
  }

  interactive: overlayFeatureForm.model.constraintsHardValid || qfieldSettings.autoSave ? true : false
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

    topMargin: overlayFeatureFormDrawer.y == 0 ? mainWindow.sceneTopMargin : 0.0
    bottomMargin: mainWindow.sceneBottomMargin

    property bool isSaved: false

    model: AttributeFormModel {
      id: attributeFormModel
      featureModel: FeatureModel {
        project: qgisProject
        positionInformation: coordinateLocator.positionInformation
        positionLocked: coordinateLocator.overrideLocation !== undefined
        topSnappingResult: coordinateLocator.topSnappingResult
        cloudUserInformation: projectInfo.cloudUserInformation
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

  Component.onCompleted: {
    if (Material.roundedScale) {
      Material.roundedScale = Material.NotRounded;
    }
    close();
  }
}
