// src/plugins/voice-polygon/MapPolygonOverlay.qml
import QtQuick
import org.qfield
import Theme

Item {
  id: root
  visible: false

  readonly property var elementColors: ({
    "wall":   "#2196F3",
    "door":   "#FF9800",
    "window": "#4CAF50",
    "column": "#9C27B0",
  })

  property var    polygonState: ({ vertices: [], segments: [], currentAngle: 0, closed: false })
  property var    originQgsPoint: null
  property string originCrs: ""

  property var geometryHighlighter: iface.findItemByObjectName('geometryHighlighter')

  Canvas {
    id: labelCanvas
    anchors.fill: parent

    onPaint: {
      const ctx = getContext("2d")
      ctx.clearRect(0, 0, width, height)
      drawLabels(ctx)
    }

    function drawLabels(ctx) {
      const segs        = root.polygonState.segments
      const mapSettings = iface.mapCanvas().mapSettings

      segs.forEach(seg => {
        if (!seg.fromReal || !seg.toReal) return

        const fromScreen = mapSettings.coordinateToScreen(
          GeometryUtils.point(seg.fromReal.x, seg.fromReal.y)
        )
        const toScreen = mapSettings.coordinateToScreen(
          GeometryUtils.point(seg.toReal.x, seg.toReal.y)
        )
        const mx    = (fromScreen.x + toScreen.x) / 2
        const my    = (fromScreen.y + toScreen.y) / 2
        const color = root.elementColors[seg.element] || root.elementColors["wall"]

        ctx.font = "bold 12px sans-serif"
        ctx.fillStyle = "rgba(0,0,0,0.6)"
        ctx.fillRect(mx - 20, my - 10, 40, 16)
        ctx.fillStyle = color
        ctx.textAlign = "center"
        ctx.textBaseline = "middle"
        ctx.fillText(seg.length ? seg.length.toFixed(2) + "m" : "", mx, my - 2)

        if (seg.element === 'door') {
          ctx.font = "14px sans-serif"
          ctx.fillText("🚪", fromScreen.x + (toScreen.x - fromScreen.x) * 0.3, my - 8)
        } else if (seg.element === 'window') {
          ctx.font = "12px sans-serif"
          ctx.fillText("⬜", fromScreen.x + (toScreen.x - fromScreen.x) * 0.3, my - 8)
        }
      })

      if (root.originQgsPoint) {
        const originScreen = mapSettings.coordinateToScreen(root.originQgsPoint)
        ctx.font = "20px sans-serif"
        ctx.fillText("📍", originScreen.x - 10, originScreen.y - 20)
      }
    }
  }

  function setOrigin(point, crs) {
    root.originQgsPoint = point
    root.originCrs = crs
  }

  function updateGeometry(state) {
    root.polygonState = state

    if (state.vertices.length < 2) {
      if (geometryHighlighter) geometryHighlighter.geometryWrapper.qgsGeometry = null
      return
    }

    const wkt = buildRealWKT(state)
    if (!wkt) return

    const crs  = CoordinateReferenceSystemUtils.fromDescription(root.originCrs)
    const geom = GeometryUtils.createGeometryFromWkt(wkt)

    if (geometryHighlighter) {
      geometryHighlighter.geometryWrapper.qgsGeometry = geom
      geometryHighlighter.geometryWrapper.crs = crs
    }
    labelCanvas.requestPaint()
  }

  function buildRealWKT(state) {
    if (!root.originQgsPoint || state.vertices.length < 2) return null

    const originX = root.originQgsPoint.x
    const originY = root.originQgsPoint.y
    const isGeographic     = root.originCrs.indexOf("EPSG:4") === 0
    const metersPerDegLat  = 111320
    const metersPerDegLng  = 111320 * Math.cos(originY * Math.PI / 180)

    const realVerts = state.vertices.map(v => {
      if (isGeographic) {
        return { x: originX + (v.x / metersPerDegLng), y: originY + (v.y / metersPerDegLat) }
      }
      return { x: originX + v.x, y: originY + v.y }
    })

    state.segments.forEach((seg, i) => {
      if (i < realVerts.length - 1) {
        seg.fromReal = realVerts[i]
        seg.toReal   = realVerts[i + 1]
      }
    })

    const coords = realVerts.map(v => `${v.x} ${v.y}`)
    if (coords[0] !== coords[coords.length - 1]) coords.push(coords[0])

    return state.closed
      ? `POLYGON((${coords.join(', ')}))`
      : `LINESTRING(${coords.join(', ')})`
  }

  function reset() {
    root.polygonState  = { vertices: [], segments: [], currentAngle: 0, closed: false }
    root.originQgsPoint = null
    if (geometryHighlighter) geometryHighlighter.geometryWrapper.qgsGeometry = null
    labelCanvas.requestPaint()
  }
}
