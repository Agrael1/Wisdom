#ifndef WIS_VK_VIDEO_COMMAND_LIST_CPP
#define WIS_VK_VIDEO_COMMAND_LIST_CPP

#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <video/generated/cpp_api.hpp>
#include <video/generated/vk_convert.hpp>
#include <bit>

#include <vk_video/vulkan_video_codec_av1std_decode.h>
#include <vk_video/vulkan_video_codec_h265std_decode.h>

// Barrier code moved to wis::detail::VKInsertBarriers in vk_detail.hpp

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisVKDestroyVideoDecodeCommandList(WisVKVideoDecodeCommandList* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKVideoDecodeCommandListImpl>(self);
    if (impl.command_buffer != VK_NULL_HANDLE) {
        // free command buffer
        auto& header = impl.command_pool_header->header;
        impl.command_list_table->vkFreeCommandBuffers(header.device, header.command_pool, 1, &impl.command_buffer);
        impl.command_buffer = VK_NULL_HANDLE;

        wis::detail::VKReleaseCommandPool(impl.command_pool_header);
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API WisResult wisVKVideoDecodeCommandListBegin(const WisVKVideoDecodeCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKVideoDecodeCommandListImpl>(self);

    VkCommandBufferBeginInfo begin_info{
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .pNext = nullptr,
        .flags = 0, // Optional flags can be set here, e.g., VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
        .pInheritanceInfo = nullptr, // Optional, only relevant for secondary command buffers
    };
    auto vr = impl.command_list_table->vkBeginCommandBuffer(impl.command_buffer, &begin_info);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to begin Vulkan command buffer recording">(vr);
    }
    return wis::detail::vk_success;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API WisResult wisVKVideoDecodeCommandListEnd(const WisVKVideoDecodeCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKVideoDecodeCommandListImpl>(self);
    auto vr = impl.command_list_table->vkEndCommandBuffer(impl.command_buffer);
    if (!wis::detail::succeeded(vr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to end Vulkan command buffer recording">(vr);
    }
    return wis::detail::vk_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisVKVideoDecodeCommandListInsertBarriers(
    const WisVKVideoDecodeCommandList* self,
    const WisVKBarrierGroup* barriers
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKVideoDecodeCommandListImpl>(self);
    wis::detail::VKInsertBarriers(impl, barriers);
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisVKVideoDecodeCommandListDecodeFrame(
    const WisVKVideoDecodeCommandList* command_list,
    const WisVKVideoDecoder* decoder,
    const WisVKVideoDecoderParameters* parameters,
    const WisVKVideoDecodeInputDesc* input_desc,
    const WisVKVideoDecodePictureDesc* picture_desc,
    uint64_t output_cpu_handle
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKVideoDecodeCommandListImpl>(command_list);
    auto& decoder_impl = wis::from_handle_ref<const wis::impl::VKVideoDecoderImpl>(decoder);
    auto& parameters_impl = wis::from_handle_ref<const wis::impl::VKVideoDecoderParametersImpl>(parameters);

    // Extract output image view and format
    auto& output_image_view = *std::bit_cast<const wis::detail::VKRenderTargetView*>(output_cpu_handle);

    // Codec-specific picture info.
    VkVideoDecodeAV1PictureInfoKHR av1_info{};
    VkVideoDecodeH265PictureInfoKHR h265_info{};
    uint32_t h265_slice_segment_offsets[1]{};
    void* codec_pnext = nullptr;

    if (picture_desc->codec >= WisStdCodecProfileAV1Main && picture_desc->codec <= WisStdCodecProfileAV1Professional) {
        av1_info.sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_PICTURE_INFO_KHR;
        av1_info.pNext = nullptr;
        av1_info.pStdPictureInfo = reinterpret_cast<const StdVideoDecodeAV1PictureInfo*>(
            picture_desc->av1_picture_info
        );
        av1_info.frameHeaderOffset = 0;
        for (auto& idx : av1_info.referenceNameSlotIndices) {
            idx = -1;
        }
        codec_pnext = &av1_info;
    } else if (
        picture_desc->codec >= WisStdCodecProfileH265Main && picture_desc->codec <= WisStdCodecProfileH265FormatRangeExt
    ) {
        h265_info.sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_PICTURE_INFO_KHR;
        h265_info.pNext = nullptr;
        h265_info.pStdPictureInfo = reinterpret_cast<const StdVideoDecodeH265PictureInfo*>(
            picture_desc->h265_picture_info
        );

        // TODO: Handle multiple slice segments if needed. For now, we assume a single slice segment.
        h265_slice_segment_offsets[0] = 0;
        h265_info.sliceSegmentCount = 1;
        h265_info.pSliceSegmentOffsets = h265_slice_segment_offsets;
        codec_pnext = &h265_info;
    }

    // Build target picture resource
    VkVideoPictureResourceInfoKHR target_pic_resource{
        .sType = VK_STRUCTURE_TYPE_VIDEO_PICTURE_RESOURCE_INFO_KHR,
        .pNext = nullptr,
        .codedOffset = {0, 0},
        .codedExtent = {output_image_view.width, output_image_view.height},
        .baseArrayLayer = 0,
        .imageViewBinding = output_image_view.view,
    };

    // Begin video coding scope
    VkVideoBeginCodingInfoKHR begin_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_BEGIN_CODING_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .videoSession = decoder_impl.video_session,
        .videoSessionParameters = parameters_impl.video_session_parameters,
        .referenceSlotCount = 0,
        .pReferenceSlots = nullptr,
    };
    impl.command_list_table->vkCmdBeginVideoCodingKHR(impl.command_buffer, &begin_info);

    VkVideoCodingControlInfoKHR control_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_CODING_CONTROL_INFO_KHR,
        .pNext = nullptr,
        .flags = VK_VIDEO_CODING_CONTROL_RESET_BIT_KHR,
    };
    impl.command_list_table->vkCmdControlVideoCodingKHR(impl.command_buffer, &control_info);

    // Decode frame
    VkVideoDecodeInfoKHR vk_decode_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_INFO_KHR,
        .pNext = codec_pnext,
        .flags = 0,
        .srcBuffer = std::bit_cast<VkBuffer>(input_desc->bitstream_buffer),
        .srcBufferOffset = input_desc->offset,
        .srcBufferRange = input_desc->size,
        .dstPictureResource = target_pic_resource,
        .pSetupReferenceSlot = nullptr,
        .referenceSlotCount = 0,
        .pReferenceSlots = nullptr,
    };
    impl.command_list_table->vkCmdDecodeVideoKHR(impl.command_buffer, &vk_decode_info);

    // End video coding scope
    VkVideoEndCodingInfoKHR end_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_END_CODING_INFO_KHR,
        .pNext = nullptr,
        .flags = 0
    };
    impl.command_list_table->vkCmdEndVideoCodingKHR(impl.command_buffer, &end_info);

    }

#endif // WIS_VK_VIDEO_CPP
