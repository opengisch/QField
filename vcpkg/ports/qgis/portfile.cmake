set(QGIS_REF 0b6fed59e797ad26de56b8b437abc26a4c3ea171)
set(QGIS_SHA512 972fcaa21694e7dfad4640af7f49c066ea3dc9469bafcbb82a00fa6925919dff8e61edd036fb529f2e29ac4397d800bf1a297fd072176c8ab741a37dc007447a)

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO qgis/QGIS
    REF ${QGIS_REF}
    SHA512   ${QGIS_SHA512}
    HEAD_REF master
    PATCHES
        qgspython.patch # Make qgis support python's debug library
        libxml2.patch
        exiv2.patch
        mesh.patch
        crssync-no-install.patch
        include-qthread.patch
        processing.patch # Needed to avoid link issue with tinygltf (ATM embedded into QGIS) and _GEOSQueryCallback defined multiple times
)


file(REMOVE ${SOURCE_PATH}/cmake/FindGDAL.cmake)
file(REMOVE ${SOURCE_PATH}/cmake/FindGEOS.cmake)
file(REMOVE ${SOURCE_PATH}/cmake/FindEXIV2.cmake)
file(REMOVE ${SOURCE_PATH}/cmake/FindExpat.cmake)
file(REMOVE ${SOURCE_PATH}/cmake/FindIconv.cmake)

vcpkg_find_acquire_program(FLEX)
vcpkg_find_acquire_program(BISON)

list(APPEND QGIS_OPTIONS "-DENABLE_TESTS:BOOL=OFF")
list(APPEND QGIS_OPTIONS "-DWITH_QTWEBKIT:BOOL=OFF")
list(APPEND QGIS_OPTIONS "-DWITH_QTPRINTER:BOOL=OFF")
list(APPEND QGIS_OPTIONS "-DWITH_GRASS7:BOOL=OFF")
list(APPEND QGIS_OPTIONS "-DWITH_SPATIALITE:BOOL=ON")
list(APPEND QGIS_OPTIONS "-DWITH_QSPATIALITE:BOOL=OFF")
list(APPEND QGIS_OPTIONS "-DWITH_PDAL:BOOL=OFF")
list(APPEND QGIS_OPTIONS "-DWITH_DRACO:BOOL=ON")
list(APPEND QGIS_OPTIONS "-DWITH_INTERNAL_POLY2TRI:BOOL=OFF")
list(APPEND QGIS_OPTIONS "-DWITH_INTERNAL_MESHOPTIMIZER:BOOL=OFF")
list(APPEND QGIS_OPTIONS "-DPREFER_INTERNAL_LIBS:BOOL=OFF")

list(APPEND QGIS_OPTIONS "-DBISON_EXECUTABLE=${BISON}")
list(APPEND QGIS_OPTIONS "-DFLEX_EXECUTABLE=${FLEX}")
# By default QGIS installs includes into "include" on Windows and into "include/qgis" everywhere else
# let's keep things clean and tidy and put them at a predictable location
list(APPEND QGIS_OPTIONS "-DQGIS_INCLUDE_SUBDIR=include/qgis")
list(APPEND QGIS_OPTIONS "-DBUILD_WITH_QT6=ON")
list(APPEND QGIS_OPTIONS "-DUSE_CCACHE=OFF")
list(APPEND QGIS_OPTIONS "-DQGIS_MACAPP_FRAMEWORK=FALSE")

if("opencl" IN_LIST FEATURES)
    list(APPEND QGIS_OPTIONS -DUSE_OPENCL:BOOL=ON)
else()
    list(APPEND QGIS_OPTIONS -DUSE_OPENCL:BOOL=OFF)
endif()

if("bindings" IN_LIST FEATURES)
    list(APPEND QGIS_OPTIONS -DWITH_BINDINGS:BOOL=ON)
else()
    list(APPEND QGIS_OPTIONS -DWITH_BINDINGS:BOOL=OFF)
endif()

if("gui" IN_LIST FEATURES)
    list(APPEND QGIS_OPTIONS -DWITH_GUI:BOOL=ON)
else()
    if("desktop" IN_LIST FEATURES OR "customwidgets" IN_LIST FEATURES)
        message(FATAL_ERROR "If QGIS is built without gui, desktop and customwidgets cannot be built. Enable gui or disable customwidgets and desktop.")
    endif()
    list(APPEND QGIS_OPTIONS -DWITH_GUI:BOOL=OFF)
endif()

