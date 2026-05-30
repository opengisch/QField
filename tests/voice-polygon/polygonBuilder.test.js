// tests/voice-polygon/polygonBuilder.test.js
const { loadQMLJS } = require('./test-helper')

let ctx
beforeAll(() => { ctx = loadQMLJS('PolygonBuilder.js') })

describe('PolygonBuilder.initialState', () => {
  test('devuelve estado inicial con vértice en origen', () => {
    const s = ctx.initialState()
    expect(s.vertices).toHaveLength(1)
    expect(s.vertices[0]).toEqual({ x: 0, y: 0 })
    expect(s.segments).toHaveLength(0)
    expect(s.currentAngle).toBe(0)
    expect(s.closed).toBe(false)
  })
})

describe('PolygonBuilder.applyCommand — WALL', () => {
  test('agrega vértice y segmento al aplicar WALL hacia la derecha (0°)', () => {
    let s = ctx.initialState()
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 10, angle: 0, label: 'Muro 10m' })
    expect(s.vertices).toHaveLength(2)
    expect(s.vertices[1].x).toBeCloseTo(10)
    expect(s.vertices[1].y).toBeCloseTo(0)
    expect(s.segments).toHaveLength(1)
    expect(s.segments[0].element).toBe('wall')
    expect(s.segments[0].length).toBe(10)
    expect(s.currentAngle).toBe(0)
  })

  test('muro hacia arriba (90°) desplaza en Y', () => {
    let s = ctx.initialState()
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 5, angle: 90 })
    expect(s.vertices[1].x).toBeCloseTo(0)
    expect(s.vertices[1].y).toBeCloseTo(5)
  })

  test('varios muros encadenados mantienen la ruta', () => {
    let s = ctx.initialState()
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 10, angle: 0 })
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 5, angle: 90 })
    expect(s.vertices).toHaveLength(3)
    expect(s.vertices[2].x).toBeCloseTo(10)
    expect(s.vertices[2].y).toBeCloseTo(5)
  })
})

describe('PolygonBuilder.applyCommand — TURN', () => {
  test('cambia currentAngle sin agregar vértice', () => {
    let s = ctx.initialState()
    s = ctx.applyCommand(s, { type: 'TURN', angle: 45 })
    expect(s.currentAngle).toBe(45)
    expect(s.vertices).toHaveLength(1)
    expect(s.segments).toHaveLength(0)
  })
})

describe('PolygonBuilder.applyCommand — CLOSE', () => {
  test('cierra el polígono agregando vértice de retorno al origen', () => {
    let s = ctx.initialState()
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 10, angle: 0 })
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 5, angle: 90 })
    s = ctx.applyCommand(s, { type: 'CLOSE' })
    expect(s.closed).toBe(true)
    const last = s.vertices[s.vertices.length - 1]
    expect(last.x).toBeCloseTo(0)
    expect(last.y).toBeCloseTo(0)
  })

  test('no duplica el vértice si ya está cerrado', () => {
    let s = ctx.initialState()
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 10, angle: 0 })
    const vertsBefore = s.vertices.length
    s = ctx.applyCommand(s, { type: 'CLOSE' })
    expect(s.vertices.length).toBe(vertsBefore + 1)
  })
})

describe('PolygonBuilder.applyCommand — UNDO', () => {
  test('elimina el último segmento y vértice', () => {
    let s = ctx.initialState()
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 10, angle: 0 })
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 5, angle: 90 })
    s = ctx.applyCommand(s, { type: 'UNDO' })
    expect(s.vertices).toHaveLength(2)
    expect(s.segments).toHaveLength(1)
  })

  test('UNDO sin segmentos no falla', () => {
    let s = ctx.initialState()
    expect(() => ctx.applyCommand(s, { type: 'UNDO' })).not.toThrow()
  })
})

describe('PolygonBuilder.applyCommand — RESET', () => {
  test('RESET devuelve estado inicial', () => {
    let s = ctx.initialState()
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 10, angle: 0 })
    s = ctx.applyCommand(s, { type: 'RESET' })
    expect(s.vertices).toHaveLength(1)
    expect(s.segments).toHaveLength(0)
  })
})

describe('PolygonBuilder.calculateArea', () => {
  test('cuadrado 10x10 = 100 m²', () => {
    let s = ctx.initialState()
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 10, angle: 0 })
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 10, angle: 90 })
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 10, angle: 180 })
    s = ctx.applyCommand(s, { type: 'CLOSE' })
    expect(ctx.calculateArea(s)).toBeCloseTo(100)
  })

  test('estado vacío devuelve 0', () => {
    expect(ctx.calculateArea(ctx.initialState())).toBe(0)
  })
})

describe('PolygonBuilder.calculatePerimeter', () => {
  test('suma las longitudes de los segmentos', () => {
    let s = ctx.initialState()
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 10, angle: 0 })
    s = ctx.applyCommand(s, { type: 'WALL', element: 'wall', length: 5, angle: 90 })
    expect(ctx.calculatePerimeter(s)).toBeCloseTo(15)
  })
})
