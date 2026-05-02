// This file is generated. Do not edit directly.
#ifndef WISDOM_VIDEO_CPP_VK_CONVERT_HPP
#define WISDOM_VIDEO_CPP_VK_CONVERT_HPP
#ifndef __cplusplus
#    error "This is a C++ only header"
#endif // __cplusplus

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.h>
#include "c_api.h"

namespace wis {
namespace detail {

constexpr inline VkVideoComponentBitDepthFlagsKHR VKConvert(WisComponentBitDepth value) noexcept
{
    VkVideoComponentBitDepthFlagsKHR result = static_cast<VkVideoComponentBitDepthFlagsKHR>(0);
    if (value & WisComponentBitDepthBit8) {
        result |= VK_VIDEO_COMPONENT_BIT_DEPTH_8_BIT_KHR;
    }
    if (value & WisComponentBitDepthBit10) {
        result |= VK_VIDEO_COMPONENT_BIT_DEPTH_10_BIT_KHR;
    }
    if (value & WisComponentBitDepthBit12) {
        result |= VK_VIDEO_COMPONENT_BIT_DEPTH_12_BIT_KHR;
    }
    return result;
}

constexpr inline VkVideoChromaSubsamplingFlagsKHR VKConvert(WisChromaSubsampling value) noexcept
{
    VkVideoChromaSubsamplingFlagsKHR result = static_cast<VkVideoChromaSubsamplingFlagsKHR>(0);
    if (value & WisChromaSubsamplingC420) {
        result |= VK_VIDEO_CHROMA_SUBSAMPLING_420_BIT_KHR;
    }
    if (value & WisChromaSubsamplingC422) {
        result |= VK_VIDEO_CHROMA_SUBSAMPLING_422_BIT_KHR;
    }
    if (value & WisChromaSubsamplingC444) {
        result |= VK_VIDEO_CHROMA_SUBSAMPLING_444_BIT_KHR;
    }
    return result;
}

} // namespace detail
} // namespace wis
#endif // WISDOM_VIDEO_CPP_VK_CONVERT_HPP
