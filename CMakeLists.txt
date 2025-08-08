
cmake_minimum_required(VERSION 3.26)

set(IPBUS_VERSION 2.17.0 CACHE STRING "Version of IPBus")

project(IPBus VERSION ${IPBUS_VERSION} LANGUAGES C CXX)

option(BUILD_UHAL "Build the C++ libraries" ON)
option(BUILD_CONTROLHUB "Build controlhub" OFF)

#
# You can override the following from the command line with
#   cmake -D BOOST_MINIMAL_VERSION=... [...]
#
# To force a given Python installation use
#   cmake -D Python_EXECUTABLE=/usr/bin/python3.13 [...]
#
set(BOOST_MINIMAL_VERSION 1.86.0 CACHE STRING "Minimum version of Boost to use")
set(PUGIXML_MINIMAL_VERSION 1.13 CACHE STRING "Minimal version of pugixml to use")
set(PYTHON_MINIMAL_VERSION 3.6 CACHE STRING "Minimal version of Python to use")

if(NOT DEFINED CPACK_PACKAGING_INSTALL_PREFIX)
  set(CPACK_PACKAGING_INSTALL_PREFIX "/opt/cactus")
endif()

# calculate ABI version
string(REGEX MATCH "[0-9]+\.[0-9]+" IPBUS_ABI_VERSION ${IPBUS_VERSION})

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

# RPM generation fine tuning
set(CPACK_RPM_PACKAGE_RELOCATABLE TRUE)
set(CPACK_RPM_PACKAGE_RELEASE_DIST TRUE)

cmake_host_system_information(RESULT os_id QUERY DISTRIB_ID)
cmake_host_system_information(RESULT os_version QUERY DISTRIB_VERSION_ID)

if(${os_id} STREQUAL "almalinux" OR
    ${os_id} STREQUAL "rhel" OR
    ${os_id} STREQUAL "rocky" OR
    ${os_id} STREQUAL "ol")
  set(os_id "el")
endif()

if(NOT ${os_id} STREQUAL "ubuntu")
  string(REGEX MATCH "[0-9]+" os_version ${os_version})
endif()

set(CPACK_RPM_FILE_NAME ${PROJECT_NAME}-${PROJECT_VERSION}.${os_id}${os_version}.${CMAKE_HOST_SYSTEM_PROCESSOR}.rpm)

include(CPack)