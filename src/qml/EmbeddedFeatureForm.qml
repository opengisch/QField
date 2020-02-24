import QtQuick 2.11
import QtQuick.Controls 2.4

import QtQuick.Layouts 1.0
import "."
import Theme 1.0

import org.qfield 1.0
import org.qgis 1.0

Popup {
    id: formPopup

    //the add entry stuff
    property var attributeFormModel: AttributeFormModel {
        featureModel: FeatureModel {
        }
    }

    property var state

    signal featureSaved
    signal featureCancelled

    onStateChanged: {
        //because the binding in form does not work
        form.state = formPopup.state
    }

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
