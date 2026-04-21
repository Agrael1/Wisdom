set(WISDOM_CORE_SOURCES)
set(WISDOM_CORE_LIBS)
set(WISDOM_CORE_DEFINITIONS
    NOMINMAX
    $<$<BOOL:${WISDOM_WINDOWS}>:WISDOM_WINDOWS=1>
    $<$<BOOL:${WISDOM_LINUX}>:WISDOM_LINUX=1>
    $<BUILD_INTERFACE:$<$<BOOL:${WISDOM_DX12}>:WISDOM_DX12=1>>
    $<BUILD_INTERFACE:$<$<BOOL:${WISDOM_VULKAN}>:WISDOM_VULKAN=1>>)

if(WISDOM_DX12)
  list(
    APPEND
    WISDOM_CORE_LIBS
    DXGI
    DXGUID
    d3d12
    DX12Allocator)

  list(
    APPEND
    WISDOM_CORE_SOURCES
    "wisdom/dx12/dx12_impl.cpp"
    "wisdom/dx12/dx12_instance.cpp"
    "wisdom/dx12/dx12_adapter_query.cpp"
    "wisdom/dx12/dx12_device.cpp"
    "wisdom/dx12/dx12_fence.cpp"
    "wisdom/dx12/dx12_command_queue.cpp"
    "wisdom/dx12/dx12_command_list.cpp"
    "wisdom/dx12/dx12_descriptor_heap.cpp"
    "wisdom/dx12/dx12_resource_allocator.cpp"
    "wisdom/dx12/dx12_command_allocator.cpp"
    "wisdom/dx12/dx12_swapchain.cpp"
    "wisdom/dx12/dx12_pipeline_cache.cpp")
endif()

if(WISDOM_VULKAN)
  list(APPEND WISDOM_CORE_LIBS vkma)
  list(
    APPEND
    WISDOM_CORE_SOURCES
    "wisdom/vulkan/vk_device.cpp"
    "wisdom/vulkan/vk_fence.cpp"
    "wisdom/vulkan/vk_extensions.cpp"
    "wisdom/vulkan/vk_impl.cpp"
    "wisdom/vulkan/vk_instance.cpp"
    "wisdom/vulkan/vk_adapter_query.cpp"
    "wisdom/vulkan/vk_command_queue.cpp"
    "wisdom/vulkan/vk_command_list.cpp"
    "wisdom/vulkan/vk_descriptor_heap.cpp"
    "wisdom/vulkan/vk_resource_allocator.cpp"
    "wisdom/vulkan/vk_command_allocator.cpp"
    "wisdom/vulkan/vk_swapchain.cpp"
    "wisdom/vulkan/vk_pipeline_cache.cpp")
endif()

# header target
add_library(wisdom-headers INTERFACE "../natvis/span.natvis"
            "../natvis/opaque_handles.natvis")
add_library(wis::wisdom-headers ALIAS wisdom-headers)

target_include_directories(
  wisdom-headers INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(wisdom-headers INTERFACE ${WISDOM_CORE_LIBS})

target_compile_definitions(wisdom-headers INTERFACE ${WISDOM_CORE_DEFINITIONS}
                                                    WISDOM_HEADER_ONLY)
install(
  TARGETS wisdom-headers
  EXPORT wisdom-targets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# static build
if(WISDOM_BUILD_STATIC)
  add_library(wisdom STATIC ${WISDOM_CORE_SOURCES})
  add_library(wis::wisdom ALIAS wisdom)

  target_link_libraries(wisdom PUBLIC ${WISDOM_CORE_LIBS})
  target_compile_definitions(wisdom PUBLIC ${WISDOM_CORE_DEFINITIONS})
  target_include_directories(
    wisdom PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  set_target_properties(wisdom PROPERTIES CXX_STANDARD 20 # UNITY_BUILD ON
                                          DEBUG_POSTFIX d)

  install(
    TARGETS wisdom
    EXPORT wisdom-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

if(WISDOM_BUILD_SHARED)
  add_library(wisdom-shared SHARED ${WISDOM_CORE_SOURCES})
  add_library(wis::wisdom-shared ALIAS wisdom-shared)

  target_link_libraries(wisdom-shared PUBLIC ${WISDOM_CORE_LIBS})
  target_compile_definitions(
    wisdom-shared
    PUBLIC WISDOM_SHARED_LIBRARY=1 ${WISDOM_CORE_DEFINITIONS}
    PRIVATE wisdom_shared_EXPORTS=1)
  target_include_directories(
    wisdom-shared PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
                         $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

  set_target_properties(
    wisdom-shared
    PROPERTIES CXX_STANDARD 20
               POSITION_INDEPENDENT_CODE ON
               DEBUG_POSTFIX d)

  include(GenerateExportHeader)
  generate_export_header(
    wisdom-shared
    BASE_NAME
    WISDOM
    EXPORT_MACRO_NAME
    WISDOM_API
    EXPORT_FILE_NAME
    ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/generated/wisdom_exports.h
    STATIC_DEFINE
    WISDOM_STATIC
    INCLUDE_GUARD_NAME
    WISDOM_CORE_EXPORTS_H)

  install(
    TARGETS wisdom-shared
    EXPORT wisdom-targets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)

install(
  EXPORT wisdom-targets
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
  NAMESPACE wis::
  FILE wisdom-targets.cmake)
