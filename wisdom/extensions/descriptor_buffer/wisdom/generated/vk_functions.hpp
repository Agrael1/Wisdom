#pragma once
#include <array>
#include <vulkan/vulkan.h>
#include <wisvk/vk_libinit.hpp>

namespace wis {

struct VKDescBufferExtDevice {
    PFN_vkCmdBindDescriptorBuffersEXT vkCmdBindDescriptorBuffersEXT;
    PFN_vkGetDescriptorSetLayoutBindingOffsetEXT vkGetDescriptorSetLayoutBindingOffsetEXT;
    PFN_vkCmdSetDescriptorBufferOffsetsEXT vkCmdSetDescriptorBufferOffsetsEXT;
    PFN_vkGetDescriptorEXT vkGetDescriptorEXT;

public:
    bool Init(VkDevice device, PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr) noexcept
    {
        vkCmdBindDescriptorBuffersEXT = (PFN_vkCmdBindDescriptorBuffersEXT)vkGetDeviceProcAddr(device, "vkCmdBindDescriptorBuffersEXT");
        if (vkCmdBindDescriptorBuffersEXT == nullptr)
            return false;
        vkGetDescriptorSetLayoutBindingOffsetEXT = (PFN_vkGetDescriptorSetLayoutBindingOffsetEXT)vkGetDeviceProcAddr(device, "vkGetDescriptorSetLayoutBindingOffsetEXT");
        if (vkGetDescriptorSetLayoutBindingOffsetEXT == nullptr)
            return false;
        vkCmdSetDescriptorBufferOffsetsEXT = (PFN_vkCmdSetDescriptorBufferOffsetsEXT)vkGetDeviceProcAddr(device, "vkCmdSetDescriptorBufferOffsetsEXT");
        if (vkCmdSetDescriptorBufferOffsetsEXT == nullptr)
            return false;
        vkGetDescriptorEXT = (PFN_vkGetDescriptorEXT)vkGetDeviceProcAddr(device, "vkGetDescriptorEXT");
        if (vkGetDescriptorEXT == nullptr)
            return false;
        return true;
    }
};

} // namespace wis
