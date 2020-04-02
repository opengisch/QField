import QtQuick 2.12
import QtQuick.Controls 2.12
import org.qfield 1.0

Popup {
    id: formPopup

    property alias state: form.state

    property alias currentLayer: formFeatureModel.currentLayer
    property alias linkedRelation: formFeatureModel.linkedRelation
    property alias linkedParentFeature: formFeatureModel.linkedParentFeature
    property alias feature: formFeatureModel.feature

    onAboutToShow: {
        if( state === 'Add' )
           formFeatureModel.resetAttributes()
    }

    signal featureSaved
    signal featureCancelled

    parent: ApplicationWindow.overlay

    x: 24 * dp
    y: 24 * dp
    padding: 0
    width: parent.width - 48 * dp
    height: parent.height - 48 * dp
    modal: true
    closePolicy: Popup.CloseOnEscape

    FeatureForm {
        id: form
        model: AttributeFormModel {
            featureModel: FeatureModel {
                id: formFeatureModel
            }
        }

        focus: true

        embedded: true
        toolbarVisible: true

        anchors.fill: parent

        onConfirmed: {
            formPopup.featureSaved()
            formPopup.close()
        }

        onCancelled: {
            formPopup.featureCancelled()
            formPopup.close()
        }
    }
}
