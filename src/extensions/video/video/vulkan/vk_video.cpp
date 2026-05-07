#ifndef WIS_VK_VIDEO_CPP
#define WIS_VK_VIDEO_CPP

#include <wisdom/generated/vk_convert.hpp>
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

struct VKVideoFormatInfo {
    VkFormat vk_format = VK_FORMAT_UNDEFINED;
    VkVideoComponentBitDepthFlagsKHR bit_depth = 0;
    VkVideoChromaSubsamplingFlagsKHR chroma_subsampling = 0;
};

template <WisDataFormat Format>
struct VKVideoFormatMapping {
    constexpr static VKVideoFormatInfo info{};
};

template <>
struct VKVideoFormatMapping<WisDataFormatNV12> {
    static constexpr VKVideoFormatInfo info{
        .vk_format = VKConvert(WisDataFormatNV12),
        .bit_depth = VKConvert(WisComponentBitDepthBit8),
        .chroma_subsampling = VKConvert(WisChromaSubsamplingC420),
    };
};

template <>
struct VKVideoFormatMapping<WisDataFormatP010> {
    static constexpr VKVideoFormatInfo info{
        .vk_format = VKConvert(WisDataFormatP010),
        .bit_depth = VKConvert(WisComponentBitDepthBit10),
        .chroma_subsampling = VKConvert(WisChromaSubsamplingC420),
    };
};

template <>
struct VKVideoFormatMapping<WisDataFormatP012> {
    static constexpr VKVideoFormatInfo info{
        .vk_format = VKConvert(WisDataFormatP012),
        .bit_depth = VKConvert(WisComponentBitDepthBit12),
        .chroma_subsampling = VKConvert(WisChromaSubsamplingC420),
    };
};

template <>
struct VKVideoFormatMapping<WisDataFormatP016> {
    static constexpr VKVideoFormatInfo info{
        .vk_format = VKConvert(WisDataFormatP016),
        .bit_depth = VKConvert(WisComponentBitDepthBit16),
        .chroma_subsampling = VKConvert(WisChromaSubsamplingC420),
    };
};

inline constexpr VKVideoFormatInfo VKGetFormatInfo(WisDataFormat format) noexcept
{
    switch (format) {
    case WisDataFormatNV12:
        return VKVideoFormatMapping<WisDataFormatNV12>::info;
    case WisDataFormatP010:
        return VKVideoFormatMapping<WisDataFormatP010>::info;
    case WisDataFormatP012:
        return VKVideoFormatMapping<WisDataFormatP012>::info;
    case WisDataFormatP016:
        return VKVideoFormatMapping<WisDataFormatP016>::info;
    default:
        return {}; // Unsupported format
    }
}

inline constexpr VkExtensionProperties VKGetStdHeaderVersion(WisStdCodecProfile profile) noexcept
{
    VkExtensionProperties result{};
    uint32_t codec_type = profile / 32; // Codec profiles are defined with step of 32

    switch (1 << codec_type) {
    case WisVideoCodecFlagsH264: {
        strncpy(result.extensionName, VK_STD_VULKAN_VIDEO_CODEC_H264_DECODE_EXTENSION_NAME, VK_MAX_EXTENSION_NAME_SIZE);
        result.specVersion = VK_STD_VULKAN_VIDEO_CODEC_H264_DECODE_SPEC_VERSION;
    } break;
    case WisVideoCodecFlagsH265: {
        strncpy(result.extensionName, VK_STD_VULKAN_VIDEO_CODEC_H265_DECODE_EXTENSION_NAME, VK_MAX_EXTENSION_NAME_SIZE);
        result.specVersion = VK_STD_VULKAN_VIDEO_CODEC_H265_DECODE_SPEC_VERSION;
    } break;
    case WisVideoCodecFlagsAV1: {
        strncpy(result.extensionName, VK_STD_VULKAN_VIDEO_CODEC_AV1_DECODE_EXTENSION_NAME, VK_MAX_EXTENSION_NAME_SIZE);
        result.specVersion = VK_STD_VULKAN_VIDEO_CODEC_AV1_DECODE_SPEC_VERSION;
    } break;
    case WisVideoCodecFlagsVP9: {
        strncpy(result.extensionName, VK_STD_VULKAN_VIDEO_CODEC_VP9_DECODE_EXTENSION_NAME, VK_MAX_EXTENSION_NAME_SIZE);
        result.specVersion = VK_STD_VULKAN_VIDEO_CODEC_VP9_DECODE_SPEC_VERSION;
    } break;
    default:
        return {};
    }
    return result;
}