if("desktop" IN_LIST FEATURES)
    list(APPEND QGIS_OPTIONS -DWITH_DESKTOP:BOOL=ON)
else()
    list(APPEND QGIS_OPTIONS -DWITH_DESKTOP:BOOL=OFF)
endif()

if("customwidgets" IN_LIST FEATURES)
    list(APPEND QGIS_OPTIONS -DWITH_CUSTOM_WIDGETS:BOOL=ON)
else()
    list(APPEND QGIS_OPTIONS -DWITH_CUSTOM_WIDGETS:BOOL=OFF)
endif()

if("server" IN_LIST FEATURES)
    list(APPEND QGIS_OPTIONS -DWITH_SERVER:BOOL=ON)
    if("bindings" IN_LIST FEATURES)
        list(APPEND QGIS_OPTIONS -DWITH_SERVER_PLUGINS:BOOL=ON)
    else()
        list(APPEND QGIS_OPTIONS -DWITH_SERVER_PLUGINS:BOOL=OFF)
    endif()
else()
    list(APPEND QGIS_OPTIONS -DWITH_SERVER:BOOL=OFF)
endif()

if("process" IN_LIST FEATURES)
    list(APPEND QGIS_OPTIONS -DWITH_QGIS_PROCESS:BOOL=ON)
else()
    list(APPEND QGIS_OPTIONS -DWITH_QGIS_PROCESS:BOOL=OFF)
endif()

if("3d" IN_LIST FEATURES)
    list(APPEND QGIS_OPTIONS -DWITH_3D:BOOL=ON)
else()
    list(APPEND QGIS_OPTIONS -DWITH_3D:BOOL=OFF)
endif()

if("quick" IN_LIST FEATURES)
    list(APPEND QGIS_OPTIONS -DWITH_QUICK:BOOL=ON)
else()
    list(APPEND QGIS_OPTIONS -DWITH_QUICK:BOOL=OFF)
endif()

# Configure debug and release library paths
macro(FIND_LIB_OPTIONS basename relname debname suffix libsuffix)
   file(TO_NATIVE_PATH "${CURRENT_INSTALLED_DIR}/lib/${VCPKG_TARGET_IMPORT_LIBRARY_PREFIX}${relname}${libsuffix}" ${basename}_LIBRARY_RELEASE)
   file(TO_NATIVE_PATH "${CURRENT_INSTALLED_DIR}/debug/lib/${VCPKG_TARGET_IMPORT_LIBRARY_PREFIX}${debname}${libsuffix}" ${basename}_LIBRARY_DEBUG)
   if( ${basename}_LIBRARY_DEBUG AND ${basename}_LIBRARY_RELEASE AND NOT ${basename}_LIBRARY_DEBUG STREQUAL ${basename}_LIBRARY_RELEASE )
        list(APPEND QGIS_OPTIONS_RELEASE -D${basename}_${suffix}:FILEPATH=${${basename}_LIBRARY_RELEASE})
        list(APPEND QGIS_OPTIONS_DEBUG -D${basename}_${suffix}:FILEPATH=${${basename}_LIBRARY_DEBUG})
   elseif( ${basename}_LIBRARY_RELEASE )
        list(APPEND QGIS_OPTIONS -D${basename}_${suffix}:FILEPATH=${${basename}_LIBRARY_RELEASE})
   elseif( ${basename}_LIBRARY_DEBUG )
        list(APPEND QGIS_OPTIONS -D${basename}_${suffix}:FILEPATH=${${basename}_LIBRARY_DEBUG})
   endif()
endmacro()

if (VCPKG_LIBRARY_LINKAGE STREQUAL "static")
  list(APPEND QGIS_OPTIONS -DFORCE_STATIC_LIBS=TRUE)
  # QGIS likes to install auth and providers to different locations on each platform
  # let's keep things clean and tidy and put them at a predictable location
  list(APPEND QGIS_OPTIONS -DQGIS_PLUGIN_SUBDIR=lib)
endif()

if(VCPKG_CROSSCOMPILING)
   list(APPEND QGIS_OPTIONS -DQT_HOST_PATH=${CURRENT_HOST_INSTALLED_DIR})
   list(APPEND QGIS_OPTIONS -DQT_HOST_PATH_CMAKE_DIR:PATH=${CURRENT_HOST_INSTALLED_DIR}/share)
endif()

