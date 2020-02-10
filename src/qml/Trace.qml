import QtQuick 2.11
import QtQuick.Controls 2.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import Qt.labs.settings 1.0 as LabSettings
import QtQml 2.2
import org.qgis 1.0
import org.qfield 1.0
import QtPositioning 5.11
import Theme 1.0

import '.'

Item{
    id: trace

    property bool running: false
    property VectorLayer traceLayer //model.VectorLayer does not work see: function start( layer )

    Component.onCompleted: {
        console.log( "index: "+index )
    }

    Rubberband {
        id: traceRubberband
        width: 4 * dp
        color: Qt.rgba(Math.random(),Math.random(),Math.random(),0.4);

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
                      traceFeatureModel.applyGeometry()

                      if( ( geometryType === QgsWkbTypes.LineGeometry && vertexCount == 3 ) ||
                          ( geometryType === QgsWkbTypes.PolygonGeometry && vertexCount == 4 ) )
                      {
                          traceFeatureModel.create()
                      }
                      else
                      {
                          traceFeatureModel.save()
                      }
                  }
              }
        }

        anchors.fill: parent
        visible: true
    }

    FeatureModel {
        id: traceFeatureModel
        currentLayer: traceLayer
        geometry: Geometry {
          id: traceGeometry
          rubberbandModel: traceRubberband.model
          vectorLayer: traceLayer
        }
    }

    function start( layer )
    {
        //layer is passed since I cannot bind traceLayer with model.VectorLayer
        traceLayer = layer
        embeddedFeatureForm.state = 'Add'
        traceFeatureModel.resetAttributes()
        embeddedFeatureForm.active = true
    }

    function stop()
    {
        traceRubberband.traceStop();
        traceRubberband.model.reset();
        running = false;
    }

    //the add entry stuff
    AttributeFormModel {
      id: embeddedAttributeFormModel
      featureModel: traceFeatureModel
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
        id: popup
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
                traceRubberband.traceStart();
                trace.running = true;
            }

            onCancelled: {
                popup.close()
            }
        }
      }
    }
}
