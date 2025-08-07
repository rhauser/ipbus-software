
add_library(cactus_uhal_grammars
  src/common/HttpResponseGrammar.cpp
  src/common/NodeTreeClassAttributeGrammar.cpp
  src/common/NodeTreeFirmwareInfoAttributeGrammar.cpp
  src/common/NodeTreeParametersGrammar.cpp
  src/common/SemicolonDelimitedUriListGrammar.cpp
  src/common/URI.cpp
  src/common/URIGrammar.cpp)

target_sources(cactus_uhal_grammars PUBLIC FILE_SET HEADERS
  BASE_DIRS include
  FILES
  include/uhal/grammars/NodeTreeFirmwareInfoAttributeGrammar.hpp
  include/uhal/grammars/URI.hpp
  include/uhal/grammars/NodeTreeClassAttributeGrammar.hpp
  include/uhal/grammars/SemicolonDelimitedUriListGrammar.hpp
  include/uhal/grammars/HttpResponseGrammar.hpp
  include/uhal/grammars/URIGrammar.hpp
  include/uhal/grammars/NodeTreeParametersGrammar.hpp
)

target_link_libraries(cactus_uhal_grammars PUBLIC Boost::headers)

install(TARGETS cactus_uhal_grammars
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})