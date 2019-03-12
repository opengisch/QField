import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2

import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

Frame{
    height: 200

    //the model
    ReferencingFeatureListModel {
        id: relationEditorModel
        relation: qgisProject.relationManager.relation(relationId)
        feature: currentFeature
    }

    //the list
    ListView {
        id: referencingFeatureListView
        anchors.fill: parent
        model: relationEditorModel
        delegate: referencingFeatureDelegate
        focus: true
        clip: true

        onCurrentItemChanged: model.referencingFeatureId + ' selected '+currentIndex
    }

    //list components
    Component {
        id: referencingFeatureDelegate

        Item {
          id: listitem
          anchors { left: parent.left; right: parent.right }

          focus: true

          height: Math.max( 24*dp, featureText.height )

          Text {
            id: featureText
            anchors { leftMargin: 10; left: parent.left; right: deleteButton.left; verticalCenter: parent.verticalCenter }
            font.bold: true
            text: { text: model.referencingFeatureId + ' - ' + model.displayString }
          }

          MouseArea {
            anchors.fill: parent

            onClicked: {
              //open to edit
            }
          }

          Row
          {
            id: editRow
            anchors { top: parent.top; right: parent.right }

            Button {
              id: deleteButton

              width: 24*dp
              height: 24*dp

              visible: true

              iconSource: Style.getThemeIcon( "ic_delete_forever_white_24dp" )

              onClicked: {
                  deleteDialog.referencingFeatureId = model.referencingFeatureId
                  deleteDialog.visible = true
              }
            }
          }

          //bottom line
          Rectangle {
            anchors.bottom: parent.bottom
            height: 1
            color: "lightGray"
            width: parent.width
          }
        }
    }

    //logical stuff
    MessageDialog {
      id: deleteDialog

      property int referencingFeatureId
      property var layerName

      visible: false

      title: "Delete feature "+referencingFeatureId+" on layer "+layerName //translation needed
      text: "Should the feature "+referencingFeatureId+" on layer "+layerName+" really be deleted?" //translation needed
      standardButtons: StandardButton.Ok | StandardButton.Cancel
      onAccepted: {
        referencingFeatureListView.model.deleteFeature( referencingFeatureId )
        console.log("delete feature "+referencingFeatureId)
        visible = false
      }
      onRejected: {
        visible = false
      }
    }
}
