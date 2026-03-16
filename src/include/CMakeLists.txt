wis_make_target_bundle(
        TARGET
        internal
        HEADERS
        "wisdom/global/definitions.h"
        "wisdom/global/internal.hpp"
        "wisdom/generated/c_api.h"
        "wisdom/generated/cpp_api.hpp"
        "wisdom/generated/wisdom_exports.h"
        "wisdom/bridge/span.hpp"
        "wisdom/bridge/source_location.hpp"
        "wisdom/bridge/format.hpp"
        "wisdom/util/com_ptr.hpp"
        "wisdom/util/string_literal.hpp"
        "wisdom/util/error_messages.hpp"
        "wisdom/util/allocation.hpp"
        "wisdom/util/xxhash.h"
        INCLUDE_DIRECTORIES
        "${CMAKE_CURRENT_SOURCE_DIR}"
        HEADER_ONLY
        COMPILE_DEFINITIONS
        NOMINMAX)

target_sources(wisdom-internal-headers PRIVATE
        "../natvis/span.natvis")

if (WISDOM_DX12)
    wis_make_target_bundle(
            TARGET
            dx12
            HEADERS
            "wisdom/dx12/dx12_types.hpp"
            "wisdom/dx12/detail/dx12_utils.hpp"
            "wisdom/dx12/detail/dx12_detail.hpp"
            "wisdom/generated/dx12_cpp_api.hpp"
            "wisdom/generated/dx12_convert.hpp"
            "wisdom/generated/dx12_api.h"
            SOURCES
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
            "wisdom/dx12/dx12_pipeline_cache.cpp"
            LINK_TARGETS
            wisdom-internal-headers
            DXGI
            DXGUID
            d3d12
            DX12Allocator
            DX12Agility
            COMPILE_DEFINITIONS
            $<BUILD_INTERFACE:WISDOM_DX12=1>)
endif ()

if (WISDOM_VULKAN)
    wis_make_target_bundle(
            TARGET
            vk
            HEADERS
            "wisdom/vulkan/vk_types.hpp"
            "wisdom/vulkan/detail/vk_utils.hpp"
            "wisdom/vulkan/detail/vk_detail.hpp"
            "wisdom/vulkan/detail/vk_ext1.hpp"
            "wisdom/vulkan/vk_loader.hpp"
            "wisdom/vulkan/vk_tables.hpp"
            "wisdom/vulkan/vk_extensions.hpp"
            "wisdom/generated/vk_cpp_api.hpp"
            "wisdom/generated/vk_api.h"
            SOURCES
            "wisdom/vulkan/vk_device.cpp"
            "wisdom/vulkan/vk_fence.cpp"
            "wisdom/vulkan/vk_types.cpp"
            "wisdom/vulkan/vk_impl.cpp"
            "wisdom/vulkan/vk_instance.cpp"
            "wisdom/vulkan/vk_adapter_query.cpp"
            "wisdom/vulkan/vk_command_queue.cpp"
            "wisdom/vulkan/vk_command_list.cpp"
            "wisdom/vulkan/vk_descriptor_heap.cpp"
            "wisdom/vulkan/vk_resource_allocator.cpp"
            "wisdom/vulkan/vk_command_allocator.cpp"
            "wisdom/vulkan/vk_pipeline_cache.cpp"
            LINK_TARGETS
            wisdom-internal-headers
            vkma
            COMPILE_DEFINITIONS
            $<BUILD_INTERFACE:WISDOM_VULKAN=1>)
endif ()

# Unified targets Create a convenience target for all headers
add_library(wisdom-headers INTERFACE)
add_library(wis::wisdom-headers ALIAS wisdom-headers)
target_link_libraries(wisdom-headers INTERFACE wisdom-internal-headers)
if (WISDOM_DX12)
    target_link_libraries(wisdom-headers INTERFACE wisdom-dx12-headers)
endif ()
if (WISDOM_VULKAN)
    target_link_libraries(wisdom-headers INTERFACE wisdom-vk-headers)
endif ()

if (WISDOM_WINDOWS)
    target_compile_definitions(wisdom-headers
            INTERFACE $<BUILD_INTERFACE:WISDOM_WINDOWS=1>)
endif ()
if (WISDOM_LINUX)
    target_compile_definitions(wisdom-headers
            INTERFACE $<BUILD_INTERFACE:WISDOM_LINUX=1>)
endif ()

# Install everything, including .cpp for header only usage
install(
        TARGETS wisdom-headers
        EXPORT wisdom-targets
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        INCLUDES
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)

# Create a convenience target for all static libraries
if (WISDOM_BUILD_STATIC)
    add_library(wisdom STATIC)
    add_library(wis::wisdom ALIAS wisdom)
    target_link_libraries(wisdom PUBLIC wisdom-headers)
    if (WISDOM_DX12)
        target_sources(wisdom PRIVATE $<TARGET_OBJECTS:wisdom-dx12-objects>)
    endif ()
    if (WISDOM_VULKAN)
        target_sources(wisdom PRIVATE $<TARGET_OBJECTS:wisdom-vk-objects>)
    endif ()
    target_compile_definitions(wisdom PUBLIC WISDOM_BUILD_BINARIES=1)
    set_target_properties(wisdom PROPERTIES CXX_STANDARD 20 DEBUG_POSTFIX d)

    install(
            TARGETS wisdom
            EXPORT wisdom-targets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()

# Create a convenience target for all dynamic libraries
if (WISDOM_BUILD_SHARED)
    add_library(wisdom-shared SHARED)
    add_library(wis::wisdom-shared ALIAS wisdom-shared)
    target_link_libraries(wisdom-shared PUBLIC wisdom-headers)
    if (WISDOM_DX12)
        target_sources(wisdom-shared
                PRIVATE $<TARGET_OBJECTS:wisdom-dx12-objects-shared>)
    endif ()
    if (WISDOM_VULKAN)
        target_sources(wisdom-shared
                PRIVATE $<TARGET_OBJECTS:wisdom-vk-objects-shared>)
    endif ()
    target_compile_definitions(wisdom-shared PUBLIC WISDOM_BUILD_BINARIES=1
            WISDOM_SHARED_LIBRARY=1)
    set_target_properties(
            wisdom-shared
            PROPERTIES CXX_STANDARD 20
            POSITION_INDEPENDENT_CODE ON
            DEBUG_POSTFIX d)

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
            WISDOM_EXPORTS_H)

    install(
            TARGETS wisdom-shared
            EXPORT wisdom-targets
            RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            INCLUDES
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()

install(
        EXPORT wisdom-targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
        NAMESPACE wis::
        FILE wisdom-targets.cmake # Not sure if this is still needed
)
