[![Release](https://img.shields.io/github/release/opengisch/QField.svg)](https://github.com/opengisch/QField/releases)

# QField for QGIS

[![Join the chat at https://gitter.im/opengisch/QField](https://badges.gitter.im/opengisch/QField.svg)](https://gitter.im/opengisch/QField?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

A simplified touch optimized interface for QGIS.

Perfect for field work on portable touch devices.

## Get it

Check the [documentation](https://opengisch.github.io/QField-docs/installation-guide/index.html)

## Build

### For Android

Build [OSGeo4A](https://github.com/opengisch/OSGeo4A)

### For Desktop

* Install Qt 5.6 dev libraries
* Build QGIS using Qt5

### For both

```sh
cd QField
cp config.pri.default config.pri
git submodule init
git submodule update
```

 * adjust the paths in config.pri
 * open QField.pro with QtCreator (installed during OSGeo4A installation)
 * hit build
