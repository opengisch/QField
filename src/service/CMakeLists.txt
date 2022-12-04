set(QFIELD_SERVICE_SRCS qfieldservice.cpp)
set(QFIELD_SERVICE_HDRS qfieldservice.h)

add_library(qfield_service STATIC ${QFIELD_SERVICE_SRCS} ${QFIELD_SERVICE_HDRS})

include(GenerateExportHeader)
generate_export_header(qfield_service)

target_include_directories(qfield_service SYSTEM
                           PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(qfield_service
                           PUBLIC ${CMAKE_SOURCE_DIR}/src/service)

target_compile_features(qfield_service PUBLIC cxx_std_17)
set_target_properties(qfield_service PROPERTIES AUTOMOC TRUE)

target_link_libraries(qfield_service PRIVATE qfield_core ${QGIS_CORE_LIBRARY})

target_link_libraries(qfield_service PUBLIC ${QT_PKG}::Core
                                            ${QT_PKG}::Positioning)

if(ANDROID)
  target_link_libraries(qfield_service PUBLIC ${QT_PKG}::AndroidExtras)
endif()

install(FILES ${QFIELD_SERVICE_HDRS} DESTINATION ${QFIELD_INCLUDE_DIR})
install(
  TARGETS qfield_service
  BUNDLE DESTINATION ${QFIELD_BIN_DIR}
  RUNTIME DESTINATION ${QFIELD_BIN_DIR}
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR}
  FRAMEWORK DESTINATION ${QFIELD_FW_SUBDIR}
  PUBLIC_HEADER DESTINATION ${QFIELD_INCLUDE_DIR})
