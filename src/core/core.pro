TEMPLATE = lib
TARGET = core

QT += widgets concurrent xml positioning printsupport svg sql opengl sensors quick quickcontrols2 qml
# MISSING in apk: printsupport svg sql opengl sensors

android {
    QT += androidextras
    HEADERS += androidplatformutilities.h \
               androidpicturesource.h \
               androidprojectsource.h \
               androidviewstatus.h
    SOURCES += androidplatformutilities.cpp \
               androidpicturesource.cpp \
               androidprojectsource.cpp \
               androidviewstatus.cpp
}

include( ../../qfield.pri )
include( ../../qgis.pri )
include( ../../version.pri )

HEADERS += \
    appinterface.h \
    featurelistextentcontroller.h \
    geometryeditorsmodel.h \
    multifeaturelistmodel.h \
    featurelistmodelselection.h \
    featuremodel.h \
    qgismobileapp.h \
    qgsgpkgflusher.h \
    qgssggeometry.h \
    sgrubberband.h \
    settings.h \
    platformutilities.h \
    geometry.h \
    modelhelper.h \
    rubberbandmodel.h \
    rubberband.h \
    picturesource.h \
    messagelogmodel.h \
    attributeformmodelbase.h \
    layertreemapcanvasbridge.h \
    identifytool.h \
    submodel.h \
    attributeformmodel.h \
    expressionvariablemodel.h \
    badlayerhandler.h \
    snappingutils.h \
    snappingresult.h \
    layertreemodel.h \
    legendimageprovider.h \
    featurelistmodel.h \
    distancearea.h \
    expressioncontextutils.h \
    focusstack.h \
    printlayoutlistmodel.h \
    vertexmodel.h \
    maptoscreen.h \
    projectsource.h \
    featureslocatorfilter.h \
    locatormodelsuperbridge.h \
    linepolygonhighlight.h \
    qgsgeometrywrapper.h \
    valuemapmodel.h \
    recentprojectlistmodel.h \
    referencingfeaturelistmodel.h \
    featurechecklistmodel.h \
    qfieldappauthrequesthandler.h \
    tracker.h \
    trackingmodel.h \
    viewstatus.h \
    utils/featureutils.h \
    utils/geometryutils.h \
    utils/fileutils.h \
    expressionutils.h

SOURCES += \
    appinterface.cpp \
    featurelistextentcontroller.cpp \
    geometryeditorsmodel.cpp \
    multifeaturelistmodel.cpp \
    featurelistmodelselection.cpp \
    featuremodel.cpp \
    qgismobileapp.cpp \
    qgsgpkgflusher.cpp \
    qgssggeometry.cpp \
    sgrubberband.cpp \
    settings.cpp \
    platformutilities.cpp \
    geometry.cpp \
    modelhelper.cpp \
    rubberbandmodel.cpp \
    rubberband.cpp \
    picturesource.cpp \
    messagelogmodel.cpp \
    attributeformmodelbase.cpp \
    layertreemapcanvasbridge.cpp \
    identifytool.cpp \
    submodel.cpp \
    attributeformmodel.cpp \
    expressionvariablemodel.cpp \
    badlayerhandler.cpp \
    snappingutils.cpp \
    snappingresult.cpp \
    layertreemodel.cpp \
    legendimageprovider.cpp \
    featurelistmodel.cpp \
    distancearea.cpp \
    expressioncontextutils.cpp \
    focusstack.cpp \
    printlayoutlistmodel.cpp \
    vertexmodel.cpp \
    maptoscreen.cpp \
    projectsource.cpp \
    featureslocatorfilter.cpp \
    locatormodelsuperbridge.cpp \
    linepolygonhighlight.cpp \
    qgsgeometrywrapper.cpp \
    valuemapmodel.cpp \
    recentprojectlistmodel.cpp \
    referencingfeaturelistmodel.cpp \
    featurechecklistmodel.cpp \
    qfieldappauthrequesthandler.cpp \
    tracker.cpp \
    trackingmodel.cpp \
    viewstatus.cpp \
    utils/featureutils.cpp \
    utils/geometryutils.cpp \
    utils/fileutils.cpp \
    expressionutils.cpp

INCLUDEPATH += ../../3rdparty/tessellate \
               ../qgsquick \
               utils

DEPENDPATH += ../qgsquick

LIBS += ../../3rdparty/tessellate/libtessellate.a \
       -L$$OUT_PWD/../qgsquick -lqgsquick



