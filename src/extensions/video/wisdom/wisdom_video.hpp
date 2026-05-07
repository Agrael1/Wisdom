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

using VideoDecoder = wis::DX12VideoDecoder;
using VideoDecodingExtension = wis::DX12VideoDecodingExtension;

} // namespace wis

#elif defined(WISDOM_VULKAN)

namespace wis {

//==============================================================
// Handles
//==============================================================

using VideoDecoder = wis::VKVideoDecoder;
using VideoDecodingExtension = wis::VKVideoDecodingExtension;

} // namespace wis
#else
#    error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection
#endif // WISDOM_VIDEO_HPP
