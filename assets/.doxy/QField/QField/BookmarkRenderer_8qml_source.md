

# File BookmarkRenderer.qml

[**File List**](files.md) **>** [**qml**](dir_1d81f7f4362e4398c0a695c4e466fcb0.md) **>** [**BookmarkRenderer.qml**](BookmarkRenderer_8qml.md)

[Go to the documentation of this file](BookmarkRenderer_8qml.md)


```C++
import QtQuick
import QtQuick.Shapes
import org.qgis
import org.qfield
import Theme

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
      geometryComponent.sourceComponent = undefined;
      if (geometryWrapper && geometryWrapper.qgsGeometry.type === Qgis.GeometryType.Point) {
        geometryComponent.sourceComponent = pointHighlight;
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
            PathArc {
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

        Component.onCompleted: {
          // Register bookmark handler
          pointHandler.registerHandler("bookmark_" + bookmarkRenderer.bookmarkId, (point, type, interactionType) => {
            if (pointHandler.pointInItem(point, bookmark)) {
              if (interactionType === "clicked") {
                displayToast(qsTr('Bookmark: %1').arg(bookmarkRenderer.bookmarkName));
                return true;
              } else if (interactionType === "pressedAndHold") {
                if (bookmarkRenderer.bookmarkUser) {
                  bookmarkProperties.bookmarkId = bookmarkRenderer.bookmarkId;
                  bookmarkProperties.bookmarkName = bookmarkRenderer.bookmarkName;
                  bookmarkProperties.bookmarkGroup = bookmarkRenderer.bookmarkGroup;
                  bookmarkProperties.open();
                } else {
                  displayToast(qsTr('Project bookmarks cannot be edited'));
                }
                return true;
              } else if (interactionType === "doubleClicked") {
                bookmarkModel.setExtentFromBookmark(bookmarkModel.index(bookmarkRenderer.bookmarkIndex, 0));
                return true;
              }
            }
            return false;
          }, MapCanvasPointHandler.Priority.Low);
        }

        Component.onDestruction: {
          // Deregister bookmark handler
          pointHandler.deregisterHandler("bookmark_" + bookmarkRenderer.bookmarkId);
        }
      }
    }
  }

  Loader {
    id: geometryComponent
    // the sourceComponent is updated with the connection on wrapper qgsGeometryChanged signal
    // but it needs to be ready on first used
    sourceComponent: geometryWrapper && geometryWrapper.qgsGeometry.type === Qgis.GeometryType.Point ? pointHighlight : undefined
  }
}
```


