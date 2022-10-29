Makefiles do. Handle both cases.\n"
        "if [ \"$1\" = \"${CMAKE_CUDA_COMPILER}\" ] ; then\n"
        "    shift\n"
        "fi\n"
        "\n"
        "export CCACHE_CPP2=true\n"
        "exec \"${CUDA_LAUNCHER}\" \"${CMAKE_CUDA_COMPILER}\" \"$@\"\n"
        )


    execute_process(COMMAND chmod a+rx
                     "${CMAKE_BINARY_DIR}/launch-c"
                     "${CMAKE_BINARY_DIR}/launch-cxx"
                     "${CMAKE_BINARY_DIR}/launch-cuda"
    )

    if(CMAKE_GENERATOR STREQUAL "Xcode")
        # Set Xcode project attributes to route compilation and linking
        # through our scripts
        set(CMAKE_XCODE_ATTRIBUTE_CC         "${CMAKE_BINARY_DIR}/launch-c")
        set(CMAKE_XCODE_ATTRIBUTE_CXX        "${CMAKE_BINARY_DIR}/launch-cxx")
        set(CMAKE_XCODE_ATTRIBUTE_LD         "${CMAKE_BINARY_DIR}/launch-c")
        set(CMAKE_XCODE_ATTRIBUTE_LDPLUSPLUS "${CMAKE_BINARY_DIR}/launch-cxx")
    else()
        # Support Unix Makefiles and Ninja
        set(CMAKE_C_COMPILER_LAUNCHER   "${CMAKE_BINARY_DIR}/launch-c")
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_BINARY_DIR}/launch-cxx")
        set(CMAKE_CUDA_COMPILER_LAUNCHER "${CMAKE_BINARY_DIR}/launch-cuda")
    endif()

    message(STATUS "Using CCache")
else()
    message(STATUS "Not using CCache")
endif()
