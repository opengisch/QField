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

target_link_libraries(qfield_sentry PRIVATE Qt::Core)

if(ANDROID)
  find_package(Qt6 REQUIRED COMPONENTS CorePrivate)
  target_link_libraries(qfield_sentry PRIVATE Qt::CorePrivate android log)
endif()

target_include_directories(
  qfield_sentry PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                       $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>)

target_compile_features(qfield_sentry PUBLIC cxx_std_17)

# qfield_core is a static library, so its private qfield_sentry linkage stays in
# the exported link interface as $<LINK_ONLY:qfield_sentry>; install(EXPORT)
# therefore requires qfield_sentry in the same export set.
install(
  TARGETS qfield_sentry
  EXPORT qfield-targets
  LIBRARY DESTINATION ${QFIELD_LIB_DIR}
  ARCHIVE DESTINATION ${QFIELD_LIB_DIR})
