#ifndef WIS_VK_RAYTRACING_TYPES_HPP
#define WIS_VK_RAYTRACING_TYPES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <raytracing/vulkan/vk_tables.hpp>

namespace wis {
//----------------------------------------------------------------------------------------------------------------------
namespace impl {
struct VKRaytracingExtensionImpl {
    VKDeviceExtensionHeader header;
    VkDevice device;
    detail::VKDeviceControlBlock* device_control_block;
    impl::VKRaytracingPipelineTable* rt_table;
};

struct VKAccelerationStructureImpl {
    VkAccelerationStructureKHR acceleration_structure;
    VkDeviceAddress device_address;
    detail::VKBufferControlBlock* buffer_control_block;
    PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
};

} // namespace impl
} // namespace wis

// Include implementation for header-only mode
#ifdef WISDOM_HEADER_ONLY
#    if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#        error "C++20 is required to build wisdom as header-only library"
#    endif // !WIS_HAS_CPP20
#    include "vk_raytracing.cpp"

#endif // WISDOM_HEADER_ONLY
#endif // WIS_VK_PLATFORM_TYPES_HPP
