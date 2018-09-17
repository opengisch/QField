TEMPLATE = subdirs
CONFIG += c++11
CONFIG += qt

TRANSLATIONS = $$PWD/i18n/*.ts

QMAKE_EXTRA_COMPILERS += lrelease
lrelease.input         = TRANSLATIONS
lrelease.output        = ${QMAKE_FILE_BASE}.qm
lrelease.commands      = $$[QT_INSTALL_BINS]/lrelease ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_BASE}.qm
lrelease.CONFIG       += no_link target_predeps

SUBDIRS += 3rdparty qgsquick core app

qgsquick.subdir = src/qgsquick
core.subdir = src/core
app.subdir = src/app

core.depends = qgsquick

app.depends = 3rdparty
app.depends = qgsquick
app.depends = core

CONFIG += ordered

# Include the tests and plugins subprojects only on debug mode
CONFIG(debug, debug|release): {
  SUBDIRS += test
  test.depends = qgsquick
  test.depends = core
}

