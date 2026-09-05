

# File QfTrackerSettings.qml

[**File List**](files.md) **>** [**app**](dir_5194e5fea318fda12687127c23f8aba1.md) **>** [**qml**](dir_408f7dc2ccecf14179dc3d37678eaa5b.md) **>** [**QfTrackerSettings.qml**](QfTrackerSettings_8qml.md)

[Go to the documentation of this file](QfTrackerSettings_8qml.md)


```C++
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qgis
import org.qfield.core
import org.qfield.gui

QfPopup {
  id: trackerSettings

  parent: mainWindow.contentItem
  width: mainWindow.width - QfTheme.popupScreenEdgeHorizontalMargin * 2
  height: mainWindow.height - Math.max(QfTheme.popupScreenEdgeVerticalMargin * 2, mainWindow.sceneTopMargin * 2 + 4, mainWindow.sceneBottomMargin * 2 + 4)
  x: QfTheme.popupScreenEdgeHorizontalMargin
  y: (mainWindow.height - height) / 2
  closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside
  focus: visible

  property alias availableLayersCount: layersComboBox.count

  property var tracker: undefined
  property var layer: undefined
  onLayerChanged: {
    trackerSettings.tracker = undefined;
    if (layer !== undefined) {
      let idx = projectInfo.restoreTracker(layer);
      if (idx.valid) {
        trackerSettings.tracker = trackings.itemAt(idx.row).tracker;
      } else {
        trackerSettings.tracker = trackingModel.trackerForLayer(layer);
      }
      if (trackerSettings.tracker !== undefined && trackerSettings.tracker !== null) {
        timeInterval.checked = trackerSettings.tracker.timeInterval > 0;
        timeIntervalValue.text = trackerSettings.tracker.timeInterval > 0 ? trackerSettings.tracker.timeInterval : positioningSettings.trackerTimeInterval;
        minimumDistance.checked = trackerSettings.tracker.minimumDistance > 0;
        minimumDistanceValue.text = trackerSettings.tracker.minimumDistance > 0 ? trackerSettings.tracker.minimumDistance : positioningSettings.trackerMinimumDistance;
        erroneousDistanceSafeguard.checked = trackerSettings.tracker.maximumDistance > 0;
        erroneousDistanceValue.text = trackerSettings.tracker.maximumDistance > 0 ? trackerSettings.tracker.maximumDistance : positioningSettings.trackerErroneousDistance;
        sensorCapture.checked = trackerSettings.tracker.sensorCapture;
        allConstraints.checked = trackerSettings.tracker.conjunction && (timeInterval.checked + minimumDistance.checked + sensorCapture.checked) > 1;
        measureComboBox.currentIndex = trackerSettings.tracker.measureType;
      } else {
        idx = trackingModel.createTracker(layer);
        trackerSettings.tracker = trackings.itemAt(idx.row).tracker;
        trackerSettings.tracker.visible = true;
        trackerSettings.tracker.minimumDistance = positioningSettings.trackerMinimumDistanceConstraint ? positioningSettings.trackerMinimumDistance : 0;
        trackerSettings.tracker.timeInterval = positioningSettings.trackerTimeIntervalConstraint ? positioningSettings.trackerTimeInterval : 0;
        trackerSettings.tracker.maximumDistance = positioningSettings.trackerErroneousDistanceSafeguard ? positioningSettings.trackerErroneousDistance : 0;
        trackerSettings.tracker.sensorCapture = positioningSettings.trackerSensorCaptureConstraint;
        trackerSettings.tracker.conjunction = positioningSettings.trackerMeetAllConstraints;
        trackerSettings.tracker.measureType = positioningSettings.trackerMeasureType;
      }
      featureModel.currentLayer = trackerSettings.tracker.vectorLayer;
      resumeTrackingButton.visible = trackerSettings.tracker.feature.id >= 0;
    }
  }

  function applySettingsToTracker() {
    trackerSettings.tracker.timeInterval = timeIntervalValue.text.length === 0 || !timeInterval.checked ? 0.0 : timeIntervalValue.text;
    trackerSettings.tracker.minimumDistance = minimumDistanceValue.text.length === 0 || !minimumDistance.checked ? 0.0 : minimumDistanceValue.text;
    trackerSettings.tracker.maximumDistance = erroneousDistanceValue.text.length === 0 || !erroneousDistanceSafeguard.checked ? 0.0 : erroneousDistanceValue.text;
    trackerSettings.tracker.sensorCapture = sensorCapture.checked;
    trackerSettings.tracker.conjunction = (timeInterval.checked + minimumDistance.checked + sensorCapture.checked) > 1 && allConstraints.checked;
    trackerSettings.tracker.measureType = measureComboBox.currentIndex;
  }

  function prepareSettings(requestedLayer = undefined) {
    let defaultLayer = requestedLayer;
    if (defaultLayer === undefined) {
      defaultLayer = trackingModel.bestAvailableLayer(qgisProject);
    }
    layersModel.trackingModel = trackingModel;
    layersModel.enabled = true;
    const defaultLayerIndex = layersModel.findLayer(defaultLayer);
    if (defaultLayerIndex >= 0) {
      trackerSettings.layer = defaultLayer;
      layersComboBox.currentIndex = defaultLayerIndex;
    } else {
      layersComboBox.currentIndex = 0;
    }
  }

  onAboutToHide: {
    if (trackerSettings.tracker !== undefined) {
      trackingModel.stopTracker(trackerSettings.tracker.vectorLayer);
      trackerSettings.tracker = undefined;
    }
    trackerSettings.layer = undefined;
  }

  Page {
    focus: true
    anchors.fill: parent
    padding: 5

    header: QfPageHeader {
      title: qsTr("Tracking")

      showApplyButton: false
      showCancelButton: false
      showBackButton: true

      onBack: {
        close();
      }
    }

    ScrollView {
      anchors.fill: parent
      anchors.bottomMargin: 10
      topPadding: 5
      bottomPadding: 5
      leftPadding: 5
      rightPadding: 5 // Considering scroll bar
      ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
      ScrollBar.vertical: QfScrollBar {}
      contentWidth: trackerSettingsGrid.width
      contentHeight: trackerSettingsGrid.height + bottomRow.height
      clip: true

      GridLayout {
        id: trackerSettingsGrid
        width: parent.parent.width
        Layout.fillWidth: true

        columns: 2
        columnSpacing: 0
        rowSpacing: 5

        Label {
          text: qsTr('Layer')
          font: QfTheme.strongFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.topMargin: 5
          Layout.columnSpan: 2
        }

        QfComboBox {
          id: layersComboBox

          Layout.fillWidth: true
          Layout.topMargin: 5
          Layout.columnSpan: 2

          model: QfMapLayerModel {
            id: layersModel
            enabled: false
            project: qgisProject
            requiresTrackingAvailability: true
          }

          textRole: 'Name'
          valueRole: 'LayerPointer'

          delegate: ItemDelegate {
            width: layersComboBox.width
            height: 36
            icon.width: 24
            icon.height: 24
            icon.source: {
              switch (GeometryType) {
              case Qgis.GeometryType.Point:
                return QfTheme.getThemeVectorIcon('ic_geometry_point_24dp');
              case Qgis.GeometryType.Line:
                return QfTheme.getThemeVectorIcon('ic_geometry_line_24dp');
              case Qgis.GeometryType.Polygon:
                return QfTheme.getThemeVectorIcon('ic_geometry_polygon_24dp');
              default:
                return '';
              }
            }
            text: Name
            font: QfTheme.defaultFont
            highlighted: layersComboBox.highlightedIndex === index
          }

          contentItem: MenuItem {
            width: layersComboBox.width
            height: 36

            icon.width: 24
            icon.height: 24
            icon.source: {
              if (layersComboBox.currentIndex >= 0) {
                switch (layersComboBox.model.get(layersComboBox.currentIndex).GeometryType) {
                case Qgis.GeometryType.Point:
                  return QfTheme.getThemeVectorIcon('ic_geometry_point_24dp');
                case Qgis.GeometryType.Line:
                  return QfTheme.getThemeVectorIcon('ic_geometry_line_24dp');
                case Qgis.GeometryType.Polygon:
                  return QfTheme.getThemeVectorIcon('ic_geometry_polygon_24dp');
                default:
                  return '';
                }
              }
              return '';
            }
            text: layersComboBox.currentText
            font: QfTheme.defaultFont

            onClicked: layersComboBox.popup.open()
          }

          onCurrentValueChanged: {
            if (trackerSettings.layer !== currentValue) {
              if (trackerSettings.tracker !== undefined) {
                trackingModel.stopTracker(trackerSettings.tracker.vectorLayer);
              }
              trackerSettings.layer = currentValue;
            }
          }
        }

        Label {
          text: qsTr('Requirement Settings')
          font: QfTheme.strongFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.topMargin: 5
          Layout.columnSpan: 2
        }

        Label {
          text: qsTr("Time requirement")
          font: QfTheme.defaultFont
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

        RowLayout {
          Layout.columnSpan: 2
          Layout.fillWidth: true
          visible: timeInterval.checked
          enabled: timeInterval.checked

          Label {
            text: qsTr("Minimum time")
            font: QfTheme.defaultFont
            color: QfTheme.mainTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          QfTextField {
            id: timeIntervalValue
            font: QfTheme.defaultFont
            horizontalAlignment: TextInput.AlignRight
            suffixText: qsTr("sec")
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: DoubleValidator {
              locale: 'C'
            }

            onTextChanged: {
              positioningSettings.trackerTimeInterval = parseFloat(text);
            }
          }
        }

        Label {
          text: qsTr("When enabled, vertex additions will occur when the time between the last and new vertex meets a configured mimimum value.")
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        Item {
          Layout.preferredWidth: allConstraints.width
        }

        Label {
          text: qsTr("Distance requirement")
          font: QfTheme.defaultFont
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

        QfDistanceArea {
          id: infoDistanceArea
          project: qgisProject
          crs: qgisProject ? qgisProject.crs : QfCoordinateReferenceSystemUtils.invalidCrs()
        }

        RowLayout {
          Layout.columnSpan: 2
          Layout.fillWidth: true
          visible: minimumDistance.checked
          enabled: minimumDistance.checked

          Label {
            text: qsTr("Minimum distance")
            font: QfTheme.defaultFont
            color: QfTheme.mainTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          QfTextField {
            id: minimumDistanceValue
            font: QfTheme.defaultFont
            horizontalAlignment: TextInput.AlignRight
            suffixText: UnitTypes.toAbbreviatedString(infoDistanceArea.lengthUnits)
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: DoubleValidator {
              locale: 'C'
            }

            onTextChanged: {
              positioningSettings.trackerMinimumDistance = parseFloat(text);
            }
          }
        }

        Label {
          text: qsTr("When enabled, vertex additions will occur when the distance between the last and new vertex meets a configured mimimum value.")
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        Item {
          Layout.preferredWidth: allConstraints.width
        }

        Label {
          text: qsTr("Sensor data requirement")
          font: QfTheme.defaultFont
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
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
        }

        Item {
          Layout.preferredWidth: allConstraints.width
        }

        Label {
          text: qsTr("Wait for all active requirements")
          font: QfTheme.defaultFont
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
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor
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
          font: QfTheme.strongFont
          color: QfTheme.mainTextColor
          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.columnSpan: 2
          Layout.topMargin: 4
        }

        Label {
          text: qsTr("Erroneous distance safeguard")
          font: QfTheme.defaultFont
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

        RowLayout {
          Layout.columnSpan: 2
          Layout.fillWidth: true
          visible: erroneousDistanceSafeguard.checked
          enabled: erroneousDistanceSafeguard.checked

          Label {
            text: qsTr("Maximum tolerated distance")
            font: QfTheme.defaultFont
            color: QfTheme.mainTextColor
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
          }

          QfTextField {
            id: erroneousDistanceValue
            font: QfTheme.defaultFont
            horizontalAlignment: TextInput.AlignRight
            suffixText: UnitTypes.toAbbreviatedString(infoDistanceArea.lengthUnits)
            inputMethodHints: Qt.ImhFormattedNumbersOnly
            validator: DoubleValidator {
              locale: 'C'
            }

            onTextChanged: {
              positioningSettings.trackerErroneousDistance = parseFloat(text);
            }
          }
        }

        Label {
          text: qsTr("When enabled, vertex addition will not occur when the distance between the last and new vertex is greater than a configured maximum value.")
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor

          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.columnSpan: 2
        }

        Label {
          id: measureLabel
          text: qsTr("Measure (M) value attached to vertices:")
          font: QfTheme.defaultFont

          wrapMode: Text.WordWrap
          Layout.fillWidth: true
          Layout.columnSpan: 2
          Layout.topMargin: 4
        }

        QfComboBox {
          id: measureComboBox
          enabled: QfLayerUtils.hasMValue(featureModel.currentLayer)
          Layout.fillWidth: true
          Layout.columnSpan: 2
          Layout.alignment: Qt.AlignVCenter
          font: QfTheme.defaultFont

          popup.font: QfTheme.defaultFont
          popup.topMargin: mainWindow.sceneTopMargin
          popup.bottomMargin: mainWindow.sceneTopMargin

          property bool loaded: false
          Component.onCompleted: {
            // This list matches the QfTracker::MeasureType enum
            var measurements = [qsTr("Elapsed time (seconds since start of tracking)"), qsTr("Timestamp (seconds since epoch)"), qsTr("Ground speed"), qsTr("Bearing"), qsTr("Horizontal accuracy"), qsTr("Vertical accuracy"), qsTr("PDOP"), qsTr("HDOP"), qsTr("VDOP")];
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
          visible: !QfLayerUtils.hasMValue(featureModel.currentLayer)
          Layout.fillWidth: true
          text: qsTr("To active the measurement functionality, make sure the vector layer's geometry type used for the tracking session has an M dimension.")
          font: QfTheme.tipFont
          color: QfTheme.secondaryTextColor

          wrapMode: Text.WordWrap
        }
      }
    }
  }

  Rectangle {
    id: bottomRow
    anchors.bottom: parent.bottom
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.leftMargin: -5
    anchors.rightMargin: -5
    anchors.bottomMargin: -5

    height: startTrackingButton.height + (resumeTrackingButton.visible ? resumeTrackingButton.height : 0) + 20
    color: QfTheme.darkTheme ? QfTheme.mainBackgroundColorSemiOpaque : QfTheme.lightestGraySemiOpaque

    Column {
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.bottom: parent.bottom
      anchors.leftMargin: 10
      anchors.rightMargin: 10
      anchors.bottomMargin: 10
      spacing: 5

      QfButton {
        id: startTrackingButton
        width: parent.width
        text: qsTr("Start tracking")
        icon.source: QfTheme.getThemeVectorIcon('directions_walk_24dp')

        onClicked: {
          applySettingsToTracker();
          featureModel.resetAttributes();
          featureModel.applyGeometry();
          trackerSettings.tracker.feature = featureModel.feature;
          trackingModel.requestTrackingSetup(trackerSettings.layer);
          trackerSettings.layer = undefined;
          trackerSettings.tracker = undefined;
          trackerSettings.close();
        }
      }

      QfButton {
        id: resumeTrackingButton
        width: parent.width
        text: qsTr("Resume tracking")
        icon.source: QfTheme.getThemeVectorIcon('directions_walk_24dp')
        icon.color: QfTheme.mainColor
        bgcolor: "transparent"
        color: QfTheme.mainColor
        visible: false

        onClicked: {
          applySettingsToTracker();
          displayToast(qsTr('Track on layer %1 resumed').arg(trackerSettings.tracker.vectorLayer.name));
          trackingModel.startTracker(trackerSettings.tracker.vectorLayer, positionSource.positionInformation, positionSource.projectedPosition);
          projectInfo.saveTracker(trackerSettings.tracker.vectorLayer);
          trackerSettings.layer = undefined;
          trackerSettings.tracker = undefined;
          trackerSettings.close();
        }
      }
    }
  }

  QfFeatureModel {
    id: featureModel
    project: qgisProject

    geometry: QfGeometry {}

    appExpressionContextScopesGenerator: QfAppExpressionContextScopesGenerator {
      positionInformation: appScopesGenerator.positionInformation
      positionLocked: true
      cloudUserInformation: appScopesGenerator.cloudUserInformation
    }
  }
}
```


