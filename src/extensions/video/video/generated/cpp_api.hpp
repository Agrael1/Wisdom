// This file is generated. Do not edit directly.
#ifndef WISDOM_VIDEO_CPP_API_HPP
#define WISDOM_VIDEO_CPP_API_HPP
#ifndef __cplusplus
#    error C++ is required to include this header.
#endif // __cplusplus

#include <wisdom/generated/cpp_api.hpp>
#include "c_api.h"
#include "wisdom_exports.h"

namespace wis {

//==============================================================
// Enums
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.1. Video codec flags. Used to request and check supported codecs.
 *
 * */
enum class VideoCodecFlags : uint32_t {
    None = 0, ///< No video is requested. The extension will not initialize.
    H264 = (1u << 0), ///< H.264 video codec.
    H265 = (1u << 1), ///< H.265 video codec.
    AV1 = (1u << 2), ///< AV1 video codec.
    VP9 = (1u << 3), ///< VP9 video codec.
};
WISDOM_DEFINE_ENUM_OPERATORS(VideoCodecFlags)

} // namespace wis

#ifdef WISDOM_DX12
#    include <video/dx12/dx12_types.hpp>

namespace wis {
struct DX12VideoDecodingExtensionDeleter {
    void operator()(WisDX12VideoDecodingExtension* handle) noexcept { ::wisDX12DestroyVideoDecodingExtension(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for the video decoding extension. Used  to manage video decoding resources
 * and operations.
 *
 * */
class DX12VideoDecodingExtension : public wis::impl::Implements<
                                       wis::impl::DX12VideoDecodingExtensionImpl,
                                       WisDX12VideoDecodingExtension,
                                       wis::DX12VideoDecodingExtensionDeleter>
{
public:
    DX12VideoDecodingExtension(wis::VideoCodecFlags request_codecs) noexcept
        : ImplType(wis::in_place)
    {
        ::wisDX12InitVideoDecodingExtension(GetStorage(), static_cast<WisVideoCodecFlags>(request_codecs));
    }
    // Operator & overload
    wis::DX12DeviceExtensionHeader* operator&() noexcept { return &GetMutableInternal().header; }

public:
    /**
     * @brief Provided by Wisdom 0.7.1. Checks if video decode is supported on the current device.
     * @return bool true if raytracing is supported, false otherwise.
     *
     * */
    WIS_NODISCARD inline bool Supported() noexcept
    {
        return (::wisDX12VideoDecodingExtensionSupported(&_impl_storage));
    }
};

} // namespace wis
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#    include <video/vulkan/vk_types.hpp>

namespace wis {
struct VKVideoDecodingExtensionDeleter {
    void operator()(WisVKVideoDecodingExtension* handle) noexcept { ::wisVKDestroyVideoDecodingExtension(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.1. Handle for the video decoding extension. Used  to manage video decoding resources
 * and operations.
 *
 * */
class VKVideoDecodingExtension : public wis::impl::Implements<
                                     wis::impl::VKVideoDecodingExtensionImpl,
                                     WisVKVideoDecodingExtension,
                                     wis::VKVideoDecodingExtensionDeleter>
{
public:
    VKVideoDecodingExtension(wis::VideoCodecFlags request_codecs) noexcept
        : ImplType(wis::in_place)
    {
        ::wisVKInitVideoDecodingExtension(GetStorage(), static_cast<WisVideoCodecFlags>(request_codecs));
    }
    // Operator & overload
    wis::VKDeviceExtensionHeader* operator&() noexcept { return &GetMutableInternal().header; }

public:
    /**
     * @brief Provided by Wisdom 0.7.1. Checks if video decode is supported on the current device.
     * @return bool true if raytracing is supported, false otherwise.
     *
     * */
    WIS_NODISCARD inline bool Supported() noexcept { return (::wisVKVideoDecodingExtensionSupported(&_impl_storage)); }
};

} // namespace wis
#endif // WISDOM_VULKAN

#endif // WISDOM_VIDEO_CPP_API_HPP