using VKVideoCapsStruct = std::variant<
    VkVideoDecodeH264CapabilitiesKHR,
    VkVideoDecodeH265CapabilitiesKHR,
    VkVideoDecodeAV1CapabilitiesKHR,
    VkVideoDecodeVP9CapabilitiesKHR>;
using VKVideoProfileInfoStruct = std::variant<
    VkVideoDecodeH264ProfileInfoKHR,
    VkVideoDecodeH265ProfileInfoKHR,
    VkVideoDecodeAV1ProfileInfoKHR,
    VkVideoDecodeVP9ProfileInfoKHR>;

template <bool WithCaps>
using VKVideoResult = std::conditional_t<
    WithCaps,
    std::tuple<VKVideoProfileInfoStruct, VkVideoProfileInfoKHR, VKVideoCapsStruct, VkVideoCapabilitiesKHR>,
    std::tuple<VKVideoProfileInfoStruct, VkVideoProfileInfoKHR>>;

template <bool WithCaps>
inline WisResult VKFillVideoStructs(
    const impl::VKVideoTable& video_table,
    VkDevice device,
    WisDataFormat data_format,
    WisStdCodecProfile codec_profile,
    VKVideoResult<WithCaps>& result
) noexcept
{
    auto format_info = VKGetFormatInfo(data_format);
    if (format_info.bit_depth == 0 || format_info.chroma_subsampling == 0) {
        return WisResult{
            .status = WisStatusError,
            .platform_code = VK_ERROR_FEATURE_NOT_PRESENT,
            .error = "VKFillVideoStructs: Unsupported data format. Please provide a supported data format for video "
                     "decoding.",
        };
    }
    VkVideoProfileInfoKHR& profile_info = std::get<1>(result) = VkVideoProfileInfoKHR{
        .sType = VK_STRUCTURE_TYPE_VIDEO_PROFILE_INFO_KHR,
        .chromaSubsampling = format_info.chroma_subsampling,
        .lumaBitDepth = format_info.bit_depth,
        .chromaBitDepth = format_info.bit_depth,
    };

    if constexpr (WithCaps) {
        std::get<3>(result) = VkVideoCapabilitiesKHR{
            .sType = VK_STRUCTURE_TYPE_VIDEO_CAPABILITIES_KHR,
            .pNext = nullptr,
        };
    }

    uint32_t codec_type = codec_profile / 32; // Codec profiles are defined with step of 32
    switch (1 << codec_type) {
    case WisVideoCodecFlagsH264: {
        if constexpr (WithCaps) {
            auto& cap = std::get<2>(result).template emplace<VkVideoDecodeH264CapabilitiesKHR>(
                VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_CAPABILITIES_KHR
            );
            std::get<3>(result).pNext = &cap;
        }

        auto& info = std::get<0>(result).template emplace<VkVideoDecodeH264ProfileInfoKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_H264_PROFILE_INFO_KHR
        );

        switch (codec_profile) {
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
            return WisResult{
                .status = WisStatusError,
                .platform_code = VK_ERROR_FEATURE_NOT_PRESENT,
                .error = "VKFillVideoStructs: Profile is not supported by Vulkan video decoding extension.",
            };
        }

        profile_info.videoCodecOperation = VK_VIDEO_CODEC_OPERATION_DECODE_H264_BIT_KHR;
        profile_info.pNext = &info;
        return wis::detail::vk_success;
    }
    case WisVideoCodecFlagsH265: {
        if constexpr (WithCaps) {
            auto& cap = std::get<2>(result).template emplace<VkVideoDecodeH265CapabilitiesKHR>(
                VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_CAPABILITIES_KHR
            );
            std::get<3>(result).pNext = &cap;
        }

        auto& info = std::get<0>(result).template emplace<VkVideoDecodeH265ProfileInfoKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_H265_PROFILE_INFO_KHR
        );

        switch (codec_profile) {
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
            return WisResult{
                .status = WisStatusError,
                .platform_code = VK_ERROR_FEATURE_NOT_PRESENT,
                .error = "VKFillVideoStructs: Profile is not supported by Vulkan video decoding extension.",
            };
        }

        profile_info.videoCodecOperation = VK_VIDEO_CODEC_OPERATION_DECODE_H265_BIT_KHR;
        profile_info.pNext = &info;
        return wis::detail::vk_success;
    }
    case WisVideoCodecFlagsAV1: {
        if constexpr (WithCaps) {
            auto& cap = std::get<2>(result).template emplace<VkVideoDecodeAV1CapabilitiesKHR>(
                VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_CAPABILITIES_KHR
            );
            std::get<3>(result).pNext = &cap;
        }

        auto& info = std::get<0>(result).template emplace<VkVideoDecodeAV1ProfileInfoKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_AV1_PROFILE_INFO_KHR
        );

        switch (codec_profile) {
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
            return WisResult{
                .status = WisStatusError,
                .platform_code = VK_ERROR_FEATURE_NOT_PRESENT,
                .error = "VKFillVideoStructs: Profile is not supported by Vulkan video decoding extension.",
            };
        }
        info.filmGrainSupport = VK_TRUE;
        profile_info.videoCodecOperation = VK_VIDEO_CODEC_OPERATION_DECODE_AV1_BIT_KHR;
        profile_info.pNext = &info;
        return wis::detail::vk_success;
    }
    case WisVideoCodecFlagsVP9: {
        if constexpr (WithCaps) {
            auto& cap = std::get<2>(result).template emplace<VkVideoDecodeVP9CapabilitiesKHR>(
                VK_STRUCTURE_TYPE_VIDEO_DECODE_VP9_CAPABILITIES_KHR
            );
            std::get<3>(result).pNext = &cap;
        }

        auto& info = std::get<0>(result).template emplace<VkVideoDecodeVP9ProfileInfoKHR>(
            VK_STRUCTURE_TYPE_VIDEO_DECODE_VP9_PROFILE_INFO_KHR
        );

        switch (codec_profile) {
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
            return WisResult{
                .status = WisStatusError,
                .platform_code = VK_ERROR_FEATURE_NOT_PRESENT,
                .error = "VKFillVideoStructs: Profile is not supported by Vulkan video decoding extension.",
            };
        }

        profile_info.videoCodecOperation = VK_VIDEO_CODEC_OPERATION_DECODE_VP9_BIT_KHR;
        profile_info.pNext = &info;
        return wis::detail::vk_success;
    }
    default:
        return WisResult{
            .status = WisStatusError,
            .platform_code = VK_ERROR_FEATURE_NOT_PRESENT,
            .error = "VKFillVideoStructs: Unsupported codec. Please provide a valid codec profile.",
        };
    }
}

} // namespace wis::detail

