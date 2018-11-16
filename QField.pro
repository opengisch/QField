TEMPLATE = subdirs
CONFIG += c++11
CONFIG += qt

TRANSLATIONS = $$PWD/i18n/qfield_en.ts /
$$PWD/18n/qfield_no.ts /
$$PWD/i18n/qfield_hr.ts /
$$PWD/i18n/qfield_fa.ts /
$$PWD/i18n/qfield_hu.ts /
$$PWD/i18n/qfield_pt.ts /
$$PWD/i18n/qfield_rm.ts /
$$PWD/i18n/qfield_el.ts /
$$PWD/i18n/qfield_it.ts /
$$PWD/i18n/qfield_ca.ts /
$$PWD/i18n/qfield_nb_NO.ts /
$$PWD/i18n/qfield_cs.ts /
$$PWD/i18n/qfield_ko.ts /
$$PWD/i18n/qfield_ro.ts /
$$PWD/i18n/qfield_nl.ts /
$$PWD/i18n/qfield_es.ts /
$$PWD/i18n/qfield_lt.ts /
$$PWD/i18n/qfield_de.ts /
$$PWD/i18n/qfield_gl.ts /
$$PWD/i18n/qfield_fi.ts /
$$PWD/i18n/qfield_zh.ts /
$$PWD/i18n/qfield_uk.ts /
$$PWD/i18n/qfield_sv.ts /
$$PWD/i18n/qfield_af.ts /
$$PWD/i18n/qfield_lv.ts /
$$PWD/i18n/qfield_et.ts /
$$PWD/i18n/qfield_he.ts /
$$PWD/i18n/qfield_tr_TR.ts /
$$PWD/i18n/qfield_fr.ts /
$$PWD/i18n/qfield_hi.ts /
$$PWD/i18n/qfield_mn.ts /
$$PWD/i18n/qfield_pl.ts


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

