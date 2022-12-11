import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.14

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

import '.'

Item {
  id: tracking

  property var track: model

  Component.onCompleted: {
    featureModel.resetAttributes()
    embeddedFeatureForm.state = 'Add'
    trackInformationDialog.active = true
  }

  RubberbandModel {
    id: rubberbandModel
    frozen: false
    vectorLayer: track.vectorLayer
    currentCoordinate: positionSource.projectedPosition

    property int measureType: track.measureType
    measureValue: {
      switch(measureType) {
        case Tracker.SecondsSinceStart:
          return ( positionSource.positionInformation.utcDateTime - track.startPositionTimestamp ) / 1000
        case Tracker.Timestamp:
          return positionSource.positionInformation.utcDateTime.getTime()
        case Tracker.GroundSpeed:
          return positionSource.positionInformation.speed
        case Tracker.Bearing:
          return positionSource.positionInformation.direction
        case Tracker.HorizontalAccuracy:
          return positionSource.positionInformation.hacc
        case Tracker.VerticalAccuracy:
          return positionSource.positionInformation.vacc
        case Tracker.PDOP:
          return positionSource.positionInformation.pdop
        case Tracker.HDOP:
          return positionSource.positionInformation.hdop
        case Tracker.VDOP:
          return positionSource.positionInformation.vdop
      }
      return 0;
    }

    currentPositionTimestamp: positionSource.positionInformation.utcDateTime
    crs: mapCanvas.mapSettings.destinationCrs

    onVertexCountChanged: {
      if (vertexCount == 0) {
        return;
      }

      if (geometryType === QgsWkbTypes.PointGeometry) {
        featureModel.applyGeometry()
        featureModel.resetFeatureId();
        featureModel.resetAttributes(true);
        featureModel.create();
      } else {
        if ((geometryType === QgsWkbTypes.LineGeometry && vertexCount > 2) ||
            (geometryType === QgsWkbTypes.PolygonGeometry &&vertexCount > 3))
        {
          featureModel.applyGeometry()

          if ((geometryType === QgsWkbTypes.LineGeometry && vertexCount == 3) ||
              (geometryType === QgsWkbTypes.PolygonGeometry && vertexCount == 4))
          {
            // indirect action, no need to check for success and display a toast, the log is enough
            featureModel.create()
            track.feature = featureModel.feature
          }
          else
          {
            // indirect action, no need to check for success and display a toast, the log is enough
            featureModel.save()
          }
        }
      }
    }
  }

  Rubberband {
    id: rubberband
    anchors.fill: parent
    visible: track.visible

    lineWidth: 4
    color: Qt.rgba(Math.min(0.75, Math.random()),
                   Math.min(0.75,Math.random()),
                   Math.min(0.75,Math.random()),
                   0.6)
    geometryType: QgsWkbTypes.LineGeometry

    mapSettings: mapCanvas.mapSettings
    model: rubberbandModel
  }

  FeatureModel {
    id: featureModel
    project: qgisProject
    currentLayer: track.vectorLayer

    geometry: Geometry {
      id: featureModelGeometry
      rubberbandModel: rubberbandModel
      vectorLayer: track.vectorLayer
    }

    positionInformation: coordinateLocator.positionInformation
    positionLocked: true
    cloudUserInformation: cloudConnection.userInformation
  }


  // Feature form to set attributes
  AttributeFormModel {
    id: embeddedAttributeFormModel
    featureModel: featureModel
  }

  Loader {
    id: embeddedFeatureForm

    sourceComponent: embeddedFeatureFormComponent
    active: false
    onLoaded: {
      item.open()
    }
  }

  Component {
    id: embeddedFeatureFormComponent

    Popup {
      id: embeddedFeatureFormPopup
      parent: ApplicationWindow.overlay

      x: Theme.popupScreenEdgeMargin
      y: Theme.popupScreenEdgeMargin
      padding: 0
      width: parent.width - Theme.popupScreenEdgeMargin * 2
      height: parent.height - Theme.popupScreenEdgeMargin * 2
      modal: true
      closePolicy: Popup.CloseOnEscape

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
          embeddedFeatureForm.active = false
          trackingModel.startTracker(track.vectorLayer)
          displayToast(qsTr('Track on layer %1 started').arg(track.vectorLayer.name))
        }

        onCancelled: {
          embeddedFeatureForm.active = false
          embeddedFeatureForm.focus = false
          trackingModel.stopTracker(track.vectorLayer)
        }
      }

      onClosed: {
        form.confirm()
      }
    }
  }


  // Dialog to set tracker properties
  Loader {
    id: trackInformationDialog

    sourceComponent: trackInformationDialogComponent
    active: false
    onLoaded: {
      item.open()
    }
  }

  Component {
    id: trackInformationDialogComponent

    Popup {
      id: trackInformationPopup
      parent: ApplicationWindow.overlay

      x: Theme.popupScreenEdgeMargin
      y: Theme.popupScreenEdgeMargin
      padding: 0
      width: parent.width - Theme.popupScreenEdgeMargin * 2
      height: parent.height - Theme.popupScreenEdgeMargin * 2
      modal: true
      closePolicy: Popup.CloseOnEscape

      Page {
        focus: true
        anchors.fill: parent

        header: PageHeader {
          title: qsTr("Tracker Settings")

          showApplyButton: false
          showCancelButton: false
          showBackButton: true

          onBack: {
            trackInformationDialog.active = false
            trackingModel.stopTracker(track.vectorLayer)
          }
        }

        ScrollView {
          padding: 20
          ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
          ScrollBar.vertical.policy: ScrollBar.AsNeeded
          contentWidth: trackerSettingsGrid.width
          contentHeight: trackerSettingsGrid.height
          anchors.fill: parent
          clip: true

          GridLayout {
            id: trackerSettingsGrid
            width: parent.parent.width
            Layout.fillWidth: true

            columns: 2
            columnSpacing: 0
            rowSpacing: 5


            Label {
              text: qsTr("Activate time constraint")
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
              checked: positioningSettings.trackerTimeIntervalConstraint
              onCheckedChanged: {
                positioningSettings.trackerTimeIntervalConstraint = checked
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

            TextField {
              id: timeIntervalValue
              width: timeInterval.width
              font: Theme.defaultFont
              enabled: timeInterval.checked
              visible: timeInterval.checked
              horizontalAlignment: TextInput.AlignHCenter
              Layout.preferredWidth: 60
              Layout.preferredHeight: font.height + 20

              inputMethodHints: Qt.ImhFormattedNumbersOnly
              validator: DoubleValidator { locale: 'C' }

              background: Rectangle {
                y: parent.height - height - parent.bottomPadding / 2
                implicitWidth: 120
                height: parent.activeFocus ? 2: 1
                color: parent.activeFocus ? Theme.accentColor : Theme.accentLightColor
              }

              Component.onCompleted: {
                text = isNaN(positioningSettings.trackerTimeInterval) ? '' : positioningSettings.trackerTimeInterval
              }

              onTextChanged: {
                if( text.length === 0 || isNaN(text) ) {
                  positioningSettings.trackerTimeInterval = NaN
                } else {
                  positioningSettings.trackerTimeInterval = parseFloat( text )
                }
              }
            }

            Label {
              text: qsTr("Activate distance constraint")
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
              checked: positioningSettings.trackerMinimumDistanceConstraint
              onCheckedChanged: {
                positioningSettings.trackerMinimumDistanceConstraint = checked
              }
            }

            DistanceArea {
              id: infoDistanceArea
              property VectorLayer currentLayer: track.vectorLayer
              project: qgisProject
              crs: qgisProject.crs
            }

            Label {
              text: qsTr("Minimum distance [%1]").arg( UnitTypes.toAbbreviatedString( infoDistanceArea.lengthUnits ) )
              font: Theme.defaultFont
              wrapMode: Text.WordWrap
              enabled: minimumDistance.checked
              visible: minimumDistance.checked
              Layout.leftMargin: 8
              Layout.fillWidth: true
            }

            TextField {
              id: minimumDistanceValue
              width: minimumDistance.width
              font: Theme.defaultFont
              enabled: minimumDistance.checked
              visible: minimumDistance.checked
              horizontalAlignment: TextInput.AlignHCenter
              Layout.preferredWidth: 60
              Layout.preferredHeight: font.height + 20

              inputMethodHints: Qt.ImhFormattedNumbersOnly
              validator: DoubleValidator { locale: 'C' }

              background: Rectangle {
                y: parent.height - height - parent.bottomPadding / 2
                implicitWidth: 120
                height: parent.activeFocus ? 2: 1
                color: parent.activeFocus ? Theme.accentColor : Theme.accentLightColor
              }

              Component.onCompleted: {
                text = isNaN(positioningSettings.trackerMinimumDistance) ? '' : positioningSettings.trackerMinimumDistance
              }

              onTextChanged: {
                if( text.length === 0 || isNaN(text) ) {
                  positioningSettings.trackerMinimumDistance = NaN
                } else {
                  positioningSettings.trackerMinimumDistance = parseFloat( text )
                }
              }
            }

            Label {
              text: qsTr("Record when both active constraints are met")
              font: Theme.defaultFont
              wrapMode: Text.WordWrap
              Layout.fillWidth: true
              enabled: timeInterval.checked && minimumDistance.checked
              visible: timeInterval.checked && minimumDistance.checked

              MouseArea {
                anchors.fill: parent
                onClicked: allConstraints.toggle()
              }
            }

            QfSwitch {
              id: allConstraints
              Layout.preferredWidth: implicitContentWidth
              Layout.alignment: Qt.AlignTop
              enabled: timeInterval.checked && minimumDistance.checked
              visible: timeInterval.checked && minimumDistance.checked
              checked: positioningSettings.trackerMeetAllConstraints
              onCheckedChanged: {
                positioningSettings.trackerMeetAllConstraints = checked
              }
            }

            Label {
              text: qsTr( "When enabled, vertices with only be recorded when both active constraints are met. If the setting is disabled, individual constraints met will trigger a vertex addition." )
              font: Theme.tipFont
              color: Theme.gray
              textFormat: Qt.RichText
              wrapMode: Text.WordWrap
              Layout.fillWidth: true
              enabled: timeInterval.checked && minimumDistance.checked
              visible: timeInterval.checked && minimumDistance.checked
            }


            Label {
              text: qsTr( "When both constraints are disabled, vertex additions will occur as frequently as delivered by the positioning device." )
              font: Theme.tipFont
              color: Theme.gray
              textFormat: Qt.RichText
              wrapMode: Text.WordWrap
              Layout.fillWidth: true
              visible: !timeInterval.checked && !minimumDistance.checked
            }

            Item {
              Layout.preferredWidth: allConstraints.width
              Layout.columnSpan: 2
            }

            Label {
                id: measureLabel
                Layout.fillWidth: true
                Layout.columnSpan: 2
                text: qsTr( "Measure (M) value attached to vertices:" )
                font: Theme.defaultFont

                wrapMode: Text.WordWrap
            }

            ComboBox {
                id: measureComboBox
                enabled: LayerUtils.hasMValue(track.vectorLayer)
                Layout.fillWidth: true
                Layout.columnSpan: 2
                Layout.alignment: Qt.AlignVCenter
                font: Theme.defaultFont
                popup.font: Theme.defaultFont

                property bool loaded: false
                Component.onCompleted: {
                    // This list matches the Tracker::MeasureType enum
                    var measurements = [
                      qsTr("Elapsed time (seconds since start of tracking)"),
                      qsTr("Timestamp (milliseconds since epoch)"),
                      qsTr("Ground speed"),
                      qsTr("Bearing"),
                      qsTr("Horizontal accuracy"),
                      qsTr("Vertical accuracy"),
                      qsTr("PDOP"),
                      qsTr("HDOP"),
                      qsTr("VDOP")
                    ];

                    model = measurements;
                    currentIndex = positioningSettings.trackerMeasureType;
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
                visible: !LayerUtils.hasMValue(track.vectorLayer)
                Layout.fillWidth: true
                text: qsTr( "To active the measurement functionality, make sure the vector layer's geometry type used for the tracking session has an M dimension." )
                font: Theme.tipFont
                color: Theme.gray

                wrapMode: Text.WordWrap
            }

            Item {
                // spacer item
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            QfButton {
              id: trackingButton
              Layout.topMargin: 8
              Layout.fillWidth: true
              Layout.columnSpan: 2
              text: qsTr( "Start tracking")
              icon.source: Theme.getThemeVectorIcon( 'directions_walk_24dp' )

              onClicked: {
                track.timeInterval = timeIntervalValue.text.length == 0 || !timeInterval.checked ? 0 : timeIntervalValue.text
                track.minimumDistance = minimumDistanceValue.text.length == 0 || !minimumDistance.checked ? 0 : minimumDistanceValue.text
                track.conjunction = timeInterval.checked && minimumDistance.checked && allConstraints.checked
                track.rubberModel = rubberbandModel
                track.measureType = measureComboBox.currentIndex
                rubberbandModel.measureType = track.measureType

                trackInformationDialog.active = false
                if (embeddedAttributeFormModel.rowCount() > 0 && !featureModel.suppressFeatureForm()) {
                  embeddedFeatureForm.active = true
                } else {
                  trackingModel.startTracker(track.vectorLayer)
                  displayToast(qsTr('Track on layer %1 started').arg(track.vectorLayer.name))
                }
              }
            }

            Item {
              // spacer item
              Layout.fillWidth: true
              Layout.fillHeight: true
            }
          }
        }
      }
    }
  }
}
