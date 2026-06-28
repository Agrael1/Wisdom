#ifndef WIS_VK_VIDEO_PARAMETERS_HPP
#define WIS_VK_VIDEO_PARAMETERS_HPP

#include <video/generated/c_api.h>
#include <video/generated/vk_convert.hpp>
#include <wisdom/vulkan/vk_types.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <video/vulkan/vk_tables.hpp>
#include <video/vulkan/vk_types.hpp>
#include <vk_video/vulkan_video_codec_h265std_decode.h>

namespace wis::detail {
//----------------------------------------------------------------------------------------------------------------------
struct VKVideoDecodingHeader {
    impl::VKVideoTable video_table;
    detail::VKDeviceControlBlock* device_control_block;
};

//----------------------------------------------------------------------------------------------------------------------
struct VKVideoDecodingControlBlock : public VKControlBlock<VKVideoDecodingHeader> {};

//----------------------------------------------------------------------------------------------------------------------
inline void VKReleaseVideoDecoding(VKVideoDecodingControlBlock* block) noexcept
{
    if (block && block->Release() == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        VKReleaseDevice(block->header.device_control_block);
        delete block;
    }
}

//----------------------------------------------------------------------------------------------------------------------
inline WisResult VKCreateDecoderParametersAV1(
    const wis::impl::VKVideoDecoderImpl& impl,
    const WisStdVideoAV1SequenceHeader* sequence_header,
    VkVideoSessionParametersKHR* parameters
)
{
    auto& video_header = impl.decoding_control_block->header;
    auto& video_table = video_header.video_table;

    VkVideoDecodeAV1SessionParametersCreateInfoKHR av1_parameters_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_SESSION_PARAMETERS_CREATE_INFO_KHR,
        .pNext = nullptr,
        .pStdSequenceHeader = reinterpret_cast<const ::StdVideoAV1SequenceHeader*>(sequence_header),
    };

    VkVideoSessionParametersCreateInfoKHR parameters_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_SESSION_PARAMETERS_CREATE_INFO_KHR,
        .pNext = &av1_parameters_info,
        .flags = 0,
        .videoSessionParametersTemplate = VK_NULL_HANDLE,
        .videoSession = impl.video_session,
    };
    auto vr = video_table.vkCreateVideoSessionParametersKHR(
        video_header.device_control_block->header.device,
        &parameters_info,
        nullptr,
        parameters
    );
    if (vr != VK_SUCCESS) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create video session parameters.">(vr);
    }

    return wis::detail::vk_success;
}

//----------------------------------------------------------------------------------------------------------------------
inline WisResult VKCreateDecoderParametersH265(
    const wis::impl::VKVideoDecoderImpl& impl,
    const WisVideoDecodeH265Desc& h265_desc,
    VkVideoSessionParametersKHR* parameters
)
{
    auto& video_header = impl.decoding_control_block->header;
    auto& video_table = video_header.video_table;
    auto& device_header = video_header.device_control_block->header;

    VkVideoDecodeH265SessionParametersAddInfoKHR add_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_SESSION_PARAMETERS_ADD_INFO_KHR,
        .pNext = nullptr,
        .stdVPSCount = h265_desc.vps_count,
        .pStdVPSs = reinterpret_cast<const ::StdVideoH265VideoParameterSet*>(h265_desc.vps),
        .stdSPSCount = h265_desc.sps_count,
        .pStdSPSs = reinterpret_cast<const ::StdVideoH265SequenceParameterSet*>(h265_desc.sps),
        .stdPPSCount = h265_desc.pps_count,
        .pStdPPSs = reinterpret_cast<const ::StdVideoH265PictureParameterSet*>(h265_desc.pps),
    };

    VkVideoDecodeH265SessionParametersCreateInfoKHR h265_params{
        .sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_SESSION_PARAMETERS_CREATE_INFO_KHR,
        .pNext = nullptr,
        .maxStdVPSCount = h265_desc.max_vps_count,
        .maxStdSPSCount = h265_desc.max_sps_count,
        .maxStdPPSCount = h265_desc.max_pps_count,
        .pParametersAddInfo = h265_desc.vps_count || h265_desc.sps_count || h265_desc.pps_count ? &add_info : nullptr,
    };

    VkVideoSessionParametersCreateInfoKHR parameters_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_SESSION_PARAMETERS_CREATE_INFO_KHR,
        .pNext = &h265_params,
        .flags = 0,
        .videoSessionParametersTemplate = VK_NULL_HANDLE,
        .videoSession = impl.video_session,
    };

    VkResult vr = video_table
                      .vkCreateVideoSessionParametersKHR(device_header.device, &parameters_info, nullptr, parameters);
    if (vr != VK_SUCCESS) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create H.265 video session parameters.">(vr);
    }

    return wis::detail::vk_success;
}
} // namespace wis::detail
#endif // WIS_VK_VIDEO_PARAMETERS_HPP
