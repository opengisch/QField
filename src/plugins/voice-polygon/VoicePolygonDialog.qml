// src/plugins/voice-polygon/VoicePolygonDialog.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import org.qfield
import Theme
import "CommandParser.js" as Parser
import "PolygonBuilder.js" as Builder
import "LayerRouter.js" as Router

QfDialog {
  id: root
  title: {
    const icons = { "construccion":"🏠", "unidad":"🏗️", "terreno":"📐" }
    return (icons[surveyType] || "🎙️") + " VoicePolygon — " + surveyType
  }
  width:  Math.min(iface.mainWindow().width * 0.92, 420)
  height: iface.mainWindow().height * 0.6
  modal: false
  y: iface.mainWindow().height - height - 32

  signal polygonSaved()

  property string surveyType: ""
  property var    originPoint: null
  property string originCrs: ""
  property var    mapOverlay: null

  property var polygonState:    Builder.initialState()
  property var commandHistory:  []

  ColumnLayout {
    anchors.fill: parent
    anchors.margins: 8
    spacing: 6

    PolygonCanvas {
      id: miniCanvas
      Layout.fillWidth: true
      Layout.preferredHeight: 120
      polygonState: root.polygonState
      surveyType:   root.surveyType
    }

    Rectangle {
      Layout.fillWidth: true
      height: 36
      color: Theme.darkBackgroundColor
      radius: 6

      Row {
        anchors.centerIn: parent
        spacing: 20

        StatLabel { label: "Segmentos"; value: root.polygonState.segments.length }
        StatLabel { label: "Área";      value: Builder.calculateArea(root.polygonState).toFixed(1) + " m²" }
        StatLabel { label: "Ángulo";    value: (root.polygonState.currentAngle || 0).toFixed(0) + "°" }
        StatLabel { label: "Perímetro"; value: Builder.calculatePerimeter(root.polygonState).toFixed(1) + " m" }
      }
    }

    Rectangle {
      Layout.fillWidth: true
      height: 60
      color: "#0D1117"
      radius: 6

      ScrollView {
        anchors.fill: parent
        anchors.margins: 6
        Text {
          width: parent.width
          wrapMode: Text.Wrap
          color: "#00ff88"
          font.family: "monospace"
          font.pixelSize: 10
          text: root.commandHistory.slice(-4).join("\n") || "Esperando comandos de voz..."
        }
      }
    }

    Rectangle {
      Layout.alignment: Qt.AlignHCenter
      width: 72; height: 72; radius: 36
      color: voiceEngine.isListening ? "#D32F2F" : Theme.mainColor

      SequentialAnimation on scale {
        running: voiceEngine.isListening
        loops: Animation.Infinite
        NumberAnimation { to: 1.12; duration: 600; easing.type: Easing.InOutSine }
        NumberAnimation { to: 1.0;  duration: 600; easing.type: Easing.InOutSine }
      }

      Text { anchors.centerIn: parent; text: voiceEngine.isListening ? "⏹" : "🎙️"; font.pixelSize: 30 }

      Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.bottom; anchors.topMargin: 4
        text: voiceEngine.isListening ? "Escuchando..." : "Toca para hablar"
        font.pixelSize: 10; color: Theme.secondaryTextColor
      }

      MouseArea {
        anchors.fill: parent
        onClicked: voiceEngine.isListening ? voiceEngine.stopListening() : voiceEngine.startListening()
      }
    }

    GridLayout {
      Layout.fillWidth: true
      columns: 4; columnSpacing: 6; rowSpacing: 6

      QfButton { text: "↩ Deshacer"; Layout.fillWidth: true; onClicked: processCommand({ type:'UNDO'  }) }
      QfButton { text: "⬛ Cerrar";  Layout.fillWidth: true; onClicked: processCommand({ type:'CLOSE' }) }
      QfButton { text: "🔄 Reset";   Layout.fillWidth: true; onClicked: processCommand({ type:'RESET' }) }
      QfButton {
        text: "💾 Guardar"
        Layout.fillWidth: true
        enabled: root.polygonState.segments.length >= 2
        onClicked: saveToLayer()
      }
    }

    RowLayout {
      id: manualInputRow
      Layout.fillWidth: true
      visible: false
      spacing: 6

      TextField {
        id: manualTextField
        Layout.fillWidth: true
        placeholderText: "ej: muro 10 metros a la derecha"
        font.pixelSize: 13
        Keys.onReturnPressed: submitManual()
      }

      QfButton {
        text: "▶"
        onClicked: submitManual()
      }
    }
  }

  VoiceEngine {
    id: voiceEngine
    onCommandReceived: function(rawText) {
      const cmd = Parser.parse(rawText, root.polygonState.currentAngle)
      root.commandHistory.push(`🎙 "${rawText}" → ${cmd.type}${cmd.label ? ' (' + cmd.label + ')' : ''}`)
      processCommand(cmd)
    }
    onErrorOccurred: function(msg) { iface.mainWindow().displayToast("⚠️ " + msg) }
    onOfflineUnavailable: {
      iface.mainWindow().displayToast("📥 Modelo offline no disponible — usa texto")
      manualInputRow.visible = true
    }
  }

  function submitManual() {
    const text = manualTextField.text.trim()
    if (!text) return
    manualTextField.text = ""
    const cmd = Parser.parse(text, root.polygonState.currentAngle)
    root.commandHistory.push(`⌨ "${text}" → ${cmd.type}${cmd.label ? ' (' + cmd.label + ')' : ''}`)
    processCommand(cmd)
  }

  function processCommand(cmd) {
    if (cmd.type === 'SAVE')    { saveToLayer(); return }
    if (cmd.type === 'UNKNOWN') { iface.mainWindow().displayToast("❓ No entendí: " + cmd.raw); return }

    root.polygonState = Builder.applyCommand(root.polygonState, cmd)
    miniCanvas.requestPaint()

    if (root.mapOverlay) root.mapOverlay.updateGeometry(root.polygonState)

    if (cmd.type === 'CLOSE') {
      const area = Builder.calculateArea(root.polygonState)
      iface.mainWindow().displayToast(`✅ Cerrado — Área: ${area.toFixed(2)} m²`)
    }
  }

  function saveToLayer() {
    if (root.polygonState.segments.length < 2) {
      iface.mainWindow().displayToast("⚠️ Necesitas al menos 2 segmentos")
      return
    }
    if (!root.polygonState.closed) processCommand({ type: 'CLOSE' })

    const layer = Router.findTargetLayer(root.surveyType)
    if (!layer) {
      const wkt = root.mapOverlay ? root.mapOverlay.buildRealWKT(root.polygonState) : ""
      if (wkt) platformUtilities.copyTextToClipboard(wkt)
      iface.mainWindow().displayToast("📋 WKT copiado — no se encontró capa '" + root.surveyType + "'")
      return
    }

    const wkt  = root.mapOverlay.buildRealWKT(root.polygonState)
    const crs  = CoordinateReferenceSystemUtils.fromDescription(root.originCrs)
    const geom = GeometryUtils.createGeometryFromWkt(wkt)

    const area      = Builder.calculateArea(root.polygonState)
    const perimeter = Builder.calculatePerimeter(root.polygonState)
    const attrs     = Router.defaultAttributes(root.surveyType, area, perimeter)

    // layer.startEditing() si no está en modo edición
    // FeatureUtils.createFeature(layer, geom, attrs)

    iface.mainWindow().displayToast(`✅ ${root.surveyType} guardado — ${area.toFixed(1)} m²`)
    root.polygonSaved()
    root.close()
  }

  component StatLabel: Column {
    property string label
    property var    value
    spacing: 1
    Text { text: parent.value; color: Theme.mainTextColor; font.pixelSize: 13; font.bold: true;
           anchors.horizontalCenter: parent.horizontalCenter }
    Text { text: parent.label; color: Theme.secondaryTextColor; font.pixelSize: 9;
           anchors.horizontalCenter: parent.horizontalCenter }
  }
}
