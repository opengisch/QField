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
    id: trace

    property VectorLayer traceLayer //model.VectorLayer does not work see: function start( layer )

    Rubberband {
        id: rubberband
        width: 4 * dp
        color: Qt.rgba(Math.random(),Math.random(),Math.random(),0.6);

        mapSettings: mapCanvas.mapSettings

        model: RubberbandModel {
            frozen: false
            currentCoordinate: positionSource.projectedPosition
            vectorLayer: traceLayer
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
                  }
                  else
                  {
                      featureModel.save()
                  }
              }
            }
        }

        anchors.fill: parent
        visible: true
    }

    FeatureModel {
        id: featureModel
        currentLayer: traceLayer
        geometry: Geometry {
          id: featureModelGeometry
          rubberbandModel: rubberband.model
          vectorLayer: traceLayer
        }
    }

    function start( layer )
    {
        //layer is passed since I cannot bind traceLayer with model.VectorLayer - don't know why
        traceLayer = layer
        featureModel.resetAttributes()
        embeddedFeatureForm.state = 'Add'
        embeddedFeatureForm.active = true
    }

    function stop()
    {
        rubberband.traceStop();
        rubberband.model.reset();
        running = false;

        displayToast( qsTr( 'Trace on layer %1 stopped' ).arg( traceLayer.name  ) )
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
                traceInformationDialog.active = true
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
      id: traceInformationDialog

      sourceComponent: traceInformationDialogComponent
      active: false
      onLoaded: {
        item.open()
      }
    }

    Component {
      id: traceInformationDialogComponent

      Popup {
        id: traceInformationPopup
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
                title: qsTr("Track'n'Trace")

                showApplyButton: true
                showCancelButton: true

                onApply: {
                    if( Number(timeIntervalText.text) + Number(distanceText.text) === 0 )
                    {
                        displayToast( qsTr( 'Cannot start trace with empty values' ) )
                    }
                    else
                    {
                        rubberband.traceTimeInterval = timeIntervalText.text.length == 0 ? 0 : timeIntervalText.text
                        rubberband.traceMinimumDistance = distanceText.text.length == 0 ? 0 : distanceText.text
                        rubberband.traceConjunction = conjunction.checked

                        rubberband.traceStart();
                        trace.running = true;

                        traceInformationDialog.active = false

                        displayToast( qsTr( 'Trace on layer %1 started' ).arg( traceLayer.name  ) )
                    }
                }
                onCancel: {
                    traceInformationDialog.active = false
                    displayToast( qsTr( 'No trace on layer %1 started' ).arg( traceLayer.name  ) )
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
                    width: parent.width
                    text: qsTr( 'Time interval in seconds' )
                    font.bold: true
                }

                TextField {
                    id: timeIntervalText
                    height: fontMetrics.height + 20 * dp
                    topPadding: 10 * dp
                    bottomPadding: 10 * dp
                    anchors.left: parent.left
                    anchors.right: parent.right
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
                    width: parent.width
                    text: qsTr( 'Distance in meters' )
                    font.bold: true
                }

                TextField {
                    id: distanceText
                    height: fontMetrics.height + 20 * dp
                    topPadding: 10 * dp
                    bottomPadding: 10 * dp
                    anchors.left: parent.left
                    anchors.right: parent.right
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
