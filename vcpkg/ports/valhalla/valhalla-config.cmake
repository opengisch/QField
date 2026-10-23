include(SelectLibraryConfigurations)

get_filename_component(_VALHALLA_ROOT "${CMAKE_CURRENT_LIST_DIR}/../.." ABSOLUTE)

find_path(Valhalla_INCLUDE_DIR 
    NAMES "valhalla.h"
    PATHS "${_VALHALLA_ROOT}/include/valhalla"
    NO_DEFAULT_PATH
)

find_library(Valhalla_LIBRARY_DEBUG 
    NAMES valhalla libvalhalla 
    PATHS "${_VALHALLA_ROOT}/debug/lib"
    NO_DEFAULT_PATH
)

find_library(Valhalla_LIBRARY_RELEASE 
    NAMES valhalla libvalhalla 
    PATHS "${_VALHALLA_ROOT}/lib"
    NO_DEFAULT_PATH
)

select_library_configurations(Valhalla)

if(Valhalla_INCLUDE_DIR AND Valhalla_LIBRARY)
    set(Valhalla_FOUND TRUE)
else()
    set(Valhalla_FOUND FALSE)
endif()

if(Valhalla_FOUND AND NOT TARGET Valhalla::valhalla)
    add_library(Valhalla::valhalla UNKNOWN IMPORTED)
    
    set_target_properties(Valhalla::valhalla PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Valhalla_INCLUDE_DIR}"
    )

    if(Valhalla_LIBRARY_RELEASE)
        set_property(TARGET Valhalla::valhalla APPEND PROPERTY 
            IMPORTED_LOCATION_RELEASE "${Valhalla_LIBRARY_RELEASE}"
        )
    endif()

    if(Valhalla_LIBRARY_DEBUG)
        set_property(TARGET Valhalla::valhalla APPEND PROPERTY 
            IMPORTED_LOCATION_DEBUG "${Valhalla_LIBRARY_DEBUG}"
        )
    endif()

    if(Valhalla_LIBRARY_RELEASE AND NOT Valhalla_LIBRARY_DEBUG)
        set_property(TARGET Valhalla::valhalla APPEND PROPERTY 
            IMPORTED_LOCATION "${Valhalla_LIBRARY_RELEASE}"
        )
    endif()
endif()
