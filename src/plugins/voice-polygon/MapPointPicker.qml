// src/plugins/voice-polygon/MapPointPicker.qml
import QtQuick
import org.qfield
import Theme

Item {
  id: root
  visible: false

  signal pointPicked(var point, string crs)
  signal cancelled()

  property string surveyType: ""

  readonly property var typeColors: ({
    "construccion": "#1565C0",
    "unidad":       "#2E7D32",
    "terreno":      "#E65100",
  })
  readonly property var typeIcons: ({
    "construccion": "🏠",
    "unidad":       "🏗️",
    "terreno":      "📐",
  })

  Rectangle {
    anchors.top: parent.top
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.margins: 16
    height: 64
    radius: 12
    color: root.typeColors[root.surveyType] || "#1565C0"
    opacity: 0.95

    Row {
      anchors.centerIn: parent
      spacing: 12

      Text {
        text: root.typeIcons[root.surveyType] || "📍"
        font.pixelSize: 28
        anchors.verticalCenter: parent.verticalCenter
      }

      Column {
        anchors.verticalCenter: parent.verticalCenter
        Text {
          text: "Toca el mapa para fijar el punto de inicio"
          color: "white"; font.pixelSize: 15; font.bold: true
        }
        Text {
          text: root.surveyType.charAt(0).toUpperCase() + root.surveyType.slice(1)
          color: "rgba(255,255,255,0.8)"; font.pixelSize: 12
        }
      }
    }
  }

  Item {
    anchors.centerIn: parent
    width: 40; height: 40
    Rectangle { anchors.horizontalCenter: parent.horizontalCenter; width: 2; height: 40; color: "red"; opacity: 0.6 }
    Rectangle { anchors.verticalCenter: parent.verticalCenter; width: 40; height: 2; color: "red"; opacity: 0.6 }
    Rectangle { anchors.centerIn: parent; width: 8; height: 8; radius: 4; color: "red" }
  }

  QfToolButton {
    anchors.bottom: parent.bottom
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.bottomMargin: 24
    text: "Cancelar"
    flat: true
    onClicked: { root.deactivate(); root.cancelled() }
  }

  property var mapCanvasHandler: null

  function activate(type) {
    root.surveyType = type
    root.visible = true
    mapCanvasHandler = Qt.createQmlObject(`
      import QtQuick
      import org.qfield
      MapCanvasPointHandler {
        onPoint: function(point, crs) { root.pointPicked(point, crs) }
      }
    `, root, "dynamicHandler")
    iface.mapCanvas().registerMapCanvasPointHandler(mapCanvasHandler)
    iface.mainWindow().displayToast("📍 Toca el mapa para iniciar")
  }

  function deactivate() {
    root.visible = false
    if (mapCanvasHandler) {
      iface.mapCanvas().unregisterMapCanvasPointHandler(mapCanvasHandler)
      mapCanvasHandler.destroy()
      mapCanvasHandler = null
    }
  }
}
