import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Window 2.14

import org.qfield 1.0

Drawer {
  id: overlayFeatureFormDrawer

  property alias featureModel: overlayFeatureForm.featureModel
  property alias state: overlayFeatureForm.state
  property alias featureForm: overlayFeatureForm
  property alias digitizingToolbar: overlayFeatureForm.digitizingToolbar
  property alias barcodeReader: overlayFeatureForm.barcodeReader
  property bool isAdding: false

  edge: parent.width < parent.height ? Qt.BottomEdge : Qt.RightEdge
  closePolicy: Popup.NoAutoClose // prevent accidental feature addition when clicking outside of the popup drawer

  width: {
      if (qfieldSettings.fullScreenIdentifyView || parent.width < parent.height || parent.width < 300) {
          parent.width
      } else {
          Math.min(Math.max( 200, parent.width / 3), parent.width)
      }
  }
  height: {
     if (qfieldSettings.fullScreenIdentifyView || parent.width > parent.height) {
         parent.height
     } else {
         Math.min(Math.max( 200, parent.height / 2 ), parent.height)
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
      isAdding = true
  }

  onClosed: {
      if ( !digitizingToolbar.geometryRequested ) {
          if( !overlayFeatureForm.isSaved ) {
              overlayFeatureForm.confirm()
          } else {
              overlayFeatureForm.isSaved = false //reset
          }
          digitizingRubberband.model.reset()
          featureModel.resetFeature()
          isAdding = false
      }
  }

  Connections {
      target: digitizingToolbar

      function onGeometryRequestedChanged() {
          if ( digitizingToolbar.geometryRequested && overlayFeatureFormDrawer.isAdding ) {
              overlayFeatureFormDrawer.close() // note: the digitizing toolbar will re-open the drawer to avoid panel stacking issues
          }
      }
  }

  FeatureForm {
    id: overlayFeatureForm
    height: parent.height
    width: parent.width
    visible: true

    topMargin: overlayFeatureFormDrawer.y == 0 ? mainWindow.sceneTopMargin : 0.0

    property alias featureModel: attributeFormModel.featureModel
    property bool isSaved: false

    digitizingToolbar: overlayFeatureFormDrawer.digitizingToolbar
    barcodeReader: overlayFeatureFormDrawer.barcodeReader

    model: AttributeFormModel {
        id: attributeFormModel
        featureModel: FeatureModel {
            project: qgisProject
            positionInformation: coordinateLocator.positionInformation
            positionLocked: coordinateLocator.overrideLocation !== undefined
            topSnappingResult: coordinateLocator.topSnappingResult
            cloudUserInformation: cloudConnection.userInformation
        }
    }

    state: "Add"

    focus: overlayFeatureFormDrawer.opened

    onConfirmed: {
      displayToast( qsTr( "Changes saved" ) )
      //close drawer if still open
      if ( overlayFeatureFormDrawer.position > 0 ) {
        overlayFeatureForm.isSaved = true; //because just saved
        overlayFeatureFormDrawer.close();
      } else {
        overlayFeatureForm.isSaved = false //reset
      }
      digitizingToolbar.digitizingLogger.writeCoordinates();
    }

    onCancelled: {
      displayToast( qsTr( "Changes discarded" ) )
      //close drawer if still open
      if ( overlayFeatureFormDrawer.position > 0 ) {
        overlayFeatureForm.isSaved = true; //because never changed
        overlayFeatureFormDrawer.close();
      } else {
        overlayFeatureForm.isSaved = false; //reset
      }
      digitizingToolbar.digitizingLogger.clearCoordinates();
    }

    Keys.onReleased: (event) => {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        if( overlayFeatureForm.model.constraintsHardValid || qfieldSettings.autoSave ) {
          overlayFeatureFormDrawer.close()
        } else {
          //block closing to fix constraints or cancel with button
          displayToast( qsTr( "Constraints not valid" ), 'warning' )
        }
        event.accepted = true
      }
    }

    Component.onCompleted: {
        focusstack.addFocusTaker( this )
    }
  }

  Component.onCompleted: {
      close()
  }
}
