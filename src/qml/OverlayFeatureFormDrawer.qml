import QtQuick 2.11
import QtQuick.Controls 2.4
import org.qfield 1.0
import QtQuick.Window 2.11

Drawer {
  id: overlayFeatureFormDrawer
  property alias featureModel: overlayFeatureForm.featureModel
  property alias state: overlayFeatureForm.state
  property alias featureForm: overlayFeatureForm

  edge: parent.width < parent.height ? Qt.BottomEdge : Qt.RightEdge
  width: {
      if (qfieldSettings.fullScreenIdentifyView || parent.width < parent.height || parent.width < 300 * dp) {
          parent.width
      } else {
          Math.min(Math.max( 200 * dp, parent.width / 3), parent.width)
      }
  }
  height: {
     if (qfieldSettings.fullScreenIdentifyView || parent.width > parent.height) {
         parent.height
     } else {
         Math.min(Math.max( 200 * dp, parent.height / 2 ), parent.height)
     }
  }

  interactive: overlayFeatureForm.model.constraintsHardValid || qfieldSettings.autoSave ? true : false
  dragMargin: 0
  Keys.enabled: true

  /**
   * If the save/cancel was initiated by button, the drawer needs to be closed in the end
   * If the drawer is closed by back key or integrated functionality (by Drawer) it has to save in the end
   * To make a difference between these scenarios we need position of the drawer and the isSaved flag of the FeatureForm
   */

  onClosed: {
      if( !overlayFeatureForm.isSaved ) {
        overlayFeatureForm.confirm()
      } else {
        overlayFeatureForm.isSaved=false //reset
      }

      digitizingRubberband.model.reset()
  }

  FeatureForm {
    id: overlayFeatureForm
    height: parent.height
    width: parent.width
    visible: true
    property alias featureModel: attributeFormModel.featureModel
    property bool isSaved: false

    model: AttributeFormModel {id: attributeFormModel}

    state: "Add"

    focus: overlayFeatureFormDrawer.opened

    onConfirmed: {
      displayToast( qsTr( "Changes saved" ) )
      //close drawer if still open
      if( overlayFeatureFormDrawer.position > 0 ) {
        overlayFeatureForm.isSaved=true //because just saved
        overlayFeatureFormDrawer.close()
      }else{
        overlayFeatureForm.isSaved=false //reset
      }
    }

    onCancelled: {
      displayToast( qsTr( "Last changes discarded" ) )
      //close drawer if still open
      if( overlayFeatureFormDrawer.position > 0 ) {
        overlayFeatureForm.isSaved=true //because never changed
        overlayFeatureFormDrawer.close()
      } else {
        overlayFeatureForm.isSaved=false //reset
      }
    }

    Keys.onReleased: {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        if( overlayFeatureForm.model.constraintsHardValid || qfieldSettings.autoSave ) {
          overlayFeatureFormDrawer.close()
        } else {
          //block closing to fix constraints or cancel with button
          displayToast( qsTr( "Constraints not valid" ) )
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

  Keys.onReleased: {
    if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
      if( overlayFeatureForm.model.constraintsHardValid || qfieldSettings.autoSave ) {
        overlayFeatureFormDrawer.close()
      } else {
        //block closing to fix constraints or cancel with button
        displayToast( qsTr( "Constraints not valid" ) )
      }
      event.accepted = true
    }
  }
}
