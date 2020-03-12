import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as Controls
import QtQuick.Dialogs 1.2
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import QtQuick.Layouts 1.3
import QtPositioning 5.3

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
        measureValue: ( positionSource.position.timestamp - mainModel.startPositionTimestamp ) / 1000
        currentPositionTimestamp: positionSource.position.timestamp
        crs: mapCanvas.mapSettings.destinationCrs

        onVertexCountChanged: {
          if( ( geometryType === QgsWkbTypes.LineGeometry && vertexCount > 2 ) ||
              ( geometryType === QgsWkbTypes.PolygonGeometry &&vertexCount > 3 ) )
          {
              featureModel.applyGeometry()

              if( ( geometryType === QgsWkbTypes.LineGeometry && vertexCount == 3 ) ||
                  ( geometryType === QgsWkbTypes.PolygonGeometry && vertexCount == 4 ) )
              {
                  featureModel.create()
                  mainModel.feature = featureModel.feature
              }
              else
              {
                  featureModel.save()
              }
          }
        }
    }

    Rubberband {
        id: rubberband
        width: 4 * dp
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

        x: 24 * dp
        y: 24 * dp
        padding: 0
        width: parent.width - 48 * dp
        height: parent.height - 48 * dp
        modal: true
        closePolicy: Popup.CloseOnEscape

        FeatureForm {
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
                //displayToast( qsTr( 'No track on layer %1 started' ).arg( model.vectorLayer.name  ) )
                embeddedFeatureForm.active = false
                embeddedFeatureForm.focus = false
                trackingModel.stopTracker( mainModel.vectorLayer )
            }
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

        x: 24 * dp
        y: 24 * dp
        padding: 0
        width: parent.width - 48 * dp
        height: parent.height - 48 * dp
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
                    margins: 4 * dp
                    topMargin: 35 * dp // Leave space for the toolbar
                }

                CheckBox {
                    id: timeIntervalCheck
                    text: qsTr( 'Time interval (s)' )
                    font: Theme.defaultFont

                    Layout.fillWidth: true
                    indicator.height: 16 * dp
                    indicator.width: 16 * dp
                    indicator.implicitHeight: 24 * dp
                    indicator.implicitWidth: 24 * dp
                }

                TextField {
                    id: timeIntervalText
                    enabled: timeIntervalCheck.checked
                    height: fontMetrics.height + 20 * dp
                    topPadding: 10 * dp
                    bottomPadding: 10 * dp
                    Layout.fillWidth: true
                    font: Theme.defaultFont
                    text: '30'

                    inputMethodHints: Qt.ImhFormattedNumbersOnly

                    validator: IntValidator {
                    }

                    background: Rectangle {
                    y: timeIntervalText.height - height - timeIntervalText.bottomPadding / 2
                    implicitWidth: 120 * dp
                    height: timeIntervalText.activeFocus ? 2 * dp : 1 * dp
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
                    indicator.height: 16 * dp
                    indicator.width: 16 * dp
                    indicator.implicitHeight: 24 * dp
                    indicator.implicitWidth: 24 * dp
                }

                TextField {
                    id: distanceText
                    enabled: distanceCheck.checked
                    height: fontMetrics.height + 20 * dp
                    topPadding: 10 * dp
                    bottomPadding: 10 * dp
                    Layout.fillWidth: true
                    font: Theme.defaultFont
                    text: '50'

                    inputMethodHints: Qt.ImhFormattedNumbersOnly

                    validator: IntValidator {
                    }

                    background: Rectangle {
                    y: distanceText.height - height - distanceText.bottomPadding / 2
                    implicitWidth: 120 * dp
                    height: distanceText.activeFocus ? 2 * dp : 1 * dp
                    color: distanceText.activeFocus ? "#4CAF50" : "#C8E6C9"
                    }
                }

                Item {
                    // spacer item
                    height: 12 * dp
                }

                CheckBox {
                    id: conjunction
                    text: qsTr('Digitize vertex only when both conditions are met')
                    font: Theme.defaultFont
                    checked: timeIntervalCheck.checked && distanceCheck.checked
                    enabled: timeIntervalCheck.checked && distanceCheck.checked

                    Layout.fillWidth: true
                    indicator.height: 16 * dp
                    indicator.width: 16 * dp
                    indicator.implicitHeight: 24 * dp
                    indicator.implicitWidth: 24 * dp
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
