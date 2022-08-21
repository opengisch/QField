import QtQuick 2.14
import QtQuick.Shapes 1.14

import org.qgis 1.0
import org.qfield 1.0

import Theme 1.0

Item {
    id: bookmarkRenderer

    property var bookmarkIndex: undefined
    property string bookmarkId: ''
    property string bookmarkName: ''
    property string bookmarkGroup: ''
    property bool bookmarkUser: false

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
                property CoordinateTransformer ct: CoordinateTransformer {
                    id: _ct
                    sourceCrs: geometryWrapper.crs
                    sourcePosition: modelData
                    destinationCrs: mapCanvas.mapSettings.destinationCrs
                    transformContext: qgisProject ? qgisProject.transformContext : CoordinateReferenceSystemUtils.emptyTransformContext()
                }

                MapToScreen {
                    id: mapToScreenPosition
                    mapSettings: mapCanvas.mapSettings
                    mapPoint: _ct.projectedPosition
                }

                Shape {
                    id: bookmark

                    x: mapToScreenPosition.screenPoint.x - width / 2
                    y: mapToScreenPosition.screenPoint.y - height + 4

                    width: 36
                    height: 40

                    ShapePath {
                        strokeWidth: 3
                        strokeColor: "white"
                        strokeStyle: ShapePath.SolidLine
                        joinStyle: ShapePath.MiterJoin
                        fillColor: {
                            switch (bookmarkRenderer.bookmarkGroup) {
                                case 'red':
                                    return Theme.bookmarkRed;
                                case 'orange':
                                    return Theme.bookmarkOrange;
                                case 'blue':
                                    return Theme.bookmarkBlue;
                                default:
                                    return Theme.bookmarkDefault;
                            }
                        }

                        startX: 6
                        startY: 16
                        PathArc {
                            x: 30
                            y: 16
                            radiusX: 12
                            radiusY: 14
                        }
                        PathArc {
                            x: 18
                            y: 36
                            radiusX: 36
                            radiusY: 36
                        }
                        PathArc{
                            x: 6
                            y: 16
                            radiusX: 36
                            radiusY: 36
                        }
                    }

                    Rectangle {
                        x: 13
                        y: 9
                        width: 10
                        height: 10
                        color: "white"
                        radius: 4
                    }

                    layer.enabled: true
                    layer.samples: 4
                    layer.effect: QfDropShadow {
                        transparentBorder: true
                        radius: 8
                        color: "#99000000"
                        horizontalOffset: 0
                        verticalOffset: 0
                    }
                }

                MouseArea {
                    anchors.fill: bookmark
                    onClicked: {
                        displayToast(qsTr('Bookmark: %1').arg(bookmarkRenderer.bookmarkName));
                    }
                    onDoubleClicked: {
                        bookmarkModel.setExtentFromBookmark(bookmarkModel.index(bookmarkRenderer.bookmarkIndex, 0));
                    }
                    onPressAndHold: {
                        if (bookmarkRenderer.bookmarkUser) {
                            bookmarkProperties.bookmarkId = bookmarkRenderer.bookmarkId;
                            bookmarkProperties.bookmarkName = bookmarkRenderer.bookmarkName;
                            bookmarkProperties.bookmarkGroup = bookmarkRenderer.bookmarkGroup;
                            bookmarkProperties.open();
                        } else {
                            displayToast(qsTr('Project bookmarks cannot be edited'))
                        }
                    }
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

