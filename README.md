[![Read the documentation](https://img.shields.io/badge/Read-the%20docs-green.svg)](https://opengisch.github.io/QField-docs/installation-guide/index.html)
[![Join the chat at https://gitter.im/opengisch/QField](https://badges.gitter.im/opengisch/QField.svg)](https://gitter.im/opengisch/QField?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Release](https://img.shields.io/github/release/opengisch/QField.svg)](https://github.com/opengisch/QField/releases)

# QField for QGIS

A simplified touch optimized interface for QGIS.

Perfect for field work on portable touch devices.

## Get it

Check the [documentation](https://opengisch.github.io/QField-docs/installation-guide/index.html)

[![Get in Google play](https://opengisch.github.io/QField-docs/_images/Get_it_on_Google_play.png)](https://play.google.com/store/apps/details?id=ch.opengis.qfield)

## Contribute

QField is an open source project, licensed under the terms of the GPLv2 or later. This means that it is free to use and modify and will stay like that.

We are very happy if this app helps you to get your job done or in whatever creative way you may use it.

If you found it useful, we will be even happier if you could give us something back. A couple of things you can do are

 * Rate the app [★★★★★](https://play.google.com/store/apps/details?id=ch.opengis.qfield&hl=en#details-reviews)
 * [Tweet about it](https://twitter.com/share?text=Looking for a good tool for field work in GIS? Check out %23QField!)
 * Write about your experience (please [let us know](mailto:info@opengis.ch)!)
 * [Help with the documentation](https://github.com/opengisch/QField-docs#documentation-process)
 * [Translate the documentation](https://github.com/opengisch/QField-docs#translation-process) or [the app](https://www.transifex.com/opengisch/qfield-for-qgis/)
 * [Sponsor a feature](https://opengisch.github.io/QField-docs/development/index.html#make-it-grow-make-it-yours)
 * And just drop by to say thank you or have a beer with us next time you meet OPENGIS.ch at a conference

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
