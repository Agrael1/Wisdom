#ifndef WIS_VK_VIDEO_CPP
#define WIS_VK_VIDEO_CPP

#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>
#include <video/generated/cpp_api.hpp>
#include <video/generated/vk_convert.hpp>
#include <bit>
#include <variant>

namespace wis::detail {
inline WisResult VKVideoDecodingExtensionInit(
    VKDeviceExtensionHeader* self,
    impl::VKDeviceImpl* device_impl,
    VKDeviceExtensionCollector* collector
) noexcept
{
    auto& impl = wis::from_handle_ref<wis::impl::VKVideoDecodingExtensionImpl>(self);

    if (!device_impl) {
        if (!impl.supported_codecs) {
            return wis::detail::vk_success; // Nothing requested
        }

        auto& coll = *collector;
        if (!coll.IsExtensionPresent(VK_KHR_VIDEO_QUEUE_EXTENSION_NAME)
            || !coll.IsExtensionPresent(VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME)) {
            impl.supported_codecs = {};
            return {}; // Required extension not present
        }

        // Video queue
        coll.EnableExtension({.name = VK_KHR_VIDEO_QUEUE_EXTENSION_NAME});

        // Video decode queue
        coll.EnableExtension({.name = VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME});

        // now for the requested codecs:
        if (impl.supported_codecs & WisVideoCodecFlagsH264) {
            coll.EnableExtension({.name = VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME});
        } else {
            impl.supported_codecs = WisVideoCodecFlags(impl.supported_codecs & ~WisVideoCodecFlagsH264);
        }
        if (impl.supported_codecs & WisVideoCodecFlagsH265) {
            coll.EnableExtension({.name = VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME});
        } else {
            impl.supported_codecs = WisVideoCodecFlags(impl.supported_codecs & ~WisVideoCodecFlagsH265);
        }
        if (impl.supported_codecs & WisVideoCodecFlagsAV1) {
            coll.EnableExtension({.name = VK_KHR_VIDEO_DECODE_AV1_EXTENSION_NAME});
        } else {
            impl.supported_codecs = WisVideoCodecFlags(impl.supported_codecs & ~WisVideoCodecFlagsAV1);
        }
        if (impl.supported_codecs & WisVideoCodecFlagsVP9) {
            coll.EnableExtension({.name = VK_KHR_VIDEO_DECODE_VP9_EXTENSION_NAME});
        } else {
            impl.supported_codecs = WisVideoCodecFlags(impl.supported_codecs & ~WisVideoCodecFlagsVP9);
        }
    } else {
        if (impl.device_control_block) {
            // Delete everything here
            delete impl.video_table;
            wis::detail::VKReleaseDevice(impl.device_control_block);
        }
        if (!impl.supported_codecs) {
            return wis::detail::vk_success; // Nothing requested
        }

        auto& aheader = device_impl->device_header->header.shared_header->header;
        auto table = wis::make_unique<impl::VKVideoTable>();
        if (!table
            || !table->Init(
                device_impl->device,
                aheader.global_table.vkGetDeviceProcAddr,
                aheader.instance,
                aheader.global_table.vkGetInstanceProcAddr
            )) {
            impl.supported_codecs = {};
            return wis::detail::vk_success; // Failed to initialize video table, treat as unsupported
        }

        impl.device = device_impl->device;
        impl.adapter = device_impl->physical_device;
        impl.device_control_block = device_impl->device_header;
        impl.video_table = table.release();
        impl.device_control_block->AddRef(); // extension holds a reference to the device control block
    }

    return wis::detail::vk_success;
}
} // namespace wis::detail



WIS_EXTERN_C WISDOM_VIDEO_API void wisVKInitVideoDecodingExtension(
    WisVKVideoDecodingExtension* self,
    WisVideoCodecFlags request_codecs
)
{
    new (self) wis::impl::VKVideoDecodingExtensionImpl{
        .header = {&wis::detail::VKVideoDecodingExtensionInit},
        .supported_codecs = request_codecs,
    };
}

