

TARGET = test
QT += testlib
TEMPLATE = app
CONFIG += qt

include( ../qfield.pri )
include( ../qgis.pri )

INCLUDEPATH += ../3rdparty/tessellate \
               ../src/qgsquick \
               ../src/core

DEPENDPATH += ../src/qgsquick ../src/core

LIBS += ../3rdparty/tessellate/libtessellate.a \
       -L$$OUT_PWD/../src/core -lcore \
       -L$$OUT_PWD/../src/qgsquick -lqgsquick


SOURCES += test_vertexmodel.cpp
