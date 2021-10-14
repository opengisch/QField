include(FindPythonInterp)

add_test(
  NAME smoke_test
  COMMAND ${PYTHON_EXECUTABLE} -m pytest --html=report/report.html
          ${CMAKE_CURRENT_SOURCE_DIR}
  WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
