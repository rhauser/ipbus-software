
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

add_subdirectory(grammars)
add_subdirectory(log)
add_subdirectory(uhal)
add_subdirectory(tests)
add_subdirectory(tools)
add_subdirectory(python)
# add_subdirectory(gui)
