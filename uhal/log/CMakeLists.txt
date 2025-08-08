
add_library(cactus_uhal_log
  src/common/exception.cpp
  src/common/log.cpp
  src/common/log_inserters.integer.cpp
  src/common/log_inserters.location.cpp
  src/common/log_inserters.quote.cpp
  src/common/log_inserters.threadID.cpp
  src/common/log_inserters.time.cpp
  src/common/LogLevels.cpp
  )

target_sources(cactus_uhal_log PUBLIC FILE_SET HEADERS
  BASE_DIRS include
  FILES
  include/uhal/log/log_inserters.threadID.hpp
  include/uhal/log/log_inserters.quote.hxx
  include/uhal/log/log_inserters.quote.hpp
  include/uhal/log/log_inserters.time.hpp
  include/uhal/log/log_inserters.integer.hxx
  include/uhal/log/LogLevels.hpp
  include/uhal/log/log_inserters.time.hxx
  include/uhal/log/exception.hpp
  include/uhal/log/log_inserters.location.hpp
  include/uhal/log/log.hpp
  include/uhal/log/log_inserters.integer.hpp
  include/uhal/log/log_inserters.type.hpp
  include/uhal/log/log_inserters.hpp
  include/uhal/log/log_inserters.type.hxx
  include/uhal/log/log.hxx
  include/uhal/log/log_inserter_helper.hpp
  )

find_package(Threads REQUIRED)
target_link_libraries(cactus_uhal_log PUBLIC Threads::Threads)
set_target_properties(cactus_uhal_log PROPERTIES VERSION ${IPBUS_VERSION})
set_target_properties(cactus_uhal_log PROPERTIES SOVERSION ${IPBUS_ABI_VERSION})

add_executable(generator
  src/common/generator.cxx)

target_compile_definitions(generator PRIVATE MAX_NUM_ARGS=32)

install(TARGETS cactus_uhal_log generator
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})