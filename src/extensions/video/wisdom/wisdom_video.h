// This file is generated. Do not edit directly.
#ifndef WISDOM_VIDEO_H
#define WISDOM_VIDEO_H

#ifdef WISDOM_UWP
static_assert(WISDOM_UWP && _WIN32, "Platform error");
#endif // WISDOM_UWP

#ifndef FORCEVK_SWITCH
#    if defined(WISDOM_VULKAN) && defined(WISDOM_FORCE_VULKAN)
#        define FORCEVK_SWITCH 1
#    else
#        define FORCEVK_SWITCH 0
#    endif // WISDOM_VULKAN_FOUND
#endif // FORCEVK_SWITCH

#include "../video/generated/c_api.h"

#if defined(WISDOM_DX12) && !FORCEVK_SWITCH

//==============================================================
// Handles
//==============================================================

typedef struct WisDX12VideoDecoder WisVideoDecoder;
typedef struct WisDX12VideoDecodeCommandList WisVideoDecodeCommandList;
typedef struct WisDX12VideoDecodingExtension WisVideoDecodingExtension;

//==============================================================
// Variants
//==============================================================

typedef struct WisDX12VideoDecodeInputDesc WisVideoDecodeInputDesc;
typedef struct WisDX12VideoDecodeOutputDesc WisVideoDecodeOutputDesc;
typedef struct WisDX12VideoDecodePictureDesc WisVideoDecodePictureDesc;

//==============================================================
// Functions
//==============================================================

#    define wisDestroyVideoDecoder                     wisDX12DestroyVideoDecoder
#    define wisDestroyVideoDecodeCommandList           wisDX12DestroyVideoDecodeCommandList
#    define wisDestroyVideoDecodingExtension           wisDX12DestroyVideoDecodingExtension
#    define wisInitVideoDecodingExtension              wisDX12InitVideoDecodingExtension
#    define wisVideoDecodingExtensionQueryCodecCaps    wisDX12VideoDecodingExtensionQueryCodecCaps
#    define wisVideoDecodingExtensionCreateDecoder     wisDX12VideoDecodingExtensionCreateDecoder
#    define wisVideoDecodingExtensionCreateCommandList wisDX12VideoDecodingExtensionCreateCommandList
#    define wisVideoDecodeCommandListBegin             wisDX12VideoDecodeCommandListBegin
#    define wisVideoDecodeCommandListEnd               wisDX12VideoDecodeCommandListEnd
#    define wisVideoDecodeCommandListInsertBarriers    wisDX12VideoDecodeCommandListInsertBarriers
#    define wisVideoDecodeCommandListDecodeFrame       wisDX12VideoDecodeCommandListDecodeFrame

#elif defined(WISDOM_VULKAN)

//==============================================================
// Handles
//==============================================================

typedef struct WisVKVideoDecoder WisVideoDecoder;
typedef struct WisVKVideoDecodeCommandList WisVideoDecodeCommandList;
typedef struct WisVKVideoDecodingExtension WisVideoDecodingExtension;

//==============================================================
// Variants
//==============================================================

typedef struct WisVKVideoDecodeInputDesc WisVideoDecodeInputDesc;
typedef struct WisVKVideoDecodeOutputDesc WisVideoDecodeOutputDesc;
typedef struct WisVKVideoDecodePictureDesc WisVideoDecodePictureDesc;

//==============================================================
// Functions
//==============================================================

#    define wisDestroyVideoDecoder                     wisVKDestroyVideoDecoder
#    define wisDestroyVideoDecodeCommandList           wisVKDestroyVideoDecodeCommandList
#    define wisDestroyVideoDecodingExtension           wisVKDestroyVideoDecodingExtension
#    define wisInitVideoDecodingExtension              wisVKInitVideoDecodingExtension
#    define wisVideoDecodingExtensionQueryCodecCaps    wisVKVideoDecodingExtensionQueryCodecCaps
#    define wisVideoDecodingExtensionCreateDecoder     wisVKVideoDecodingExtensionCreateDecoder
#    define wisVideoDecodingExtensionCreateCommandList wisVKVideoDecodingExtensionCreateCommandList
#    define wisVideoDecodeCommandListBegin             wisVKVideoDecodeCommandListBegin
#    define wisVideoDecodeCommandListEnd               wisVKVideoDecodeCommandListEnd
#    define wisVideoDecodeCommandListInsertBarriers    wisVKVideoDecodeCommandListInsertBarriers
#    define wisVideoDecodeCommandListDecodeFrame       wisVKVideoDecodeCommandListDecodeFrame

#else
#    error "No API selected for Wisdom. Define WISDOM_DX12 or WISDOM_VULKAN."
#endif // API selection

#ifndef WISDOM_HANDLE_VALID_DEFINED
#    define WISDOM_HANDLE_VALID_DEFINED
static inline bool wisHandleValid(const void* handle)
{
    const uint64_t zero = 0;
    return memcmp(handle, &zero, sizeof(uint64_t)) != 0;
}

#endif // WISDOM_HANDLE_VALID_DEFINED

#endif // WISDOM_VIDEO_H
