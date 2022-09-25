vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO faaxm/spix
    REF 4de44072d76b49ceaae9c57046f2260948c065a4
    SHA512 07555c17f1836e1297ff91cc5dd05f7171a2a23dc6896be4bb6ab88b9b8d96ff0908b41758b4effcc719257a9b7eacff258a7fece896d5075082afcbfe534fc0
    HEAD_REF master
)

vcpkg_cmake_configure(
    SOURCE_PATH "${SOURCE_PATH}"
    OPTIONS
        -DSPIX_BUILD_EXAMPLES=OFF
        -DSPIX_BUILD_TESTS=OFF
        -DSPIX_QT_MAJOR=5
)

vcpkg_cmake_install()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/share")

file(INSTALL "${SOURCE_PATH}/LICENSE.txt" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)

vcpkg_copy_pdbs()
