[![Read the documentation](https://img.shields.io/badge/Read-the%20docs-green.svg)](http://qfield.org/docs/installation-guide/index.html)
[![Join the chat at https://gitter.im/opengisch/QField](https://badges.gitter.im/opengisch/QField.svg)](https://gitter.im/opengisch/QField?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)
[![Release](https://img.shields.io/github/release/opengisch/QField.svg)](https://github.com/opengisch/QField/releases)

# QField for QGIS

A simplified touch optimized interface for QGIS.

Perfect for field work on portable touch devices.

## Get it

Check the [documentation](http://qfield.org/docs)

[![Get in Google play](http://www.qfield.org/images/Get_it_on_Google_play.png)](https://play.google.com/store/apps/details?id=ch.opengis.qfield)

## Contribute

QField is an open source project, licensed under the terms of the GPLv2 or later. This means that it is free to use and modify and will stay like that.

We are very happy if this app helps you to get your job done or in whatever creative way you may use it.

If you found it useful, we will be even happier if you could give something back. A couple of things you can do are

 * Rate the app [★★★★★](https://play.google.com/store/apps/details?id=ch.opengis.qfield&hl=en#details-reviews)
 * [Tweet about it](https://twitter.com/share?text=Looking for a good tool for field work in GIS? Check out %23QField!)
 * Write about your experience (please [let us know](mailto:info@opengis.ch)!)
 * [Help with the documentation](https://github.com/opengisch/QField-docs#documentation-process)
 * [Translate the documentation](https://github.com/opengisch/QField-docs#translation-process) or [the app](https://www.transifex.com/opengisch/qfield-for-qgis/)
 * [Sponsor a feature](https://opengisch.github.io/QField-docs/development/index.html#make-it-grow-make-it-yours)
 * And just drop by to say thank you or have a beer with us next time you meet OPENGIS.ch at a conference

## Build

### For Android

#### Quick and dirty

Use the dockerized QField SDK.

```
# get the latest tag from https://hub.docker.com/r/opengisch/qfield-sdk/tags/
TAG=20180821
cd QField

git submodule init
git submodule update

docker run -v $(pwd):/usr/src/qfield docker.io/opengisch/qfield-sdk:$TAG /usr/src/qfield/scripts/docker-build.sh --rm
```

This will put the apk into a subfolder `build-docker/out/build/outputs/apk`

#### Go the hard way

Build [OSGeo4A](https://github.com/opengisch/OSGeo4A)

```sh
cd QField

git submodule init
git submodule update

cp config.pri.default config.pri
# Edit config.pri
```

The advantage of this is, you will be able to build and deploy directly from QtCreator.

### For Desktop

* Get QGIS 2.99 (3.0) development libraries.

```sh
cd QField
git submodule init
git submodule update

cp config.pri.default config.pri
# Edit config.pri
```

 * open QField.pro with QtCreator (installed during OSGeo4A installation)
 * hit build

 #### On Mac

Building on a Mac requires some adjustments:

1. In `Qt Creator` > `Projects` > `Run` >
* Check `Use debug version of frameworks`
* In Run Environment, add QGIS frameworks by editing following variables (Homebrew path might require adjustments):
  * `DYLD_FRAMEWORK_PATH` add `:/usr/local/Cellar/qgis3-dev/2.99/QGIS.app/Contents/Frameworks`
  * `DYLD_LIBRARY_PATH` add `:/usr/local/Cellar/qgis3-dev/2.99/QGIS.app/Contents/Frameworks/qgis_core.framework/Versions/Current`

2. If you get the error `invalid version number in '-mmacosx-version-min='`, you might need to modify some Qt file. In `/usr/local/Cellar/qt/5.10.1/mkspecs/features/mac/default_post.prf`, hardcode the minimum deployment target by setting `version_min_flag = -m$${version_identifier}-version-min=10.10`
Or by running `gsed -i "s/version_min_flag = -m\$\${version_identifier}-version-min=\$\$deployment_target/version_min_flag = -m\$\${version_identifier}-version-min=10\.10/" /usr/local/Cellar/qt/5.10.1/mkspecs/features/mac/default_post.prf`
