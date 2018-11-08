TEMPLATE = lib
TARGET = qgsquick

include( ../../qfield.pri )
include( ../../qgis.pri )
include( ../../version.pri )

#QT += widgets concurrent xml positioning printsupport svg sql opengl sensors quick quickcontrols2
QT += widgets xml positioning quick printsupport

SOURCES += \
    qgsquickmapcanvasmap.cpp \
    qgsquickmaptransform.cpp \
    qgsquickutils.cpp \
    qgsquickmapsettings.cpp \
    qgsquickcoordinatetransformer.cpp \
    qgsquickfeaturelayerpair.cpp

HEADERS += \
    qgsquickmapcanvasmap.h \
    qgsquickmaptransform.h \
    qgsquickutils.h \
    qgsquickmapsettings.h \
    qgsquickcoordinatetransformer.h \
    qgsquickfeaturelayerpair.h



