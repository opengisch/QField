set(QFIELD_GUI_SRCS
    attributeformmodel.cpp
    attributeformmodelbase.cpp
    barcodeimageprovider.cpp
    drawingtemplatemodel.cpp
    expressionevaluator.cpp
    expressionvariablemodel.cpp
    featurechecklistmodel.cpp
    focusstack.cpp
    geometryeditorsmodel.cpp
    legendimageprovider.cpp
    localfilesimageprovider.cpp
    localfilesmodel.cpp
    messagelogmodel.cpp
    orderedrelationmodel.cpp
    parameterizedimage.cpp
    printlayoutlistmodel.cpp
    projectsimageprovider.cpp
    recentprojectlistmodel.cpp
    referencingfeaturelistmodel.cpp
    sensorlistmodel.cpp
    submodel.cpp
    theme.cpp
    valuemapmodel.cpp
    valuemapmodelbase.cpp)

set(QFIELD_GUI_HDRS
    attributeformmodel.h
    attributeformmodelbase.h
    barcodeimageprovider.h
    drawingtemplatemodel.h
    expressionevaluator.h
    expressionvariablemodel.h
    featurechecklistmodel.h
    focusstack.h
    geometryeditorsmodel.h
    legendimageprovider.h
    localfilesimageprovider.h
    localfilesmodel.h
    messagelogmodel.h
    orderedrelationmodel.h
    parameterizedimage.h
    printlayoutlistmodel.h
    projectsimageprovider.h
    recentprojectlistmodel.h
    referencingfeaturelistmodel.h
    sensorlistmodel.h
    submodel.h
    theme.h
    valuemapmodel.h
    valuemapmodelbase.h)

add_library(qfield_gui STATIC ${QFIELD_GUI_SRCS} ${QFIELD_GUI_HDRS})

include(GenerateExportHeader)
generate_export_header(qfield_gui)

target_include_directories(qfield_gui SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(qfield_gui PUBLIC ${CMAKE_SOURCE_DIR}/src/gui)

target_compile_features(qfield_gui PUBLIC cxx_std_20)

set_target_properties(qfield_gui PROPERTIES AUTOMOC TRUE)

target_link_libraries(qfield_gui PUBLIC qfield_core)
