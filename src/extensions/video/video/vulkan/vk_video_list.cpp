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
    const WisVKVideoDecodeOutputDesc* output_desc,
    const WisVKVideoDecodePictureDesc* picture_desc
)
{
    using wis::detail::VKConvert;

    auto& impl = wis::from_handle_ref<const wis::impl::VKVideoDecodeCommandListImpl>(command_list);
    auto& decoder_impl = wis::from_handle_ref<const wis::impl::VKVideoDecoderImpl>(decoder);
    auto& parameters_impl = wis::from_handle_ref<const wis::impl::VKVideoDecoderParametersImpl>(parameters);

    auto& device_table = impl.command_pool_header->header.device_header->header.device_table;
    auto device = impl.command_pool_header->header.device;

    // Extract output image and format
    VkImage output_image = std::bit_cast<VkImage>(output_desc->output_texture);
    VkFormat output_format = VKConvert(output_desc->format);
    uint32_t subresource = output_desc->subresource;

    // Create temporary image view for the output
    VkImageViewCreateInfo view_info{
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .pNext = nullptr,
        .flags = 0,
        .image = output_image,
        .viewType = VK_IMAGE_VIEW_TYPE_2D,
        .format = output_format,
        .components =
            {VK_COMPONENT_SWIZZLE_IDENTITY,
             VK_COMPONENT_SWIZZLE_IDENTITY,
             VK_COMPONENT_SWIZZLE_IDENTITY,
             VK_COMPONENT_SWIZZLE_IDENTITY},
        .subresourceRange = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = subresource,
            .layerCount = 1
        },
    };
    VkImageView output_image_view = VK_NULL_HANDLE;
    device_table.vkCreateImageView(device, &view_info, nullptr, &output_image_view);

    // Codec-specific picture info and DPB slot info
    VkVideoDecodeAV1PictureInfoKHR av1_info{};
    VkVideoDecodeH265PictureInfoKHR h265_info{};
    VkVideoDecodeAV1DpbSlotInfoKHR av1_dpb_slot{};
    VkVideoDecodeH265DpbSlotInfoKHR h265_dpb_slot{};
    void* codec_pnext = nullptr;
    void* dpb_slot_pnext = nullptr;

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

        av1_dpb_slot.sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_DPB_SLOT_INFO_KHR;
        av1_dpb_slot.pNext = nullptr;
        av1_dpb_slot.pStdReferenceInfo = reinterpret_cast<const StdVideoDecodeAV1ReferenceInfo*>(
            picture_desc->av1_reference_info
        );
        dpb_slot_pnext = &av1_dpb_slot;
    } else if (
        picture_desc->codec >= WisStdCodecProfileH265Main && picture_desc->codec <= WisStdCodecProfileH265FormatRangeExt
    ) {
        h265_info.sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_PICTURE_INFO_KHR;
        h265_info.pNext = nullptr;
        h265_info.pStdPictureInfo = reinterpret_cast<const StdVideoDecodeH265PictureInfo*>(
            picture_desc->h265_picture_info
        );
        h265_info.sliceSegmentCount = 0;
        h265_info.pSliceSegmentOffsets = nullptr;
        codec_pnext = &h265_info;

        h265_dpb_slot.sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_DPB_SLOT_INFO_KHR;
        h265_dpb_slot.pNext = nullptr;
        h265_dpb_slot.pStdReferenceInfo = reinterpret_cast<const StdVideoDecodeH265ReferenceInfo*>(
            picture_desc->h265_reference_info
        );
        dpb_slot_pnext = &h265_dpb_slot;
    }

    // Build target picture resource
    VkVideoPictureResourceInfoKHR target_pic_resource{
        .sType = VK_STRUCTURE_TYPE_VIDEO_PICTURE_RESOURCE_INFO_KHR,
        .pNext = nullptr,
        .codedOffset = {0, 0},
        .codedExtent = {decoder_impl.width, decoder_impl.height},
        .baseArrayLayer = subresource,
        .imageViewBinding = output_image_view,
    };

    // Setup reference slot for current frame (if it will be used as reference)
    VkVideoReferenceSlotInfoKHR setup_reference_slot{};
    bool is_reference = false;
    if (picture_desc->codec >= WisStdCodecProfileAV1Main && picture_desc->codec <= WisStdCodecProfileAV1Professional) {
        if (picture_desc->av1_picture_info && picture_desc->av1_picture_info->refresh_frame_flags != 0) {
            is_reference = true;
        }
    } else if (
        (picture_desc->codec >= WisStdCodecProfileH265Main
         && picture_desc->codec <= WisStdCodecProfileH265FormatRangeExt)
        && picture_desc->h265_picture_info
    ) {
        if (picture_desc->h265_picture_info->flags.IsReference) {
            is_reference = true;
        }
    }

    if (is_reference) {
        setup_reference_slot.sType = VK_STRUCTURE_TYPE_VIDEO_REFERENCE_SLOT_INFO_KHR;
        setup_reference_slot.pNext = dpb_slot_pnext;
        setup_reference_slot.slotIndex = 0;
        setup_reference_slot.pPictureResource = &target_pic_resource;
    }

    // Reference slots for inter-frame prediction (currently empty, future enhancement)
    uint32_t ref_slot_count = 0;
    const VkVideoReferenceSlotInfoKHR* ref_slots = nullptr;

    // Begin video coding scope
    VkVideoBeginCodingInfoKHR begin_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_BEGIN_CODING_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .videoSession = decoder_impl.video_session,
        .videoSessionParameters = parameters_impl.video_session_parameters,
        .referenceSlotCount = is_reference ? 1u : 0u,
        .pReferenceSlots = is_reference ? &setup_reference_slot : nullptr,
    };
    impl.command_list_table->vkCmdBeginVideoCodingKHR(impl.command_buffer, &begin_info);

    // Decode frame
    VkVideoDecodeInfoKHR vk_decode_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_INFO_KHR,
        .pNext = codec_pnext,
        .flags = 0,
        .srcBuffer = std::bit_cast<VkBuffer>(input_desc->bitstream_buffer),
        .srcBufferOffset = input_desc->offset,
        .srcBufferRange = input_desc->size,
        .dstPictureResource = target_pic_resource,
        .pSetupReferenceSlot = is_reference ? &setup_reference_slot : nullptr,
        .referenceSlotCount = ref_slot_count,
        .pReferenceSlots = ref_slots,
    };
    impl.command_list_table->vkCmdDecodeVideoKHR(impl.command_buffer, &vk_decode_info);

    // End video coding scope
    VkVideoEndCodingInfoKHR end_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_END_CODING_INFO_KHR,
        .pNext = nullptr,
        .flags = 0
    };
    impl.command_list_table->vkCmdEndVideoCodingKHR(impl.command_buffer, &end_info);

    // Destroy temporary image view
    device_table.vkDestroyImageView(device, output_image_view, nullptr);
}

#endif // WIS_VK_VIDEO_CPP
