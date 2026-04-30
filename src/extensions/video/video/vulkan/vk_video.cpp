#ifndef WIS_VK_VIDEO_CPP
#define WIS_VK_VIDEO_CPP

#include <wisdom/vulkan/detail/vk_detail.hpp>
#include <wisdom/vulkan/detail/vk_utils.hpp>
#include <wisdom/vulkan/vk_extensions.hpp>
#include <video/generated/cpp_api.hpp>

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
            wis::detail::VKReleaseDevice(impl.device_control_block);
        }
        if (!impl.supported_codecs) {
            return wis::detail::vk_success; // Nothing requested
        }

        impl.device = device_impl->device;
        impl.device_control_block = device_impl->device_header;
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
        wis::detail::VKReleaseDevice(impl.device_control_block);
        impl.device_control_block = nullptr;
    }
    impl.header = {nullptr};
}

WIS_EXTERN_C WISDOM_VIDEO_API bool wisVKVideoDecodingExtensionSupported(WisVKVideoDecodingExtension* self)
{
    auto& impl = wis::from_handle_ref<wis::impl::VKVideoDecodingExtensionImpl>(self);
    return impl.supported_codecs != 0; // Supported if at least one codec was successfully initialized
}

#endif // WIS_VK_VIDEO_CPP