if(VCPKG_TARGET_IS_WINDOWS)
    FIND_LIB_OPTIONS(GDAL gdal gdald LIBRARY ${VCPKG_TARGET_IMPORT_LIBRARY_SUFFIX})
    FIND_LIB_OPTIONS(POSTGRES libpq libpq LIBRARY ${VCPKG_TARGET_IMPORT_LIBRARY_SUFFIX})
    FIND_LIB_OPTIONS(QCA qca qcad LIBRARY ${VCPKG_TARGET_IMPORT_LIBRARY_SUFFIX})
    if("server" IN_LIST FEATURES)
        FIND_LIB_OPTIONS(FCGI libfcgi libfcgi LIBRARY ${VCPKG_TARGET_IMPORT_LIBRARY_SUFFIX})
        list(APPEND QGIS_OPTIONS -DFCGI_INCLUDE_DIR="${CURRENT_INSTALLED_DIR}/include/fastcgi")
    endif()
    if("gui" IN_LIST FEATURES)
        FIND_LIB_OPTIONS(QWT qwt qwtd LIBRARY ${VCPKG_TARGET_IMPORT_LIBRARY_SUFFIX})
    endif()

    set(SPATIALINDEX_LIB_NAME spatialindex)
    if( VCPKG_TARGET_ARCHITECTURE STREQUAL "x64" OR VCPKG_TARGET_ARCHITECTURE STREQUAL "arm64" )
        set( SPATIALINDEX_LIB_NAME "spatialindex-64" )
    else()
        set( SPATIALINDEX_LIB_NAME "spatialindex-32" )
    endif()
    FIND_LIB_OPTIONS(SPATIALINDEX ${SPATIALINDEX_LIB_NAME} ${SPATIALINDEX_LIB_NAME}d LIBRARY ${VCPKG_TARGET_IMPORT_LIBRARY_SUFFIX})
else() # Build in UNIX
    list(APPEND QGIS_OPTIONS -DGSL_INCLUDE_DIR:PATH=${CURRENT_INSTALLED_DIR}/include)
    list(APPEND QGIS_OPTIONS -DPROJ_INCLUDE_DIR:PATH=${CURRENT_INSTALLED_DIR}/include)
    list(APPEND QGIS_OPTIONS_DEBUG -DGSL_LIBRARIES:FILEPATH=${CURRENT_INSTALLED_DIR}/debug/lib/${VCPKG_TARGET_STATIC_LIBRARY_PREFIX}gsld${VCPKG_TARGET_STATIC_LIBRARY_SUFFIX};${CURRENT_INSTALLED_DIR}/debug/lib/${VCPKG_TARGET_STATIC_LIBRARY_PREFIX}gslcblasd${VCPKG_TARGET_STATIC_LIBRARY_SUFFIX})
    list(APPEND QGIS_OPTIONS -DPROJ_INCLUDE_DIR:PATH=${CURRENT_INSTALLED_DIR}/include)
    list(APPEND QGIS_OPTIONS_RELEASE -DGSL_LIBRARIES:FILEPATH=${CURRENT_INSTALLED_DIR}/lib/${VCPKG_TARGET_STATIC_LIBRARY_PREFIX}gsl${VCPKG_TARGET_STATIC_LIBRARY_SUFFIX} ${CURRENT_INSTALLED_DIR}/lib/${VCPKG_TARGET_STATIC_LIBRARY_PREFIX}gslcblas${VCPKG_TARGET_STATIC_LIBRARY_SUFFIX})
    if("server" IN_LIST FEATURES)
        FIND_LIB_OPTIONS(FCGI fcgi fcgi LIBRARY ${VCPKG_TARGET_SHARED_LIBRARY_SUFFIX})
        list(APPEND QGIS_OPTIONS -DFCGI_INCLUDE_DIR="${CURRENT_INSTALLED_DIR}/include/fastcgi")
    endif()
endif()

if(VCPKG_TARGET_IS_IOS)
    list(APPEND QGIS_OPTIONS -DWITH_QTSERIALPORT=FALSE)
endif()

# Build crssync only runs when building natively.
if(NOT HOST_TRIPLET STREQUAL TARGET_TRIPLET)
    list(APPEND QGIS_OPTIONS -DNATIVE_CRSSYNC_BIN=true)
endif()

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
    #PREFER_NINJA
    OPTIONS ${QGIS_OPTIONS} 
    OPTIONS_DEBUG ${QGIS_OPTIONS_DEBUG}
    OPTIONS_RELEASE ${QGIS_OPTIONS_RELEASE}
)

vcpkg_install_cmake()

