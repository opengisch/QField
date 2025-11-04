import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield
import Theme

/**
 * \ingroup qml
 */
QfPopup {
  id: trackInformationPopup

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
      tracker = trackings.itemAt(trackerIndex.row).tracker;
      if (!skipSettings) {
        trackerSettings.open();
        trackerSettings.focus = true;
      } else {
        featureModel.resetAttributes();
        featureModel.applyGeometry();
        tracker.feature = featureModel.feature;
        if (embeddedAttributeFormModel.rowCount() > 0 && !featureModel.suppressFeatureForm()) {
          embeddedFeatureForm.active = true;
        } else {
          startTracking();
        }
      }
    }
  }

  function startTracking() {
    trackingModel.startTracker(tracker.vectorLayer, positionSource.positionInformation, positionSource.projectedPosition);
    displayToast(qsTr('Track on layer %1 started').arg(tracker.vectorLayer.name));
    if (featureModel.currentLayer.geometryType === Qgis.GeometryType.Point) {
      projectInfo.saveTracker(featureModel.currentLayer);
    }
    tracker = undefined;
    trackingModel.trackingSetupDone();
  }

  function resumeTracking() {
    trackingModel.startTracker(tracker.vectorLayer, positionSource.positionInformation, positionSource.projectedPosition);
    displayToast(qsTr('Track on layer %1 started').arg(tracker.vectorLayer.name));
    projectInfo.saveTracker(featureModel.currentLayer);
    tracker = undefined;
    trackingModel.trackingSetupDone();
  }

  onTrackerChanged: {
    if (tracker != undefined) {
      featureModel.currentLayer = tracker.vectorLayer;
      timeInterval.checked = tracker.timeInterval > 0;
      timeIntervalValue.text = tracker.timeInterval > 0 ? tracker.timeInterval : positioningSettings.trackerTimeInterval;
      minimumDistance.checked = tracker.minimumDistance > 0;
      minimumDistanceValue.text = tracker.minimumDistance > 0 ? tracker.minimumDistance : positioningSettings.trackerMinimumDistance;
      erroneousDistanceSafeguard.checked = tracker.maximumDistance > 0;
      erroneousDistanceValue.text = tracker.maximumDistance > 0 ? tracker.maximumDistance : positioningSettings.trackerErroneousDistance;
      sensorCapture.checked = tracker.sensorCapture;
      allConstraints.checked = tracker.conjunction && (timeInterval.checked + minimumDistance.checked + sensorCapture.checked) > 1;
      measureComboBox.currentIndex = tracker.measureType;
      resumeTrackingButton.visible = tracker.feature.id >= 0;
    }
  }

  function applySettings() {
    tracker.timeInterval = timeIntervalValue.text.length == 0 || !timeInterval.checked ? 0.0 : timeIntervalValue.text;
    tracker.minimumDistance = minimumDistanceValue.text.length == 0 || !minimumDistance.checked ? 0.0 : minimumDistanceValue.text;
    tracker.maximumDistance = erroneousDistanceValue.text.length == 0 || !erroneousDistanceSafeguard.checked ? 0.0 : erroneousDistanceValue.text;
    tracker.sensorCapture = sensorCapture.checked;
    tracker.conjunction = (timeInterval.checked + minimumDistance.checked + sensorCapture.checked) > 1 && allConstraints.checked;
    tracker.measureType = measureComboBox.currentIndex;
  }

  Page {
    focus: true
    anchors.fill: parent
    padding: 5

    header: QfPageHeader {
      title: tracker !== undefined && tracker.vectorLayer ? qsTr("Tracking: %1").arg(tracker.vectorLayer.name) : qsTr("Tracking")

      showApplyButton: false
      showCancelButton: false
      showBackButton: true

      onBack: {
        trackingModel.trackingSetupDone();
        if (tracker != undefined) {
          trackingModel.stopTracker(tracker.vectorLayer);
        }
        close();
      }
    }

    ColumnLayout {
      anchors.fill: parent
      anchors.bottomMargin: 10

      ScrollView {
        Layout.fillWidth: true
        Layout.fillHeight: true
        padding: 10
        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
        ScrollBar.vertical: QfScrollBar {
        }
        contentWidth: trackerSettingsGrid.width
        contentHeight: trackerSettingsGrid.height
        clip: true

        GridLayout {
          id: trackerSettingsGrid
          width: parent.parent.width
          Layout.fillWidth: true

          columns: 2
          columnSpacing: 0
          rowSpacing: 5

          Label {
            text: qsTr('Requirement Settings')
            font: Theme.strongFont
            color: Theme.mainTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            Layout.topMargin: 5
            Layout.columnSpan: 2
          }

          Label {
            text: qsTr("Time requirement")
            font: Theme.defaultFont
            wrapMode: Text.WordWrap
            Layout.fillWidth: true

            MouseArea {
              anchors.fill: parent
              onClicked: timeInterval.toggle()
            }
          }

          QfSwitch {
            id: timeInterval
            Layout.preferredWidth: implicitContentWidth
            Layout.alignment: Qt.AlignTop
            checked: false
            onCheckedChanged: {
              positioningSettings.trackerTimeIntervalConstraint = checked;
            }
          }

          Label {
            text: qsTr("Minimum time [sec]")
            font: Theme.defaultFont
            wrapMode: Text.WordWrap
            enabled: timeInterval.checked
            visible: timeInterval.checked
            Layout.leftMargin: 8
            Layout.fillWidth: true
          }

          QfTextField {
            id: timeIntervalValue
            width: timeInterval.width
            font: Theme.defaultFont
            enabled: timeInterval.checked
            visible: timeInterval.checked
            horizontalAlignment: TextInput.AlignHCenter
            Layout.preferredWidth: 60
            Layout.preferredHeight: font.height + 20

            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: DoubleValidator {
              locale: 'C'
            }

            onTextChanged: {
              positioningSettings.trackerTimeInterval = parseFloat(text);
            }
          }

          Label {
            text: qsTr("When enabled, vertex additions will occur when the time between the last and new vertex meets a configured mimimum value.")
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          Item {
            Layout.preferredWidth: allConstraints.width
          }

          Label {
            text: qsTr("Distance requirement")
            font: Theme.defaultFont
            wrapMode: Text.WordWrap
            Layout.fillWidth: true

            MouseArea {
              anchors.fill: parent
              onClicked: minimumDistance.toggle()
            }
          }

          QfSwitch {
            id: minimumDistance
            Layout.preferredWidth: implicitContentWidth
            Layout.alignment: Qt.AlignTop
            checked: false
            onCheckedChanged: {
              positioningSettings.trackerMinimumDistanceConstraint = checked;
            }
          }

          DistanceArea {
            id: infoDistanceArea
            project: qgisProject
            crs: qgisProject ? qgisProject.crs : CoordinateReferenceSystemUtils.invalidCrs()
          }

          Label {
            text: qsTr("Minimum distance [%1]").arg(UnitTypes.toAbbreviatedString(infoDistanceArea.lengthUnits))
            font: Theme.defaultFont
            wrapMode: Text.WordWrap
            enabled: minimumDistance.checked
            visible: minimumDistance.checked
            Layout.leftMargin: 8
            Layout.fillWidth: true
          }

          QfTextField {
            id: minimumDistanceValue
            width: minimumDistance.width
            font: Theme.defaultFont
            enabled: minimumDistance.checked
            visible: minimumDistance.checked
            horizontalAlignment: TextInput.AlignHCenter
            Layout.preferredWidth: 60
            Layout.preferredHeight: font.height + 20

            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: DoubleValidator {
              locale: 'C'
            }

            onTextChanged: {
              positioningSettings.trackerMinimumDistance = parseFloat(text);
            }
          }

          Label {
            text: qsTr("When enabled, vertex additions will occur when the distance between the last and new vertex meets a configured mimimum value.")
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          Item {
            Layout.preferredWidth: allConstraints.width
          }

          Label {
            text: qsTr("Sensor data requirement")
            font: Theme.defaultFont
            wrapMode: Text.WordWrap
            Layout.fillWidth: true

            MouseArea {
              anchors.fill: parent
              onClicked: sensorCapture.toggle()
            }
          }

          QfSwitch {
            id: sensorCapture
            Layout.preferredWidth: implicitContentWidth
            Layout.alignment: Qt.AlignTop
            checked: false
            onCheckedChanged: {
              positioningSettings.trackerSensorCaptureConstraint = checked;
            }
          }

          Label {
            text: qsTr("When enabled, vertex additions will occur when sensors have captured new data.")
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          Item {
            Layout.preferredWidth: allConstraints.width
          }

          Label {
            text: qsTr("Wait for all active requirements")
            font: Theme.defaultFont
            wrapMode: Text.WordWrap
            Layout.fillWidth: true

            MouseArea {
              anchors.fill: parent
              onClicked: allConstraints.toggle()
            }
          }

          QfSwitch {
            id: allConstraints
            Layout.preferredWidth: implicitContentWidth
            Layout.alignment: Qt.AlignTop
            checked: false
            onCheckedChanged: {
              positioningSettings.trackerMeetAllConstraints = checked;
            }
          }

          Label {
            text: qsTr("When enabled, vertices will only be recorded when all active requirements are met. When disabled, individual requirement met will trigger vertex additions.")
            font: Theme.tipFont
            color: Theme.secondaryTextColor
            textFormat: Qt.RichText
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          Item {
            Layout.preferredWidth: allConstraints.width
            Layout.columnSpan: 2
          }

          Label {
            text: qsTr('General Settings')
            font: Theme.strongFont
            color: Theme.mainTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.topMargin: 4
          }

          Label {
            text: qsTr("Erroneous distance safeguard")
            font: Theme.defaultFont
            wrapMode: Text.WordWrap
            Layout.fillWidth: true

            MouseArea {
              anchors.fill: parent
              onClicked: erroneousDistanceSafeguard.toggle()
            }
          }

          QfSwitch {
            id: erroneousDistanceSafeguard
            Layout.preferredWidth: implicitContentWidth
            Layout.alignment: Qt.AlignTop
            checked: false
            onCheckedChanged: {
              positioningSettings.trackerErroneousDistanceSafeguard = checked;
            }
          }

          Label {
            text: qsTr("Maximum tolerated distance [%1]").arg(UnitTypes.toAbbreviatedString(infoDistanceArea.lengthUnits))
            font: Theme.defaultFont
            wrapMode: Text.WordWrap
            enabled: erroneousDistanceSafeguard.checked
            visible: erroneousDistanceSafeguard.checked
            Layout.leftMargin: 8
            Layout.fillWidth: true
          }

          QfTextField {
            id: erroneousDistanceValue
            width: erroneousDistanceSafeguard.width
            font: Theme.defaultFont
            enabled: erroneousDistanceSafeguard.checked
            visible: erroneousDistanceSafeguard.checked
            horizontalAlignment: TextInput.AlignHCenter
            Layout.preferredWidth: 60
            Layout.preferredHeight: font.height + 20

            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: DoubleValidator {
              locale: 'C'
            }

            onTextChanged: {
              positioningSettings.trackerErroneousDistance = parseFloat(text);
            }
          }

          Label {
            text: qsTr("When enabled, vertex addition will not occur when the distance between the last and new vertex is greater than a configured maximum value.")
            font: Theme.tipFont
            color: Theme.secondaryTextColor

            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            Layout.columnSpan: 2
          }

          Label {
            id: measureLabel
            text: qsTr("Measure (M) value attached to vertices:")
            font: Theme.defaultFont

            wrapMode: Text.WordWrap
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.topMargin: 4
          }

          ComboBox {
            id: measureComboBox
            enabled: LayerUtils.hasMValue(featureModel.currentLayer)
            Layout.fillWidth: true
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignVCenter
            font: Theme.defaultFont

            popup.font: Theme.defaultFont
            popup.topMargin: mainWindow.sceneTopMargin
            popup.bottomMargin: mainWindow.sceneTopMargin

            property bool loaded: false
            Component.onCompleted: {
              // This list matches the Tracker::MeasureType enum
              var measurements = [qsTr("Elapsed time (seconds since start of tracking)"), qsTr("Timestamp (milliseconds since epoch)"), qsTr("Ground speed"), qsTr("Bearing"), qsTr("Horizontal accuracy"), qsTr("Vertical accuracy"), qsTr("PDOP"), qsTr("HDOP"), qsTr("VDOP")];
              model = measurements;
              loaded = true;
            }

            onCurrentIndexChanged: {
              if (loaded) {
                positioningSettings.trackerMeasureType = currentIndex;
              }
            }
          }

          Label {
            id: measureTipLabel
            visible: !LayerUtils.hasMValue(featureModel.currentLayer)
            Layout.fillWidth: true
            text: qsTr("To active the measurement functionality, make sure the vector layer's geometry type used for the tracking session has an M dimension.")
            font: Theme.tipFont
            color: Theme.secondaryTextColor

            wrapMode: Text.WordWrap
          }
        }
      }

      QfButton {
        id: startTrackingButton
        Layout.fillWidth: true
        Layout.leftMargin: 10
        Layout.rightMargin: 10
        text: qsTr("Start tracking")
        icon.source: Theme.getThemeVectorIcon('directions_walk_24dp')

        onClicked: {
          applySettings();
          featureModel.resetAttributes();
          featureModel.applyGeometry();
          tracker.feature = featureModel.feature;
          if (embeddedAttributeFormModel.rowCount() > 0 && !featureModel.suppressFeatureForm()) {
            embeddedFeatureForm.active = true;
          } else {
            startTracking();
            trackerSettings.close();
          }
        }
      }

      QfButton {
        id: resumeTrackingButton
        Layout.fillWidth: true
        Layout.leftMargin: 10
        Layout.rightMargin: 10
        text: qsTr("Resume tracking")
        icon.source: Theme.getThemeVectorIcon('directions_walk_24dp')
        icon.color: Theme.mainColor
        bgcolor: "transparent"
        color: Theme.mainColor
        visible: false

        onClicked: {
          applySettings();
          resumeTracking();
          trackerSettings.close();
        }
      }
    }
  }

  FeatureModel {
    id: featureModel
    project: qgisProject

    geometry: Geometry {
    }

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
          tracker.feature = featureModel.feature;
          embeddedFeatureFormPopup.close();
          embeddedFeatureForm.active = false;
          startTracking();
          trackerSettings.close();
        }

        onCancelled: {
          embeddedFeatureFormPopup.close();
          embeddedFeatureForm.active = false;
          embeddedFeatureForm.focus = false;
          trackingModel.stopTracker(tracker.vectorLayer);
          tracker = undefined;
          trackerSettings.close();
          trackingModel.trackingSetupDone();
        }

        onRequestJumpToPoint: function (center, scale, handleMargins) {
          trackInformationPopup.requestJumpToPoint(center, scale, handleMargins);
        }
      }
    }
  }
}
