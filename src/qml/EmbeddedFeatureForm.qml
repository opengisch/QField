import QtQuick 2.11
import QtQuick.Controls 2.4
import org.qfield 1.0

Popup {
    id: formPopup

    property alias state: form.state
    //featureModel needs to be created to be accessible from outside
    property AttributeFormModel attributeFormModel: AttributeFormModel {
        featureModel: FeatureModel {
        }
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
        model: formPopup.attributeFormModel

        focus: true

        embedded: true
        toolbarVisible: true

        anchors.fill: parent

        onSaved: {
            formPopup.featureSaved()
            formPopup.close()
        }

        onCancelled: {
            formPopup.featureCancelled()
            formPopup.close()
        }
    }
}
