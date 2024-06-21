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
    wisdom-extended-allocation STATIC "wisdom/wisdom_extended_allocation.h"
                                      "wisdom/wisdom_extended_allocation.cpp")
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

install(
  TARGETS wisdom-extended-allocation wisdom-extended-allocation-headers
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)