//----------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------
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

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API WisResult
wisVKVideoDecodingExtensionQueryCodecCaps(WisVKVideoDecodingExtension* self, const WisVideoCodecDesc* codec_desc)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKVideoDecodingExtensionImpl>(self);
    VkVideoDecodeCapabilitiesKHR decode_caps{
        .sType = VK_STRUCTURE_TYPE_VIDEO_DECODE_CAPABILITIES_KHR,
    };

    wis::detail::VKVideoResult<true> fill_result{};
    auto res = wis::detail::VKFillVideoStructs<true>(
        *impl.video_table,
        impl.device,
        codec_desc->image_format,
        codec_desc->codec_profile,
        fill_result
    );
    if (!wis::detail::succeeded(VkResult(res.platform_code))) {
        return res;
    }

    auto& [profile_struct, profile_info, cap_struct, video_caps] = fill_result;
    decode_caps.pNext = video_caps.pNext;
    video_caps.pNext = &decode_caps;

    auto vr = impl.video_table->vkGetPhysicalDeviceVideoCapabilitiesKHR(impl.adapter, &profile_info, &video_caps);
    if (vr != VK_SUCCESS) {
        return wis::detail::make_result<wis::detail::Func(), "Unsupported codec parameter combination.">(vr);
    }

    // Check size
    if (codec_desc->width > video_caps.maxCodedExtent.width || codec_desc->height > video_caps.maxCodedExtent.height) {
        return wis::detail::make_result<
            wis::detail::Func(),
            "Requested resolution exceeds the maximum supported coded extent.">(VK_ERROR_FEATURE_NOT_PRESENT);
    }

    // Fill out the decode info based on the queried capabilities
    VkVideoProfileListInfoKHR profile_list_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_PROFILE_LIST_INFO_KHR,
        .pNext = nullptr,
        .profileCount = 1,
        .pProfiles = &profile_info,
    };
    VkPhysicalDeviceVideoFormatInfoKHR video_format_info{
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VIDEO_FORMAT_INFO_KHR,
        .pNext = &profile_list_info,
        .imageUsage = VK_IMAGE_USAGE_VIDEO_DECODE_DST_BIT_KHR | VK_IMAGE_USAGE_VIDEO_DECODE_DPB_BIT_KHR
    };
    uint32_t format_count = 0;
    vr = impl.video_table
             ->vkGetPhysicalDeviceVideoFormatPropertiesKHR(impl.adapter, &video_format_info, &format_count, nullptr);

    if (vr != VK_SUCCESS) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to query supported video formats.">(vr);
    }

    static constexpr uint32_t reasonable_format_count = 16;
    VkVideoFormatPropertiesKHR reasonable_formats[reasonable_format_count];
    wis::span<VkVideoFormatPropertiesKHR> format_props{reasonable_formats, format_count};
    std::unique_ptr<VkVideoFormatPropertiesKHR[]> dynamic_formats;
    if (format_count > reasonable_format_count) {
        dynamic_formats = wis::make_unique<VkVideoFormatPropertiesKHR[]>(format_count);
        if (!dynamic_formats) {
            return wis::detail::make_result<
                wis::detail::Func(),
                "Failed to allocate memory for video format properties.">(VK_ERROR_OUT_OF_HOST_MEMORY);
        }

        format_props = wis::span(dynamic_formats.get(), format_count);
    }
    for (auto& prop : format_props) {
        prop = {
            .sType = VK_STRUCTURE_TYPE_VIDEO_FORMAT_PROPERTIES_KHR,
        };
    }

    vr = impl.video_table->vkGetPhysicalDeviceVideoFormatPropertiesKHR(
        impl.adapter,
        &video_format_info,
        &format_count,
        format_props.data()
    );

    if (vr != VK_SUCCESS) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to query supported video formats.">(vr);
    }

    // Check if any of the supported formats match the requested format
    VkFormat requested_vk_format = wis::detail::VKConvert(codec_desc->image_format);
    for (const auto& prop : format_props) {
        if (prop.format == requested_vk_format) {
            return wis::detail::vk_success;
        }
    }

    return wis::detail::make_result<wis::detail::Func(), "Requested Format is not supported for selected codec.">(
        VK_ERROR_FORMAT_NOT_SUPPORTED
    );
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API WisResult wisVKVideoDecodingExtensionCreateDecoder(
    const WisVKVideoDecodingExtension* self,
    const WisVideoDecoderDesc* decoder_desc,
    WisVKVideoDecoder* video_decoder
)
{
    auto& impl = wis::from_handle_ref<const wis::impl::VKVideoDecodingExtensionImpl>(self);
    auto& device_header = impl.device_control_block->header;

    wis::detail::VKVideoResult<false> fill_result{};
    auto result = wis::detail::VKFillVideoStructs<false>(
        *impl.video_table,
        impl.device,
        decoder_desc->image_format,
        decoder_desc->codec_profile,
        fill_result
    );
    if (!wis::detail::succeeded(VkResult(result.platform_code))) {
        return result;
    }

    auto& [profile_struct, profile_info] = fill_result;

    VkExtensionProperties std_header_version = wis::detail::VKGetStdHeaderVersion(decoder_desc->codec_profile);
    VkVideoSessionCreateInfoKHR create_info{
        .sType = VK_STRUCTURE_TYPE_VIDEO_SESSION_CREATE_INFO_KHR,
        .pNext = nullptr,
        .queueFamilyIndex = device_header.queue_families[device_header.queue_residency[WisCommandQueueTypeVideoDecode]]
                                .family_index,
        .flags = 0,
        .pVideoProfile = &profile_info,
        .pictureFormat = wis::detail::VKConvert(decoder_desc->image_format),
        .maxCodedExtent =
            {
                .width = decoder_desc->width,
                .height = decoder_desc->height,
            },
        .referencePictureFormat = wis::detail::VKConvert(decoder_desc->image_format),
        .maxDpbSlots = decoder_desc->decode_picture_buffer_count,
        .maxActiveReferencePictures = decoder_desc->decode_picture_buffer_count,
        .pStdHeaderVersion = &std_header_version,
    };

    VkVideoSessionKHR video_session = VK_NULL_HANDLE;
    auto vr = impl.video_table->vkCreateVideoSessionKHR(impl.device, &create_info, nullptr, &video_session);
    if (vr != VK_SUCCESS) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to create video session.">(vr);
    }
    auto session_guard = wis::detail::VKMakeScopeGuard(video_session, [&]() {
        impl.video_table->vkDestroyVideoSessionKHR(impl.device, video_session, nullptr);
    });

    uint32_t memory_req_count = 0;
    vr = impl.video_table
             ->vkGetVideoSessionMemoryRequirementsKHR(impl.device, video_session, &memory_req_count, nullptr);

    if (vr != VK_SUCCESS) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to query video session memory requirements.">(vr);
    }

    static constexpr uint32_t reasonable_req_count = 8;
    VkVideoSessionMemoryRequirementsKHR reasonable_reqs[reasonable_req_count];
    std::unique_ptr<VkVideoSessionMemoryRequirementsKHR[]> dynamic_reqs;
    wis::span<VkVideoSessionMemoryRequirementsKHR> reqs{reasonable_reqs, memory_req_count};
    if (memory_req_count > reasonable_req_count) {
        dynamic_reqs = wis::make_unique<VkVideoSessionMemoryRequirementsKHR[]>(memory_req_count);
        if (!dynamic_reqs) {
            return wis::detail::make_result<
                wis::detail::Func(),
                "Failed to allocate memory for video session memory requirements.">(VK_ERROR_OUT_OF_HOST_MEMORY);
        }
        reqs = wis::span(dynamic_reqs.get(), memory_req_count);
    }
    for (auto& req : reqs) {
        req = {
            .sType = VK_STRUCTURE_TYPE_VIDEO_SESSION_MEMORY_REQUIREMENTS_KHR,
        };
    }

    vr = impl.video_table
             ->vkGetVideoSessionMemoryRequirementsKHR(impl.device, video_session, &memory_req_count, reqs.data());
    if (vr != VK_SUCCESS) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to query video session memory requirements.">(vr);
    }

    // TODO: Allocate the memory

    return wis::detail::vk_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisVKDestroyVideoDecoder(WisVKVideoDecoder* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKVideoDecoderImpl>(self);
    if (impl.video_session != VK_NULL_HANDLE) {
        impl.video_table
            ->vkDestroyVideoSessionKHR(impl.device_control_block->header.device, impl.video_session, nullptr);
        impl.video_session = VK_NULL_HANDLE;
    }
}
#endif // WIS_VK_VIDEO_CPP
