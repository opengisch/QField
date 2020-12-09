import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtPositioning 5.3

import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0

import '.'

Item{
    id: tracking

    property var mainModel: model

    Component.onCompleted: {
        featureModel.resetAttributes()
        embeddedFeatureForm.state = 'Add'
        trackInformationDialog.active = true
    }

    RubberbandModel {
        id: rubberbandModel
        frozen: false
        vectorLayer: mainModel.vectorLayer
        currentCoordinate: positionSource.projectedPosition
        measureValue: ( positionSource.positionInfo.utcDateTime - mainModel.startPositionTimestamp ) / 1000
        currentPositionTimestamp: positionSource.positionInfo.utcDateTime
        crs: mapCanvas.mapSettings.destinationCrs

        onVertexCountChanged: {
          if( ( geometryType === QgsWkbTypes.LineGeometry && vertexCount > 2 ) ||
              ( geometryType === QgsWkbTypes.PolygonGeometry &&vertexCount > 3 ) )
          {
              featureModel.applyGeometry()

              if( ( geometryType === QgsWkbTypes.LineGeometry && vertexCount == 3 ) ||
                  ( geometryType === QgsWkbTypes.PolygonGeometry && vertexCount == 4 ) )
              {
                  // indirect action, no need to check for success and display a toast, the log is enough
                  featureModel.create()
                  mainModel.feature = featureModel.feature
              }
              else
              {
                  // indirect action, no need to check for success and display a toast, the log is enough
                  featureModel.save()
              }
          }
        }
    }

    Rubberband {
        id: rubberband
        width: 4
        color: Qt.rgba(Math.random(),Math.random(),Math.random(),0.6);

        mapSettings: mapCanvas.mapSettings

        model: rubberbandModel

        anchors.fill: parent

        visible: mainModel.visible
    }

    FeatureModel {
        id: featureModel
        currentLayer: mainModel.vectorLayer
        geometry: Geometry {
          id: featureModelGeometry
          rubberbandModel: rubberbandModel
          vectorLayer: mainModel.vectorLayer
        }
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

        x: 24
        y: 24
        padding: 0
        width: parent.width - 48
        height: parent.height - 48
        modal: true
        closePolicy: Popup.CloseOnEscape

        FeatureForm {
            id: form
            model: embeddedAttributeFormModel

            focus: true
            dontSave: true
            embedded: true
            toolbarVisible: true

            anchors.fill: parent

            state: 'Add'

            onTemporaryStored: {
                embeddedFeatureForm.active = false
                trackingModel.startTracker( mainModel.vectorLayer )
                displayToast( qsTr( 'Track on layer %1 started' ).arg( mainModel.vectorLayer.name  ) )
            }

            onCancelled: {
                embeddedFeatureForm.active = false
                embeddedFeatureForm.focus = false
                trackingModel.stopTracker( mainModel.vectorLayer )
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

        x: 24
        y: 24
        padding: 0
        width: parent.width - 48
        height: parent.height - 48
        modal: true
        closePolicy: Popup.CloseOnEscape

        Page {
            focus: true
            anchors.fill: parent

            header: PageHeader {
                title: qsTr("Tracker Settings")

                showApplyButton: true
                showCancelButton: true

                onApply: {
                    if( Number(timeIntervalText.text) + Number(distanceText.text) === 0 ||
                        ( timeIntervalCheck.checked && distanceCheck.checked && conjunction.checked &&
                          ( Number(timeIntervalText.text) === 0 || Number(distanceText.text) === 0 ) ) )
                    {
                        displayToast( qsTr( 'Cannot start track with empty values' ) )
                    }
                    else
                    {
                        mainModel.timeInterval = timeIntervalText.text.length == 0 || !timeIntervalCheck.checked ? 0 : timeIntervalText.text
                        mainModel.minimumDistance = distanceText.text.length == 0 || !distanceCheck.checked ? 0 : distanceText.text
                        mainModel.conjunction = conjunction.checked
                        mainModel.rubberModel = rubberbandModel

                        trackInformationDialog.active = false
                        embeddedFeatureForm.active = true
                    }
                }
                onCancel: {
                    trackInformationDialog.active = false
                    trackingModel.stopTracker( mainModel.vectorLayer )
                }
            }

            ColumnLayout{
                anchors.fill: parent
                Layout.fillWidth: true
                Layout.fillHeight: true

                spacing: 2
                anchors {
                    margins: 4
                    topMargin: 35// Leave space for the toolbar
                }

                CheckBox {
                    id: timeIntervalCheck
                    text: qsTr( 'Time interval (s)' )
                    font: Theme.defaultFont

                    Layout.fillWidth: true
                    indicator.height: 16
                    indicator.width: 16
                    indicator.implicitHeight: 24
                    indicator.implicitWidth: 24
                }

                TextField {
                    id: timeIntervalText
                    enabled: timeIntervalCheck.checked
                    height: fontMetrics.height + 20
                    topPadding: 10
                    bottomPadding: 10
                    Layout.fillWidth: true
                    font: Theme.defaultFont
                    text: '30'

                    inputMethodHints: Qt.ImhFormattedNumbersOnly

                    validator: IntValidator {
                    }

                    background: Rectangle {
                    y: timeIntervalText.height - height - timeIntervalText.bottomPadding / 2
                    implicitWidth: 120
                    height: timeIntervalText.activeFocus ? 2: 1
                    color: timeIntervalText.activeFocus ? "#4CAF50" : "#C8E6C9"
                    }
                }

                CheckBox {
                    DistanceArea {
                      id: infoDistanceArea
                      property VectorLayer currentLayer: mainModel.vectorLayer
                      project: qgisProject
                      crs: qgisProject.crs
                    }

                    id: distanceCheck

                    text: qsTr( 'Distance (%1)' ).arg( UnitTypes.toAbbreviatedString( infoDistanceArea.lengthUnits ) )
                    font: Theme.defaultFont

                    Layout.fillWidth: true
                    indicator.height: 16
                    indicator.width: 16
                    indicator.implicitHeight: 24
                    indicator.implicitWidth: 24
                }

                TextField {
                    id: distanceText
                    enabled: distanceCheck.checked
                    height: fontMetrics.height + 20
                    topPadding: 10
                    bottomPadding: 10
                    Layout.fillWidth: true
                    font: Theme.defaultFont
                    text: '50'

                    inputMethodHints: Qt.ImhFormattedNumbersOnly

                    validator: IntValidator {
                    }

                    background: Rectangle {
                    y: distanceText.height - height - distanceText.bottomPadding / 2
                    implicitWidth: 120
                    height: distanceText.activeFocus ? 2: 1
                    color: distanceText.activeFocus ? "#4CAF50" : "#C8E6C9"
                    }
                }

                Item {
                    // spacer item
                    height: 12
                }

                CheckBox {
                    id: conjunction
                    text: qsTr('Digitize vertex only when both conditions are met')
                    font: Theme.defaultFont
                    checked: timeIntervalCheck.checked && distanceCheck.checked
                    enabled: timeIntervalCheck.checked && distanceCheck.checked

                    Layout.fillWidth: true
                    indicator.height: 16
                    indicator.width: 16
                    indicator.implicitHeight: 24
                    indicator.implicitWidth: 24
                }

                Item {
                    // spacer item
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }

            onVisibleChanged: {
                if (visible) {
                    timeIntervalText.forceActiveFocus();
                }
            }
        }
      }
    }
}
