project(wisdom-raytracing)

add_library(wisdom-raytracing-headers INTERFACE)
add_library(wis::raytracing-headers ALIAS
            wisdom-raytracing-headers)

target_link_libraries(wisdom-raytracing-headers
                      INTERFACE wisdom-headers)

target_include_directories(
  wisdom-raytracing-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

if(WISDOM_BUILD_BINARIES)
  add_library(
    wisdom-raytracing STATIC
    "wisdom/wisdom_raytracing.hpp"
    "wisdom/impl/impl.vk.cpp"
   
   "wisdom/vk_rtas.h")
  add_library(wis::raytracing ALIAS wisdom-raytracing)
  target_link_libraries(wisdom-raytracing PUBLIC wis::wisdom)
  target_include_directories(
    wisdom-raytracing
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
else()
  add_library(wisdom-raytracing INTERFACE)
  add_library(wis::raytracing ALIAS wisdom-raytracing)
  target_link_libraries(wisdom-raytracing
                        INTERFACE wisdom-raytracing-headers)
endif()

# link the debug library to the main wisdom extension library
target_link_libraries(wisdom-extension INTERFACE wisdom-raytracing)
target_link_libraries(wisdom-extension-headers
                      INTERFACE wisdom-raytracing-headers)

install(
  TARGETS wisdom-raytracing wisdom-raytracing-headers
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)
