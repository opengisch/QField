.pragma library

function nextPoint(origin, angle, distance) {
  const rad = angle * Math.PI / 180
  return { x: origin.x + distance * Math.cos(rad), y: origin.y + distance * Math.sin(rad) }
}

function applyCommand(state, command) {
  const s = JSON.parse(JSON.stringify(state))

  switch (command.type) {
    case 'WALL': {
      const last = s.vertices[s.vertices.length - 1]
      const next = nextPoint(last, command.angle, command.length)
      s.currentAngle = command.angle
      s.vertices.push(next)
      s.segments.push({ from: last, to: next, element: command.element,
                        length: command.length, label: command.label, angle: command.angle })
      break
    }
    case 'TURN':  { s.currentAngle = command.angle; break }
    case 'CLOSE': {
      const first = s.vertices[0]
      const last2 = s.vertices[s.vertices.length - 1]
      if (last2.x !== first.x || last2.y !== first.y) {
        s.segments.push({ from: last2, to: { ...first }, element: 'wall', label: 'Cierre' })
        s.vertices.push({ ...first })
      }
      s.closed = true
      break
    }
    case 'UNDO': {
      if (s.segments.length > 0) {
        s.segments.pop()
        s.vertices.pop()
        s.currentAngle = s.segments.length > 0 ? (s.segments[s.segments.length-1].angle || 0) : 0
      }
      break
    }
    case 'RESET': return initialState()
  }
  return s
}

function initialState() {
  return { vertices: [{ x: 0, y: 0 }], segments: [], currentAngle: 0, closed: false }
}

function calculateArea(state) {
  const v = state.vertices
  if (v.length < 3) return 0
  let area = 0
  for (let i = 0; i < v.length - 1; i++) {
    area += v[i].x * v[i+1].y - v[i+1].x * v[i].y
  }
  return Math.abs(area) / 2
}

function calculatePerimeter(state) {
  return state.segments.reduce((sum, seg) => sum + (seg.length || 0), 0)
}
