set(VCPKG_POLICY_EMPTY_INCLUDE_FOLDER enabled)
file(INSTALL ${CURRENT_PORT_DIR}/../qt/copyright DESTINATION ${CURRENT_PACKAGES_DIR}/share/${PORT} RENAME copyright)
configure_file("${CMAKE_CURRENT_LIST_DIR}/vcpkg-cmake-wrapper.cmake" "${CURRENT_PACKAGES_DIR}/share/qtcore/vcpkg-cmake-wrapper.cmake" @ONLY)
configure_file("${CMAKE_CURRENT_LIST_DIR}/vcpkg-cmake-wrapper.cmake" "${CURRENT_PACKAGES_DIR}/share/qt6/vcpkg-cmake-wrapper.cmake" @ONLY)
