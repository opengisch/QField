import QtQuick 2.12
import QtGraphicalEffects 1.0

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Item {
    id: bookmarkRenderer

    property var bookmarkIndex: undefined
    property string bookmarkName: ''
    property string bookmarkId: ''

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
                Image {
                    id: bookmark

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

                    x: mapToScreenPosition.screenPoint.x - width/2
                    y: mapToScreenPosition.screenPoint.y - height

                    width: 36
                    height: 36
                    source: Theme.getThemeVectorIcon("ic_place_white_24dp")
                    sourceSize.width: 36 * screen.devicePixelRatio
                    sourceSize.height: 36 * screen.devicePixelRatio

                    ColorOverlay {
                        anchors.fill: bookmark
                        source: bookmark
                        color: Theme.mainColor
                    }

                    MouseArea {
                        anchors.fill: bookmark
                        onClicked: {
                            displayToast(qsTr('Bookmark: %1').arg(bookmarkRenderer.bookmarkName));
                        }
                        onDoubleClicked: {
                            bookmarkModel.setExtentFromBookmark(bookmarkModel.index(bookmarkRenderer.bookmarkIndex, 0));
                        }
                    }
                }

                Glow {
                    anchors.fill: bookmark
                    source: bookmark
                    radius: 7
                    samples: 17
                    color: "#44000000"
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

