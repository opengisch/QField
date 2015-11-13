QField for QGIS
===============

A simplified touch optimized interface for QGIS.

Perfect for field work on portable touch devices.


Build
-----
First build [OSGeo4A](https://github.com/opengisch/OSGeo4A) then call:

```sh
cd QField
cp config.pri.default config.pri
git submodule init
git submodule update
```

and add the right paths to config.pri
open QField.pro with QtCreator installed during OSGeo4A installation
hit build 
