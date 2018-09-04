TEMPLATE = subdirs
CONFIG += c++11

TRANSLATIONS = \
  i18n/qfield_af.ts \
  i18n/qfield_ca.ts \
  i18n/qfield_de.ts \
  i18n/qfield_el.ts \
  i18n/qfield_en.ts \
  i18n/qfield_es.ts \
  i18n/qfield_et.ts \
  i18n/qfield_fa.ts \
  i18n/qfield_fi.ts \
  i18n/qfield_fr.ts \
  i18n/qfield_gl.ts \
  i18n/qfield_he.ts \
  i18n/qfield_hi.ts \
  i18n/qfield_hu.ts \
  i18n/qfield_it.ts \
  i18n/qfield_lt.ts \
  i18n/qfield_lv.ts \
  i18n/qfield_mn.ts \
  i18n/qfield_nl.ts \
  i18n/qfield_pl.ts \
  i18n/qfield_pt.ts \
  i18n/qfield_rm.ts \
  i18n/qfield_ro.ts \
  i18n/qfield_uk.ts \
  i18n/qfield_zh.ts

SUBDIRS += 3rdparty
SUBDIRS += src

src.depends=3rdparty
