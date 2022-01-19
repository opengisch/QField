include(SelectLibraryConfigurations)

find_path(QGIS_INCLUDE_DIR
    NAMES qgis.h
    PATHS "${CMAKE_CURRENT_LIST_DIR}/../../include/qgis"
    NO_DEFAULT_PATH
)
find_library(QGIS_CORE_LIBRARY_DEBUG
    NAMES qgis_core
    NAMES_PER_DIR
    PATHS "${CMAKE_CURRENT_LIST_DIR}/../../debug/lib"
    NO_DEFAULT_PATH
)
find_library(QGIS_CORE_LIBRARY_RELEASE
    NAMES qgis_core
    NAMES_PER_DIR
    PATHS "${CMAKE_CURRENT_LIST_DIR}/../../lib"
    NO_DEFAULT_PATH
)
find_library(QGIS_ANALYSIS_LIBRARY_DEBUG
    NAMES qgis_analysis
    NAMES_PER_DIR
    PATHS "${CMAKE_CURRENT_LIST_DIR}/../../debug/lib"
    NO_DEFAULT_PATH
)
find_library(QGIS_ANALYSIS_LIBRARY_RELEASE
    NAMES qgis_analysis
    NAMES_PER_DIR
    PATHS "${CMAKE_CURRENT_LIST_DIR}/../../lib"
    NO_DEFAULT_PATH
)
select_library_configurations(QGIS_CORE)
select_library_configurations(QGIS_ANALYSIS)

if(NOT QGIS_INCLUDE_DIR OR NOT QGIS_CORE_LIBRARY OR NOT QGIS_ANALYSIS_LIBRARY)
  message(FATAL_ERROR "Installation of vcpkg port qgis is broken. Include dir: ${QGIS_INCLUDE_DIR}, Core lib: ${QGIS_CORE_LIBRARY}, Analysis lib: ${QGIS_ANALYSIS_LIBRARY}")
endif()

set(FindQGIS_SKIP_QGIS_CONFIG TRUE)

_find_package(${ARGS})

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
      target_link_libraries(qgis_core INTERFACE ${dependency})
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

if(QGIS_FOUND AND "@VCPKG_LIBRARY_LINKAGE@" STREQUAL "static")
  _find_and_link_library(authmethod_basic_a qgis_core)
  _find_and_link_library(authmethod_esritoken_a qgis_core)
  _find_and_link_library(authmethod_identcert_a qgis_core)
  _find_and_link_library(authmethod_oauth2_a qgis_core)
  _find_and_link_library(authmethod_pkcs12_a qgis_core)
  _find_and_link_library(authmethod_pkipaths_a qgis_core)
  _find_and_link_library(provider_postgres_a qgis_core)
  _find_and_link_library(provider_postgresraster_a qgis_core)
  _find_and_link_library(provider_wms_a qgis_core)
  _find_and_link_library(provider_delimitedtext_a qgis_core)
  _find_and_link_library(provider_arcgisfeatureserver_a qgis_core)
  _find_and_link_library(provider_arcgismapserver_a qgis_core)
  _find_and_link_library(provider_spatialite_a qgis_core)
  _find_and_link_library(provider_wfs_a qgis_core)
  _find_and_link_library(provider_wcs_a qgis_core)
  _find_and_link_library(provider_virtuallayer_a qgis_core)

  _find_and_link_library(pq qgis_core)
  # Relink qgis_core in the end, to make all the qgis plugins happy that need symbols from it
  _find_and_link_library(qgis_core qgis_core)

  _find_and_link_library(spatialite qgis_core)
  #  _find_and_link_library(z qgis_core) # transient dependency for spatialite ... disabled because of windows ... required or conditional?
  _qgis_core_add_dependency(qca Qca CONFIG)
  _qgis_core_add_dependency(Protobuf Protobuf)
  _qgis_core_add_dependency(GDAL::GDAL GDAL)
  _qgis_core_add_dependency(exiv2lib exiv2)
  _qgis_core_add_dependency(exiv2-xmp exiv2)
  _qgis_core_add_dependency(libzip::zip libzip)
  _qgis_core_add_dependency(ZLIB::ZLIB ZLIB)
  _find_and_link_library(spatialindex qgis_core spatialindex spatialindex-64)
  _find_and_link_library(freexl qgis_core)
  _find_and_link_library(qt5keychain qgis_core)
  _find_and_link_library(poppler qgis_core)
  _find_and_link_library(freetype qgis_core)
  _find_and_link_library(brotlidec-static qgis_core)
  _find_and_link_library(brotlicommon-static qgis_core)
  find_package(Qt5 COMPONENTS Core Gui Network Xml Svg Concurrent Sql SerialPort)
  target_link_libraries(qgis_core INTERFACE
      Qt5::Core
      Qt5::Network
      Qt5::Xml
      Qt5::Svg
      Qt5::Concurrent
      Qt5::Sql
      Qt5::SerialPort
    )
  if(APPLE)
    find_package(Qt5 COMPONENTS MacExtras)
    target_link_libraries(qgis_core INTERFACE
      Qt5::MacExtras
    )
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
    target_link_libraries(qgis_core INTERFACE
      ${LIBTASN1_LIBRARY}
    )

    # QtKeychain
    target_link_libraries(qgis_core INTERFACE "-framework Foundation" "-framework Security")
  endif()
  if(UNIX AND NOT APPLE AND NOT ANDROID)
    # poppler fixup for linux
    _find_and_link_library(lcms2 qgis_core)

    # QtKeychain
    find_package(Qt5 COMPONENTS DBus REQUIRED)
    target_link_libraries(qgis_core INTERFACE
      Qt5::DBus
    )
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)
    target_link_libraries(qgis_core INTERFACE PkgConfig::deps)
  endif()
  target_link_libraries(qgis_analysis INTERFACE qgis_core)
endif()