if(VCPKG_TARGET_IS_WINDOWS)
    function(copy_path basepath targetdir)
        file(GLOB ${basepath}_PATH ${CURRENT_PACKAGES_DIR}/${basepath}/*)
        if( ${basepath}_PATH )
            file(MAKE_DIRECTORY ${CURRENT_PACKAGES_DIR}/${targetdir}/qgis/${basepath})
            file(COPY ${${basepath}_PATH} DESTINATION ${CURRENT_PACKAGES_DIR}/${targetdir}/qgis/${basepath})
        endif()
 
        if(EXISTS "${CURRENT_PACKAGES_DIR}/${basepath}/")
            file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/${basepath}/)
        endif()
    endfunction()

    file(GLOB QGIS_TOOL_PATH ${CURRENT_PACKAGES_DIR}/bin/*${VCPKG_TARGET_EXECUTABLE_SUFFIX} ${CURRENT_PACKAGES_DIR}/*${VCPKG_TARGET_EXECUTABLE_SUFFIX})
    if(QGIS_TOOL_PATH)
        file(MAKE_DIRECTORY ${CURRENT_PACKAGES_DIR}/tools/qgis/bin)
        file(COPY ${QGIS_TOOL_PATH} DESTINATION ${CURRENT_PACKAGES_DIR}/tools/qgis/bin)
        file(REMOVE_RECURSE ${QGIS_TOOL_PATH})
        file(GLOB QGIS_TOOL_PATH ${CURRENT_PACKAGES_DIR}/bin/* )
        file(COPY ${QGIS_TOOL_PATH} DESTINATION ${CURRENT_PACKAGES_DIR}/tools/qgis/bin)
    endif()
    
    file(GLOB QGIS_TOOL_PATH_DEBUG ${CURRENT_PACKAGES_DIR}/debug/bin/*${VCPKG_TARGET_EXECUTABLE_SUFFIX} ${CURRENT_PACKAGES_DIR}/debug/*${VCPKG_TARGET_EXECUTABLE_SUFFIX})
    if(QGIS_TOOL_PATH_DEBUG)
        if("debug-tools" IN_LIST FEATURES)
            file(MAKE_DIRECTORY ${CURRENT_PACKAGES_DIR}/debug/tools/qgis/bin)
            file(COPY ${QGIS_TOOL_PATH_DEBUG} DESTINATION ${CURRENT_PACKAGES_DIR}/debug/tools/qgis/bin)
            file(REMOVE_RECURSE ${QGIS_TOOL_PATH_DEBUG})
            file(GLOB QGIS_TOOL_PATH_DEBUG ${CURRENT_PACKAGES_DIR}/debug/bin/* )
            file(COPY ${QGIS_TOOL_PATH_DEBUG} DESTINATION ${CURRENT_PACKAGES_DIR}/debug/tools/qgis/bin)
        else()
            file(REMOVE_RECURSE ${QGIS_TOOL_PATH_DEBUG})
        endif()
    endif()

    copy_path(doc share)
    copy_path(i18n share)
    copy_path(icons share)
    copy_path(images share)
    copy_path(plugins tools)
    copy_path(resources share)
    copy_path(svg share)
endif()

# crssync only runs when building natively. If we are cross-compiling, copy the srs.cb from the host installation.
if(NOT HOST_TRIPLET STREQUAL TARGET_TRIPLET)
  file(COPY "${CURRENT_HOST_INSTALLED_DIR}/share/qgis/resources/srs.db" DESTINATION "${CURRENT_PACKAGES_DIR}/share/qgis/resources")
endif()

file(GLOB QGIS_CMAKE_PATH ${CURRENT_PACKAGES_DIR}/*.cmake)
if(QGIS_CMAKE_PATH)
    file(COPY ${QGIS_CMAKE_PATH} DESTINATION ${CURRENT_PACKAGES_DIR}/share/cmake/qgis)
    file(REMOVE_RECURSE ${QGIS_CMAKE_PATH})
endif()

file(GLOB QGIS_CMAKE_PATH_DEBUG ${CURRENT_PACKAGES_DIR}/debug/*.cmake)
if( QGIS_CMAKE_PATH_DEBUG )
    file(REMOVE_RECURSE ${QGIS_CMAKE_PATH_DEBUG})
endif()

file(REMOVE_RECURSE
    ${CURRENT_PACKAGES_DIR}/debug/include
)
file(REMOVE_RECURSE # Added for debug porpose
    ${CURRENT_PACKAGES_DIR}/debug/share
)

# Handle copyright
vcpkg_install_copyright(FILE_LIST ${SOURCE_PATH}/COPYING)
