import QtQuick
import QtQuick.Controls
import org.qfield
import Theme

/**
 * \ingroup qml
 */
Popup {
  id: formPopup

  property alias state: form.state
  property alias embeddedLevel: form.embeddedLevel
  property alias currentLayer: formFeatureModel.currentLayer
  property alias linkedRelation: formFeatureModel.linkedRelation
  property alias linkedRelationOrderingField: formFeatureModel.linkedRelationOrderingField
  property alias linkedParentFeature: formFeatureModel.linkedParentFeature
  property alias feature: formFeatureModel.feature
  property alias attributeFormModel: formAttributeFormModel
  property alias digitizingToolbar: form.digitizingToolbar
  property alias codeReader: form.codeReader

  Connections {
    target: digitizingToolbar

    property bool wasVisible: false
    function onGeometryRequestedChanged() {
      if (digitizingToolbar.geometryRequested && formPopup.visible) {
        wasVisible = true;
        formPopup.visible = false;
      } else if (!digitizingToolbar.geometryRequested && wasVisible) {
        wasVisible = false;
        formPopup.visible = true;
      }
    }
  }

  onAboutToShow: {
    if (state === 'Add') {
      form.featureCreated = false;
      formFeatureModel.resetAttributes();
    }
  }

  signal featureSaved(int id)
  signal featureCancelled

  parent: mainWindow.contentItem
  closePolicy: form.state === "ReadOnly" ? Popup.CloseOnEscape : Popup.NoAutoClose // prevent accidental feature addition and editing

  padding: 0
  width: mainWindow.width - Theme.popupScreenEdgeMargin
  height: mainWindow.height - Math.max(Theme.popupScreenEdgeMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
  x: Theme.popupScreenEdgeMargin / 2
  y: (mainWindow.height - height) / 2
  z: 1000 + embeddedLevel
  modal: true
  focus: visible

  FeatureForm {
    id: form
    property bool isSaved: false

    model: AttributeFormModel {
      id: formAttributeFormModel
      featureModel: FeatureModel {
        id: formFeatureModel
        project: qgisProject
        positionInformation: coordinateLocator.positionInformation
        positionLocked: coordinateLocator.overrideLocation !== undefined
        topSnappingResult: coordinateLocator.topSnappingResult
        cloudUserInformation: projectInfo.cloudUserInformation
      }
    }

    embedded: true
    toolbarVisible: true

    anchors.fill: parent

    onConfirmed: {
      form.resetTabs();
      formPopup.featureSaved(formFeatureModel.feature.id);
      closePopup();
    }

    onCancelled: {
      form.resetTabs();
      formPopup.featureCancelled();
      closePopup();
    }

    function closePopup() {
      if (formPopup.opened) {
        isSaved = true;
        formPopup.close();
      } else {
        isSaved = false;
      }
    }
  }

  onClosed: {
    if (!form.isSaved) {
      form.confirm();
      digitizingToolbar.digitizingLogger.writeCoordinates();
    } else {
      form.isSaved = false;
      digitizingToolbar.digitizingLogger.clearCoordinates();
    }
  }

  function applyGeometry(geometry) {
    formFeatureModel.geometry = geometry;
    formFeatureModel.applyGeometry();
  }
}
