// src/plugins/voice-polygon/PolygonCanvas.qml
// Mini-canvas 2D de esquema interno. Dibuja el polígono en construcción
// con colores por tipo de elemento y auto-escala al bounding box.
import QtQuick
import Theme

Canvas {
  id: root

  property var    polygonState: ({ vertices: [], segments: [], currentAngle: 0, closed: false })
  property string surveyType: ""

  readonly property var elementColors: ({
    "wall":   "#2196F3",
    "door":   "#FF9800",
    "window": "#4CAF50",
    "column": "#9C27B0",
  })
  readonly property var typeColors: ({
    "construccion": "#1565C0",
    "unidad":       "#2E7D32",
    "terreno":      "#E65100",
  })

  background: Rectangle { color: "#0D1117"; radius: 6 }

  onPaint: {
    const ctx = getContext("2d")
    ctx.clearRect(0, 0, width, height)

    const verts = polygonState.vertices
    if (verts.length < 2) {
      ctx.fillStyle = "#555"
      ctx.font = "11px sans-serif"
      ctx.textAlign = "center"
      ctx.textBaseline = "middle"
      ctx.fillText("Habla para trazar el polígono", width / 2, height / 2)
      return
    }

    let minX = Infinity, maxX = -Infinity, minY = Infinity, maxY = -Infinity
    verts.forEach(v => {
      minX = Math.min(minX, v.x); maxX = Math.max(maxX, v.x)
      minY = Math.min(minY, v.y); maxY = Math.max(maxY, v.y)
    })

    const pad = 18
    const rangeX = maxX - minX || 1
    const rangeY = maxY - minY || 1
    const scale  = Math.min((width - 2*pad) / rangeX, (height - 2*pad) / rangeY)

    const offX = pad + (width  - 2*pad - rangeX*scale) / 2 - minX*scale
    const offY = pad + (height - 2*pad - rangeY*scale) / 2 - minY*scale

    function toScreen(v) { return { x: offX + v.x*scale, y: height - (offY + v.y*scale) } }

    polygonState.segments.forEach(seg => {
      const color = elementColors[seg.element] || elementColors["wall"]
      const from  = toScreen(seg.from)
      const to    = toScreen(seg.to)

      ctx.beginPath()
      ctx.moveTo(from.x, from.y)
      if (seg.element === 'door' || seg.element === 'window') {
        ctx.setLineDash([5, 3])
        ctx.lineWidth = 1.5
      } else {
        ctx.setLineDash([])
        ctx.lineWidth = 2.5
      }
      ctx.lineTo(to.x, to.y)
      ctx.strokeStyle = color
      ctx.stroke()

      if (seg.length) {
        const mx = (from.x + to.x) / 2
        const my = (from.y + to.y) / 2
        ctx.setLineDash([])
        ctx.font = "bold 9px sans-serif"
        ctx.fillStyle = color
        ctx.textAlign = "center"
        ctx.textBaseline = "middle"
        ctx.fillText(seg.length.toFixed(1) + "m", mx, my - 6)
      }
    })
    ctx.setLineDash([])

    const accent = typeColors[root.surveyType] || "#2196F3"
    verts.forEach((v, i) => {
      const s = toScreen(v)
      ctx.beginPath()
      ctx.arc(s.x, s.y, i === 0 ? 5 : 3, 0, 2*Math.PI)
      ctx.fillStyle = i === 0 ? "#FF5252" : accent
      ctx.fill()
    })
  }
}
