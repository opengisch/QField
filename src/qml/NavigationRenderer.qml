import QtQuick 2.12

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Item {
    id: navigationRenderer

    property var pointIndex: undefined
    property int pointType: NavigationModel.Destination

    property MapSettings mapSettings
    property alias geometryWrapper: geometryWrapper

    QgsGeometryWrapper {
        id: geometryWrapper
    }

    Connections {
        target: geometryWrapper

        function onQgsGeometryChanged() {
            geometryComponent.sourceComponent = undefined
            if (geometryWrapper && geometryWrapper.qgsGeometry.type === QgsWkbTypes.PointGeometry) {
                geometryComponent.sourceComponent = pointHighlight
            }
        }
    }

    Component {
        id: pointHighlight

        Repeater {
            model: geometryWrapper.pointList()

            Item {
                Rectangle {
                    id: point

                    property CoordinateTransformer ct: CoordinateTransformer {
                        id: _ct
                        sourceCrs: geometryWrapper.crs
                        sourcePosition: modelData
                        destinationCrs: mapCanvas.mapSettings.destinationCrs
                        transformContext: qgisProject.transformContext
                    }

                    MapToScreen {
                        id: mapToScreenPosition
                        mapSettings: mapCanvas.mapSettings
                        mapPoint: _ct.projectedPosition
                    }

                    property bool isOnCanvas: mapToScreenPosition.screenPoint.x > 0
                                              && mapToScreenPosition.screenPoint.x < mapCanvas.width
                                              && mapToScreenPosition.screenPoint.y > 0
                                              && mapToScreenPosition.screenPoint.y < mapCanvas.height
                    x: Math.min(mapCanvas.width, Math.max(0,mapToScreenPosition.screenPoint.x)) - width/2
                    y: Math.min(mapCanvas.height, Math.max(0,mapToScreenPosition.screenPoint.y)) - height/2

                    width: isOnCanvas ? 20 : 10
                    height: isOnCanvas ? 20 : 10
                    color: "#FF0000"
                    border.color: "#FFFFFF"
                    border.width: isOnCanvas ? 3 : 1
                    transform: Rotation { origin.x: point.width / 2; origin.y: point.width / 2; angle: 45}
                }
            }
        }
    }

    Loader {
        id: geometryComponent
        // the sourceComponent is updated with the connection on wrapper qgsGeometryChanged signal
        // but it needs to be ready on first used
        sourceComponent: geometryWrapper && geometryWrapper.qgsGeometry.type === QgsWkbTypes.PointGeometry ? pointHighlight : undefined
    }
}

