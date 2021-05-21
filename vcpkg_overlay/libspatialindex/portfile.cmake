vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO libspatialindex/libspatialindex
    REF 1.9.3
    SHA512   7922807a2f7026542a014d3d7943da5410429b291cf469267ffea5b8075eab2a94e68b6c013425615d221b930fd319c403f3bf43404c9b1d2c92d0e2baae3066
    HEAD_REF master
)

vcpkg_configure_cmake(
    SOURCE_PATH ${SOURCE_PATH}
	OPTIONS -DCMAKE_DEBUG_POSTFIX=d -DSIDX_BUILD_TESTS:BOOL=OFF
)

vcpkg_install_cmake()

vcpkg_copy_pdbs()

#Debug
file(REMOVE_RECURSE ${CURRENT_PACKAGES_DIR}/debug/include)

# Handle copyright
file(COPY ${SOURCE_PATH}/COPYING DESTINATION ${CURRENT_PACKAGES_DIR}/share/libspatialindex)
file(RENAME ${CURRENT_PACKAGES_DIR}/share/libspatialindex/COPYING ${CURRENT_PACKAGES_DIR}/share/libspatialindex/copyright)
