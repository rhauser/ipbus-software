
add_library(cactus_uhal_uhal
  src/common/Buffers.cpp
  src/common/ClientFactory.cpp
  src/common/ClientInterface.cpp
  src/common/ConnectionManager.cpp
  src/common/DerivedNodeFactory.cpp
  src/common/HwInterface.cpp
  src/common/IPbusInspector.cpp
  src/common/Node.cpp
  src/common/NodeTreeBuilder.cpp
  src/common/ProtocolControlHub.cpp
  src/common/ProtocolIPbusCore.cpp
  src/common/ProtocolIPbus.cpp
  src/common/ProtocolMmap.cpp
  src/common/ProtocolPCIe.cpp
  src/common/ProtocolTCP.cpp
  src/common/ProtocolUDP.cpp
  src/common/SigBusGuard.cpp
  src/common/ValMem.cpp

  src/common/detail/PacketFmt.cpp
  src/common/detail/RobustSessionMutex.cpp
  src/common/detail/SharedObject.cpp
  src/common/detail/utilities.cpp
  src/common/detail/PacketFmt.cpp
  src/common/detail/RobustSessionMutex.cpp
  src/common/detail/SharedObject.cpp
  src/common/detail/utilities.cpp

  src/common/utilities/bits.cpp
  src/common/utilities/files.cpp
  src/common/utilities/TimeIntervalStats.cpp
  src/common/utilities/xml.cpp
  )

target_sources(cactus_uhal_uhal PUBLIC FILE_SET HEADERS
  BASE_DIRS include
  FILES
  include/uhal/ClientFactory.hpp
  include/uhal/uhal.hpp
  include/uhal/version.hpp
  include/uhal/ProtocolIPbus.hpp
  include/uhal/DerivedNode.hpp
  include/uhal/SigBusGuard.hpp
  include/uhal/ValMem.hpp
  include/uhal/utilities/xml.hpp
  include/uhal/utilities/TimeIntervalStats.hpp
  include/uhal/utilities/bits.hpp
  include/uhal/utilities/files.hpp
  include/uhal/ProtocolUDP.hpp
  include/uhal/Utilities.hpp
  include/uhal/TemplateDefinitions/DerivedNode.hxx
  include/uhal/TemplateDefinitions/ValMem.hxx
  include/uhal/TemplateDefinitions/Node.hxx
  include/uhal/TemplateDefinitions/XmlParser.hxx
  include/uhal/TemplateDefinitions/HwInterface.hxx
  include/uhal/TemplateDefinitions/ClientFactory.hxx
  include/uhal/TemplateDefinitions/Buffers.hxx
  include/uhal/TemplateDefinitions/DerivedNodeFactory.hxx
  include/uhal/NodeTreeBuilder.hpp
  include/uhal/detail/RobustSessionMutex.hpp
  include/uhal/detail/PacketFmt.hpp
  include/uhal/detail/utilities.hpp
  include/uhal/detail/SharedObject.hpp
  include/uhal/Node.hpp
  include/uhal/ProtocolMmap.hpp
  include/uhal/XmlParser.hpp
  include/uhal/exception.hpp
  include/uhal/HwInterface.hpp
  include/uhal/Buffers.hpp
  include/uhal/ProtocolIPbusCore.hpp
  include/uhal/definitions.hpp
  include/uhal/ProtocolTCP.hpp
  include/uhal/DerivedNodeFactory.hpp
  include/uhal/ConnectionManager.hpp
  include/uhal/ProtocolControlHub.hpp
  include/uhal/IPbusInspector.hpp
  include/uhal/ProtocolPCIe.hpp
  include/uhal/ClientInterface.hpp
  )

find_package(Threads REQUIRED)
target_link_libraries(cactus_uhal_uhal PUBLIC
  cactus_uhal_grammars
  cactus_uhal_log
  pugixml::shared
  Boost::headers
  Boost::filesystem
  Boost::system
  Boost::chrono
  Boost::date_time)

# target_compile_options(cactus_uhal_uhal PRIVATE -fdelayed-template-parsing -Wno-c++-extensions)

install(TARGETS cactus_uhal_uhal
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
  )