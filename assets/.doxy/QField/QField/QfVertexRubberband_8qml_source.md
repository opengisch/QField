

# File QfVertexRubberband.qml

[**File List**](files.md) **>** [**core**](dir_aebb8dcc11953d78e620bbef0b9e2183.md) **>** [**qml**](dir_5e9804d2322dd99f421cb2c64b2c04c3.md) **>** [**QfVertexRubberband.qml**](QfVertexRubberband_8qml.md)

[Go to the documentation of this file](QfVertexRubberband_8qml.md)


```C++
import QtQuick
import QtQml
import org.qgis
import org.qfield.core

Repeater {
  id: vertexRubberband

  property MapSettings mapSettings
  property bool isVisible: false
  property bool isCycling: model.currentVertexIndex !== -1
  property bool isAddingVertex: model.editingMode === QfVertexModel.AddVertex

  property color vertexColor: "#40FF0000"
  property color vertexBorderColor: "#FF0000"
  property color selectedVertexColor: "#200000FF"
  property color selectedVertexBorderColor: "#0000FF"
  property color newVertexColor: "#404CAF50"
  property color newVertexBorderColor: "#4CAF50"

  delegate: Rectangle {
    QfMapToScreen {
      id: mapToScreen
      mapSettings: vertexRubberband.mapSettings
      mapPoint: Point
    }

    visible: vertexRubberband.isVisible

    x: mapToScreen.screenPoint.x - width / 2
    y: mapToScreen.screenPoint.y - width / 2
    opacity: !isCycling || (isAddingVertex && !ExistingVertex) || (!isAddingVertex && ExistingVertex) ? 1.0 : 0.25

    width: ((isAddingVertex && !ExistingVertex) || (!isAddingVertex && ExistingVertex) ? 16 : 8) * (CurrentVertex ? 1.33 : 1) / (rotation == 0 ? 1 : 1.25)
    height: width
    radius: ExistingVertex ? width / 2 : 0
    rotation: ExistingVertex ? 0 : 45

    color: "transparent"

    Rectangle {
      anchors.fill: parent
      radius: ExistingVertex ? width / 2 : 0
      color: "transparent"
      border.color: "#90FFFFFF"
      border.width: (QfVertexModel.ExistingVertex ? 4 : 2) * (CurrentVertex ? 1.5 : 1) + 2
    }

    Rectangle {
      anchors.fill: parent
      anchors.margins: 1
      radius: ExistingVertex ? width / 2 : 0
      color: CurrentVertex ? isAddingVertex ? vertexRubberband.newVertexColor : vertexRubberband.selectedVertexColor : vertexRubberband.vertexColor
      border.color: CurrentVertex ? isAddingVertex ? vertexRubberband.newVertexBorderColor : vertexRubberband.selectedVertexBorderColor : vertexRubberband.vertexBorderColor
      border.width: (QfVertexModel.ExistingVertex ? 4 : 2) * (CurrentVertex ? 1.5 : 1)
    }
  }
}
```


