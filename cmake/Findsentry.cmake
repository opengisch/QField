find_package(sentry CONFIG)

if(CMAKE_SYSTEM_NAME STREQUAL "Android")
  add_library(sentry::sentry SHARED IMPORTED)

  set_target_properties(sentry::sentry PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${SENTRY_IMPORT_PREFIX}/include"
    INTERFACE_LINK_LIBRARIES "${SENTRY_IMPORT_PREFIX}/${ANDROID_ABI}/libsentry.so"
    IMPORTED_LOCATION "${SENTRY_IMPORT_PREFIX}/${ANDROID_ABI}/libsentry.so"
    IMPORTED_SONAME "libsentry.so"
    )
endif()
