set(WISDOM_VIDEO_SOURCES)

if(WISDOM_DX12)
  list(APPEND WISDOM_VIDEO_SOURCES
       "video/dx12/dx12_video.cpp")
endif()

if(WISDOM_VULKAN)
  list(
    APPEND
    WISDOM_VIDEO_SOURCES
    "video/vulkan/vk_video.cpp")
endif()

# Platform header target
add_library(wisdom-video-headers INTERFACE)
add_library(wis::wisdom-video-headers ALIAS wisdom-video-headers)

target_include_directories(
  wisdom-video-headers
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(wisdom-video-headers INTERFACE wis::wisdom-headers)
target_compile_definitions(
  wisdom-video-headers INTERFACE WISDOM_VIDEO_STATIC=1)

install(
  TARGETS wisdom-video-headers
  EXPORT wisdom-video-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Platform library target
if(WISDOM_BUILD_STATIC)
  add_library(wisdom-video STATIC ${WISDOM_VIDEO_SOURCES})
  add_library(wis::wisdom-video ALIAS wisdom-video)

  target_link_libraries(wisdom-video PRIVATE wisdom)
  target_compile_definitions(wisdom-video PUBLIC WISDOM_VIDEO_STATIC=1)
  target_include_directories(
    wisdom-video PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  set_target_properties(
    wisdom-video PROPERTIES CXX_STANDARD 20
                               DEBUG_POSTFIX d)

  install(
    TARGETS wisdom-video
    EXPORT wisdom-video-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

if(WISDOM_BUILD_SHARED)
  add_library(wisdom-video-shared SHARED ${WISDOM_VIDEO_SOURCES})
  add_library(wis::wisdom-video-shared ALIAS wisdom-video-shared)

  target_link_libraries(wisdom-video-shared PRIVATE wisdom-shared)
  target_include_directories(
    wisdom-video-shared
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
  target_compile_definitions(
    wisdom-video-shared
    PUBLIC WISDOM_VIDEO_SHARED_LIBRARY=1
    PRIVATE video_shared_EXPORTS=1)

  set_target_properties(
    wisdom-video-shared
    PROPERTIES CXX_STANDARD 20
               POSITION_INDEPENDENT_CODE ON
               DEBUG_POSTFIX d)

  include(GenerateExportHeader)
  generate_export_header(
    wisdom-video-shared
    BASE_NAME
    WISDOM_VIDEO
    EXPORT_MACRO_NAME
    WISDOM_VIDEO_API
    EXPORT_FILE_NAME
    ${CMAKE_CURRENT_SOURCE_DIR}/video/generated/wisdom_exports.h
    STATIC_DEFINE
    WISDOM_VIDEO_STATIC
    INCLUDE_GUARD_NAME
    WISDOM_VIDEO_EXPORTS_H)

  install(
    TARGETS wisdom-video-shared
    EXPORT wisdom-video-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/video/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/video)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)

install(
  EXPORT wisdom-video-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
  NAMESPACE wis::
  FILE wisdom-video-targets.cmake)