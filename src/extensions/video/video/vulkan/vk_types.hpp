#ifndef WIS_VK_VIDEO_TYPES_HPP
#define WIS_VK_VIDEO_TYPES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <video/vulkan/vk_tables.hpp>

namespace wis {
//----------------------------------------------------------------------------------------------------------------------
namespace impl {
struct VKVideoDecodingExtensionImpl {
    VKDeviceExtensionHeader header;
    WisVideoCodecFlags supported_codecs;
    VkDevice device;
    VkPhysicalDevice adapter;
    detail::VKDeviceControlBlock* device_control_block;
    impl::VKVideoTable* video_table;
};
struct VKVideoDecoderImpl {
    VkVideoSessionKHR video_session;
    detail::VKDeviceControlBlock* device_control_block;
    impl::VKVideoTable* video_table;
};

} // namespace impl
} // namespace wis

// Include implementation for header-only mode
#ifdef WISDOM_HEADER_ONLY
#    if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#        error "C++20 is required to build wisdom as header-only library"
#    endif // !WIS_HAS_CPP20
#    include "vk_video.cpp"

#endif // WISDOM_HEADER_ONLY
#endif // WIS_VK_VIDEO_TYPES_HPP
