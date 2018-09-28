TEMPLATE = lib
TARGET = qgsquick

include( ../../qfield.pri )
include( ../../qgis.pri )
include( ../../version.pri )

#QT += widgets concurrent xml positioning printsupport svg sql opengl sensors quick quickcontrols2
QT += widgets xml positioning quick printsupport

SOURCES += \
  qgsquickmapcanvasmap.cpp \
  qgsquickutils.cpp \
  qgsquickmapsettings.cpp

HEADERS += \
  qgsquickmapcanvasmap.h \
  qgsquickutils.h \
  qgsquickmapsettings.h



