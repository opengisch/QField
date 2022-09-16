function(find_and_link_library TARGET LIBRARY)
  find_library(${LIBRARY}-LIBRARY ${LIBRARY} ${ADDITIONAL_ARGS})
  if(${LIBRARY}-LIBRARY)
    message(STATUS " -- Link ${${LIBRARY}-LIBRARY} interface to ${TARGET}")
    target_link_libraries(${TARGET} INTERFACE ${${LIBRARY}-LIBRARY})
  else()
    message(FATAL_ERROR "Fail to find library ${LIBRARY}. Make sure it is present in CMAKE_PREFIX_PATH")
  endif()
endfunction()

set(_qgis_dep_find_args "")
if(";${ARGS};" MATCHES ";REQUIRED;")
  list(APPEND _qgis_dep_find_args "REQUIRED")
endif()
function(_qgis_core_add_dependency target package)
  find_package(${package} ${ARGN} ${_qgis_dep_find_args})
  if(${${package}_FOUND})
    foreach(suffix IN ITEMS "" "-shared" "_shared" "-static" "_static" "-NOTFOUND")
      set(dependency "${target}${suffix}")
      if(TARGET ${dependency})
        break()
      endif()
    endforeach()
    if(NOT TARGET ${dependency})
      string(TOUPPER ${package} _qgis_deps_package)
      if(DEFINED ${_qgis_deps_package}_LIBRARIES)
        set(dependency ${${_qgis_deps_package}_LIBRARIES})
      elseif(DEFINED ${package}_LIBRARIES)
        set(dependency ${${package}_LIBRARIES})
      elseif(DEFINED ${_qgis_deps_package}_LIBRARY)
        set(dependency ${${_qgis_deps_package}_LIBRARY})
      elseif(DEFINED ${package}_LIBRARY)
        set(dependency ${${package}_LIBRARY})
      endif()
    endif()
    if(dependency)
      target_link_libraries(QGIS::Core INTERFACE ${dependency})
    else()
      message(WARNING "Did not find which libraries are exported by ${package}")
        set(QGIS_FOUND false PARENT_SCOPE)
      endif()
    else()
      set(QGIS_FOUND false PARENT_SCOPE)
    endif()

endfunction()

function(_find_and_link_library library target)
  find_library(${library}-LIBRARY NAMES ${library} ${ADDITIONAL_ARGS})
  if(${library}-LIBRARY)
    message(STATUS "  Link ${target} interface to ${${library}-LIBRARY}")
    target_link_libraries(${target} INTERFACE ${${library}-LIBRARY})
  else()
    message(FATAL_ERROR "Fail to find library ${library}.")
  endif()
endfunction()

# https://stackoverflow.com/a/32771883
#if(CMAKE_... STREQUAL "iOS")
#  set(CMAKE_THREAD_LIBS_INIT "-lpthread")
#  set(CMAKE_HAVE_THREADS_LIBRARY 1)
#  set(CMAKE_USE_WIN32_THREADS_INIT 0)
#  set(CMAKE_USE_PTHREADS_INIT 1)
#endif()


