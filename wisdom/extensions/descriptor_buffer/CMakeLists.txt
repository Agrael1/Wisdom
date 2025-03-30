project(wisdom-descriptor-buffer)

add_library(wisdom-descriptor-buffer-headers INTERFACE)
add_library(wis::descriptor-buffer-headers ALIAS
            wisdom-descriptor-buffer-headers)

target_link_libraries(wisdom-descriptor-buffer-headers INTERFACE wisdom-headers)

target_include_directories(
  wisdom-descriptor-buffer-headers
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/extensions/descriptor_buffer>
)

if(WISDOM_BUILD_BINARIES)
  add_library(
    wisdom-descriptor-buffer STATIC
    "wisdom/wisdom_descriptor_buffer.hpp"
    "wisdom/vk_descriptor_buffer.hpp"
    "wisdom/impl/vk_descriptor_buffer.cpp"
    "wisdom/dx12_descriptor_buffer.hpp"
    "wisdom/impl/dx12_descriptor_buffer.cpp"
    "wisdom/impl/impl.dx.cpp"
    "wisdom/impl/impl.vk.cpp")
  add_library(wis::descriptor-buffer ALIAS wisdom-descriptor-buffer)
  target_link_libraries(wisdom-descriptor-buffer PUBLIC wis::wisdom)
  target_include_directories(
    wisdom-descriptor-buffer
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/extensions/descriptor_buffer>
  )
else()
  add_library(wisdom-descriptor-buffer INTERFACE)
  add_library(wis::descriptor-buffer ALIAS wisdom-descriptor-buffer)
  target_link_libraries(wisdom-descriptor-buffer
                        INTERFACE wisdom-descriptor-buffer-headers)
endif()

if(WISDOM_CPP_MODULES_SUPPORTED)
  add_library(wisdom-descriptor-buffer-module STATIC
              "wisdom/descriptor_buffer.include.h")
  add_library(wis::descriptor-buffer-module ALIAS
              wisdom-descriptor-buffer-module)
  target_sources(
    wisdom-descriptor-buffer-module
    PUBLIC FILE_SET CXX_MODULES FILES "wisdom/wisdom_descriptor_buffer.ixx"
           "wisdom/wisdom_descriptor_buffer.fvk.ixx")
  target_link_libraries(wisdom-descriptor-buffer-module
                        PUBLIC wisdom-module wisdom-descriptor-buffer-headers)
  add_dependent_target(wisdom-descriptor-buffer-module PUBLIC WISDOM_VULKAN
                       wisdom-vk-module)
  add_dependent_target(wisdom-descriptor-buffer-module PUBLIC WISDOM_DX12
                       wisdom-dx12-module)

  set_target_properties(wisdom-descriptor-buffer-module
                        PROPERTIES CXX_STANDARD 20 CXX_SCAN_FOR_MODULES ON)
endif()

# link the debug library to the main wisdom extension library
target_link_libraries(wisdom-extension INTERFACE wisdom-descriptor-buffer)
target_link_libraries(wisdom-extension-headers
                      INTERFACE wisdom-descriptor-buffer-headers)

install(
  TARGETS wisdom-descriptor-buffer wisdom-descriptor-buffer-headers
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

if(WISDOM_CPP_MODULES_SUPPORTED)
  install(
    TARGETS wisdom-descriptor-buffer-module
    EXPORT wisdom-module-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET CXX_MODULES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/extensions/descriptor_buffer)
endif()

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/extensions/descriptor_buffer/wisdom)

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
