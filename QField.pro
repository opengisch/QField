TEMPLATE = subdirs
CONFIG += c++11

TRANSLATIONS = \
  i18n/qfield_ca.ts \
  i18n/qfield_de.ts \
  i18n/qfield_en.ts \
  i18n/qfield_es.ts \
  i18n/qfield_fi.ts \
  i18n/qfield_fr.ts \
  i18n/qfield_gl.ts \
  i18n/qfield_hu.ts \
  i18n/qfield_it.ts \
  i18n/qfield_lt.ts \
  i18n/qfield_mn.ts \
  i18n/qfield_nl.ts \
  i18n/qfield_pl.ts \
  i18n/qfield_pt.ts \
  i18n/qfield_ro.ts \
  i18n/qfield_uk.ts \
  i18n/qfield_zh.ts

SUBDIRS += 3rdparty
SUBDIRS += src

src.depends=3rdparty
