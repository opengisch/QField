.pragma library

const PATTERNS = {
  WALL:   /^(muro|pared|paramento|colindancia|lindero)\s+([\d.,]+)\s*(metros?|m|mt)?\s*(?:(?:al\s+|a\s+(?:la\s+)?)(derecha|izquierda|arriba|abajo|norte|sur|este|oeste))?(?:\s*a\s*([\d.,]+)\s*grados?)?/i,
  DOOR:   /^(puerta|entrada|acceso)\s*(doble|simple)?\s*([\d.,]+)\s*(metros?|m|mt)?/i,
  WINDOW: /^(ventana|abertura|vano)\s*([\d.,]+)\s*(metros?|m|mt)?/i,
  COLUMN: /^(columna|pilar|poste)\s*([\d.,]+)\s*(metros?|m|mt)?/i,
  TURN:   /^(girar|doblar|rotar|voltear|virar)\s*(?:a\s+(?:la\s+)?(derecha|izquierda))?\s*(?:([\d.,]+)\s*grados?)?/i,
  CLOSE:  /^(cerrar|terminar|finalizar|completar|listo)\s*(polígono|figura|shape|trazado|lindero|predio)?/i,
  UNDO:   /^(deshacer|borrar|eliminar|quitar)\s*(último|anterior)?/i,
  RESET:  /^(nueva\s+figura|reiniciar|limpiar|empezar\s+de\s+nuevo)/i,
  SAVE:   /^(guardar|salvar|exportar|grabar)/i,
}

const DIRECTION_ANGLES = {
  "derecha": 0,   "este": 0,
  "arriba": 90,   "norte": 90,
  "izquierda": 180, "oeste": 180,
  "abajo": 270,   "sur": 270,
}

function parse(text, currentAngle) {
  const clean = text.trim().toLowerCase()
    .replace(/\bpunto\b/g, '.').replace(/\bcoma\b/g, '.').replace(/\s+/g, ' ')

  if (PATTERNS.CLOSE.test(clean))  return { type: 'CLOSE' }
  if (PATTERNS.UNDO.test(clean))   return { type: 'UNDO' }
  if (PATTERNS.RESET.test(clean))  return { type: 'RESET' }
  if (PATTERNS.SAVE.test(clean))   return { type: 'SAVE'  }

  const turnMatch = clean.match(PATTERNS.TURN)
  if (turnMatch) {
    const direction = turnMatch[2] || 'derecha'
    const degrees   = parseFloat((turnMatch[3] || '90').replace(',', '.'))
    const sign      = direction === 'izquierda' ? 1 : -1
    return { type: 'TURN', angle: currentAngle + (sign * degrees), degrees, direction }
  }

  const wallMatch = clean.match(PATTERNS.WALL)
  if (wallMatch) {
    const length        = parseFloat((wallMatch[2] || '0').replace(',', '.'))
    const direction     = wallMatch[4] || null
    const explicitAngle = wallMatch[5] ? parseFloat(wallMatch[5].replace(',', '.')) : null
    const angle = explicitAngle !== null ? explicitAngle
                : direction ? DIRECTION_ANGLES[direction]
                : currentAngle
    return { type: 'WALL', element: 'wall', length, angle, label: `Muro ${length}m` }
  }

  const doorMatch = clean.match(PATTERNS.DOOR)
  if (doorMatch) {
    const isDouble = !!doorMatch[2]
    const length   = parseFloat((doorMatch[3] || '0.9').replace(',', '.'))
    return { type: 'WALL', element: 'door', length, angle: currentAngle,
             label: `Puerta${isDouble?' doble':''} ${length}m` }
  }

  const winMatch = clean.match(PATTERNS.WINDOW)
  if (winMatch) {
    const length = parseFloat((winMatch[2] || '0.8').replace(',', '.'))
    return { type: 'WALL', element: 'window', length, angle: currentAngle, label: `Ventana ${length}m` }
  }

  const colMatch = clean.match(PATTERNS.COLUMN)
  if (colMatch) {
    const size = parseFloat((colMatch[2] || '0.3').replace(',', '.'))
    return { type: 'WALL', element: 'column', length: size, angle: currentAngle, label: `Columna ${size}m` }
  }

  return { type: 'UNKNOWN', raw: clean }
}