WIS_EXTERN_C WISDOM_VIDEO_API void wisVKDestroyVideoDecodingExtension(WisVKVideoDecodingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKVideoDecodingExtensionImpl>(self);
    if (impl.device_control_block) {
        delete impl.video_table;
        wis::detail::VKReleaseDevice(impl.device_control_block);
        impl.device_control_block = nullptr;
    }
    impl.header = {nullptr};
}

WIS_EXTERN_C WISDOM_VIDEO_API WisResult wisVKVideoDecodingExtensionQueryCodecCaps(
    WisVKVideoDecodingExtension* self,
    const WisVideoCodecDesc* codec_desc,
    WisVideoDecodeInfo* decode_info
)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKVideoDecodingExtensionImpl>(self);
    VkVideoDecodeCapabilitiesKHR decode_caps{
        .sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_CAPABILITIES_KHR,
    };

    VkVideoCapabilitiesKHR video_caps{
        .sType = VK_STRUCTURE_TYPE_VIDEO_CAPABILITIES_KHR,
        .pNext = &decode_caps,
    };

    std::variant<
        VkVideoDecodeH264CapabilitiesKHR,
        VkVideoDecodeH265CapabilitiesKHR,
        VkVideoDecodeAV1CapabilitiesKHR,
        VkVideoDecodeVP9CapabilitiesKHR>
        codec_caps;

    std::variant<
        VkVideoDecodeH264ProfileInfoKHR,
        VkVideoDecodeH265ProfileInfoKHR,
        VkVideoDecodeAV1ProfileInfoKHR,
        VkVideoDecodeVP9ProfileInfoKHR>
        decode_profile_infos;

    VkVideoProfileInfoKHR profile_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_PROFILE_INFO_KHR,
        //.chromaSubsampling = wis::detail::VKConvert(codec_desc->chroma_subsampling),
        //.lumaBitDepth = wis::detail::VKConvert(codec_desc->bit_depth),
        //.chromaBitDepth = wis::detail::VKConvert(codec_desc->bit_depth),
    };
    uint32_t codec_type = codec_desc->codec_profile / 32; // Codec profiles are defined with step of 32
    switch (1 << codec_type) {
    case WisVideoCodecFlagsH264: {
        auto& cap = codec_caps.emplace<VkVideoDecodeH264CapabilitiesKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_CAPABILITIES_KHR
        );
        auto& info = decode_profile_infos.emplace<VkVideoDecodeH264ProfileInfoKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_PROFILE_INFO_KHR
        );

        switch (codec_desc->codec_profile) {
        case WisStdCodecProfileH264Baseline:
            info.stdProfileIdc = STD_VIDEO_H264_PROFILE_IDC_BASELINE;
            break;
        case WisStdCodecProfileH264Main:
            info.stdProfileIdc = STD_VIDEO_H264_PROFILE_IDC_MAIN;
            break;
        case WisStdCodecProfileH264High:
            info.stdProfileIdc = STD_VIDEO_H264_PROFILE_IDC_HIGH;
            break;
        case WisStdCodecProfileH264HighPredictive:
            info.stdProfileIdc = STD_VIDEO_H264_PROFILE_IDC_HIGH_444_PREDICTIVE;
            break;
        default:
            info.stdProfileIdc = STD_VIDEO_H264_PROFILE_IDC_INVALID;
            break;
        }

        profile_info.videoCodecOperation = VK_VIDEO_CODEC_OPERATION_DECODE_H264_BIT_KHR;
        profile_info.pNext = &info;
        decode_caps.pNext = &cap;
    } break;
    case WisVideoCodecFlagsH265: {
        auto& cap = codec_caps.emplace<VkVideoDecodeH265CapabilitiesKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_CAPABILITIES_KHR
        );
        auto& info = decode_profile_infos.emplace<VkVideoDecodeH265ProfileInfoKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_PROFILE_INFO_KHR
        );

        switch (codec_desc->codec_profile) {
        case WisStdCodecProfileH265Main:
            info.stdProfileIdc = STD_VIDEO_H265_PROFILE_IDC_MAIN;
            break;
        case WisStdCodecProfileH265Main10:
            info.stdProfileIdc = STD_VIDEO_H265_PROFILE_IDC_MAIN_10;
            break;
        case WisStdCodecProfileH265FormatRangeExt:
            info.stdProfileIdc = STD_VIDEO_H265_PROFILE_IDC_FORMAT_RANGE_EXTENSIONS;
            break;
        default:
            info.stdProfileIdc = STD_VIDEO_H265_PROFILE_IDC_INVALID;
            break;
        }

        profile_info.videoCodecOperation = VK_VIDEO_CODEC_OPERATION_DECODE_H265_BIT_KHR;
        profile_info.pNext = &info;
        decode_caps.pNext = &cap;
    } break;
    case WisVideoCodecFlagsAV1: {
        auto& cap = codec_caps.emplace<VkVideoDecodeAV1CapabilitiesKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_CAPABILITIES_KHR
        );
        auto& info = decode_profile_infos.emplace<VkVideoDecodeAV1ProfileInfoKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_PROFILE_INFO_KHR
        );

        switch (codec_desc->codec_profile) {
        case WisStdCodecProfileAV1Main:
            info.stdProfile = STD_VIDEO_AV1_PROFILE_MAIN;
            break;
        case WisStdCodecProfileAV1High:
            info.stdProfile = STD_VIDEO_AV1_PROFILE_HIGH;
            break;
        case WisStdCodecProfileAV1Professional:
            info.stdProfile = STD_VIDEO_AV1_PROFILE_PROFESSIONAL;
            break;
        default:
            info.stdProfile = STD_VIDEO_AV1_PROFILE_INVALID;
            break;
        }
        info.filmGrainSupport = VK_TRUE;
        profile_info.videoCodecOperation = VK_VIDEO_CODEC_OPERATION_DECODE_AV1_BIT_KHR;
        profile_info.pNext = &info;
        decode_caps.pNext = &cap;
    } break;
    case WisVideoCodecFlagsVP9: {
        auto& cap = codec_caps.emplace<VkVideoDecodeVP9CapabilitiesKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_VP9_CAPABILITIES_KHR
        );
        auto& info = decode_profile_infos.emplace<VkVideoDecodeVP9ProfileInfoKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_VP9_PROFILE_INFO_KHR
        );

        switch (codec_desc->codec_profile) {
        case WisStdCodecProfileVP9Profile0:
            info.stdProfile = STD_VIDEO_VP9_PROFILE_0;
            break;
        case WisStdCodecProfileVP9Profile1:
            info.stdProfile = STD_VIDEO_VP9_PROFILE_1;
            break;
        case WisStdCodecProfileVP9Profile2:
            info.stdProfile = STD_VIDEO_VP9_PROFILE_2;
            break;
        case WisStdCodecProfileVP9Profile3:
            info.stdProfile = STD_VIDEO_VP9_PROFILE_3;
            break;
        default:
            info.stdProfile = STD_VIDEO_VP9_PROFILE_INVALID;
            break;
        }

        profile_info.videoCodecOperation = VK_VIDEO_CODEC_OPERATION_DECODE_VP9_BIT_KHR;
        profile_info.pNext = &info;
        decode_caps.pNext = &cap;
    } break;
    default:
        *decode_info = {}; // Clear output info
        return wis::detail::make_result<
            wis::detail::Func(),
            "Unsupported codec. Please provide a valid codec profile.">(VK_ERROR_UNKNOWN);
    }

    auto vr = impl.video_table->vkGetPhysicalDeviceVideoCapabilitiesKHR(impl.adapter, &profile_info, &video_caps);
    if (vr != VK_SUCCESS) {
        *decode_info = {}; // Clear output info
        return wis::detail::make_result<wis::detail::Func(), "Failed to query video capabilities">(vr);
    }

    // Fill out the decode info based on the queried capabilities
    *decode_info = {
        .max_width = video_caps.maxCodedExtent.width,
        .max_height = video_caps.maxCodedExtent.height,
    };

    return wis::detail::vk_success;
}

#endif // WIS_VK_VIDEO_CPP
