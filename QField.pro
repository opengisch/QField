TEMPLATE = subdirs
CONFIG += c++11
CONFIG += qt

TRANSLATIONS = $$files(i18n/*.ts)

SUBDIRS += 3rdparty qgsquick core app

qgsquick.subdir = src/qgsquick
core.subdir = src/core
app.subdir = src/app

core.depends = qgsquick

app.depends = 3rdparty
app.depends = qgsquick
app.depends = core

CONFIG += ordered

QML_IMPORT_PATH += src/qml/imports