if(TRUE) # Should possibly have a "static only" check
  find_package(PkgConfig QUIET)

  _find_and_link_library(authmethod_basic_a QGIS::Core)
  _find_and_link_library(authmethod_esritoken_a QGIS::Core)
  _find_and_link_library(authmethod_identcert_a QGIS::Core)
  _find_and_link_library(authmethod_oauth2_a QGIS::Core)
  _find_and_link_library(authmethod_pkcs12_a QGIS::Core)
  _find_and_link_library(authmethod_pkipaths_a QGIS::Core)
  _find_and_link_library(provider_postgres_a QGIS::Core)
  _find_and_link_library(provider_postgresraster_a QGIS::Core)
  _find_and_link_library(provider_wms_a QGIS::Core)
  _find_and_link_library(provider_delimitedtext_a QGIS::Core)
  _find_and_link_library(provider_arcgisfeatureserver_a QGIS::Core)
  _find_and_link_library(provider_arcgismapserver_a QGIS::Core)
  _find_and_link_library(provider_spatialite_a QGIS::Core)
  _find_and_link_library(provider_wfs_a QGIS::Core)
  _find_and_link_library(provider_wcs_a QGIS::Core)
  _find_and_link_library(provider_virtuallayer_a QGIS::Core)

  _qgis_core_add_dependency(PostgreSQL::PostgreSQL PostgreSQL)

  # Relink qgis_core in the end, to make all the qgis plugins happy that need symbols from it
  _find_and_link_library(qgis_core QGIS::Core)

  # Disabled because pkgconfig finds libc++ for the wrong architecture
  #  and we already link to it through gdal
  #
  #  if(PKG_CONFIG_FOUND)
  #    pkg_check_modules(spatialite REQUIRED IMPORTED_TARGET spatialite)
  #    target_link_libraries(qgis_core INTERFACE PkgConfig::spatialite)
  #  endif()

  _qgis_core_add_dependency(qca Qca CONFIG)
  _qgis_core_add_dependency(Protobuf Protobuf)
  # Terrible hack ahead
  # 1. geos and proj add libc++.so to their pkgconfig linker instruction
  # 2. This is propagated through spatialite and GDAL
  # 3. pkgconfig finds the build system instead of target system lib
  # The variable pkgcfg_lib_PC_SPATIALITE_c++ is introduced by GDAL's FindSPATIALITE, patched in the gdal portfile
  if(ANDROID)
    find_library(libdl dl)
    get_filename_component(arch_path ${libdl} DIRECTORY)
    set(pkgcfg_lib_PC_SPATIALITE_c++ "${arch_path}/${ANDROID_PLATFORM_LEVEL}/libc++.so")
    if(NOT EXISTS ${pkgcfg_lib_PC_SPATIALITE_c++})
      set(pkgcfg_lib_PC_SPATIALITE_c++ "${arch_path}/libc++.so")
    endif()

    # libspatialite needs log (needed when building with docker)
    target_link_libraries(QGIS::Core INTERFACE log)
  endif()
  # End Terrible hack

  find_library(Qca-ossl_LIBRARIES NAMES qca-ossl PATH_SUFFIXES Qca/crypto)
  target_link_libraries(QGIS::Core INTERFACE ${Qca-ossl_LIBRARIES})

  _qgis_core_add_dependency(GDAL::GDAL GDAL)


  _qgis_core_add_dependency(exiv2lib exiv2)
  _qgis_core_add_dependency(exiv2-xmp exiv2)
  _qgis_core_add_dependency(libzip::zip libzip)
  _qgis_core_add_dependency(ZLIB::ZLIB ZLIB)
  if(MSVC)
    _find_and_link_library(spatialindex-64 QGIS::Core)
  else()
    _find_and_link_library(spatialindex QGIS::Core)
  endif()
  find_package(poly2tri CONFIG)
  target_link_libraries(QGIS::Core INTERFACE poly2tri::poly2tri)

  if(PKG_CONFIG_FOUND)
    pkg_check_modules(freexl REQUIRED IMPORTED_TARGET freexl)
    target_link_libraries(QGIS::Core INTERFACE PkgConfig::freexl)
  endif()
  if(BUILD_WITH_QT6)
    _find_and_link_library(qt6keychain QGIS::Core)
  else()
    _find_and_link_library(qt5keychain QGIS::Core)
  endif()

  find_package(${QT_PKG} COMPONENTS Core Gui Network Xml Svg Concurrent Sql Positioning)
  target_link_libraries(QGIS::Core INTERFACE
      ${QT_PKG}::Gui
      ${QT_PKG}::Core
      ${QT_PKG}::Network
      ${QT_PKG}::Xml
      ${QT_PKG}::Svg
      ${QT_PKG}::Concurrent
      ${QT_PKG}::Sql
      ${QT_PKG}::Positioning
    )
  if(BUILD_WITH_QT6)
    find_package(${QT_PKG} COMPONENTS Core5Compat)
    target_link_libraries(QGIS::Core INTERFACE
      ${QT_PKG}::Core5Compat
      )
  endif()
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "iOS")
    find_package(${QT_PKG} COMPONENTS SerialPort)
    target_link_libraries(QGIS::Core INTERFACE
      ${QT_PKG}::SerialPort
    )
  endif()
  if(APPLE)
    if(NOT BUILD_WITH_QT6)
      find_package(${QT_PKG} COMPONENTS MacExtras)
      target_link_libraries(QGIS::Core INTERFACE
        ${QT_PKG}::MacExtras
      )
    endif()
    find_path(LIBTASN1_INCLUDE_DIR
        NAMES libtasn1.h
        PATHS
        ${LIB_DIR}/include
        "$ENV{LIB_DIR}/include"
        $ENV{INCLUDE}
        /usr/local/include
        /usr/include
    )

    find_library(LIBTASN1_LIBRARY
        NAMES tasn1
        PATHS
        ${LIB_DIR}
        $ENV{LIB_DIR}/lib
        "$ENV{LIB_DIR}"
        $ENV{LIB}
        /usr/local/lib
        /usr/lib
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args(
      Libtasn1
      REQUIRED_VARS LIBTASN1_INCLUDE_DIR LIBTASN1_LIBRARY
      FOUND_VAR LIBTASN1_FOUND
    )
    target_link_libraries(QGIS::Core INTERFACE
      ${LIBTASN1_LIBRARY}
    )

    # QtKeychain
    target_link_libraries(QGIS::Core INTERFACE "-framework Foundation" "-framework Security")
  endif()
  if(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
     # poppler fixup for linux and macos
     # _find_and_link_library(lcms2 QGIS::Core)

    # QtKeychain
    find_package(${QT_PKG} COMPONENTS DBus REQUIRED)
    target_link_libraries(QGIS::Core INTERFACE
      ${QT_PKG}::DBus
    )
    if(PKG_CONFIG_FOUND)
      pkg_check_modules(glib2 REQUIRED IMPORTED_TARGET glib-2.0)
      target_link_libraries(QGIS::Core INTERFACE PkgConfig::glib2)
    endif()
  endif()
  target_link_libraries(QGIS::Analysis INTERFACE QGIS::Core)
endif()
