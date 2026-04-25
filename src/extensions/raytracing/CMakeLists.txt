set(WISDOM_RAYTRACING_SOURCES)

if(WISDOM_DX12)
  list(APPEND WISDOM_RAYTRACING_SOURCES
       "raytracing/dx12/dx12_raytracing.cpp")
endif()

if(WISDOM_VULKAN)
  list(
    APPEND
    WISDOM_RAYTRACING_SOURCES
    "raytracing/vulkan/vk_raytracing.cpp")
endif()

# Platform header target
add_library(wisdom-raytracing-headers INTERFACE)
add_library(wis::wisdom-raytracing-headers ALIAS wisdom-raytracing-headers)

target_include_directories(
  wisdom-raytracing-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(wisdom-raytracing-headers INTERFACE wis::wisdom-headers)
target_compile_definitions(
  wisdom-raytracing-headers INTERFACE WISDOM_RAYTRACING_STATIC=1)

install(
  TARGETS wisdom-raytracing-headers
  EXPORT wisdom-raytracing-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Platform library target
if(WISDOM_BUILD_STATIC)
  add_library(wisdom-raytracing STATIC ${WISDOM_RAYTRACING_SOURCES})
  add_library(wis::wisdom-raytracing ALIAS wisdom-raytracing)

  target_link_libraries(wisdom-raytracing PRIVATE wisdom)
  target_compile_definitions(wisdom-raytracing PUBLIC WISDOM_RAYTRACING_STATIC=1)
  target_include_directories(
    wisdom-raytracing PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  set_target_properties(
    wisdom-raytracing PROPERTIES CXX_STANDARD 20
                               DEBUG_POSTFIX d)

  install(
    TARGETS wisdom-raytracing
    EXPORT wisdom-raytracing-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

if(WISDOM_BUILD_SHARED)
  add_library(wisdom-raytracing-shared SHARED ${WISDOM_RAYTRACING_SOURCES})
  add_library(wis::wisdom-raytracing-shared ALIAS wisdom-raytracing-shared)

  target_link_libraries(wisdom-raytracing-shared PRIVATE wisdom-shared)
  target_include_directories(
    wisdom-raytracing-shared
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_compile_definitions(
    wisdom-raytracing-shared
    PUBLIC WISDOM_RAYTRACING_SHARED_LIBRARY=1
    PRIVATE raytracing_shared_EXPORTS=1)

  set_target_properties(
    wisdom-raytracing-shared
    PROPERTIES CXX_STANDARD 20
               POSITION_INDEPENDENT_CODE ON
               DEBUG_POSTFIX d)

  include(GenerateExportHeader)
  generate_export_header(
    wisdom-raytracing-shared
    BASE_NAME
    WISDOM_RAYTRACING
    EXPORT_MACRO_NAME
    WISDOM_RAYTRACING_API
    EXPORT_FILE_NAME
    ${CMAKE_CURRENT_SOURCE_DIR}/raytracing/generated/wisdom_exports.h
    STATIC_DEFINE
    WISDOM_RAYTRACING_STATIC
    INCLUDE_GUARD_NAME
    WISDOM_RAYTRACING_EXPORTS_H)

  install(
    TARGETS wisdom-raytracing-shared
    EXPORT wisdom-raytracing-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/raytracing/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/raytracing)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)

install(
  EXPORT wisdom-raytracing-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
  NAMESPACE wis::
  FILE wisdom-raytracing-targets.cmake)
