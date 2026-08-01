#ifndef WIS_VK_VIDEO_TYPES_HPP
#define WIS_VK_VIDEO_TYPES_HPP
#ifndef __cplusplus
#    error "This header requires C++"
#endif // __cplusplus

#include <video/vulkan/vk_tables.hpp>

namespace wis {
namespace detail {
struct VKVideoDecodingControlBlock;
}
//----------------------------------------------------------------------------------------------------------------------
namespace impl {
struct VKVideoDecodingExtensionImpl {
    VKDeviceExtensionHeader header;
    WisVideoCodecFlags supported_codecs;
    VkDevice device;
    VkPhysicalDevice adapter;
    detail::VKVideoDecodingControlBlock* decoding_control_block;
};

struct VKVideoDecoderImpl {
    VkVideoSessionKHR video_session;
    VmaAllocation video_memory;
    WisVideoCodecFlags codec;
    detail::VKVideoDecodingControlBlock* decoding_control_block;
};

struct VKVideoDecoderParametersImpl {
    VkVideoSessionParametersKHR video_session_parameters;
    detail::VKVideoDecodingControlBlock* decoding_control_block;
};

struct VKVideoDecodeCommandListImpl {
    VkCommandBuffer command_buffer;
    impl::VKMainCommandList* command_list_table;
    detail::VKCommandPoolControlBlock* command_pool_header;

    detail::VKQueueFamilyExtras* queue_indices;
    uint32_t maintenance9          : 1;
    WisCommandQueueType queue_type : 31;

    mutable uint32_t scratch_memory_size;
    mutable uint8_t* scratch_memory;
};

} // namespace impl
} // namespace wis

// Include implementation for header-only mode
#ifdef WISDOM_HEADER_ONLY
#    if !WIS_HAS_CPP20 && !defined(WISDOM_LANG_DISABLE_CHECK)
#        error "C++20 is required to build wisdom as header-only library"
#    endif // !WIS_HAS_CPP20
#    include "vk_video.cpp"
#    include "vk_video_list.cpp"

#endif // WISDOM_HEADER_ONLY
#endif // WIS_VK_VIDEO_TYPES_HPP
