

# File TrackerFeatureForm.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**TrackerFeatureForm.qml**](TrackerFeatureForm_8qml.md)

[Go to the documentation of this file](TrackerFeatureForm_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

QfPopup {
  id: trackingFeatureForm

  parent: mainWindow.contentItem
  width: mainWindow.width - Theme.popupScreenEdgeHorizontalMargin * 2
  height: mainWindow.height - Math.max(Theme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
  x: Theme.popupScreenEdgeHorizontalMargin
  y: (mainWindow.height - height) / 2
  closePolicy: Popup.NoAutoClose

  property var tracker: undefined

  signal requestJumpToPoint(var center, real scale, bool handleMargins)

  Connections {
    target: trackingModel

    function onTrackingSetupRequested(trackerIndex, skipSettings) {
      trackingFeatureForm.tracker = trackings.itemAt(trackerIndex.row).tracker;
      featureModel.currentLayer = trackingFeatureForm.tracker.vectorLayer;
      featureModel.resetAttributes();
      featureModel.applyGeometry();
      trackingFeatureForm.tracker.feature = featureModel.feature;
      if (embeddedAttributeFormModel.rowCount() > 0 && !featureModel.suppressFeatureForm()) {
        embeddedFeatureForm.active = true;
      } else {
        startTracking();
      }
    }
  }

  function startTracking() {
    trackingModel.startTracker(trackingFeatureForm.tracker.vectorLayer, positionSource.positionInformation, positionSource.projectedPosition);
    displayToast(qsTr('Track on layer %1 started').arg(trackingFeatureForm.tracker.vectorLayer.name));
    if (featureModel.currentLayer.geometryType === Qgis.GeometryType.Point) {
      projectInfo.saveTracker(featureModel.currentLayer);
    }
    trackingFeatureForm.tracker = undefined;
    trackingModel.trackingSetupDone();
  }

  FeatureModel {
    id: featureModel
    project: qgisProject

    geometry: Geometry {}

    appExpressionContextScopesGenerator: AppExpressionContextScopesGenerator {
      positionInformation: appScopesGenerator.positionInformation
      positionLocked: true
      cloudUserInformation: appScopesGenerator.cloudUserInformation
    }
  }

  AttributeFormModel {
    id: embeddedAttributeFormModel
    featureModel: featureModel
  }

  Loader {
    id: embeddedFeatureForm

    sourceComponent: embeddedFeatureFormComponent
    active: false
    onLoaded: {
      item.open();
      item.forceActiveFocus();
    }
  }

  Component {
    id: embeddedFeatureFormComponent

    QfPopup {
      id: embeddedFeatureFormPopup
      parent: mainWindow.contentItem

      x: Theme.popupScreenEdgeHorizontalMargin
      y: Theme.popupScreenEdgeVerticalMargin
      width: parent.width - Theme.popupScreenEdgeHorizontalMargin * 2
      height: parent.height - Theme.popupScreenEdgeVerticalMargin * 2
      closePolicy: Popup.NoAutoClose

      FeatureForm {
        id: form
        model: embeddedAttributeFormModel

        focus: true
        setupOnly: true
        embedded: true
        toolbarVisible: true

        anchors.fill: parent

        state: 'Add'

        onTemporaryStored: {
          trackingFeatureForm.tracker.feature = featureModel.feature;
          embeddedFeatureFormPopup.close();
          embeddedFeatureForm.active = false;
          startTracking();
        }

        onCancelled: {
          embeddedFeatureFormPopup.close();
          embeddedFeatureForm.active = false;
          embeddedFeatureForm.focus = false;
          trackingModel.stopTracker(trackingFeatureForm.tracker.vectorLayer);
          trackingFeatureForm.tracker = undefined;
          trackingModel.trackingSetupDone();
        }

        onRequestJumpToPoint: function (center, scale, handleMargins) {
          trackInformationPopup.requestJumpToPoint(center, scale, handleMargins);
        }
      }
    }
  }
}
```


