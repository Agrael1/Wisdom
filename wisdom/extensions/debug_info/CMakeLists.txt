project(wisdom-debug)

add_library(wisdom-debug-headers INTERFACE)
add_library(wis::debug-headers ALIAS wisdom-debug-headers)

target_link_libraries(wisdom-debug-headers INTERFACE wisdom-headers)

target_include_directories(
  wisdom-debug-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(WISDOM_BUILD_BINARIES)
  add_library(wisdom-debug STATIC "wisdom/wisdom_debug.hpp"
                                  "wisdom/wisdom_debug.cpp")
  add_library(wis::debug ALIAS wisdom-debug)
  target_link_libraries(wisdom-debug PUBLIC wis::wisdom)
  target_include_directories(
    wisdom-debug PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
else()
  add_library(wisdom-debug INTERFACE)
  add_library(wis::debug ALIAS wisdom-debug)
  target_link_libraries(wisdom-debug INTERFACE wisdom-debug-headers)
endif()

# link the debug library to the main wisdom extension library
target_link_libraries(wisdom-extension INTERFACE wisdom-debug)
target_link_libraries(wisdom-extension-headers INTERFACE wisdom-debug-headers)

install(
  TARGETS wisdom-debug wisdom-debug-headers
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)
