import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Controls 1.4 as Controls
import QtQuick.Dialogs 1.2
import org.qgis 1.0
import org.qfield 1.0
import Theme 1.0
import QtQuick.Layouts 1.3

import '.'


Item{
    id: tracking

    property VectorLayer trackingLayer //layerTree.data(layerTree.index(index,0), LayerTreeModel.VectorLayer )
    property bool activated: false
    property bool running: false
    property var feature: undefined

    signal trackingStarted
    signal trackingStopped

    RubberbandModel {
        id: rubberbandModel
        frozen: false
        currentCoordinate: positionSource.projectedPosition
        vectorLayer: trackingLayer
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
                  feature = featureModel.feature
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

        visible: layerTree.data(layerTree.index(index,0), LayerTreeModel.Visible) //binding does not work and I don't know why
    }

    Track {
        id: track

        model: rubberbandModel
    }

    FeatureModel {
        id: featureModel
        currentLayer: trackingLayer
        geometry: Geometry {
          id: featureModelGeometry
          rubberbandModel: rubberbandModel
          vectorLayer: trackingLayer
        }
    }

    function start( layer )
    {
        //layer is passed since I cannot bind trackingLayer with model.VectorLayer - don't know why
        trackingLayer = layer
        featureModel.resetAttributes()
        embeddedFeatureForm.state = 'Add'
        embeddedFeatureForm.active = true
    }

    function stop()
    {
        track.stop();
        rubberbandModel.reset();
        feature = undefined
        trackingStopped()
    }

    //the add entry stuff
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
                trackInformationDialog.active = true
                embeddedFeatureForm.active = false
            }

            onCancelled: {
                embeddedFeatureForm.active = false
                embeddedFeatureForm.focus = false
            }
        }
      }
    }

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
                title: qsTr("Track'n'Track")

                showApplyButton: true
                showCancelButton: true

                onApply: {
                    if( Number(timeIntervalText.text) + Number(distanceText.text) === 0 )
                    {
                        displayToast( qsTr( 'Cannot start track with empty values' ) )
                    }
                    else
                    {
                        track.timeInterval = timeIntervalText.text.length == 0 ? 0 : timeIntervalText.text
                        track.minimumDistance = distanceText.text.length == 0 ? 0 : distanceText.text
                        track.conjunction = conjunction.checked
                        track.start();

                        trackInformationDialog.active = false
                        trackingStarted()
                    }
                }
                onCancel: {
                    trackInformationDialog.active = false
                    displayToast( qsTr( 'No track on layer %1 started' ).arg( trackingLayer.name  ) )
                }
            }

            ColumnLayout{
                anchors.fill: parent
                Layout.fillWidth: true
                Layout.fillHeight: true

                spacing: 2
                anchors {
                    margins: 4 * dp
                    topMargin: 52 * dp // Leave space for the toolbar
                }

                Controls.Label {
                    id: timeIntervalLabel
                    Layout.fillWidth: true
                    text: qsTr( 'Time interval in seconds' )
                    font.bold: true
                }

                TextField {
                    id: timeIntervalText
                    height: fontMetrics.height + 20 * dp
                    topPadding: 10 * dp
                    bottomPadding: 10 * dp
                    Layout.fillWidth: true
                    font: Theme.defaultFont

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

                Item {
                    // spacer item
                    height: 35 * dp
                }

                Controls.Label {
                    id: distanceLabel
                    Layout.fillWidth: true
                    text: qsTr( 'Distance in meters' )
                    font.bold: true
                }

                TextField {
                    id: distanceText
                    height: fontMetrics.height + 20 * dp
                    topPadding: 10 * dp
                    bottomPadding: 10 * dp
                    Layout.fillWidth: true
                    font: Theme.defaultFont

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
                    height: 35 * dp
                }

                CheckBox {
                    id: conjunction
                    text: qsTr('Both conditions need to be fullfilled')
                    font: Theme.defaultFont
                    checked: timeIntervalText.text.length > 0 && distanceText.text.length > 0
                    enabled: timeIntervalText.text.length > 0 && distanceText.text.length > 0

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
                    timeInterval.forceActiveFocus();
                }
            }
        }
      }
    }
}
