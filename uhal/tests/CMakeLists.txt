
add_library(cactus_uhal_tests
  src/common/DummyClient.cpp
  src/common/DummyDerivedNode.cpp
  src/common/DummyHardware.cpp
  src/common/fixtures.cpp
  src/common/PCIeDummyHardware.cpp
  src/common/PerfTester_static.cpp
  src/common/TCPDummyHardware.cpp
  src/common/test_block.cpp
  src/common/test_check_permissions.cpp
  src/common/test_client_factory.cpp
  src/common/test_config_space.cpp
  src/common/test_empty_dispatch.cpp
  src/common/test_hierarchy.cpp
  src/common/test_masking.cpp
  src/common/test_multithreaded.cpp
  src/common/test_nodes.cpp
  src/common/test_nonreachable.cpp
  src/common/test_rawclient.cpp
  src/common/test_single.cpp
  src/common/test_soak.cpp
  src/common/test_timeout.cpp
  src/common/test_uri.cpp
  src/common/tools.cpp
  src/common/UDPDummyHardware.cpp)

target_sources(cactus_uhal_tests PUBLIC FILE_SET HEADERS
  BASE_DIRS include
  FILES
  include/uhal/tests/definitions.hpp
  include/uhal/tests/DummyClient.hpp
  include/uhal/tests/DummyDerivedNode.hpp
  include/uhal/tests/DummyHardware.hpp
  include/uhal/tests/DummyHardwareOptions.hpp
  include/uhal/tests/fixtures.hpp
  include/uhal/tests/PCIeDummyHardware.hpp
  # include/uhal/tests/PerfTester.hxx
  include/uhal/tests/TCPDummyHardware.hpp
  include/uhal/tests/tools.hpp
  include/uhal/tests/UDPDummyHardware.hpp)

target_compile_definitions(cactus_uhal_tests PRIVATE BOOST_TEST_DYN_LINK)

target_link_libraries(cactus_uhal_tests PUBLIC
  cactus_uhal_uhal
  cactus_uhal_grammars
  cactus_uhal_log
  Boost::filesystem
  Boost::program_options
  Boost::system
  Boost::unit_test_framework
  )

set_target_properties(cactus_uhal_tests PROPERTIES VERSION ${IPBUS_VERSION})
set_target_properties(cactus_uhal_tests PROPERTIES SOVERSION ${IPBUS_ABI_VERSION})

add_executable(DummyHardwareTcp src/common/DummyHardwareTcp.cxx)
target_link_libraries(DummyHardwareTcp PRIVATE cactus_uhal_tests)
add_test(NAME DummyHardwareTcp COMMAND DummyHardwareTcp)

add_executable(DummyHardwareUdp src/common/DummyHardwareUdp.cxx)
target_link_libraries(DummyHardwareUdp PRIVATE cactus_uhal_tests)
add_test(NAME DummyHardwareUdp COMMAND DummyHardwareUdp)

add_executable(PerfTester src/common/PerfTester.cxx)
target_link_libraries(PerfTester PRIVATE cactus_uhal_tests)
add_test(NAME PerfTester COMMAND PerfTester)

add_executable(run_uhal_tests src/common/run_uhal_tests.cxx)
target_link_libraries(run_uhal_tests PRIVATE cactus_uhal_tests)
add_test(NAME run_uhal_tests COMMAND run_uhal_tests)

add_executable(test_log src/common/test_log.cxx)
target_link_libraries(test_log PRIVATE cactus_uhal_tests)
add_test(NAME test_log COMMAND test_log)

install(TARGETS cactus_uhal_tests
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})