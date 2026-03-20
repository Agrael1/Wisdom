wis_make_target_bundle(
        TARGET
        internal
        HEADERS
        "wisdom/global/definitions.h"
        "wisdom/global/internal.hpp"
        "wisdom/generated/c_api.h"
        "wisdom/generated/backend_api.h"
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
        EXPORT_SET
        wisdom-targets
        COMPILE_DEFINITIONS
        NOMINMAX)

target_sources(wisdom-internal-headers PRIVATE
        "../natvis/span.natvis")

if (WISDOM_WINDOWS)
    target_compile_definitions(wisdom-internal-headers INTERFACE $<BUILD_INTERFACE:WISDOM_WINDOWS=1>)
endif ()
if (WISDOM_LINUX)
    target_compile_definitions(wisdom-internal-headers INTERFACE $<BUILD_INTERFACE:WISDOM_LINUX=1>)
endif ()

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
            EXPORT_SET
            wisdom-targets
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
            SOURCES
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
            "wisdom/vulkan/vk_pipeline_cache.cpp"
            LINK_TARGETS
            wisdom-internal-headers
            vkma
            EXPORT_SET
            wisdom-targets
            COMPILE_DEFINITIONS
            $<BUILD_INTERFACE:WISDOM_VULKAN=1>)
endif ()

set(WISDOM_INCLUDE_BACKENDS)
if (WISDOM_DX12)
    list(APPEND WISDOM_INCLUDE_BACKENDS dx12)
endif ()
if (WISDOM_VULKAN)
    list(APPEND WISDOM_INCLUDE_BACKENDS vk)
endif ()


wis_make_unified_targets(
        TARGET wisdom
        EXPORT_SET wisdom-targets
        BACKENDS ${WISDOM_INCLUDE_BACKENDS}
        HEADER_LINK_TARGETS wisdom-internal-headers
        HEADER_COMPILE_DEFINITIONS WISDOM_PLATFORM_STATIC=1
        STATIC_COMPILE_DEFINITIONS WISDOM_BUILD_BINARIES=1
        SHARED_COMPILE_DEFINITIONS WISDOM_BUILD_BINARIES=1 WISDOM_SHARED_LIBRARY=1
        EXPORT_BASE_NAME WISDOM
        EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/generated/wisdom_exports.h)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/wisdom/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/wisdom)

install(
        EXPORT wisdom-targets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/wisdom
        NAMESPACE wis::
        FILE wisdom-targets.cmake
)
