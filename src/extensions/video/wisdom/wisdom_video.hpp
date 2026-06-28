// This file is generated. Do not edit directly.
#ifndef WISDOM_VIDEO_HPP
#define WISDOM_VIDEO_HPP

#ifndef __cplusplus
#    error "This is a C++ only header"
#endif // __cplusplus

#ifndef FORCEVK_SWITCH
#    if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#        define FORCEVK_SWITCH 1
#    else
#        define FORCEVK_SWITCH 0
#    endif // WISDOM_VULKAN_FOUND
#endif // FORCEVK_SWITCH

#include "../video/generated/cpp_api.hpp"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH

namespace wis {

//==============================================================
// Handles
//==============================================================

using VideoDecoderParameters = wis::DX12VideoDecoderParameters;
using VideoDecoder = wis::DX12VideoDecoder;
using VideoDecodeCommandList = wis::DX12VideoDecodeCommandList;
using VideoDecodingExtension = wis::DX12VideoDecodingExtension;

//==============================================================
// Variants
//==============================================================

using VideoDecodeInputDesc = wis::DX12VideoDecodeInputDesc;
using VideoDecodeOutputDesc = wis::DX12VideoDecodeOutputDesc;
using VideoDecodePictureDesc = wis::DX12VideoDecodePictureDesc;

} // namespace wis

#elif defined(WISDOM_VULKAN)

namespace wis {

//==============================================================
// Handles
//==============================================================

using VideoDecoderParameters = wis::VKVideoDecoderParameters;
using VideoDecoder = wis::VKVideoDecoder;
using VideoDecodeCommandList = wis::VKVideoDecodeCommandList;
using VideoDecodingExtension = wis::VKVideoDecodingExtension;

//==============================================================
// Variants
//==============================================================

using VideoDecodeInputDesc = wis::VKVideoDecodeInputDesc;
using VideoDecodeOutputDesc = wis::VKVideoDecodeOutputDesc;
using VideoDecodePictureDesc = wis::VKVideoDecodePictureDesc;

} // namespace wis
#else
#    error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_VIDEO_HPP
