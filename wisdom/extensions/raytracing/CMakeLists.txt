project(wisdom-raytracing)

add_library(wisdom-raytracing-headers INTERFACE)
add_library(wis::raytracing-headers ALIAS wisdom-raytracing-headers)

target_link_libraries(wisdom-raytracing-headers INTERFACE wisdom-headers)

target_include_directories(
  wisdom-raytracing-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/extensions/raytracing>)

if(WISDOM_BUILD_BINARIES)
  add_library(
    wisdom-raytracing STATIC
    "wisdom/wisdom_raytracing.hpp"
    "wisdom/impl/impl.vk.cpp"
    "wisdom/vk_rtas.h"
    "wisdom/impl.dx12.h"
    "wisdom/impl.vk.h"
    "wisdom/impl.h"
    "wisdom/dx12_raytracing_pipeline.h"
    "wisdom/impl/impl.dx12.cpp"
    "wisdom/vk_raytracing_pipeline.h")
  add_library(wis::raytracing ALIAS wisdom-raytracing)
  target_link_libraries(wisdom-raytracing PUBLIC wis::wisdom)
  target_include_directories(
    wisdom-raytracing PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                             $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/extensions/raytracing>)
else()
  add_library(wisdom-raytracing INTERFACE)
  add_library(wis::raytracing ALIAS wisdom-raytracing)
  target_link_libraries(wisdom-raytracing INTERFACE wisdom-raytracing-headers)
endif()

# link the debug library to the main wisdom extension library
target_link_libraries(wisdom-extension INTERFACE wisdom-raytracing)
target_link_libraries(wisdom-extension-headers
                      INTERFACE wisdom-raytracing-headers)

if(WISDOM_CPP_MODULES_SUPPORTED)
  add_library(wisdom-raytracing-module STATIC "wisdom/raytracing.include.h")
  add_library(wis::raytracing-module ALIAS wisdom-raytracing-module)
  target_sources(
    wisdom-raytracing-module
    PUBLIC FILE_SET CXX_MODULES FILES "wisdom/wisdom_raytracing.ixx"
           "wisdom/wisdom_raytracing.fvk.ixx")
  target_link_libraries(wisdom-raytracing-module
                        PUBLIC wisdom-module wisdom-raytracing-headers)
  set_target_properties(wisdom-raytracing-module
                        PROPERTIES CXX_STANDARD 20 CXX_SCAN_FOR_MODULES ON)
endif()

install(
  TARGETS wisdom-raytracing wisdom-raytracing-headers
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/extensions/raytracing/wisdom)

if(WISDOM_CPP_MODULES_SUPPORTED)
  install(
    TARGETS wisdom-raytracing-module
    EXPORT wisdom-module-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/extensions/raytracing)
endif()

if(WISDOM_GENERATE_FUNCTIONS)
  if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    # Generate a shell script for Linux
    set(LAUNCHER_SCRIPT
        "#!/bin/sh\nexec \"\$<TARGET_FILE:WisVk_generator>\" -l -i ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/gen/vk_functions.in -o ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/generated/vk_functions.hpp \"$@\"\n"
    )
    file(
      GENERATE
      OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/wisdom/gen/generate_functions.sh"
      CONTENT "${LAUNCHER_SCRIPT}")
    execute_process(
      COMMAND chmod +x
              ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/gen/generate_functions.sh)
  elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    # Generate a batch file for Windows
    set(LAUNCHER_SCRIPT
        "@echo off\nstart /B \"\" \"\$<TARGET_FILE:WisVk_generator>\" -l -i ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/gen/vk_functions.in -o ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/generated/vk_functions.hpp %*\n"
    )
    file(
      GENERATE
      OUTPUT "${CMAKE_CURRENT_SOURCE_DIR}/wisdom/gen/generate_functions.bat"
      CONTENT "${LAUNCHER_SCRIPT}")
  endif()

  if(WISDOM_GENERATE_API)
    add_custom_target(
      GenerateFunctions_${PROJECT_NAME}
      COMMAND
        $<TARGET_FILE:WisVk_generator> -l -i
        ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/gen/vk_functions.in -o
        ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/generated/vk_functions.hpp)
    add_dependencies(wisdom-descriptor-buffer-headers
                     GenerateFunctions_${PROJECT_NAME})
  endif()
endif()
