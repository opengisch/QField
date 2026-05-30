// tests/voice-polygon/layerRouter.test.js
const vm = require('vm')
const fs = require('fs')
const path = require('path')

function loadWithMockProject(layers) {
  const content = fs
    .readFileSync(path.join(__dirname, '../../src/plugins/voice-polygon/LayerRouter.js'), 'utf8')
    .replace(/\.pragma\s+library\s*/g, '')
  const context = vm.createContext({
    qgisProject: { mapLayers: () => layers }
  })
  vm.runInContext(content, context)
  return context
}

function makeLayer(name, type, geometryType, editable = true) {
  return { name: () => name, type: () => type, geometryType: () => geometryType, isEditable: () => editable }
}

describe('LayerRouter.findTargetLayer', () => {
  test('encuentra capa "construcciones" para surveyType "construccion"', () => {
    const layers = { 'id1': makeLayer('construcciones', 0, 2), 'id2': makeLayer('predios', 0, 2) }
    const ctx = loadWithMockProject(layers)
    const layer = ctx.findTargetLayer('construccion')
    expect(layer).not.toBeNull()
    expect(layer.name().toLowerCase()).toContain('construcci')
  })

  test('encuentra capa "predios" para surveyType "terreno"', () => {
    const layers = { 'id1': makeLayer('predios', 0, 2) }
    const ctx = loadWithMockProject(layers)
    const layer = ctx.findTargetLayer('terreno')
    expect(layer).not.toBeNull()
    expect(layer.name().toLowerCase()).toContain('predio')
  })

  test('retorna null si no hay capas de polígono', () => {
    const ctx = loadWithMockProject({})
    expect(ctx.findTargetLayer('construccion')).toBeNull()
  })

  test('fallback a cualquier capa polígono editable si no coincide nombre', () => {
    const layers = { 'id1': makeLayer('mi_capa_rara', 0, 2, true) }
    const ctx = loadWithMockProject(layers)
    const layer = ctx.findTargetLayer('construccion')
    expect(layer).not.toBeNull()
  })
})

describe('LayerRouter.defaultAttributes', () => {
  let ctx
  beforeAll(() => { ctx = loadWithMockProject({}) })

  test('construccion incluye uso, niveles, material', () => {
    const attrs = ctx.defaultAttributes('construccion', 100.5, 40.2)
    expect(attrs.area_m2).toBe(100.5)
    expect(attrs.perimetro_m).toBe(40.2)
    expect(attrs.metodo).toBe('voz')
    expect(attrs).toHaveProperty('uso')
    expect(attrs).toHaveProperty('niveles')
    expect(attrs).toHaveProperty('material')
  })

  test('terreno incluye clave_catastral y colindancias', () => {
    const attrs = ctx.defaultAttributes('terreno', 500, 90)
    expect(attrs).toHaveProperty('clave_catastral')
    expect(attrs).toHaveProperty('colindancias')
  })

  test('unidad incluye numero_unidad y nivel', () => {
    const attrs = ctx.defaultAttributes('unidad', 50, 28)
    expect(attrs).toHaveProperty('numero_unidad')
    expect(attrs).toHaveProperty('nivel')
  })
})
