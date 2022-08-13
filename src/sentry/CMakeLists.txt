if(APPLE)
  list(APPEND QFIELD_SOURCES sentry_cocoa.mm)
else()
  list(APPEND QFIELD_SOURCES sentry_classic.cpp)
endif()

configure_file(sentry_config.h.in sentry_config.h @ONLY)

add_library(qfield_sentry ${QFIELD_SOURCES})

if(APPLE)
  find_package(SentryCocoa REQUIRED)
  target_link_libraries(qfield_sentry PRIVATE SentryCocoa::sentry_cocoa)
else()
  find_package(sentry REQUIRED)
  target_link_libraries(qfield_sentry PRIVATE sentry::sentry)
endif()

target_link_libraries(qfield_sentry PRIVATE ${QT_PKG}::Core)

if(ANDROID)
  target_link_libraries(qfield_sentry PRIVATE android log
                                              ${QT_PKG}::AndroidExtras)
endif()

target_include_directories(qfield_sentry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
                                                ${CMAKE_CURRENT_BINARY_DIR}/)

target_compile_features(qfield_sentry PUBLIC cxx_std_17)
