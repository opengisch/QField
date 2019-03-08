import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2

import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0
import ".."
import "../js/style.js" as Style

import org.qfield 1.0
import org.qgis 1.0

/*
Frame{
    height: 100
    width: 400

    ListView {
        implicitHeight: 400
        implicitWidth: 100
        clip: true
        model: relationEditorModel
        delegate: RowLayout {
            focus: true
            width: parent.width
            TextField { text: model.featureId + ' - ' + model.displayString }
        }
    }
}
*/
Frame{
    height: 100
    width: 300

    Component {
        id: referencingFeatureDelegate
        Rectangle {
            id: listitem
            width: parent.width
            height: 40
            border.color: black
            border.width: 2*dp
            Column {
                Text { text: model.featureId + ' - ' + model.displayString }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    //referencingFeatureListView.currentIndex = index
                    deleteDialog.featureId = model.featureId
                    deleteDialog.visible = true
                }
            }
        }
    }

    ListView {
        id: referencingFeatureListView
        anchors.fill: parent
        model: relationEditorModel
        delegate: referencingFeatureDelegate
        focus: true

        onCurrentItemChanged: model.featureId + ' selected '+currentIndex
    }

    MessageDialog {
      id: deleteDialog

      property int featureId
      property var layerName

      visible: false

      title: "Delete feature "+featureId+" on layer "+layerName //translation needed
      text: "Should the feature "+featureId+" on layer "+layerName+" really be deleted?" //translation needed
      standardButtons: StandardButton.Ok | StandardButton.Cancel
      onAccepted: {
        relationEditorModel.deleteFeature( featureId )
        console.log("delete feature "+featureId)
        visible = false
      }
      onRejected: {
        visible = false
      }
    }
}

/*

Item {
    width: 200; height: 100
    focus: true

    Component {
        id: referencingFeatureDelegate
        Item {
            id: listitem
            width: 400; height: 40
            Column {
                Text { text: model.featureId + ' - ' + model.displayString }
            }

            MouseArea{

                anchors.fill: parent
                onPressed: {
                    listitem.forceActiveFocus()
                }
            }
        }
    }

    ListView {
        id: referencingFeatureListView
        anchors.fill: parent
        model: relationEditorModel
        delegate: referencingFeatureDelegate
        focus: true
    }

}
*/

