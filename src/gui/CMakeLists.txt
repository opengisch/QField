set(QFIELD_GUI_SRCS localfilesimageprovider.cpp localfilesmodel.cpp
                    projectsimageprovider.cpp recentprojectlistmodel.cpp)

set(QFIELD_GUI_HDRS localfilesimageprovider.h localfilesmodel.h
                    projectsimageprovider.h recentprojectlistmodel.h)

add_library(qfield_gui STATIC ${QFIELD_GUI_SRCS} ${QFIELD_GUI_HDRS})

include(GenerateExportHeader)
generate_export_header(qfield_gui)

target_include_directories(qfield_gui SYSTEM PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(qfield_gui PUBLIC ${CMAKE_SOURCE_DIR}/src/gui)

target_compile_features(qfield_gui PUBLIC cxx_std_20)

set_target_properties(qfield_gui PROPERTIES AUTOMOC TRUE)

target_link_libraries(qfield_gui PUBLIC qfield_core)
