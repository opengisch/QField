# include doxygen documentation
set (WITH_APIDOC FALSE CACHE BOOL "Determines whether the QField API doxygen documentation should be built")

if(WITH_APIDOC)
  find_package(Doxygen REQUIRED)
  if(DOXYGEN_FOUND)
    configure_file(${CMAKE_SOURCE_DIR}/doc/cmake_template/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
    add_custom_target(doc_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)
  else()
    message(STATUS "QField API documentation not built - Doxygen not found")
  endif()
endif()

#SUBDIRS (images plugins install_guide)
