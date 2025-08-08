
cmake_minimum_required(VERSION 3.26)

#
# You can override the following from the command line with
#   cmake -D BOOST_MINIMAL_VERSION=... [...]
#
# To force a given Python installation use
#   cmake -D Python_EXECUTABLE=/usr/bin/python3.13 [...]
#
set(IPBUS_VERSION 2.17.0 CACHE STRING "Version of IPBus")
set(BOOST_MINIMAL_VERSION 1.86.0 CACHE STRING "Minimum version of Boost to use")
set(PUGIXML_MINIMAL_VERSION 1.13 CACHE STRING "Minimal version of pugixml to use")
set(PYTHON_MINIMAL_VERSION 3.6 CACHE STRING "Minimal version of Python to use")

option(BUILD_UHAL "Build the C++ libraries" ON)
option(BUILD_CONTROLHUB "Build controlhub" OFF)

# calculate ABI version
string(REGEX MATCH "[0-9]+\.[0-9]+" IPBUS_ABI_VERSION ${IPBUS_VERSION})
message(STATUS "IPBus ${IPBUS_VERSION} ${IPBUS_ABI_VERSION}")

project(IPBus VERSION ${IPBUS_VERSION} LANGUAGES C CXX)

include(CTest)
include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

set(BUILD_SHARED_LIBS TRUE)

if(BUILD_UHAL)
  add_subdirectory(uhal)
endif()

if(BUILD_CONTROLHUB)
  add_subdirectory(controlhub)
endif()

include(CPack)