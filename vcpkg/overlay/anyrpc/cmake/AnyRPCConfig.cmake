function(set_library_target NAMESPACE LIB_NAME DEBUG_LIB_FILE_NAME RELEASE_LIB_FILE_NAME INCLUDE_DIR)
    add_library(${NAMESPACE}::${LIB_NAME} STATIC IMPORTED)
    set_target_properties(${NAMESPACE}::${LIB_NAME} PROPERTIES
                          IMPORTED_CONFIGURATIONS "RELEASE;DEBUG"
                          IMPORTED_LOCATION_RELEASE "${RELEASE_LIB_FILE_NAME}"
                          IMPORTED_LOCATION_DEBUG "${DEBUG_LIB_FILE_NAME}"
                          INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIR}"
                          )
    set(${NAMESPACE}_${LIB_NAME}_FOUND 1)
endfunction()

get_filename_component(ROOT "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(ROOT "${ROOT}" PATH)
get_filename_component(ROOT "${ROOT}" PATH)

if (CMAKE_HOST_WIN32)
  set_library_target("AnyRPC" "anyrpc" "${ROOT}/debug/lib/anyrpc.lib" "${ROOT}/lib/anyrpc.lib" "${ROOT}/include/")
else ()
  set_library_target("AnyRPC" "anyrpc" "${ROOT}/debug/lib/libanyrpc.a" "${ROOT}/lib/libanyrpc.a" "${ROOT}/include/")
endif ()
