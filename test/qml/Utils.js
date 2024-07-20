// where to put this js file?

function findChildren(parent, objName) {
  if (parent.objectName === objName) {
    return parent
  }

  if (parent.children === undefined) {
    return null
  }

  for (var i = 0; i < parent.children.length; ++i) {
    let children = findChildren(parent.children[i], objName)
    if (children !== null)
      return children
  }
  return null
}

function getMethods(obj) {
  var res = []
  for (var m in obj) {
    if (typeof obj[m] == "function") {
      res.push(m)
    }
  }
  return res
}
