find_program(NOSE_EXECUTABLE "nosetests")
mark_as_advanced(NOSE_EXECUTABLE)

if (Pism_DEBUG)
  add_test(NAME "Config:metadata_structure"
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/config_test.py pism_config.nc
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR})
endif ()

if (Pism_BUILD_PYTHON_BINDINGS AND NOSE_EXECUTABLE)
  message(STATUS "Enabling PISM Python tests that use nose")
  add_test(NAME "Python:nose:misc"
    COMMAND ${NOSE_EXECUTABLE} "-v" "-s" ${CMAKE_CURRENT_SOURCE_DIR}/nosetests.py)
  add_test(NAME "Python:nose:enthalpy:converter"
    COMMAND ${NOSE_EXECUTABLE} "-v" "-s" ${CMAKE_CURRENT_SOURCE_DIR}/enthalpy/converter.py)
  add_test(NAME "Python:nose:enthalpy:column"
    COMMAND ${NOSE_EXECUTABLE} "-v" "-s" ${CMAKE_CURRENT_SOURCE_DIR}/enthalpy/column.py)
  add_test(NAME "Python:nose:sia:bed_smoother"
    COMMAND ${NOSE_EXECUTABLE} "-v" "-s" ${CMAKE_CURRENT_SOURCE_DIR}/bed_smoother.py)
  add_test(NAME "Python:nose:bed_deformation:LC"
    COMMAND ${NOSE_EXECUTABLE} "-v" "-s" ${PROJECT_SOURCE_DIR}/examples/python/bed_deformation.py)
  add_test(NAME "Python:Verification:nose:bed_deformation:LC"
    COMMAND ${NOSE_EXECUTABLE} "-v" "-s" ${CMAKE_CURRENT_SOURCE_DIR}/bed_deformation.py)
  add_test(NAME "Python:Verification:nose:mass_transport"
    COMMAND ${NOSE_EXECUTABLE} "-v" "-s" ${CMAKE_CURRENT_SOURCE_DIR}/mass_transport.py)
endif()
