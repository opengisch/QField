// test-helper.js
const fs = require('fs')
const path = require('path')
const vm = require('vm')

const PLUGIN_DIR = path.join(__dirname, '../../src/plugins/voice-polygon')

function loadQMLJS(filename) {
  const content = fs
    .readFileSync(path.join(PLUGIN_DIR, filename), 'utf8')
    .replace(/\.pragma\s+library\s*/g, '')
  const context = vm.createContext({})
  vm.runInContext(content, context)
  return context
}

module.exports = { loadQMLJS }
