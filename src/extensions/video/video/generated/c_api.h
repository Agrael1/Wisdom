// This file is generated. Do not edit directly.
#ifndef WISDOM_VIDEO_C_API_H
#define WISDOM_VIDEO_C_API_H
#include <wisdom/generated/c_api.h>
#include "wisdom_exports.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//==============================================================
// Enums
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.1. Video codec flags. Used to request and check supported codecs.
 *
 * */
typedef enum WisVideoCodecFlags {
    WisVideoCodecFlagsNone = 0, ///< No video is requested. The extension will not initialize.
    WisVideoCodecFlagsH264 = (1u << 0), ///< H.264 video codec.
    WisVideoCodecFlagsH265 = (1u << 1), ///< H.265 video codec.
    WisVideoCodecFlagsAV1 = (1u << 2), ///< AV1 video codec.
    WisVideoCodecFlagsVP9 = (1u << 3), ///< VP9 video codec.
} WisVideoCodecFlags;

#ifdef WISDOM_DX12
/**
 * @brief Provided by Wisdom 0.7.1. Handle for the video decoding extension. Used  to manage video decoding resources
 * and operations.
 *
 * */
WIS_DEFINE_DX12_DEVICE_EXT_HANDLE(WisDX12VideoDecodingExtension, 2);

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisVideoDecodingExtension handle.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 *
 * */
WISDOM_VIDEO_API void wisDX12DestroyVideoDecodingExtension(WisDX12VideoDecodingExtension* self);

/**
 * @brief Provided by Wisdom 0.7.1. Initializes WisVideoDecodingExtension with a request of codecs.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @param request_codecs Bitmask of requested video codecs. The extension will attempt to initialize with support for
 * these codecs.
 *
 * */
WISDOM_VIDEO_API void wisDX12InitVideoDecodingExtension(
    WisDX12VideoDecodingExtension* self,
    WisVideoCodecFlags request_codecs
);

/**
 * @brief Provided by Wisdom 0.7.1. Checks if video decode is supported on the current device.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @return bool true if raytracing is supported, false otherwise.
 *
 * */
WISDOM_VIDEO_API bool wisDX12VideoDecodingExtensionSupported(WisDX12VideoDecodingExtension* self);

#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
/**
 * @brief Provided by Wisdom 0.7.1. Handle for the video decoding extension. Used  to manage video decoding resources
 * and operations.
 *
 * */
WIS_DEFINE_VK_DEVICE_EXT_HANDLE(WisVKVideoDecodingExtension, 4);

/**
 * @brief Provided by Wisdom 0.7.1. Destroys a WisVideoDecodingExtension handle.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 *
 * */
WISDOM_VIDEO_API void wisVKDestroyVideoDecodingExtension(WisVKVideoDecodingExtension* self);

/**
 * @brief Provided by Wisdom 0.7.1. Initializes WisVideoDecodingExtension with a request of codecs.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @param request_codecs Bitmask of requested video codecs. The extension will attempt to initialize with support for
 * these codecs.
 *
 * */
WISDOM_VIDEO_API void wisVKInitVideoDecodingExtension(
    WisVKVideoDecodingExtension* self,
    WisVideoCodecFlags request_codecs
);

/**
 * @brief Provided by Wisdom 0.7.1. Checks if video decode is supported on the current device.
 * @param self is a pointer to the valid WisVideoDecodingExtension instance.
 * @return bool true if raytracing is supported, false otherwise.
 *
 * */
WISDOM_VIDEO_API bool wisVKVideoDecodingExtensionSupported(WisVKVideoDecodingExtension* self);

#endif // WISDOM_VULKAN

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_VIDEO_C_API_H
