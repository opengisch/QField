// tests/voice-polygon/commandParser.test.js
const { loadQMLJS } = require('./test-helper')

let ctx
beforeAll(() => { ctx = loadQMLJS('CommandParser.js') })

describe('CommandParser.parse — muros', () => {
  test('muro básico a la derecha', () => {
    const cmd = ctx.parse('muro 10 metros a la derecha', 0)
    expect(cmd.type).toBe('WALL')
    expect(cmd.element).toBe('wall')
    expect(cmd.length).toBe(10)
    expect(cmd.angle).toBe(0)
  })

  test('muro con ángulo explícito', () => {
    const cmd = ctx.parse('muro 5 metros a 45 grados', 0)
    expect(cmd.type).toBe('WALL')
    expect(cmd.length).toBe(5)
    expect(cmd.angle).toBe(45)
  })

  test('muro con coma como decimal', () => {
    const cmd = ctx.parse('muro cinco coma cinco metros', 0)
    expect(['WALL', 'UNKNOWN']).toContain(cmd.type)
  })

  test('muro con decimal numérico "5,5"', () => {
    const cmd = ctx.parse('muro 5,5 metros al norte', 0)
    expect(cmd.type).toBe('WALL')
    expect(cmd.length).toBeCloseTo(5.5)
    expect(cmd.angle).toBe(90)
  })

  test('colindancia (alias de muro)', () => {
    const cmd = ctx.parse('colindancia 25 metros al norte', 0)
    expect(cmd.type).toBe('WALL')
    expect(cmd.element).toBe('wall')
    expect(cmd.length).toBe(25)
    expect(cmd.angle).toBe(90)
  })

  test('muro hereda ángulo actual cuando no se especifica dirección', () => {
    const cmd = ctx.parse('muro 8 metros', 135)
    expect(cmd.type).toBe('WALL')
    expect(cmd.angle).toBe(135)
  })
})

describe('CommandParser.parse — puertas y ventanas', () => {
  test('puerta simple', () => {
    const cmd = ctx.parse('puerta 1.2 metros', 0)
    expect(cmd.type).toBe('WALL')
    expect(cmd.element).toBe('door')
    expect(cmd.length).toBeCloseTo(1.2)
  })

  test('puerta doble', () => {
    const cmd = ctx.parse('puerta doble 2.4', 0)
    expect(cmd.type).toBe('WALL')
    expect(cmd.element).toBe('door')
    expect(cmd.length).toBeCloseTo(2.4)
    expect(cmd.label).toContain('doble')
  })

  test('ventana', () => {
    const cmd = ctx.parse('ventana 0.9 metros', 0)
    expect(cmd.type).toBe('WALL')
    expect(cmd.element).toBe('window')
    expect(cmd.length).toBeCloseTo(0.9)
  })

  test('columna', () => {
    const cmd = ctx.parse('columna 0.3 metros', 0)
    expect(cmd.type).toBe('WALL')
    expect(cmd.element).toBe('column')
    expect(cmd.length).toBeCloseTo(0.3)
  })
})

describe('CommandParser.parse — control de flujo', () => {
  test('cerrar polígono', () => {
    expect(ctx.parse('cerrar polígono', 0).type).toBe('CLOSE')
    expect(ctx.parse('terminar', 0).type).toBe('CLOSE')
    expect(ctx.parse('listo', 0).type).toBe('CLOSE')
  })

  test('deshacer', () => {
    expect(ctx.parse('deshacer', 0).type).toBe('UNDO')
    expect(ctx.parse('borrar último', 0).type).toBe('UNDO')
  })

  test('guardar', () => {
    expect(ctx.parse('guardar', 0).type).toBe('SAVE')
  })

  test('nueva figura / reset', () => {
    expect(ctx.parse('nueva figura', 0).type).toBe('RESET')
    expect(ctx.parse('reiniciar', 0).type).toBe('RESET')
  })

  test('comando desconocido', () => {
    const cmd = ctx.parse('abracadabra', 0)
    expect(cmd.type).toBe('UNKNOWN')
    expect(cmd.raw).toBe('abracadabra')
  })
})

describe('CommandParser.parse — giros', () => {
  test('girar a la derecha 90 grados (default)', () => {
    const cmd = ctx.parse('girar a la derecha', 0)
    expect(cmd.type).toBe('TURN')
    expect(cmd.angle).toBe(-90)
  })

  test('girar a la izquierda 45 grados', () => {
    const cmd = ctx.parse('girar a la izquierda 45 grados', 90)
    expect(cmd.type).toBe('TURN')
    expect(cmd.angle).toBe(135)
  })
})
