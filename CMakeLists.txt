
cmake_minimum_required(VERSION 3.26)

#
# You can override the following from the command line with
# cmake -D BOOST_MINIMAL_VERSION=... -B build
#
set(IPBUS_VERSION 2.17.0 CACHE STRING "Version of IPBus")
set(BOOST_MINIMAL_VERSION 1.86.0 CACHE STRING "Minimum version of Boost to use")
set(PUGIXML_MINIMAL_VERSION 1.13 CACHE STRING "Minimal version of pugixml to use")

# calculate ABI version
string(REGEX MATCH "[0-9]+\.[0-9]+" IPBUS_ABI_VERSION ${IPBUS_VERSION})
message(STATUS "IPBus ${IPBUS_VERSION} ${IPBUS_ABI_VERSION}")

project(IPBus VERSION ${IPBUS_VERSION} LANGUAGES C CXX)

include(CTest)
# include(CTestUseLaunchers)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS TRUE)

include(FetchContent)

FetchContent_Declare(pugixml
  URL https://github.com/zeux/pugixml/releases/download/v${PUGIXML_MINIMAL_VERSION}/pugixml-${PUGIXML_MINIMAL_VERSION}.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  FIND_PACKAGE_ARGS ${PUGIXML_MINIMAL_VERSION})

set(BOOST_INCLUDE_LIBRARIES thread filesystem system chrono date_time program_options test)
set(BOOST_ENABLE_CMAKE ON)

FetchContent_Declare(Boost
  URL https://github.com/boostorg/boost/releases/download/boost-${BOOST_MINIMAL_VERSION}/boost-${BOOST_MINIMAL_VERSION}-cmake.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
  FIND_PACKAGE_ARGS ${BOOST_MINIMAL_VERSION}
  )

FetchContent_MakeAvailable(pugixml Boost)

add_subdirectory(uhal)
