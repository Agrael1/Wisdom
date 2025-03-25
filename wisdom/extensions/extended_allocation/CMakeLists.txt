project(wisdom-extended-allocation)

add_library(wisdom-extended-allocation-headers INTERFACE)
add_library(wis::extended-allocation-headers ALIAS
            wisdom-extended-allocation-headers)

target_link_libraries(wisdom-extended-allocation-headers
                      INTERFACE wisdom-headers)

target_include_directories(
  wisdom-extended-allocation-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(WISDOM_BUILD_BINARIES)
  add_library(
    wisdom-extended-allocation STATIC
    "wisdom/wisdom_extended_allocation.hpp" "wisdom/impl/impl.dx.cpp"
    "wisdom/impl/impl.vk.cpp")
  add_library(wis::extended-allocation ALIAS wisdom-extended-allocation)
  target_link_libraries(wisdom-extended-allocation PUBLIC wis::wisdom)
  target_include_directories(
    wisdom-extended-allocation
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
else()
  add_library(wisdom-extended-allocation INTERFACE)
  add_library(wis::extended-allocation ALIAS wisdom-extended-allocation)
  target_link_libraries(wisdom-extended-allocation
                        INTERFACE wisdom-extended-allocation-headers)
endif()

# link the debug library to the main wisdom extension library
target_link_libraries(wisdom-extension INTERFACE wisdom-extended-allocation)
target_link_libraries(wisdom-extension-headers
                      INTERFACE wisdom-extended-allocation-headers)

if(WISDOM_CPP_MODULES_SUPPORTED)
  add_library(wisdom-extended-allocation-module STATIC
              "wisdom/extended_allocation.include.h")
  add_library(wis::extended-allocation-module ALIAS
              wisdom-extended-allocation-module)
  target_sources(
    wisdom-extended-allocation-module
    PUBLIC FILE_SET CXX_MODULES FILES "wisdom/wisdom_extended_allocation.ixx"
           "wisdom/wisdom_extended_allocation.fvk.ixx")
  target_link_libraries(wisdom-extended-allocation-module
                        PUBLIC wisdom-module wisdom-extended-allocation-headers)
  set_target_properties(wisdom-extended-allocation-module
                        PROPERTIES CXX_STANDARD 20 CXX_SCAN_FOR_MODULES ON)
endif()

install(
  TARGETS wisdom-extended-allocation wisdom-extended-allocation-headers
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(WISDOM_CPP_MODULES_SUPPORTED)
  install(
    TARGETS wisdom-extended-allocation-module
    EXPORT wisdom-module-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)
