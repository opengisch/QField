include(FindPythonInterp)

add_test(NAME smoke_test COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/smoke_test.py WORKING_DIRECTORY ${CMAKE_BINARY_DIR} )

