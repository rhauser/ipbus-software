
find_package(Python ${PYTHON_MINIMAL_VERSION} REQUIRED COMPONENTS Interpreter Development.Module)

Python_add_library(_core MODULE
  src/common/cactus_pycohal.cpp
  src/common/enums_logging.cpp
  src/common/exceptions.cpp)

target_include_directories(_core PRIVATE include pybind11/include ../tests/include)
target_link_libraries(_core PRIVATE
  cactus_uhal_grammars
  cactus_uhal_log
  cactus_uhal_uhal
  )

target_compile_options(_core PRIVATE -ftemplate-depth-128 -O0 -rdynamic -finline-functions -Wno-inline)
target_compile_definitions(_core PRIVATE -DNDEBUG)

install(TARGETS _core LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
