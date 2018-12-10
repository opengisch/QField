import QtQuick 2.0
import QtQuick.Controls 2.4
import org.qfield 1.0

Drawer {
  id: overlayFeatureFormDrawer
  property alias featureModel: overlayFeatureForm.featureModel
  height: parent.height
  width: qfieldSettings.fullScreenIdentifyView ? parent.width : parent.width / 3
  edge: Qt.RightEdge
  interactive: overlayFeatureForm.model.constraintsValid
  dragMargin: 0
  Keys.enabled: true

  /**
   * If the save/cancel was initiated by button, the drawer needs to be closed in the end
   * If the drawer is closed by back key or integrated functionality (by Drawer) it has to save in the end
   * To make a difference between these scenarios we need position of the drawer and the isSaved flag of the FeatureForm
   */

  onClosed: {
      if( !overlayFeatureForm.isSaved ) {
        overlayFeatureForm.save()
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

    focus: parent.opened

    onSaved: {
      displayToast( qsTr( "Changes saved" ) )
      //close drawer if still open
      if( overlayFeatureFormDrawer.position > 0 ) {
        overlayFeatureForm.isSaved=true //because just saved
        overlayFeatureFormDrawer.close()
      }
    }

    onCancelled: {
      displayToast( qsTr( "Changes discarded" ) )
      overlayFeatureForm.isSaved=true //because never changed
      overlayFeatureFormDrawer.close()
    }

    Keys.onReleased: {
      if (event.key === Qt.Key_Back || event.key === Qt.Key_Escape) {
        if( overlayFeatureForm.model.constraintsValid ) {
          overlayFeatureFormDrawer.close()
        } else {
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
      if( overlayFeatureForm.model.constraintsValid ) {
        overlayFeatureFormDrawer.close()
      } else {
        displayToast( qsTr( "Constraints not valid" ) )
      }
      event.accepted = true
    }
  }
}
