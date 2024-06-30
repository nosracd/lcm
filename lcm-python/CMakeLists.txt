# FindPython added in 3.12
cmake_minimum_required(VERSION 3.12)

set(lcm_python_sources
  module.c
  pyeventlog.c
  pylcm.c
  pylcm_subscription.c
)

add_library(lcm-python MODULE ${lcm_python_sources})

set_target_properties(lcm-python PROPERTIES
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/python/lcm
  LIBRARY_OUTPUT_NAME _lcm
  PREFIX ""
)

if (WIN32 AND NOT CYGWIN)
  set_target_properties(lcm-python PROPERTIES SUFFIX .pyd)
endif ()

if (WIN32)
  find_package(Python COMPONENTS Interpreter Development.Module)

  target_include_directories(lcm-python PRIVATE
    ${Python_INCLUDE_DIRS}
  )
else ()
  find_package(Python3 COMPONENTS Interpreter Development.Module)

  target_include_directories(lcm-python PRIVATE
  ${Python3_INCLUDE_DIRS}
  )
endif ()

target_link_libraries(lcm-python PRIVATE
  lcm-static
)

if(APPLE)
  set_target_properties(lcm-python PROPERTIES LINK_FLAGS "-undefined dynamic_lookup -Wl,-no_fixup_chains")
else()
  target_link_libraries(lcm-python PRIVATE
    ${Python_LIBRARIES}
  )
endif()

install(TARGETS lcm-python
  RUNTIME DESTINATION lcm
  LIBRARY DESTINATION lcm
)

lcm_copy_file_target(lcm-python-init
  ${CMAKE_CURRENT_SOURCE_DIR}/lcm/__init__.py
  ${CMAKE_BINARY_DIR}/python/lcm/__init__.py
)

install(FILES lcm/__init__.py DESTINATION lcm)
