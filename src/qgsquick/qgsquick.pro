TEMPLATE = lib

include( ../../qfield.pri )
include( ../../qgis.pri )

#QT += widgets concurrent xml positioning printsupport svg sql opengl sensors quick quickcontrols2
QT += widgets xml positioning

SOURCES += mapsettings.cpp \
           qgsquickutils.cpp

HEADERS += mapsettings.h \
           qgsquickutils.h

