.pragma library

const LAYER_NAMES = {
  "construccion": ["construcciones", "construccion", "buildings", "edificaciones"],
  "unidad":       ["unidades", "unidad_construccion", "units", "locales"],
  "terreno":      ["predios", "terreno", "linderos", "parcelas", "lotes"],
}

function findTargetLayer(surveyType) {
  const candidateNames = LAYER_NAMES[surveyType] || []
  const layers = qgisProject.mapLayers()

  for (const id in layers) {
    const layer = layers[id]
    if (layer.type() !== 0 || layer.geometryType() !== 2) continue
    const layerName = layer.name().toLowerCase()
    if (candidateNames.some(name => layerName.includes(name))) return layer
  }

  for (const id in layers) {
    const layer = layers[id]
    if (layer.type() === 0 && layer.geometryType() === 2 && layer.isEditable()) return layer
  }

  return null
}

function defaultAttributes(surveyType, area, perimeter) {
  const now = new Date().toISOString()
  const base = {
    area_m2: Math.round(area * 100) / 100,
    perimetro_m: Math.round(perimeter * 100) / 100,
    fecha_captura: now,
    metodo: "voz",
  }
  switch (surveyType) {
    case "construccion": return { ...base, uso: "", niveles: 1, material: "" }
    case "unidad":       return { ...base, numero_unidad: "", nivel: 1 }
    case "terreno":      return { ...base, clave_catastral: "", colindancias: "" }
    default:             return base
  }
}
