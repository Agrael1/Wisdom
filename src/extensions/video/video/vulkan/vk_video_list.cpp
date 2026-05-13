#ifndef WIS_VK_VIDEO_COMMAND_LIST_CPP
#define WIS_VK_VIDEO_COMMAND_LIST_CPP

#include <wisdom/generated/vk_convert.hpp>
#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <video/generated/cpp_api.hpp>
#include <video/generated/vk_convert.hpp>
#include <bit>

#include <vk_video/vulkan_video_codec_av1std_decode.h>

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
WIS_EXTERN_C WISDOM_VIDEO_API void wisVKVideoDecodeCommandListDecodeFrame(
    const WisVKVideoDecodeCommandList* command_list,
    const WisVKVideoDecoder* decoder,
    const WisVKVideoDecoderParameters* decoder_parameters,
    const WisVKVideoDecodeInputDesc* input_desc
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKVideoDecodeCommandListImpl>(command_list);
    auto& decoder_impl = wis::from_handle_ref<const wis::impl::VKVideoDecoderImpl>(decoder);
    auto& params = wis::from_handle_ref<const wis::impl::VKVideoDecoderParametersImpl>(decoder_parameters);

    // TODO: Add WisVKVideoDecodeOutputDesc to the function parameters which will contain the decoded
    // target picture (dpb/output) and setup/reference slots for the DPB (VkVideoPictureResourceInfoKHR).

    // TODO: Add WisVKVideoDecoderParameters to the function parameters which will contain
    // the StdVideo parameters (sequence headers, picture headers, slice headers).
    // You will need to map these to codec specific Vulkan extensions like VkVideoDecodeAV1PictureInfoKHR.

    // Dummy struct representing what will eventually be provided by the parameters/output descriptors
    struct DummyVkVideoParams {
        VkVideoDecodeAV1PictureInfoKHR av1_pic_info{};
        VkVideoPictureResourceInfoKHR target_pic_resource{};
        VkVideoReferenceSlotInfoKHR setup_reference_slot{};
        VkVideoReferenceSlotInfoKHR reference_slots[8]{};
    } dummy_params{};

    // Begin video coding scope
    VkVideoBeginCodingInfoKHR beginInfo = {
        .sType = VK_STRUCTURE_TYPE_VIDEO_BEGIN_CODING_INFO_KHR,
        .pNext = nullptr,
        .flags = 0,
        .videoSession = decoder_impl.video_session,
        .videoSessionParameters = params.video_session_parameters,

        .referenceSlotCount = 0,   // TODO: Fill from active dummy_params.reference_slots length
        .pReferenceSlots = nullptr // TODO: Point to dummy_params.reference_slots
    };
    impl.command_list_table->vkCmdBeginVideoCodingKHR(impl.command_buffer, &beginInfo);

    // Convert decode info to Vulkan structures
    VkVideoDecodeInfoKHR vk_decode_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_INFO_KHR,
        .pNext = &dummy_params.av1_pic_info, // TODO: Link codec-specific struct from parameters here
        .flags = 0,
        .srcBuffer = std::bit_cast<VkBuffer>(input_desc->bitstream_buffer),
        .srcBufferOffset = input_desc->offset,
        .srcBufferRange = input_desc->size,
        .dstPictureResource = dummy_params.target_pic_resource, // TODO: Fill from output descriptor
        .pSetupReferenceSlot = &dummy_params.setup_reference_slot, // TODO: Fill from output descriptor
        .referenceSlotCount = 0, // TODO: Fill from output descriptor
        .pReferenceSlots = nullptr, // TODO: Fill from output descriptor
    };
    impl.command_list_table->vkCmdDecodeVideoKHR(impl.command_buffer, &vk_decode_info);

    // End video coding scope
    VkVideoEndCodingInfoKHR end_info = {
        .sType = VK_STRUCTURE_TYPE_VIDEO_END_CODING_INFO_KHR,
        .pNext = nullptr,
        .flags = 0
    };
    impl.command_list_table->vkCmdEndVideoCodingKHR(impl.command_buffer, &end_info);
}

#endif // WIS_VK_VIDEO_CPP
