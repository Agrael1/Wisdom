// This file is generated. Do not edit directly.
#ifndef WISDOM_CORE_C_API_H
#define WISDOM_CORE_C_API_H
#include <wisdom/global/definitions.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//==============================================================
// Enums
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Defines common return status codes. Compare against `WisStatusOk` for success.
 *
 * */
typedef enum WisStatus {
    WisStatusOk                = 0, ///< Operation succeeded.
    WisStatusTimeout           = 1, ///< Operation timed out.
    WisStatusPartial           = 2, ///< Operation partially succeeded.
    WisStatusInvalidArgument   = -1, ///< One or more arguments, or parts of arguments passed to the function were incorrect.
    WisStatusOutOfHostMemory   = -2, ///< There is no more host memory available.
    WisStatusOutOfDeviceMemory = -3, ///< There is no more device memory available.
    WisStatusDeviceLost        = -4, ///< Device driver was forcefully stopped.
    WisStatusOccluded          = -5, ///< Swap chain presentation was not visible to the user. Rendering is too fast.
    WisStatusValidationFailed  = -6, ///< A validation layer found an error.
    WisStatusFail              = -7, ///< Operation expectedly failed.
    WisStatusError             = -10000, ///< Operation failed.
} WisStatus;

/**
 * @brief Provided by Wisdom 0.7.0. Data format for data.
 * Used as a template for data in several cases.
 * Can be used to describe:
 * - Vertex buffer data format
 * - Texture data format
 * - Render target data format
 * - Depth stencil data format
 *
 * */
typedef enum WisDataFormat {
    WisDataFormatUnknown = 0, ///< Unknown format.
    /**
     * @brief
     * 32 bit per channel RGBA format.
     * A four-component, 128-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    WisDataFormatRGBA32Float = 2,
    /**
     * @brief
     * 32 bit per channel RGBA format.
     * A four-component, 128-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    WisDataFormatRGBA32Uint = 3,
    /**
     * @brief
     * 32 bit per channel RGBA format.
     * A four-component, 128-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    WisDataFormatRGBA32Sint = 4,
    /**
     * @brief
     * 32 bit per channel RGB format.
     * A three-component, 96-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    WisDataFormatRGB32Float = 6,
    /**
     * @brief
     * 32 bit per channel RGB format.
     * A three-component, 96-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    WisDataFormatRGB32Uint = 7,
    /**
     * @brief
     * 32 bit per channel RGB format.
     * A three-component, 96-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    WisDataFormatRGB32Sint = 8,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 64-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    WisDataFormatRGBA16Float = 10,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 64-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    WisDataFormatRGBA16Unorm = 11,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 64-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    WisDataFormatRGBA16Uint = 12,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 64-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    WisDataFormatRGBA16Snorm = 13,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 64-bit signed integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    WisDataFormatRGBA16Sint = 14,
    /**
     * @brief
     * 32 bit per channel RG format.
     * A two-component, 64-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    WisDataFormatRG32Float = 16,
    /**
     * @brief
     * 32 bit per channel RG format.
     * A two-component, 64-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    WisDataFormatRG32Uint = 17,
    /**
     * @brief
     * 32 bit per channel RG format.
     * A two-component, 64-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    WisDataFormatRG32Sint = 18,
    /**
     * @brief
     * 32 bit depth, 8 bit stencil format.
     * A combined depth/stencil format with a 32-bit depth component in bytes 0..3
     * and an 8-bit stencil component in bytes 4..4.
     * */
    WisDataFormatD32FloatS8Uint = 20,
    /**
     * @brief
     * 10 bit per channel RGB format with 2 bit alpha.
     * A four-component, 32-bit unsigned normalized format that has
     * a 10-bit R component in bits 0..9,
     * a 10-bit G component in bits 10..19,
     * a 10-bit B component in bits 20..29,
     * a 2-bit A component in bits 30..31.
     * */
    WisDataFormatRGB10A2Unorm = 24,
    /**
     * @brief
     * 10 bit per channel RGB format with 2 bit alpha.
     * A four-component, 32-bit unsigned integer format that has
     * a 10-bit R component in bits 0..9,
     * a 10-bit G component in bits 10..19,
     * a 10-bit B component in bits 20..29,
     * a 2-bit A component in bits 30..31.
     * */
    WisDataFormatRGB10A2Uint = 25,
    /**
     * @brief
     * 11 bit per channel RG format with 10 bit blue.
     * A three-component, 32-bit signed floating-point format that has
     * a 11-bit R component in bits 0..10,
     * a 11-bit G component in bits 11..21,
     * a 10-bit B component in bits 22..31.
     * */
    WisDataFormatRG11B10Float = 26,
    /**
     * @brief
     * 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    WisDataFormatRGBA8Unorm = 28,
    /**
     * @brief
     * 8 bit per channel RGBA format. SRGB non-linear color space.
     * A four-component, 32-bit unsigned normalized non-linear SRGB format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    WisDataFormatRGBA8UnormSrgb = 29,
    /**
     * @brief
     * 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    WisDataFormatRGBA8Uint = 30,
    /**
     * @brief
     * 8 bit per channel RGBA format.
     * A four-component, 32-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    WisDataFormatRGBA8Snorm = 31,
    /**
     * @brief
     * 8 bit per channel RGBA format.
     * A four-component, 32-bit signed integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    WisDataFormatRGBA8Sint = 32,
    /**
     * @brief
     * 16 bit per channel RG format.
     * A two-component, 32-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    WisDataFormatRG16Float = 34,
    /**
     * @brief
     * 16 bit per channel RG format.
     * A two-component, 32-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    WisDataFormatRG16Unorm = 35,
    /**
     * @brief
     * 16 bit per channel RG format.
     * A two-component, 32-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    WisDataFormatRG16Uint = 36,
    /**
     * @brief
     * 16 bit per channel RG format.
     * A two-component, 32-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    WisDataFormatRG16Snorm = 37,
    /**
     * @brief
     * 16 bit per channel RG format.
     * A two-component, 32-bit signed integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    WisDataFormatRG16Sint = 38,
    /**
     * @brief
     * 32 bit depth format.
     * A 32-bit depth format supporting 32-bit floating-point depth values.
     * */
    WisDataFormatD32Float = 40,
    /**
     * @brief
     * 32 bit per channel R format.
     * A one-component, 32-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3.
     * */
    WisDataFormatR32Float = 41,
    /**
     * @brief
     * 32 bit per channel R format.
     * A one-component, 32-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3.
     * */
    WisDataFormatR32Uint = 42,
    /**
     * @brief
     * 32 bit per channel R format.
     * A one-component, 32-bit signed integer format that has
     * a 32-bit R component in bytes 0..3.
     * */
    WisDataFormatR32Sint = 43,
    /**
     * @brief
     * 24 bit depth, 8 bit stencil format.
     * A combined depth/stencil format with a 24-bit depth component in bytes 0..2
     * and an 8-bit stencil component in bytes 3..3.
     * */
    WisDataFormatD24UnormS8Uint = 45,
    /**
     * @brief
     * 8 bit per channel RG format.
     * A two-component, 16-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    WisDataFormatRG8Unorm = 49,
    /**
     * @brief
     * 8 bit per channel RG format.
     * A two-component, 16-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    WisDataFormatRG8Uint = 50,
    /**
     * @brief
     * 8 bit per channel RG format.
     * A two-component, 16-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    WisDataFormatRG8Snorm = 51,
    /**
     * @brief
     * 8 bit per channel RG format.
     * A two-component, 16-bit signed integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    WisDataFormatRG8Sint = 52,
    /**
     * @brief
     * 16 bit per channel R format.
     * A one-component, 16-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1.
     * */
    WisDataFormatR16Float = 54,
    /**
     * @brief
     * 16 bit depth format.
     * A 16-bit depth format supporting 16-bit unsigned normalized depth values.
     * */
    WisDataFormatD16Unorm = 55,
    /**
     * @brief
     * 16 bit per channel R format.
     * A one-component, 16-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1.
     * */
    WisDataFormatR16Unorm = 56,
    /**
     * @brief
     * 16 bit per channel R format.
     * A one-component, 16-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1.
     * */
    WisDataFormatR16Uint = 57,
    /**
     * @brief
     * 16 bit per channel R format.
     * A one-component, 16-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1.
     * */
    WisDataFormatR16Snorm = 58,
    /**
     * @brief
     * 16 bit per channel R format.
     * A one-component, 16-bit signed integer format that has
     * a 16-bit R component in bytes 0..1.
     * */
    WisDataFormatR16Sint = 59,
    /**
     * @brief
     * 8 bit per channel R format.
     * A one-component, 8-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0.
     * */
    WisDataFormatR8Unorm = 61,
    /**
     * @brief
     * 8 bit per channel R format.
     * A one-component, 8-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0.
     * */
    WisDataFormatR8Uint = 62,
    /**
     * @brief
     * 8 bit per channel R format.
     * A one-component, 8-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0.
     * */
    WisDataFormatR8Snorm = 63,
    /**
     * @brief
     * 8 bit per channel R format.
     * A one-component, 8-bit signed integer format that has
     * an 8-bit R component in bytes 0..0.
     * */
    WisDataFormatR8Sint = 64,
    /**
     * @brief
     * 9 bit per channel RGB format with shared exponent.
     * A three-component, 32-bit unsigned floating-point format that has
     * a 9-bit R component in bits 0..8,
     * a 9-bit G component in bits 9..17,
     * a 9-bit B component in bits 18..26,
     * a shared 5-bit exponent in bits 27..31.
     * */
    WisDataFormatRGB9E5UFloat = 67,
    /**
     * @brief
     * BC1/DXT1 compressed format.
     * A four-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGB texel data, and provides 1 bit of alpha.
     * */
    WisDataFormatBC1RGBAUnorm = 71,
    /**
     * @brief
     * BC1/DXT1 compressed format.
     * A four-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGB texel data with sRGB nonlinear encoding, and provides 1 bit of alpha.
     * */
    WisDataFormatBC1RGBAUnormSrgb = 72,
    /**
     * @brief
     * BC2/DXT3 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    WisDataFormatBC2RGBAUnorm = 74,
    /**
     * @brief
     * BC2/DXT3 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    WisDataFormatBC2RGBAUnormSrgb = 75,
    /**
     * @brief
     * BC3/DXT5 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values
     * followed by 64 bits encoding RGB values
     * */
    WisDataFormatBC3RGBAUnorm = 77,
    /**
     * @brief
     * BC3/DXT5 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    WisDataFormatBC3RGBAUnormSrgb = 78,
    /**
     * @brief
     * BC4 compressed format.
     * A single-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized red texel data
     * */
    WisDataFormatBC4RUnorm = 80,
    /**
     * @brief
     * BC4 compressed format.
     * A single-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed normalized red texel data
     * */
    WisDataFormatBC4RSnorm = 81,
    /**
     * @brief
     * BC5 compressed format.
     * A two-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized red and green texel data
     * */
    WisDataFormatBC5RGUnorm = 83,
    /**
     * @brief
     * BC5 compressed format.
     * A two-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed normalized red and green texel data
     * */
    WisDataFormatBC5RGSnorm = 84,
    /**
     * @brief
     * 16 bit per channel RGB format.
     * A three-component, 16-bit unsigned normalized format that has
     * a 5-bit B component in bits 0..4,
     * a 6-bit G component in bits 5..10,
     * a 5-bit R component in bits 11..15.
     * */
    WisDataFormatB5G6R5Unorm = 85,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 16-bit unsigned normalized format that has
     * a 5-bit B component in bits 0..4,
     * a 5-bit G component in bits 5..9,
     * a 5-bit R component in bits 10..14,
     * a 1-bit A component in bits 15..15.
     * */
    WisDataFormatB5G5R5A1Unorm = 86,
    /**
     * @brief
     * 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned normalized format that has
     * an 8-bit B component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit R component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    WisDataFormatBGRA8Unorm = 87,
    /**
     * @brief
     * 8 bit per channel RGBA format. SRGB non-linear color space.
     * A four-component, 32-bit unsigned normalized non-linear SRGB format that has
     * an 8-bit B component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit R component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    WisDataFormatBGRA8UnormSrgb = 91,
    /**
     * @brief
     * BC6H compressed format.
     * A three-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned floating-point RGB texel data
     * */
    WisDataFormatBC6HUfloat16 = 95,
    /**
     * @brief
     * BC6H compressed format.
     * A three-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed floating-point RGB texel data
     * */
    WisDataFormatBC6HSfloat16 = 96,
    /**
     * @brief
     * BC7 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data
     * */
    WisDataFormatBC7RGBAUnorm = 98,
    /**
     * @brief
     * BC7 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * */
    WisDataFormatBC7RGBAUnormSrgb = 99,
    /**
     * @brief
     * 4 bit per channel RGB format.
     * A four-component, 16-bit unsigned normalized format that has
     * a 4-bit B component in bits 0..3,
     * a 4-bit G component in bits 4..7,
     * a 4-bit R component in bits 8..11,
     * a 4-bit A component in bits 12..15.
     * */
    WisDataFormatBGRA4Unorm = 115,
} WisDataFormat;

/**
 * @brief Provided by Wisdom 0.7.0. Sample rate for multisampling.
 *
 * */
typedef enum WisSampleCount {
    WisSampleCountS1  = 1, ///< 1 sample per pixel.
    WisSampleCountS2  = 2, ///< 2 samples per pixel.
    WisSampleCountS4  = 4, ///< 4 samples per pixel.
    WisSampleCountS8  = 8, ///< 8 samples per pixel.
    WisSampleCountS16 = 16, ///< 16 samples per pixel.
} WisSampleCount;

/**
 * @brief Provided by Wisdom 0.7.0. Texture layout. Determines how texture will be accessed.
 *
 * */
typedef enum WisTextureLayout {
    WisTextureLayoutTexture1D        = 2, ///< Texture is 1D array of data. Behaves similarly to Buffer.
    WisTextureLayoutTexture1DArray   = 3, ///< Texture is an array of 1D data.
    WisTextureLayoutTexture2D        = 4, ///< Texture is 2D image, default texture type.
    WisTextureLayoutTexture2DArray   = 5, ///< Texture is an array of 2D images. Can also be used for Cube maps.
    WisTextureLayoutTexture2DMS      = 6, ///< Texture is 2D multisampled image.
    WisTextureLayoutTexture2DMSArray = 7, ///< Texture is an array of 2D multisampled images.
    WisTextureLayoutTexture3D        = 8, ///< Texture is 3D volume.
    WisTextureLayoutTextureCube      = 9, ///< Texture is a cube map. Behaves similarly to Texture2DArray with 6 layers.
    WisTextureLayoutTextureCubeArray = 10, ///< Texture is an array of cube maps. Behaves similarly to Texture2DArray with 6 layers per cube map.
} WisTextureLayout;

/**
 * @brief Provided by Wisdom 0.7.0. Orders the adapters according to preference using builtin heuristics of underlying APIs. For DirectX 12, this translates directly to `DXGI_GPU_PREFERENCE`. For Vulkan, sorting is based on `VkPhysicalDeviceType` heuristics.
 *
 * */
typedef enum WisAdapterPreference {
    WisAdapterPreferenceNone           = 0, ///< No particular preference, list adapters in system devised order.
    WisAdapterPreferenceMinConsumption = 1, ///< List the adapters from low power consumption to high. DirectX 12: Integrated, Discrete, External, Software. Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU.
    WisAdapterPreferencePerformance    = 2, ///< List the adapters from high performance to low. DirectX 12: External, Discrete, Integrated, Software. Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
} WisAdapterPreference;

/**
 * @brief Provided by Wisdom 0.7.0. Defines severity levels for logging and debugging messages.
 *
 * */
typedef enum WisSeverity {
    WisSeverityVerbose = 0, ///< Verbose level messages, typically used for detailed debugging information.
    WisSeverityInfo    = 1, ///< Informational messages that highlight the progress of the application.
    WisSeverityWarning = 2, ///< Potentially harmful situations that warrant attention but do not prevent normal operation.
    WisSeverityError   = 3, ///< Error events that might still allow the application to continue running.
    WisSeverityFatal   = 4, ///< Severe error events that will presumably lead the application to abort.
} WisSeverity;

/**
 * @brief Provided by Wisdom 0.7.0. Defines types of command queues that can be created on the device. Each type corresponds to a specific kind of workload that the GPU can handle.
 *
 * */
typedef enum WisCommandQueueType {
    WisCommandQueueTypeGraphics    = 0, ///< Command queue for graphics operations, including rendering and compute tasks.
    WisCommandQueueTypeCompute     = 1, ///< Command queue dedicated to compute operations, optimized for parallel processing tasks.
    WisCommandQueueTypeTransfer    = 2, ///< Command queue for data transfer operations, such as copying resources between buffers and images.
    WisCommandQueueTypeVideoDecode = 3, ///< Command queue for video decoding operations.
    WisCommandQueueTypeVideoEncode = 4, ///< Command queue for video encoding operations.
    WisCommandQueueTypeCount       = 5, ///< Number of command queue types available.
} WisCommandQueueType;

/**
 * @brief Provided by Wisdom 0.7.0. Global queue priority. Higher priority queues get more GPU time, but @wis_may cause performance issues if overused.
 *
 * */
typedef enum WisCommandQueuePriority {
    WisCommandQueuePriorityNormal   = 0, ///< Normal queue priority.
    WisCommandQueuePriorityHigh     = 1, ///< High queue priority.
    WisCommandQueuePriorityRealtime = 2, ///< Global realtime queue priority. Requires special GPU support and @wis_may cause performance issues if used on unsupported hardware.
} WisCommandQueuePriority;

/**
 * @brief Provided by Wisdom 0.7.0. Defines types of pipelines that can be created on the device. Each type corresponds to a specific kind of workload that the GPU can handle.
 *
 * */
typedef enum WisPipelineType {
    WisPipelineTypeGraphics   = 0, ///< Pipeline for graphics operations, including rendering tasks.
    WisPipelineTypeCompute    = 1, ///< Pipeline dedicated to compute operations, optimized for parallel processing tasks.
    WisPipelineTypeRayTracing = 2, ///< Pipeline for ray tracing operations, optimized for ray tracing workloads.
} WisPipelineType;

/**
 * @brief Provided by Wisdom 0.7.0. Shader stages that can be used in the pipeline. Main use is Root signature and descriptor management. Stages have no granularity, either all or one can be selected.
 *
 * */
typedef enum WisShaderVisibility {
    WisShaderVisibilityAll           = 0, ///< All shader stages.
    WisShaderVisibilityVertex        = 1, ///< Vertex shader stage.
    WisShaderVisibilityHull          = 2, ///< Hull/Tessellation control shader stage.
    WisShaderVisibilityDomain        = 3, ///< Domain/Tessellation evaluation shader stage.
    WisShaderVisibilityGeometry      = 4, ///< Geometry shader stage.
    WisShaderVisibilityPixel         = 5, ///< Pixel/Fragment shader stage.
    WisShaderVisibilityAmplification = 6, ///< Amplification shader stage.
    WisShaderVisibilityMesh          = 7, ///< Mesh shader stage.
    WisShaderVisibilityCount         = 8, ///< Number of stages.
} WisShaderVisibility;

/**
 * @brief Provided by Wisdom 0.7.0. Type of the descriptor in the descriptor table.
 *
 * */
typedef enum WisDescriptorType {
    WisDescriptorTypeSampler               = 0, ///< Descriptor is a sampler.
    WisDescriptorTypeConstantBuffer        = 1, ///< Descriptor is a constant buffer.
    WisDescriptorTypeTexture               = 2, ///< Descriptor is a texture.
    WisDescriptorTypeRWTexture             = 3, ///< Descriptor is an unordered access read-write texture.
    WisDescriptorTypeRWBuffer              = 4, ///< Descriptor is an unordered access read-write buffer.
    WisDescriptorTypeBuffer                = 5, ///< Descriptor is a shader resource buffer.
    WisDescriptorTypeAccelerationStructure = 6, ///< Descriptor is an acceleration structure.
    WisDescriptorTypeCount                 = 7, ///< Descriptor is a mutable type.
} WisDescriptorType;

/**
 * @brief Provided by Wisdom 0.7.0. Comparison function for depth and stencil operations.
 *
 * */
typedef enum WisCompareOp {
    WisCompareOpNone         = 0, ///< No comparison.
    WisCompareOpNever        = 1, ///< Always fail the comparison.
    WisCompareOpLess         = 2, ///< Pass the comparison if the source value is less than the destination value.
    WisCompareOpEqual        = 3, ///< Pass the comparison if the source value is equal to the destination value.
    WisCompareOpLessEqual    = 4, ///< Pass the comparison if the source value is less than or equal to the destination value.
    WisCompareOpGreater      = 5, ///< Pass the comparison if the source value is greater than the destination value.
    WisCompareOpNotEqual     = 6, ///< Pass the comparison if the source value is not equal to the destination value.
    WisCompareOpGreaterEqual = 7, ///< Pass the comparison if the source value is greater than or equal to the destination value.
    WisCompareOpAlways       = 8, ///< Always pass the comparison.
} WisCompareOp;

/**
 * @brief Provided by Wisdom 0.7.0. Address mode for texture sampling.
 *
 * */
typedef enum WisAddressMode {
    WisAddressModeRepeat            = 0, ///< Repeat the texture.
    WisAddressModeMirroredRepeat    = 1, ///< Repeat the texture with mirroring.
    WisAddressModeClampToEdge       = 2, ///< Clamp the texture to the edge.
    WisAddressModeClampToBorder     = 3, ///< Clamp the texture to the border.
    WisAddressModeMirrorClampToEdge = 4, ///< Mirror and clamp the texture to the edge.
} WisAddressMode;

/**
 * @brief Provided by Wisdom 0.7.0. Filtering mode for texture sampling.
 *
 * */
typedef enum WisFilter {
    WisFilterPoint  = 0, ///< Nearest neighbor filtering.
    WisFilterLinear = 1, ///< Linear filtering.
} WisFilter;

/**
 * @brief Provided by Wisdom 0.7.0. Reduction mode for texture sampling when min and mag filters are different.
 *
 * */
typedef enum WisReductionMode {
    WisReductionModeStandard = 0, ///< Standard filtering behavior.
    WisReductionModeMin      = 1, ///< Use the minimum value of the samples for filtering.
    WisReductionModeMax      = 2, ///< Use the maximum value of the samples for filtering.
} WisReductionMode;

/**
 * @brief Provided by Wisdom 0.7.0. Predefined static border colors for samplers. Used when address mode is set to Border.
 *
 * */
typedef enum WisStaticBorder {
    WisStaticBorderTransparentBlack = 0, ///< Transparent black border color.
    WisStaticBorderOpaqueBlack      = 1, ///< Opaque black border color.
    WisStaticBorderOpaqueWhite      = 2, ///< Opaque white border color.
} WisStaticBorder;

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor heap type.
 *
 * */
typedef enum WisDescriptorHeapType {
    WisDescriptorHeapTypeDescriptor = 0, ///< Descriptor heap type. Used for all descriptor types, except for samplers.
    WisDescriptorHeapTypeSampler    = 1, ///< Sampler heap type. Used for sampler descriptors.
} WisDescriptorHeapType;

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor memory type. Decides if descriptors are visible and can be bound to GPU.
 *
 * */
typedef enum WisDescriptorMemoryType {
    WisDescriptorMemoryTypeCpuOnly       = 0, ///< Descriptors are only visible to CPU. May be used for copying descriptors to the GPU visible pool.
    WisDescriptorMemoryTypeShaderVisible = 1, ///< Descriptors are visible to GPU. Descriptors can be bound to the GPU pipeline directly, but can't be copied from.
} WisDescriptorMemoryType;

/**
 * @brief Provided by Wisdom 0.7.0. Component swizzle for texture sampling.
 *
 * */
typedef enum WisComponentSwizzle {
    WisComponentSwizzleIdentity = 0, ///< Use the component as is for sampling.
    WisComponentSwizzleRed      = 1, ///< Use the red component for sampling.
    WisComponentSwizzleGreen    = 2, ///< Use the green component for sampling.
    WisComponentSwizzleBlue     = 3, ///< Use the blue component for sampling.
    WisComponentSwizzleAlpha    = 4, ///< Use the alpha component for sampling.
    WisComponentSwizzleZero     = 5, ///< Use zero for sampling.
    WisComponentSwizzleOne      = 6, ///< Use one for sampling.
} WisComponentSwizzle;

/**
 * @brief Provided by Wisdom 0.7.0. Determines the behavior when wait for multiple fences is issued.
 *
 * */
typedef enum WisMutiWaitType {
    WisMutiWaitTypeAll = 0, ///< All the fences in the batch are triggered.
    WisMutiWaitTypeAny = 1, ///< At least one of the fences from the batch is triggered.
} WisMutiWaitType;

/**
 * @brief Provided by Wisdom 0.7.0. Input classification for vertex buffer data.
 *
 * */
typedef enum WisInputClass {
    WisInputClassPerVertex   = 0, ///< Vertex buffer data is vertex data.
    WisInputClassPerInstance = 1, ///< Vertex buffer data is per instance data.
} WisInputClass;

/**
 * @brief Provided by Wisdom 0.7.0. Query type for GPU queries.
 *
 * */
typedef enum WisQueryPropertyType {
    WisQueryPropertyTypeDeviceCommandQueueProperties   = 0, ///< Properties of the device command queues. Expects a WisDeviceCommandQueueProperties struct.
    WisQueryPropertyTypeDeviceDescriptorHeapProperties = 1, ///< Properties of the device descriptor heap. Expects a WisDeviceDescriptorHeapProperties struct.
    WisQueryPropertyTypeDeviceMemoryProperties         = 2, ///< Properties of the device descriptor heap. Expects a WisDeviceMemoryProperties struct.
    WisQueryPropertyTypeDeviceBindingProperties        = 3, ///< Properties of the device resource binding. Expects a WisDeviceBindingProperties struct.
} WisQueryPropertyType;

/**
 * @brief Provided by Wisdom 0.7.0. Memory type for resource allocation.
 *
 * */
typedef enum WisMemoryType {
    WisMemoryTypeDefault = 0, ///< Default memory type. Alias for `WisMemoryTypeDeviceLocal`
    /**
     * @brief
     * Default memory type.
     * Local device memory, most efficient for rendering.
     * */
    WisMemoryTypeDeviceLocal = 0,
    /**
     * @brief
     * Upload memory type.
     * Used for data that is uploaded to the GPU Local memory using copy operations.
     * */
    WisMemoryTypeUpload = 1,
    /**
     * @brief
     * Readback memory type.
     * Used for data that is read back from the GPU Local memory using copy operations.
     * */
    WisMemoryTypeReadback = 2,
    /**
     * @brief
     * GPU upload memory type.
     * Used for data that is directly uploaded to the GPU Local memory using copy operations.
     * Support of this memory @wis_must be queried.
     * */
    WisMemoryTypeGPUUpload = 3,
} WisMemoryType;

/**
 * @brief Provided by Wisdom 0.7.0. Texture state for resource transitions.
 *
 * */
typedef enum WisTextureState {
    WisTextureStateUndefined         = -1, ///< Undefined state.
    WisTextureStateCommon            = 0, ///< Common state.
    WisTextureStateRead              = 1, ///< General Read state.
    WisTextureStateRenderTarget      = 2, ///< Render Target state.
    WisTextureStateUnorderedAccess   = 3, ///< Unordered Access state.
    WisTextureStateDepthStencilWrite = 4, ///< Depth Stencil Write state.
    WisTextureStateDepthStencilRead  = 5, ///< Depth Stencil Read state.
    WisTextureStateShaderResource    = 6, ///< Shader Resource state.
    WisTextureStateCopySrc           = 7, ///< Copy Source state.
    WisTextureStateCopyDst           = 8, ///< Copy Destination state.
    WisTextureStatePresent           = 9, ///< Present swapchain state.
    WisTextureStateShadingRate       = 10, ///< Shading Rate state. Used for Variable Shading Rate.
    WisTextureStateVideoDecodeRead   = 11, ///< Video Decode Read state.
    WisTextureStateVideoDecodeWrite  = 12, ///< Video Decode Write state.
} WisTextureState;

/**
 * @brief Provided by Wisdom 0.7.0. Intermediate shader representation.
 *
 * */
typedef enum WisShaderIntermediate {
    WisShaderIntermediateDXIL  = 0, ///< DirectX Intermediate Language.
    WisShaderIntermediateSPIRV = 1, ///< Standard Portable Intermediate Representation for Vulkan.
} WisShaderIntermediate;

/**
 * @brief Provided by Wisdom 0.7.0. Stencil operation for depth and stencil operations.
 *
 * */
typedef enum WisStencilOp {
    WisStencilOpKeep     = 1, ///< Keep the current value.
    WisStencilOpZero     = 2, ///< Set the value to zero.
    WisStencilOpReplace  = 3, ///< Replace the value with the reference value.
    WisStencilOpIncClamp = 4, ///< Increment the value and clamp to the maximum value.
    WisStencilOpDecClamp = 5, ///< Decrement the value and clamp to the minimum value.
    WisStencilOpInvert   = 6, ///< Invert the value.
    WisStencilOpIncWrap  = 7, ///< Increment the value and wrap to zero when the maximum value is exceeded.
    WisStencilOpDecWrap  = 8, ///< Decrement the value and wrap to the maximum value when the minimum value is exceeded.
} WisStencilOp;

/**
 * @brief Provided by Wisdom 0.7.0. Primitive topology type for WisGraphicsPipelineDesc.
 *
 * */
typedef enum WisTopologyType {
    WisTopologyTypePoint    = 1, ///< Render points for each vertex.
    WisTopologyTypeLine     = 2, ///< Render lines between vertices.
    WisTopologyTypeTriangle = 3, ///< Render triangles between vertices.
    WisTopologyTypePatch    = 4, ///< Vertices are interpret as patch list. Used in tesselation process.
} WisTopologyType;

/**
 * @brief Provided by Wisdom 0.7.0. Fill mode for rasterizer.
 *
 * */
typedef enum WisFillMode {
    WisFillModeLines = 2, ///< Draw lines between vertices. Wireframe rendering.
    WisFillModeSolid = 3, ///< Fill the area between vertices forming polygons.
} WisFillMode;

/**
 * @brief Provided by Wisdom 0.7.0. Cull mode for rasterizer.
 * Triangle culling depends on WisWindingOrder option.
 *
 * */
typedef enum WisCullMode {
    WisCullModeNone  = 1, ///< No culling.
    WisCullModeFront = 2, ///< Cull front-facing triangles.
    WisCullModeBack  = 3, ///< Cull back-facing triangles.
} WisCullMode;

/**
 * @brief Provided by Wisdom 0.7.0. Winding order for front-facing triangles.
 *
 * */
typedef enum WisWindingOrder {
    WisWindingOrderClockwise        = 0, ///< Front-facing triangles have clockwise winding order.
    WisWindingOrderCounterClockwise = 1, ///< Front-facing triangles have counter-clockwise winding order.
} WisWindingOrder;

/**
 * @brief Provided by Wisdom 0.7.0. Conservative rasterization mode.
 *
 * */
typedef enum WisConservativeRasterization {
    WisConservativeRasterizationOff          = 0, ///< Conservative rasterization is disabled.
    WisConservativeRasterizationOverestimate = 1, ///< Conservative rasterization is enabled.
} WisConservativeRasterization;

/**
 * @brief Provided by Wisdom 0.7.0. Line rasterization mode.
 *
 * */
typedef enum WisLineRasterization {
    WisLineRasterizationDefault          = 0, ///< Line rasterization is disabled. Lines are not rendered.
    WisLineRasterizationRectangular      = 1, ///< Lines are rasterized as rectangles.
    WisLineRasterizationAlphaAntialiased = 2, ///< Lines are rasterized as rectangles with anti-aliasing.
} WisLineRasterization;

/**
 * @brief Provided by Wisdom 0.7.0. Blend factor for color blending operations.
 *
 * */
typedef enum WisBlendFactor {
    WisBlendFactorZero           = 1, ///< Use zero for blending.
    WisBlendFactorOne            = 2, ///< Use one for blending.
    WisBlendFactorSrcColor       = 3, ///< Use the source color for blending.
    WisBlendFactorInvSrcColor    = 4, ///< Use the inverse source color for blending.
    WisBlendFactorSrcAlpha       = 5, ///< Use the source alpha for blending.
    WisBlendFactorInvSrcAlpha    = 6, ///< Use the inverse source alpha for blending.
    WisBlendFactorDestAlpha      = 7, ///< Use the destination alpha for blending.
    WisBlendFactorInvDestAlpha   = 8, ///< Use the inverse destination alpha for blending.
    WisBlendFactorDestColor      = 9, ///< Use the destination color for blending.
    WisBlendFactorInvDestColor   = 10, ///< Use the inverse destination color for blending.
    WisBlendFactorSrcAlphaSat    = 11, ///< Use the source alpha saturated for blending.
    WisBlendFactorConstantColor  = 14, ///< Use a constant blend factor for blending.
    WisBlendFactorInvBlendFactor = 15, ///< Use the inverse constant blend factor for blending.
    WisBlendFactorSrc1Color      = 16, ///< Use the source color for blending. Dual source blending mode.
    WisBlendFactorInvSrc1Color   = 17, ///< Use the inverse source color for blending. Dual source blending mode.
    WisBlendFactorSrc1Alpha      = 18, ///< Use the source alpha for blending. Dual source blending mode.
    WisBlendFactorInvSrc1Alpha   = 19, ///< Use the inverse source alpha for blending. Dual source blending mode.
} WisBlendFactor;

/**
 * @brief Provided by Wisdom 0.7.0. Blend operation for color blending operations.
 *
 * */
typedef enum WisBlendOp {
    WisBlendOpAdd         = 1, ///< Add the source and destination colors.
    WisBlendOpSubtract    = 2, ///< Subtract the source color from the destination color.
    WisBlendOpRevSubtract = 3, ///< Subtract the destination color from the source color.
    WisBlendOpMin         = 4, ///< Use the minimum of the source and destination colors.
    WisBlendOpMax         = 5, ///< Use the maximum of the source and destination colors.
} WisBlendOp;

/**
 * @brief Provided by Wisdom 0.7.0. Logic operation for color blending operations.
 *
 * */
typedef enum WisLogicOp {
    WisLogicOpClear        = 0, ///< Clear the destination value.
    WisLogicOpSet          = 1, ///< Set the destination value.
    WisLogicOpCopy         = 2, ///< Copy the source value to the destination.
    WisLogicOpCopyInverted = 3, ///< Copy the inverted source value to the destination.
    WisLogicOpNoop         = 4, ///< Do not modify the destination value.
    WisLogicOpInvert       = 5, ///< Invert the destination value.
    WisLogicOpAnd          = 6, ///< Perform a bitwise AND operation on the source and destination values.
    WisLogicOpNand         = 7, ///< Perform a bitwise NAND operation on the source and destination values.
    WisLogicOpOr           = 8, ///< Perform a bitwise OR operation on the source and destination values.
    WisLogicOpNor          = 9, ///< Perform a bitwise NOR operation on the source and destination values.
    WisLogicOpXor          = 10, ///< Perform a bitwise XOR operation on the source and destination values.
    WisLogicOpEquiv        = 11, ///< Perform a bitwise equivalent operation on the source and destination values.
    WisLogicOpAndReverse   = 12, ///< Perform a bitwise AND operation on the source and inverted destination values.
    WisLogicOpAndInverted  = 13, ///< Perform a bitwise AND operation on the inverted source and destination values.
    WisLogicOpOrReverse    = 14, ///< Perform a bitwise OR operation on the source and inverted destination values.
    WisLogicOpOrInverted   = 15, ///< Perform a bitwise OR operation on the inverted source and destination values.
} WisLogicOp;

/**
 * @brief Provided by Wisdom 0.7.0. Primitive topology for rendering.
 * More info could be found [here](https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies).
 *
 * */
typedef enum WisPrimitiveTopology {
    WisPrimitiveTopologyPointList        = 1, ///< Render points for each vertex.
    WisPrimitiveTopologyLineList         = 2, ///< Render lines between vertices.
    WisPrimitiveTopologyLineStrip        = 3, ///< Render lines between vertices in a strip.
    WisPrimitiveTopologyTriangleList     = 4, ///< Render triangles between vertices.
    WisPrimitiveTopologyTriangleStrip    = 5, ///< Render triangles between vertices in a strip.
    WisPrimitiveTopologyTriangleFan      = 6, ///< Interpret vertex data to form a fan of triangles.
    WisPrimitiveTopologyLineListAdj      = 10, ///< Render lines between vertices with adjacency.
    WisPrimitiveTopologyLineStripAdj     = 11, ///< Render lines between vertices in a strip with adjacency.
    WisPrimitiveTopologyTriangleListAdj  = 12, ///< Render triangles between vertices with adjacency.
    WisPrimitiveTopologyTriangleStripAdj = 13, ///< Render triangles between vertices in a strip with adjacency.
} WisPrimitiveTopology;

/**
 * @brief Provided by Wisdom 0.7.0. Primitive restart value for indexed drawing with primitive restart enabled.
 *
 * */
typedef enum WisPrimitiveRestartValue {
    WisPrimitiveRestartValueNone      = 0, ///< Primitive restart is disabled. No primitive restart value is used.
    WisPrimitiveRestartValueUInt16Max = 65535, ///< Use the maximum value of uint16_t as the primitive restart value.
    WisPrimitiveRestartValueUInt32Max = -1, ///< Use the maximum value of uint32_t as the primitive restart value.
} WisPrimitiveRestartValue;

/**
 * @brief Provided by Wisdom 0.7.0. Type of the view descriptor heap.
 *
 * */
typedef enum WisViewHeapType {
    WisViewHeapTypeRenderTarget = 0, ///< Descriptor heap for render target views.
    WisViewHeapTypeDepthStencil = 1, ///< Descriptor heap for depth stencil views.
} WisViewHeapType;

/**
 * @brief Provided by Wisdom 0.7.0. Swapchain scaling mode.
 *
 * */
typedef enum WisSwapchainScaling {
    WisSwapchainScalingNone    = 0, ///< No scaling. The swapchain size is equal to the window size.
    WisSwapchainScalingStretch = 1, ///< Stretch scaling. The swapchain size is stretched to the window size.
    WisSwapchainScalingAspect  = 2, ///< Aspect scaling. The swapchain size is scaled to the window size with aspect ratio preserved.
} WisSwapchainScaling;

/**
 * @brief Provided by Wisdom 0.7.0. Composite alpha flags for swapchain creation.
 *
 * */
typedef enum WisCompositeAlpha {
    WisCompositeAlphaOpaque         = 0, ///< The alpha channel, if it exists, is ignored. The image is treated as opaque.
    WisCompositeAlphaPreMultiplied  = 1, ///< The alpha channel, if it exists, is respected and used in compositing. The premultiplied alpha format is expected.
    WisCompositeAlphaPostMultiplied = 2, ///< The alpha channel, if it exists, is respected and used in compositing. The postmultiplied alpha format is expected.
    WisCompositeAlphaInherit        = 3, ///< The alpha channel, if it exists, is respected and used in compositing based on the platform's default behavior.
} WisCompositeAlpha;

/**
 * @brief Provided by Wisdom 0.7.0. Flags that describe adapter.
 *
 * */
typedef enum WisAdapterFlags {
    WisAdapterFlagsNone     = 0, ///< No flags set. Adapter @wis_may be discrete or embedded.
    WisAdapterFlagsRemote   = (1u << 0), ///< Adapter is remote. Used for remote rendering.
    WisAdapterFlagsSoftware = (1u << 1), ///< Adapter is software. Uses CPU for software rendering.
} WisAdapterFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Flags for descriptor heap creation.
 *
 * */
typedef enum WisDescriptorHeapFlags {
    WisDescriptorHeapFlagsNone                     = 0, ///< No flags set.
    WisDescriptorHeapFlagsDisallowEmbeddedSamplers = (1u << 1), ///< Heap is used in full for dynamic samplers. There @wis_must_not be any shader that use embedded samplers that uses that heap. User @wis_may allocate more samplers in the heap than it would normally be.
} WisDescriptorHeapFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Flags for sampler creation.
 *
 * */
typedef enum WisSamplerFlags {
    WisSamplerFlagsNone                     = 0, ///< No flags set.
    WisSamplerFlagsNonNormalizedCoordinates = (1u << 0), ///< Use non-normalized texture coordinates.
} WisSamplerFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Buffer usage flags.
 * Determine how the buffer can be used throughout its lifetime.
 *
 * */
typedef enum WisBufferUsageFlags {
    WisBufferUsageFlagsNone                        = 0, ///< No flags set. Buffer is not used.
    WisBufferUsageFlagsCopySrc                     = (1u << 0), ///< Buffer is used as a source for copy operations.
    WisBufferUsageFlagsCopyDst                     = (1u << 1), ///< Buffer is used as a destination for copy operations.
    WisBufferUsageFlagsConstantBuffer              = (1u << 2), ///< Buffer is used as a constant buffer.
    WisBufferUsageFlagsIndexBuffer                 = (1u << 3), ///< Buffer is used as an index buffer.
    WisBufferUsageFlagsVertexBuffer                = (1u << 4), ///< Buffer is used as a vertex buffer or an instance buffer.
    WisBufferUsageFlagsIndirectBuffer              = (1u << 5), ///< Buffer is used as an indirect buffer.
    WisBufferUsageFlagsStorageBuffer               = (1u << 6), ///< Buffer is used as a storage unordered access buffer.
    WisBufferUsageFlagsAccelerationStructureBuffer = (1u << 7), ///< Buffer is used as an acceleration structure buffer.
    WisBufferUsageFlagsAccelerationStructureInput  = (1u << 8), ///< Buffer is used as a read only acceleration instance input buffer.
    WisBufferUsageFlagsShaderBindingTable          = (1u << 9), ///< Buffer is used as a shader binding table buffer.
} WisBufferUsageFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Texture usage flags.
 * Determine how the texture can be used throughout its lifetime.
 *
 * */
typedef enum WisTextureUsageFlags {
    WisTextureUsageFlagsNone            = 0, ///< No flags set. Texture is not used.
    WisTextureUsageFlagsRenderTarget    = (1u << 0), ///< Texture is used as a render target.
    WisTextureUsageFlagsDepthStencil    = (1u << 1), ///< Texture is used as a depth stencil buffer.
    WisTextureUsageFlagsCopySrc         = (1u << 2), ///< Texture is used as a source for copy operations.
    WisTextureUsageFlagsCopyDst         = (1u << 3), ///< Texture is used as a destination for copy operations.
    WisTextureUsageFlagsShaderResource  = (1u << 4), ///< Texture is used as a shader resource.
    WisTextureUsageFlagsUnorderedAccess = (1u << 5), ///< Texture is used as an unordered access resource.
    WisTextureUsageFlagsHostCopy        = (1u << 7), ///< Texture is used for host copy operations. Works with GPUUpload heap.
} WisTextureUsageFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Memory flags.
 * Determine optional properties of the memory allocation.
 *
 * */
typedef enum WisMemoryFlags {
    WisMemoryFlagsNone = 0, ///< No flags set. Memory is regular.
    /**
     * @brief
     * Memory is dedicated.
     * Used for resources that require dedicated memory.
     * Useful for big resources that are not shared with other resources.
     * E.g. fullscreen textures, big buffers, etc.
     * */
    WisMemoryFlagsDedicatedAllocation = (1u << 0),
    /**
     * @brief
     * Memory is mapped.
     * Used in combination with `WisMemoryTypeUpload` or `WisMemoryTypeReadback` to map memory for CPU access.
     * */
    WisMemoryFlagsMapped = (1u << 1),
    /**
     * @brief
     * Memory is exportable.
     * If set, memory can be exported to other processes or APIs.
     * Works only with Device Local memory (`WisMemoryTypeDefault`) and only on AllocateXMemory calls.
     * Outside of AllocateXMemory the flag is ignored.
     * */
    WisMemoryFlagsExportable = (1u << 2),
} WisMemoryFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Texture creation flags. Reserved for future features.
 *
 * */
typedef enum WisTextureFlags {
    WisTextureFlagsNone = 0, ///< No flags set. Texture is regular.
} WisTextureFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Texture binding flags, used for extra options.
 *
 * */
typedef enum WisTextureBindingFlags {
    WisTextureBindingFlagsNone        = 0, ///< No flags set. Texture view is regular. Implies color read.
    WisTextureBindingFlagsDepthView   = (1u << 0), ///< Texture view is used to read depth. Used for special formats that feature depth and stencil. The bound texture @wis_must be in TODO: specific layout before being used by shader.
    WisTextureBindingFlagsStencilView = (1u << 1), ///< Texture view is used to read stencil. Used for special formats that feature depth and stencil. The bound texture @wis_must be in TODO: specific layout before being used by shader. Cannot be combined with `WisTextureBindingFlagsDepthView`.
} WisTextureBindingFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Barrier synchronization flags for resource barriers.
 *
 * */
typedef enum WisBarrierSync {
    WisBarrierSyncNone            = 0, ///< No synchronization is performed.
    WisBarrierSyncAll             = (1u << 0), ///< Synchronize all commands.
    WisBarrierSyncDraw            = (1u << 1), ///< Synchronize draw commands.
    WisBarrierSyncIndexInput      = (1u << 2), ///< Synchronize index input commands.
    WisBarrierSyncVertexShading   = (1u << 3), ///< Synchronize vertex shading commands.
    WisBarrierSyncPixelShading    = (1u << 4), ///< Synchronize pixel shading commands.
    WisBarrierSyncDepthStencil    = (1u << 5), ///< Synchronize depth-stencil commands.
    WisBarrierSyncRenderTarget    = (1u << 6), ///< Synchronize render target commands.
    WisBarrierSyncCompute         = (1u << 7), ///< Synchronize compute commands.
    WisBarrierSyncRaytracing      = (1u << 8), ///< Synchronize ray tracing commands.
    WisBarrierSyncCopy            = (1u << 9), ///< Synchronize copy commands.
    WisBarrierSyncResolve         = (1u << 10), ///< Synchronize resolve commands.
    WisBarrierSyncExecuteIndirect = (1u << 11), ///< Synchronize execute indirect commands.
    WisBarrierSyncAllShading      = (1u << 12), ///< Synchronize all shading commands.
    WisBarrierSyncNonPixelShading = (1u << 13), ///< Synchronize non-pixel shading commands.
    WisBarrierSyncClearUAV        = (1u << 14), ///< Synchronize UAV clear commands.
    WisBarrierSyncVideoDecode     = (1u << 15), ///< Synchronize video decode commands.
    WisBarrierSyncVideoEncode     = (1u << 16), ///< Synchronize video encode commands.
    WisBarrierSyncBuildRTAS       = (1u << 17), ///< Synchronize ray tracing acceleration structure build commands.
    WisBarrierSyncCopyRTAS        = (1u << 18), ///< Synchronize ray tracing acceleration structure copy commands.
} WisBarrierSync;

/**
 * @brief Provided by Wisdom 0.7.0. Resource access flags for resource barriers.
 *
 * */
typedef enum WisResourceAccess {
    WisResourceAccessCommon                     = 0, ///< Common access.
    WisResourceAccessVertexBuffer               = (1u << 0), ///< Vertex buffer access. Applies only to buffers.
    WisResourceAccessConstantBuffer             = (1u << 1), ///< Constant buffer access. Applies only to buffers.
    WisResourceAccessIndexBuffer                = (1u << 2), ///< Index buffer access. Applies only to buffers.
    WisResourceAccessRenderTarget               = (1u << 3), ///< Render target access. Applies only to textures.
    WisResourceAccessUnorderedAccess            = (1u << 4), ///< Unordered access.
    WisResourceAccessDepthWrite                 = (1u << 5), ///< Depth write access. Applies only to depth-stencil textures.
    WisResourceAccessDepthRead                  = (1u << 6), ///< Depth read access. Applies only to depth-stencil textures.
    WisResourceAccessShaderResource             = (1u << 7), ///< Shader resource access.
    WisResourceAccessStreamOutput               = (1u << 8), ///< Stream output access. Applies only to buffers.
    WisResourceAccessIndirectArgument           = (1u << 9), ///< Indirect argument access.
    WisResourceAccessCopyDst                    = (1u << 10), ///< Copy destination access.
    WisResourceAccessCopySrc                    = (1u << 11), ///< Copy source access.
    WisResourceAccessConditionalRendering       = (1u << 12), ///< Conditional rendering access.
    WisResourceAccessAccelerationStructureRead  = (1u << 13), ///< Acceleration structure read access.
    WisResourceAccessAccelerationStructureWrite = (1u << 14), ///< Acceleration structure write access.
    WisResourceAccessShadingRate                = (1u << 15), ///< Shading rate access.
    WisResourceAccessVideoDecodeRead            = (1u << 16), ///< Video decode read access.
    WisResourceAccessVideoDecodeWrite           = (1u << 17), ///< Video decode write access.
    WisResourceAccessResolveDst                 = (1u << 18), ///< Resolve destination access.
    WisResourceAccessResolveSrc                 = (1u << 19), ///< Resolve source access.
    WisResourceAccessNone                       = (1u << 31), ///< No access. Used to indicate no access throughout the pipeline.
} WisResourceAccess;

/**
 * @brief Provided by Wisdom 0.7.0. Barrier flags for resource barriers.
 *
 * */
typedef enum WisBarrierFlags {
    WisBarrierFlagsNone            = 0, ///< No flags set. Barrier is regular.
    WisBarrierFlagsDiscardContent  = (1u << 0), ///< Discard resource content. The content of the resource before the barrier is treated as if resource was not initialized.
    WisBarrierFlagsDepthResource   = (1u << 1), ///< Resource is a depth resource. This flag @wis_must be set for all depth resources to make transitions on them.
    WisBarrierFlagsStencilResource = (1u << 2), ///< Resource is a stencil resource. This flag @wis_must be set for all stencil resources to make transitions on them. If resource has format `WisDataFormatD24UnormS8Uint` both `WisBarrierFlagsDepthResource` and `WisBarrierFlagsStencilResource` @wis_must be set.
    WisBarrierFlagsWholeRange      = (1u << 3), ///< Transition whole resource. If not set, the transition is applied only to the specified subresource range. If set, the subresource range is ignored and the transition is applied to all subresources of the resource.
    WisBarrierFlagsPlanarImage     = (1u << 4), ///< Resource is a planar image. If the flag is not set, plane slices in WisSubresourceRange are ignored.
} WisBarrierFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Pipeline creation flags.
 *
 * */
typedef enum WisPipelineFlags {
    WisPipelineFlagsNone                   = 0, ///< No flags set. Pipeline is regular.
    WisPipelineFlagsFailOnCacheMiss        = (1u << 0), ///< Fail pipeline creation if the pipeline cache is missing or incompatible. If not set, the implementation @wis_may choose to create the pipeline without using the cache, which @wis_may result in longer creation time.
    WisPipelineFlagsEnablePrimitiveRestart = (1u << 1), ///< Enable primitive restart for graphics pipelines. If not set, primitive restart is disabled and the implementation @wis_may choose to ignore restart indices in draw calls.
    WisPipelineFlagsDynamicDepthBias       = (1u << 2), ///< Enable dynamic depth bias for graphics pipelines. If not set, depth bias is static and @wis_must be specified at pipeline creation time.
} WisPipelineFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Color component flags.
 * Used for color blending operations.
 *
 * */
typedef enum WisColorComponents {
    WisColorComponentsNone = 0, ///< No flags set. Color blending is not used.
    WisColorComponentsR    = (1u << 0), ///< Use red component for blending.
    WisColorComponentsG    = (1u << 1), ///< Use green component for blending.
    WisColorComponentsB    = (1u << 2), ///< Use blue component for blending.
    WisColorComponentsA    = (1u << 3), ///< Use alpha component for blending.
    WisColorComponentsAll  = 15, ///< Use all color components for blending.
} WisColorComponents;

/**
 * @brief Provided by Wisdom 0.7.0. Swapchain creation flags.
 *
 * */
typedef enum WisSwapchainFlags {
    WisSwapchainFlagsNone         = 0, ///< No flags set. Swapchain is regular.
    WisSwapchainFlagsAllowTearing = (1u << 0), ///< Allow tearing.
    WisSwapchainFlagsVSync        = (1u << 1), ///< Present with vertical sync. If set, the swapchain is presented with vertical sync pulse.
    WisSwapchainFlagsStereo       = (1u << 2), ///< Stereo swapchain. If set, the swapchain is created for stereo rendering. If not set, the swapchain is created for mono rendering.
} WisSwapchainFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Swapchain creation flags.
 *
 * */
typedef enum WisPresentFlags {
    WisPresentFlagsNone           = 0, ///< No flags set. Swapchain is regular.
    WisPresentFlagsTimeoutOnBlock = (1u << 0), ///< Fail present if the presentation engine is busy. If not set, the implementation @wis_may choose to block until the presentation engine is available.
} WisPresentFlags;

//==============================================================
// Delegates
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Defines the debug callback function signature. Used for logging and debugging messages from the graphics API.
 * @param severity defines message severity level.
 * @param message contains the debug message string.
 * @param device handle to the device that generated the message. Can be `0` if message is not device specific.
 * @param user_data user defined data pointer passed during callback registration.
 *
 * */
typedef void (*WisDebugCallback)(WisSeverity severity, const char* message, uint64_t device, void* user_data);

//==============================================================
// Structs
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Main source of communication of operation success.
 *
 * */
typedef struct WIS_NODISCARD WisResult {
    WisStatus   status; ///< defines operation status. Compare with `WisStatusOk`.
    int32_t     platform_code; ///< defines platform code from underlying implementation. Is an `HRESULT` for DX12 and a `VkResult` for Vulkan.
    const char* error; ///< contains a human readable error message.
} WisResult;

/**
 * @brief Provided by Wisdom 0.7.0. Adapter description. Describes hardware driver identification as well as memory limits.
 *
 * */
typedef struct WisAdapterDesc {
    char            description[256]; ///< Adapter description. Contains name of the graphics adapter.
    uint32_t        vendor_id; ///< denotes Vendor ID. Can be used to find the correct adapter.
    uint32_t        device_id; ///< denotes Device ID. Together with `WisAdapterDesc::vendor_id` uniquely identifies the device.
    uint64_t        dedicated_video_memory; ///< measures dedicated video memory in bytes. Used for device local memory type.
    uint64_t        shared_system_memory; ///< measures memory that is shared with CPU in bytes. Used for upload and readback.
    uint64_t        adapter_id; ///< denotes adapter unique ID (LUID). Can be used to find the correct adapter.
    uint8_t         adapter_uuid[16]; ///< stores UUID of the adapter, used only with Vulkan API on systems with no LUID.
    WisAdapterFlags flags; ///< Adapter flags. Describe the adapter kind.
} WisAdapterDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Debugging and logging description. Used to configure debug callback behavior.
 *
 * */
typedef struct WisDebugDesc {
    bool             enable_debug_layer; ///< enables or disables debug layer on both DX12 and VK backends.
    WisDebugCallback callback; ///< defines the debug callback function.
    void*            user_data; ///< user defined data pointer passed to the callback.
} WisDebugDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Command queue description for WisCommandQueue creation.
 *
 * */
typedef struct WisCommandQueueDesc {
    WisCommandQueueType     type; ///< defines the type of the command queue.
    WisCommandQueuePriority priority; ///< defines command queue priority. Used to set priority of the command queues of the type.
} WisCommandQueueDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Sampler description for  creation.
 *
 * */
typedef struct WisSamplerDesc {
    WisFilter        min_filter; ///< Minification filter.
    WisFilter        mag_filter; ///< Magnification filter.
    WisFilter        mip_filter; ///< Mip level filter.
    WisReductionMode reduction_mode; ///< Reduction mode for min/mag/mip filters.
    bool             is_anisotropic; ///< Anisotropic filtering enable.
    uint32_t         max_anisotropy; ///< Max anisotropy level. Min is 1, Max is 16.
    WisAddressMode   address_u; ///< Address mode for U coordinate.
    WisAddressMode   address_v; ///< Address mode for V coordinate.
    WisAddressMode   address_w; ///< Address mode for W coordinate.
    float            min_lod; ///< Min LOD value.
    float            max_lod; ///< Max LOD value.
    float            mip_lod_bias; ///< Mip LOD bias value.
    WisCompareOp     comparison_op; ///< Comparison operation for comparison samplers.
    WisStaticBorder  static_border_color; ///< Static border color. Used if any address mode is set to WisAddressMode.
    WisSamplerFlags  flags; ///< Sampler flags. Used to set additional sampler options.
} WisSamplerDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Static sampler description for  creation.
 *
 * */
typedef struct WisStaticSamplerDesc {
    WisSamplerDesc      sampler; ///< Sampler description.
    WisShaderVisibility visibility; ///< Shader stage visibility. Defines the stage where the sampler is used.
    uint32_t            bind_register; ///< Bind register number in HLSL.
} WisStaticSamplerDesc;

/**
 * @brief Provided by Wisdom 0.7.0. A set of constants that get read directly from root buffer.
 *
 * */
typedef struct WisPushConstant {
    WisShaderVisibility visibility; ///< Shader stage visibility for the push constant.
    uint32_t            size_bytes; ///< Size of the constant in bytes. Must be divisible by 4.
    uint32_t            bind_register; ///< Bind register number in HLSL.
    uint32_t            bind_space; ///< Bind space number in HLSL. `register(regN, spaceN)`
} WisPushConstant;

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor. Used to push data directly to pipeline.
 *
 * */
typedef struct WisPushDescriptor {
    WisShaderVisibility visibility; ///< Shader stage visibility for the push descriptor.
    WisDescriptorType   type; ///< Descriptor type. Works only with buffer bindings.
    uint32_t            bind_register; ///< Bind register number in HLSL.
    uint32_t            bind_space; ///< Bind space number in HLSL. `register(regN, spaceN)`
} WisPushDescriptor;

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor table entry for WisDescriptorTable.
 *
 * */
typedef struct WisDescriptorTableEntry {
    WisDescriptorType type; ///< Descriptor type.
    uint32_t          bind_register; ///< Bind register number in HLSL.
    uint32_t          bind_space; ///< Bind space number in HLSL. `register(regN, spaceN)`
    uint32_t          count; ///< Descriptor count for Array descriptors. UINT32_MAX means unbounded array. 0 means single register, same as 1.
    uint32_t          descriptor_offset; ///< Offset in descriptors from the heap start. Used for calculating descriptor indices when binding descriptor tables.
} WisDescriptorTableEntry;

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor table for  creation.
 *
 * */
typedef struct WisDescriptorTable {
    WisShaderVisibility            visibility; ///< Shader stage. Defines the stage where the table is used.
    const WisDescriptorTableEntry* entries; ///< Descriptor table entries array.
    size_t                         entry_count; ///< Descriptor table entries count.
} WisDescriptorTable;

/**
 * @brief Provided by Wisdom 0.7.0. Pipeline layout description. Defines resource bindings for shaders.
 *
 * */
typedef struct WisRootSignatureDesc {
    const WisPushConstant*    push_constants; ///< describes the global shader push data.
    size_t                    push_constant_count; ///< counts the number of push constants in the `WisRootSignatureDesc::push_constants` array.
    const WisPushDescriptor*  push_descriptors; ///< points to an array of WisPushDescriptor.
    size_t                    push_descriptor_count; ///< counts the number of push descriptors in the `WisRootSignatureDesc::push_descriptors` array.
    const WisDescriptorTable* descriptor_tables; ///< points to an array of WisDescriptorTable.
    size_t                    descriptor_table_count; ///< counts the number of descriptor tables in the `WisRootSignatureDesc::descriptor_tables` array.
} WisRootSignatureDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor heap description for WisDescriptorHeap creation.
 *
 * */
typedef struct WisDescriptorHeapDesc {
    WisDescriptorHeapType   type; ///< indicates the type of descriptor heap to create (sampler or descriptor).
    WisDescriptorMemoryType memory_type; ///< indicates where the descriptor heap will be allocated.
    uint32_t                descriptor_count; ///< indicates the amount of descriptors, present in the heap.
    WisDescriptorHeapFlags  flags; ///< describe additional options for the descriptor heap.
} WisDescriptorHeapDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Component mapping for .
 *
 * */
typedef struct WisComponentMapping {
    WisComponentSwizzle r; ///< Component mapping for Red channel. Default is `WisComponentSwizzleRed`.
    WisComponentSwizzle g; ///< Component mapping for Green channel. Default is `WisComponentSwizzleGreen`.
    WisComponentSwizzle b; ///< Component mapping for Blue channel. Default is `WisComponentSwizzleBlue`.
    WisComponentSwizzle a; ///< Component mapping for Alpha channel. Default is `WisComponentSwizzleAlpha`.
} WisComponentMapping;

/**
 * @brief Provided by Wisdom 0.7.0. Subresource description for texture data updates and copies.
 *
 * */
typedef struct WisSubresourceRange {
    uint16_t base_mip_level; ///< Mipmap level of the subresource.
    uint16_t mip_level_count; ///< Number of mip levels in the subresource.
    uint16_t base_array_layer; ///< Array layer of the subresource. For 3D textures, this defines the depth slice.
    uint16_t array_layer_count; ///< Number of array layers in the subresource. For 3D textures, this defines the number of depth slices.
    uint16_t plane_slice; ///< Base depth slice of the subresource. Used only for 2D textures (YUV).
    uint16_t plane_slice_count; ///< Number of depth slices in the subresource. Used only for 2D textures (YUV). Max value is 3.
} WisSubresourceRange;

/**
 * @brief Provided by Wisdom 0.7.0. Buffer description for WisBuffer creation.
 *
 * */
typedef struct WisBufferDesc {
    uint64_t            size_bytes; ///< Size of the buffer in bytes.
    WisBufferUsageFlags usage_flags; ///< Buffer usage flags. Describe how the buffer will be used.
    WisMemoryType       memory_type; ///< indicates where the buffer will be allocated.
    WisMemoryFlags      memory_flags; ///< The flags of the memory to allocate for the buffer.
} WisBufferDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Texture description for WisTexture creation.
 *
 * */
typedef struct WisTextureDesc {
    uint32_t             width; ///< Texture width in pixels.
    uint32_t             height; ///< Texture height in pixels.
    uint16_t             depth_or_array_size; ///< Texture depth in pixels. Used only for 3D textures.
    uint16_t             mip_levels; ///< Number of mip levels in the texture.
    WisDataFormat        format; ///< Texture format.
    WisSampleCount       sample_count; ///< Number of samples per pixel. Used only for multisampled textures.
    WisTextureLayout     layout; ///< Texture layout. Default is `WisTextureLayoutTexture2D`.
    WisTextureUsageFlags usage_flags; ///< Texture usage flags. Describe how the texture will be used.
    WisTextureFlags      flags; ///< Texture flags. Describe additional options for the texture.
    WisMemoryType        memory_type; ///< indicates where the texture will be allocated.
    WisMemoryFlags       memory_flags; ///< The flags of the memory to allocate for the texture.
} WisTextureDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Push constant set description for wisCommandListSetPushConstants.
 *
 * */
typedef struct WisPushConstantDataDesc {
    WisPipelineType pipeline; ///< defines the pipeline type to set the push constants for.
    uint32_t        root_index; ///< defines the root index in the root signature to set the push constants for.
    const void*     data; ///< points to the data to set as push constants.
    uint32_t        data_size; ///< defines the size of the data in bytes. It @wis_must be less than or equal to the maximum push constant size defined by the device and 4-byte aligned.
    uint32_t        push_offset; ///< defines the offset in bytes from the start of the push constant root parameter to set the data to. It @wis_must be less than the maximum push constant size defined by the device and 4-byte aligned.
} WisPushConstantDataDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor set description for wisCommandListSetPushDescriptor.
 *
 * */
typedef struct WisPushDescriptorDataDesc {
    WisPipelineType   pipeline; ///< defines the pipeline type to set the push descriptors for.
    uint32_t          root_index; ///< defines the root index in the root signature to set the push descriptors for.
    WisDescriptorType descriptor_type; ///< defines the type of the descriptors to push.
    uint64_t          buffer_address; ///< defines buffer device address to push.
} WisPushDescriptorDataDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor set description for wisCommandListSetDescriptorTable.
 *
 * */
typedef struct WisDescriptorTableDataDesc {
    WisPipelineType       pipeline; ///< defines the pipeline type to set the push descriptors for.
    uint32_t              root_index; ///< defines the root index in the root signature to set the push descriptors for.
    WisDescriptorHeapType heap_type; ///< defines the type of the descriptor heap to bind.
    uint32_t              heap_offset; ///< defines the offset in descriptors from the start of the heap to set the descriptor table to. Used for calculating descriptor indices when binding descriptor tables.
} WisDescriptorTableDataDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Constant buffer binding description for wisDescriptorHeapWriteConstantBuffer and .
 *
 * */
typedef struct WisConstantBufferBinding {
    uint64_t buffer_address; ///< defines buffer device address to bind.
    uint32_t size_bytes; ///< defines the size of the buffer region to bind in bytes.
} WisConstantBufferBinding;

/**
 * @brief Provided by Wisdom 0.7.0. Buffer binding description for wisDescriptorHeapWriteStructuredBuffer.
 *
 * */
typedef struct WisBufferBinding {
    uint64_t array_offset; ///< defines offset in buffer in structures.
    uint32_t stride_bytes; ///< defines the size of the single structure in buffer.
    uint32_t structure_count; ///< defines the number of structures in the buffer region to bind.
} WisBufferBinding;

/**
 * @brief Provided by Wisdom 0.7.0. Texture binding description for wisDescriptorHeapWriteTexture and wisDescriptorHeapWriteRWTexture.
 *
 * */
typedef struct WisTextureBinding {
    WisDataFormat          format; ///< defines the format of the view.
    WisTextureLayout       layout; ///< defines the layout of the texture. Default is `WisTextureLayoutTexture2D`.
    WisTextureBindingFlags flags; ///< Texture binding flags. Describe additional options for the texture binding.
    WisComponentMapping    component_mapping; ///< Component mapping for the texture view.
    WisSubresourceRange    range; ///< Subresource description for the texture view.
} WisTextureBinding;

/**
 * @brief Provided by Wisdom 0.7.0. Stencil description for WisDepthStencilDesc.
 *
 * */
typedef struct WisStencilDesc {
    WisStencilOp fail_op; ///< Stencil operation if the stencil test fails. Default is `WisStencilOpKeep`.
    WisStencilOp depth_fail_op; ///< Stencil operation if the stencil test passes and the depth test fails. Default is `WisStencilOpKeep`.
    WisStencilOp pass_op; ///< Stencil operation if the stencil test passes. Default is `WisStencilOpKeep`.
    WisCompareOp stencil_comp; ///< Stencil comparison function. Default is .
    uint8_t      read_mask; ///< Stencil read mask. Default is 0xff.
    uint8_t      write_mask; ///< Stencil write mask. Default is 0xff.
} WisStencilDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Depth stencil description for WisGraphicsPipelineDesc.
 *
 * */
typedef struct WisDepthStencilDesc {
    bool           depth_enable; ///< Depth test enable. Default is false.
    bool           depth_write_enable; ///< Depth write enable. Default is false.
    WisCompareOp   depth_comp; ///< Depth comparison function. Default is .
    bool           stencil_enable; ///< Stencil test enable. Default is false.
    WisStencilDesc stencil_front; ///< Stencil description for front faces.
    WisStencilDesc stencil_back; ///< Stencil description for back faces.
    bool           depth_bound_test; ///< Depth bound test enable. Default is false.
} WisDepthStencilDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Input slot description for WisInputLayout.
 *
 * */
typedef struct WisInputBindingDesc {
    uint32_t      slot; ///< Input slot number. Must be unique.
    uint32_t      stride_bytes; ///< Stride in bytes. Size of one vertex in the slot.
    WisInputClass input_class; ///< Input class. Defines how the data is read (Per vertex or Per instance).
} WisInputBindingDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Input attribute description for WisInputLayout.
 *
 * */
typedef struct WisInputAttributeDesc {
    uint32_t      binding_index; ///< Index into an array of bindings that the attribute is tied to.
    const char*   semantic_name; ///< Semantic name of the attribute in HLSL. Must be unique and null terminated.
    uint32_t      semantic_index; ///< Semantic index of the attribute in HLSL. Must be unique.
    uint32_t      location; ///< Location of the attribute in HLSL. Must be unique.
    WisDataFormat format; ///< Data format of the attribute.
    uint32_t      offset_bytes; ///< Offset in bytes from the beginning of the vertex.
} WisInputAttributeDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Input layout description for WisGraphicsPipelineDesc.
 *
 * */
typedef struct WisInputLayout {
    const WisInputBindingDesc*   bindings; ///< Input slots array. Made to pick up data from several arrays of vertex data.
    size_t                       binding_count; ///< Input slots count. Max number is 16.
    const WisInputAttributeDesc* attributes; ///< Input attributes array. Describes how the vertex data is read by the HLSL shader.
    size_t                       attribute_count; ///< Input attributes count.
} WisInputLayout;

/**
 * @brief Provided by Wisdom 0.7.0. Rasterizer description for WisGraphicsPipelineDesc.
 *
 * */
typedef struct WisRasterizerDesc {
    WisFillMode                  fill_mode; ///< Fill mode. Solid or Wireframe. Default is `WisFillModeSolid`.
    WisCullMode                  cull_mode; ///< Cull mode. None, Front, Back. Default is `WisCullModeBack`.
    WisWindingOrder              front_face; ///< Front face winding order. Clockwise or CounterClockwise. Default is `WisWindingOrderClockwise`.
    bool                         depth_bias_enable; ///< Depth bias enable. Default is false.
    float                        depth_bias; ///< Depth bias. Default is 0.0f.
    float                        depth_bias_clamp; ///< Depth bias clamp. Default is 0.0f.
    float                        depth_bias_slope_factor; ///< Depth bias slope factor e.g. for shadows. Default is 0.0f.
    bool                         depth_clip_enable; ///< Depth clip enable. Default is true.
    WisLineRasterization         line_rasterization; ///< Line rasterization mode. Default is `WisLineRasterizationDefault`.
    WisConservativeRasterization conservative_rasterization; ///< Conservative rasterization mode. Default is `WisConservativeRasterizationOff`.
} WisRasterizerDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Sample description of Multisampling for WisGraphicsPipelineDesc.
 *
 * */
typedef struct WisSampleDesc {
    WisSampleCount rate; ///< Sample rate. Default is `WisSampleCountS1`.
    uint32_t       sample_mask; ///< Sample mask. Default is 0xffffffff.
    bool           alpha_to_coverage_enable; ///< Alpha to coverage enable. Default is false.
} WisSampleDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Render attachments description for WisGraphicsPipelineDesc.
 *
 * */
typedef struct WisRenderAttachmentsDesc {
    WisDataFormat attachment_formats[8]; ///< Attachment formats array. Describes the format of the render target.
    uint32_t      attachments_count; ///< Attachment formats count. Max is 8.
    WisDataFormat depth_attachment; ///< Depth attachment format. Describes the format of the depth buffer.
    uint32_t      view_mask; ///< View mask for multiview rendering. Each bit represents a view that can be rendered to with the pipeline. Default is 0, meaning no multiview support.
} WisRenderAttachmentsDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Blend attachment description for WisBlendStateDesc.
 *
 * */
typedef struct WisBlendAttachmentDesc {
    bool               blend_enable; ///< Blend enable. Default is false.
    WisBlendFactor     src_color_blend; ///< Source color blend factor. Default is `WisBlendFactorOne`.
    WisBlendFactor     dst_color_blend; ///< Destination color blend factor. Default is `WisBlendFactorZero`.
    WisBlendOp         color_blend_op; ///< Color blend operation. Default is `WisBlendOpAdd`.
    WisBlendFactor     src_alpha_blend; ///< Source alpha blend factor. Default is `WisBlendFactorOne`.
    WisBlendFactor     dst_alpha_blend; ///< Destination alpha blend factor. Default is `WisBlendFactorZero`.
    WisBlendOp         alpha_blend_op; ///< Alpha blend operation. Default is `WisBlendOpAdd`.
    WisColorComponents color_write_mask; ///< Color write mask. Default is `WisColorComponentsAll`.
} WisBlendAttachmentDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Viewport description for WisCommandList.
 * Viewport is considered from Top Left corner.
 *
 * */
typedef struct WisViewport {
    float x; ///< Top left corner x coordinate.
    float y; ///< Top left corner y coordinate.
    float width; ///< Viewport width.
    float height; ///< Viewport height.
    float min_depth; ///< Minimum depth of the viewport.
    float max_depth; ///< Maximum depth of the viewport.
} WisViewport;

/**
 * @brief Provided by Wisdom 0.7.0. Scissor rect for WisCommandList and present rect.
 *
 * */
typedef struct WisRect {
    int32_t  x; ///< Left corner x coordinate.
    int32_t  y; ///< Top corner y coordinate.
    uint32_t width; ///< Width of the rect.
    uint32_t height; ///< Height of the rect.
} WisRect;

/**
 * @brief Provided by Wisdom 0.7.0. Blend state description for WisGraphicsPipelineDesc.
 *
 * */
typedef struct WisBlendStateDesc {
    bool                   logic_op_enable; ///< Logic operation enable. Default is false.
    WisLogicOp             logic_op; ///< Logic operation. Default is `WisLogicOpNoop`.
    WisBlendAttachmentDesc attachments[8]; ///< Blend attachment descriptions. Max Array size is 8.
    uint32_t               attachment_count; ///< Blend attachment count. If set as 0 - broadcast to all attachments. Max is 8.
} WisBlendStateDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Box description for texture copy regions.
 *
 * */
typedef struct WisBox {
    uint32_t x; ///< Left corner x coordinate.
    uint32_t y; ///< Top corner y coordinate.
    uint32_t z; ///< Front corner z coordinate. Used only for 3D textures.
    uint32_t width; ///< Width of the box.
    uint32_t height; ///< Height of the box.
    uint32_t depth; ///< Depth of the box. Used only for 3D textures.
} WisBox;

/**
 * @brief Provided by Wisdom 0.7.0. Target subresource description for texture copy regions.
 *
 * */
typedef struct WisTargetSubresource {
    uint16_t mip_level; ///< Mipmap level of the target subresource.
    uint16_t array_layer; ///< Array layer of the target subresource. For 3D textures @wis_must be 0.
    uint16_t plane_slice; ///< Depth slice of the target subresource. Used only for 2D textures (YUV).
} WisTargetSubresource;

/**
 * @brief Provided by Wisdom 0.7.0. Texture copy region description for texture copy operations.
 *
 * */
typedef struct WisTextureRegion {
    WisBox               box; ///< Box defining the region to copy.
    WisTargetSubresource target_subresource; ///< Target subresource description for the region.
} WisTextureRegion;

/**
 * @brief Provided by Wisdom 0.7.0. Render target description for wisViewHeapWriteRenderTarget and wisViewHeapWriteDepthStencil.
 *
 * */
typedef struct WisRenderTargetDesc {
    WisDataFormat    format; ///< Render target format.
    WisTextureLayout layout; ///< Render target layout. Default is `WisTextureLayoutTexture2D`.
    uint16_t         mip_level; ///< Mipmap level of the target subresource.
    uint16_t         base_array_layer; ///< Array layer of the target subresource. For 3D textures, this defines the base depth slice.
    uint16_t         array_layer_count; ///< Number of array layers in the target subresource. For 3D textures, this defines the number of depth slices.
    uint16_t         plane_slice; ///< Depth slice of the target subresource. Used only for 2D textures (YUV).
} WisRenderTargetDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Surface parameters for WisSurface creation.
 *
 * */
typedef struct WisSurfaceParameters {
    uint32_t             min_swapchain_images; ///< Minimum number of images in the swapchain.
    uint32_t             max_swapchain_images; ///< Maximum number of images in the swapchain.
    uint32_t             alpha_modes_supported; ///< Bitmask of supported alpha modes for the swapchain. Each bit represents a different alpha mode. Used to determine the supported alpha modes for the swapchain.
    WisTextureUsageFlags texture_usage_flags_supported; ///< Bitmask of supported texture usage flags for the swapchain images.
    bool                 stereo_supported; ///< Indicates if stereo rendering is supported. If true, the surface can be used to create a swapchain with stereo support.
} WisSurfaceParameters;

/**
 * @brief Provided by Wisdom 0.7.0. Swapchain description for WisSwapchain creation.
 *
 * */
typedef struct WisSwapchainDesc {
    uint32_t             width; ///< Swapchain image width in pixels.
    uint32_t             height; ///< Swapchain image height in pixels.
    uint32_t             image_count; ///< Number of images in the swapchain.
    WisTextureUsageFlags texture_usage_flags; ///< Texture usage flags for the swapchain images. Describe how the swapchain images will be used.
    WisDataFormat        format; ///< Swapchain image format.
    WisSwapchainScaling  scaling; ///< Swapchain scaling mode.
    WisSwapchainFlags    flags; ///< Swapchain flags. Describe additional options for the swapchain.
    WisCompositeAlpha    composite_alpha; ///< Composite alpha mode. Describe how the alpha channel of the swapchain images is treated during compositing.
} WisSwapchainDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Query struct header. Used as a header for all query structs.
 *
 * */
typedef struct WisQueryStructHeader {
    WisQueryPropertyType property_type; ///< Defines the type of the queried property. Used to determine what struct is passed.
    void*                next_in_chain; ///< Pointer to the next queried data struct.
} WisQueryStructHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Device binding properties. Used to query resource binding support and limits.
 *
 * */
typedef struct WisDeviceBindingProperties {
    WisQueryPropertyType property_type; ///< Defines the type of the queried property. Used to determine what struct is passed. @wis_must be `WisQueryPropertyTypeDeviceBindingProperties`.
    void*                next_in_chain; ///< Pointer to the next queried data struct.
    uint32_t             max_vertex_input_attributes; ///< Maximum number of vertex input attributes supported by the device. Used for vertex buffer bindings.
    uint32_t             max_vertex_input_bindings; ///< Maximum number of vertex input bindings supported by the device. Used for vertex buffer bindings.
    bool                 multiple_viewports_supported; ///< Indicates if multiple viewports are supported. If true, the device supports up to 16 viewports and scissor rectangles. If false, only one viewport and scissor rectangle is supported.
} WisDeviceBindingProperties;

/**
 * @brief Provided by Wisdom 0.7.0. Device descriptor heap properties. Used to query descriptor heap support and limits.
 *
 * */
typedef struct WisDeviceDescriptorHeapProperties {
    WisQueryPropertyType property_type; ///< Defines the type of the queried property. Used to determine what struct is passed. @wis_must be `WisQueryPropertyTypeDeviceDescriptorHeapProperties`.
    void*                next_in_chain; ///< Pointer to the next queried data struct.
    uint32_t             max_descriptor_heap_size; ///< Maximum number of descriptors in a single descriptor heap.
    uint32_t             max_sampler_heap_size; ///< Maximum number of samplers in a single descriptor heap.
    uint32_t             max_sampler_heap_size_with_embedded; ///< Maximum number of samplers in a single descriptor heap, if embedded samplers are used.
    uint32_t             descriptor_increment_size; ///< Size of a single descriptor in the descriptor heap. Used for calculating descriptor offsets.
    uint32_t             sampler_increment_size; ///< Size of a single sampler in the sampler heap. Used for calculating sampler offsets.
} WisDeviceDescriptorHeapProperties;

/**
 * @brief Provided by Wisdom 0.7.0. Device memory properties. Used to query memory type support and limits.
 *
 * */
typedef struct WisDeviceCommandQueueProperties {
    WisQueryPropertyType    property_type; ///< Defines the type of the queried property. @wis_must be WisQueryPropertyType..
    void*                   next_in_chain; ///< Pointer to the next queried data struct.
    bool                    supported_queues[5]; ///< Array of supported queue types. If a queue type is supported, the value is `1`, otherwise `0`. Order of queue types is the same as in WisCommandQueueType enum.
    bool                    relaxed_queue_transition; ///< Indicates if relaxed queue transition is supported. This feature allows executing command lists that contain buffers used on different queue types without explicit resource state transitions when the buffers is used on a different queue type. It is supported on Windows 10 22H2 and later with WDDM 3.0 or later. On Vulkan it requires `VK_KHR_maintenance9` extension.
    WisCommandQueuePriority max_queue_priority[5]; ///< Array of maximum supported priorities for each queue type. If a queue type is not supported, the value is `0`. Order of queue types is the same as in WisCommandQueueType enum.
} WisDeviceCommandQueueProperties;

/**
 * @brief Provided by Wisdom 0.7.0. Device memory properties. Used to query memory type support and limits.
 *
 * */
typedef struct WisDeviceMemoryProperties {
    WisQueryPropertyType property_type; ///< Defines the type of the queried property. @wis_must be `WisQueryPropertyTypeDeviceMemoryProperties`..
    void*                next_in_chain; ///< Pointer to the next queried data struct.
    bool                 gpu_upload_supported; ///< Indicates if GPU upload memory type is supported. This memory type allows mapping the memory and writing to it from CPU, while being accessible from GPU. It is usually implemented as write-combined memory on integrated GPUs and as a part of shared system memory on discrete GPUs.
    bool                 host_image_copy_supported; ///< Indicates if host image copy is supported. This feature allows copying data directly from CPU memory to optimal tiled image layout on GPU, without the need for an intermediate staging buffer. It is supported on Windows 10 22H2 and later with WDDM 3.0 or later. On Vulkan it requires `VK_EXT_host_image_copy` extension.
    uint32_t             supported_initial_transitions; ///< Bitfield of supported initial resource state transitions for buffers and textures. If a transition is supported, the corresponding bit is set to `1`, otherwise `0`. Bit positions are the same as in WisTextureState enum. `WisTextureStateUndefined` is always supported.
} WisDeviceMemoryProperties;

//==============================================================
// Constants
//==============================================================

/// @brief Provided by Wisdom 0.7.0. Append descriptor table entry after previous table.
#define WIS_DESCRIPTOR_OFFSET_APPEND ((uint32_t)0xffffffff)

/// @brief Provided by Wisdom 0.7.0. Select all remaining array layers.
#define WIS_REMAINING_LAYERS ((uint32_t)0xffffffff)

/// @brief Provided by Wisdom 0.7.0. Select all remaining mip levels.
#define WIS_REMAINING_MIPS ((uint32_t)0xffffffff)

/// @brief Provided by Wisdom 0.7.0. Defines the amount of usable space in root signature.
#define WIS_ROOT_SIGNATURE_DWORDS ((uint32_t)64)

/// @brief Provided by Wisdom 0.7.0. Defines the amount of barriers of all types that will not trigger allocation.
#define WIS_TRANSIENT_MAX_BARRIER_COUNT ((uint32_t)32)

/// @brief Provided by Wisdom 0.7.0. Defines the amount of planes that can be present on the single (YUV) image.
#define WIS_MAX_PLANE_COUNT ((uint32_t)3)

/// @brief Provided by Wisdom 0.7.0. Defines the minimum amount of vertex attributes that @wis_must be supported by the implementation.
#define WIS_MIN_SUPPORTED_INPUT_ATTRIBUTES ((uint32_t)16)

/// @brief Provided by Wisdom 0.7.0. Defines the minimum amount of vertex bindings that @wis_must be supported by the implementation.
#define WIS_MIN_SUPPORTED_INPUT_BINDINGS ((uint32_t)16)

/// @brief Provided by Wisdom 0.7.0. Defines the maximum amount of render targets that can be bound at once.
#define WIS_MAX_RENDER_TARGETS ((uint32_t)8)

/// @brief Provided by Wisdom 0.7.0. Defines the maximum amount of viewports that can be bound at once. The same count applies to scissors.
#define WIS_MAX_VIEWPORTS ((uint32_t)16)

/// @brief Provided by Wisdom 0.7.0. Defines the maximum amount of present rectangles and copy regions in the Copy* commands that can be used in a single operation.
#define WIS_MAX_COPY_REGIONS ((uint32_t)16)

/// @brief Provided by Wisdom 0.7.0. Select whole size of a resource.
#define WIS_WHOLE_SIZE ((uint64_t)0xffffffffffffffff)

#ifdef WISDOM_DX12
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a swapchain, which is a collection of render targets used for presentation.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Swapchain, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU surface, which can be used as a target for rendering and presentation.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Surface, 2);
WIS_DEFINE_HANDLE_VIEW(WisDX12Surface, 1);

static inline WisDX12SurfaceView wisGetDX12SurfaceView(const WisDX12Surface* handle)
{
    WisDX12SurfaceView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for resource views used in contiguous array.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12ViewHeap, 4);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU pipeline state object, which encapsulates the state of the GPU pipeline and allows to execute draw and dispatch calls with it.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Pipeline, 1);
WIS_DEFINE_HANDLE_VIEW(WisDX12Pipeline, 1);

static inline WisDX12PipelineView wisGetDX12PipelineView(const WisDX12Pipeline* handle)
{
    WisDX12PipelineView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU shader module, which contains shader code and allows to create pipeline state objects with it.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Shader, 1);
WIS_DEFINE_HANDLE_VIEW(WisDX12Shader, 1);

static inline WisDX12ShaderView wisGetDX12ShaderView(const WisDX12Shader* handle)
{
    WisDX12ShaderView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a cache for pipeline state objects, which allows to reuse already created pipelines and speed up pipeline creation.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12PipelineCache, 2);
WIS_DEFINE_HANDLE_VIEW(WisDX12PipelineCache, 1);

static inline WisDX12PipelineCacheView wisGetDX12PipelineCacheView(const WisDX12PipelineCache* handle)
{
    WisDX12PipelineCacheView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU texture resource.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Texture, 3);
WIS_DEFINE_HANDLE_VIEW(WisDX12Texture, 1);

static inline WisDX12TextureView wisGetDX12TextureView(const WisDX12Texture* handle)
{
    WisDX12TextureView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU buffer resource.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Buffer, 3);
WIS_DEFINE_HANDLE_VIEW(WisDX12Buffer, 1);

static inline WisDX12BufferView wisGetDX12BufferView(const WisDX12Buffer* handle)
{
    WisDX12BufferView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for descriptors used in contiguous array.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12DescriptorHeap, 5);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pipeline layout and a constant data storage, which defines resource bindings for shaders.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12RootSignature, 1);
WIS_DEFINE_HANDLE_VIEW(WisDX12RootSignature, 1);

static inline WisDX12RootSignatureView wisGetDX12RootSignatureView(const WisDX12RootSignature* handle)
{
    WisDX12RootSignatureView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class for allocating and managing GPU resources like buffers and textures.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12ResourceAllocator, 1);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a fence for GPU-CPU and GPU-GPU synchronization.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Fence, 2);
WIS_DEFINE_HANDLE_VIEW(WisDX12Fence, 1);

static inline WisDX12FenceView wisGetDX12FenceView(const WisDX12Fence* handle)
{
    WisDX12FenceView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command list for recording GPU commands.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12CommandList, 7);
WIS_DEFINE_HANDLE_VIEW(WisDX12CommandList, 1);

static inline WisDX12CommandListView wisGetDX12CommandListView(const WisDX12CommandList* handle)
{
    WisDX12CommandListView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pool allocator for command lists for recording GPU commands.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12CommandAllocator, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command queue for submitting command lists to the GPU.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12CommandQueue, 1);

/**
 * @brief Provided by Wisdom 0.7.0. Central class representing logical device.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Device, 5);

/**
 * @brief Provided by Wisdom 0.7.0. Class that contains a snapshot of adapters that are present on the system.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12AdapterQuery, 4);

/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
WIS_DEFINE_HANDLE(WisDX12Instance, 2);

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing device extension header. Used in extension development.
 *
 * */
typedef struct WisDX12DeviceExtensionHeader {
    const void* opaque; ///< defines opaque member. In @wis_mustnot be changed directly outside extension development.
} WisDX12DeviceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing instance extension header. Used in extension development.
 *
 * */
typedef struct WisDX12InstanceExtensionHeader {
    const void* opaque; ///< defines opaque member. It @wis_mustnot be changed directly outside extension development.
} WisDX12InstanceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Device requirements. Used to specify required features and properties for device creation.
 *
 * */
typedef struct WisDX12DeviceRequirements {
    const WisCommandQueueDesc*     queue_descs; ///< points to an array of WisCommandQueueDesc for which queues will be created during device creation.
    size_t                         queue_desc_count; ///< counts the number of queue descriptions in the wisAdapterQueryCreateDevice queue_descs array.
    WisDX12DeviceExtensionHeader** extensions; ///< points to an array of extensions that are to be initialized with pointers to WisDeviceExtensionHeader.
    size_t                         extension_count; ///< counts the number of extensions in the wisAdapterQueryCreateDevice extensions array.
} WisDX12DeviceRequirements;

/**
 * @brief Provided by Wisdom 0.7.0. Buffer barrier with the buffer handle.
 *
 * */
typedef struct WisDX12BufferBarrier {
    WisBarrierSync      sync_before; ///< Synchronization scope before the barrier.
    WisBarrierSync      sync_after; ///< Synchronization scope after the barrier.
    WisResourceAccess   access_before; ///< Access scope before the barrier.
    WisResourceAccess   access_after; ///< Access scope after the barrier.
    WisDX12BufferView   buffer; ///< Buffer view.
    uint64_t            offset; ///< Offset in bytes from the start of the buffer. Default is 0.
    uint64_t            size; ///< Barrier size in bytes. Default is `UINT64_MAX`, which means the whole buffer range.
    WisCommandQueueType queue_type_before; ///< Type of the queue the barrier is executed on before the synchronization point. Used for cross-queue barriers.
    WisCommandQueueType queue_type_after; ///< Type of the queue the barrier is executed on after the synchronization point. Used for cross-queue barriers.
} WisDX12BufferBarrier;

/**
 * @brief Provided by Wisdom 0.7.0. Global barrier without resource handles.
 *
 * */
typedef struct WisDX12GlobalBarrier {
    WisBarrierSync    sync_before; ///< Synchronization scope before the barrier.
    WisBarrierSync    sync_after; ///< Synchronization scope after the barrier.
    WisResourceAccess access_before; ///< Access scope before the barrier.
    WisResourceAccess access_after; ///< Access scope after the barrier.
} WisDX12GlobalBarrier;

/**
 * @brief Provided by Wisdom 0.7.0. Texture barrier with the texture handle.
 *
 * */
typedef struct WisDX12TextureBarrier {
    WisBarrierSync      sync_before; ///< Synchronization scope before the barrier.
    WisBarrierSync      sync_after; ///< Synchronization scope after the barrier.
    WisResourceAccess   access_before; ///< Access scope before the barrier.
    WisResourceAccess   access_after; ///< Access scope after the barrier.
    WisTextureState     state_before; ///< Texture state before the barrier.
    WisTextureState     state_after; ///< Texture state after the barrier.
    WisBarrierFlags     flags; ///< Barrier flags. Describe additional options for the barrier.
    WisDX12TextureView  texture; ///< Texture view.
    WisSubresourceRange subresource_range; ///< Subresource range for the barrier.
    WisCommandQueueType queue_type_before; ///< Type of the queue the barrier is executed on before the synchronization point. Used for cross-queue barriers.
    WisCommandQueueType queue_type_after; ///< Type of the queue the barrier is executed on after the synchronization point. Used for cross-queue barriers.
} WisDX12TextureBarrier;

/**
 * @brief Provided by Wisdom 0.7.0. Barrier group for multiple barriers submission.
 *
 * */
typedef struct WisDX12BarrierGroup {
    const WisDX12BufferBarrier*  buffer_barriers; ///< Array of buffer barriers.
    size_t                       buffer_barrier_count; ///< Number of buffer barriers in the `WisBarrierGroup::buffer_barriers` array.
    const WisDX12TextureBarrier* texture_barriers; ///< Array of texture barriers.
    size_t                       texture_barrier_count; ///< Number of texture barriers in the `WisBarrierGroup::texture_barriers` array.
    const WisDX12GlobalBarrier*  global_barriers; ///< Array of global barriers.
    size_t                       global_barrier_count; ///< Number of global barriers in the `WisBarrierGroup::global_barriers` array.
} WisDX12BarrierGroup;

/**
 * @brief Provided by Wisdom 0.7.0. Compute pipeline description for WisPipeline creation.
 *
 * */
typedef struct WisDX12ComputePipelineDesc {
    WisDX12RootSignatureView root_signature; ///< Root signature description for the pipeline.
    WisDX12ShaderView        compute_shader; ///< Compute shader bytecode.
    WisDX12PipelineCacheView cache; ///< Pipeline cache data. Used to speed up pipeline creation if available.
    WisPipelineFlags         flags; ///< Pipeline flags. Describe additional options for the pipeline.
} WisDX12ComputePipelineDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Graphics pipeline description for WisPipeline creation.
 *
 * */
typedef struct WisDX12GraphicsPipelineDesc {
    WisDX12RootSignatureView   root_signature; ///< Root signature description for the pipeline.
    WisDX12ShaderView          vertex_shader; ///< Vertex shader bytecode.
    WisDX12ShaderView          hull_shader; ///< Hull shader bytecode. If not set, the pipeline will be created without a hull shader.
    WisDX12ShaderView          domain_shader; ///< Domain shader bytecode. If not set, the pipeline will be created without a domain shader.
    WisDX12ShaderView          geometry_shader; ///< Geometry shader bytecode. If not set, the pipeline will be created without a geometry shader.
    WisDX12ShaderView          pixel_shader; ///< Pixel shader bytecode. If not set, the pipeline will be created without a pixel shader.
    WisRenderAttachmentsDesc   render_attachments; ///< Render attachments description for the pipeline. Used to create the compatible render pass for the pipeline.
    WisInputLayout             input_layout; ///< Input layout description for the pipeline. If not set, the pipeline will be created without an input layout.
    WisTopologyType            topology_type; ///< Topology type. Default is `WisTopologyTypeTriangle`.
    const WisRasterizerDesc*   rasterizer_desc; ///< Rasterizer description for the pipeline. If not set, the pipeline will be created with default rasterizer state.
    const WisSampleDesc*       sample_desc; ///< Sample description for the pipeline. If not set, the pipeline will be created with default sample state (no multisampling).
    const WisDepthStencilDesc* depth_stencil_desc; ///< Depth stencil description for the pipeline. If not set, the pipeline will be created with depth testing and stencil testing disabled.
    const WisBlendStateDesc*   blend_state_desc; ///< Blend state description for the pipeline. If not set, the pipeline will be created with blending disabled.
    WisDX12PipelineCacheView   cache; ///< Pipeline cache data. Used to speed up pipeline creation if available.
    WisPipelineFlags           flags; ///< Pipeline flags. Describe additional options for the pipeline.
} WisDX12GraphicsPipelineDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisSwapchain handle.
 * @param self is a pointer to the valid WisSwapchain instance.
 *
 * */
WISDOM_API void wisDX12DestroySwapchain(WisDX12Swapchain* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisSurface handle.
 * @param self is a pointer to the valid WisSurface instance.
 *
 * */
WISDOM_API void wisDX12DestroySurface(WisDX12Surface* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisViewHeap handle.
 * @param self is a pointer to the valid WisViewHeap instance.
 *
 * */
WISDOM_API void wisDX12DestroyViewHeap(WisDX12ViewHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisPipeline handle.
 * @param self is a pointer to the valid WisPipeline instance.
 *
 * */
WISDOM_API void wisDX12DestroyPipeline(WisDX12Pipeline* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisShader handle.
 * @param self is a pointer to the valid WisShader instance.
 *
 * */
WISDOM_API void wisDX12DestroyShader(WisDX12Shader* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisPipelineCache handle.
 * @param self is a pointer to the valid WisPipelineCache instance.
 *
 * */
WISDOM_API void wisDX12DestroyPipelineCache(WisDX12PipelineCache* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisTexture handle.
 * @param self is a pointer to the valid WisTexture instance.
 *
 * */
WISDOM_API void wisDX12DestroyTexture(WisDX12Texture* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisBuffer handle.
 * @param self is a pointer to the valid WisBuffer instance.
 *
 * */
WISDOM_API void wisDX12DestroyBuffer(WisDX12Buffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisDescriptorHeap handle.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 *
 * */
WISDOM_API void wisDX12DestroyDescriptorHeap(WisDX12DescriptorHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisRootSignature handle.
 * @param self is a pointer to the valid WisRootSignature instance.
 *
 * */
WISDOM_API void wisDX12DestroyRootSignature(WisDX12RootSignature* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisResourceAllocator handle.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 *
 * */
WISDOM_API void wisDX12DestroyResourceAllocator(WisDX12ResourceAllocator* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisFence handle.
 * @param self is a pointer to the valid WisFence instance.
 *
 * */
WISDOM_API void wisDX12DestroyFence(WisDX12Fence* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandList handle.
 * @param self is a pointer to the valid WisCommandList instance.
 *
 * */
WISDOM_API void wisDX12DestroyCommandList(WisDX12CommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandAllocator handle.
 * @param self is a pointer to the valid WisCommandAllocator instance.
 *
 * */
WISDOM_API void wisDX12DestroyCommandAllocator(WisDX12CommandAllocator* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandQueue handle.
 * @param self is a pointer to the valid WisCommandQueue instance.
 *
 * */
WISDOM_API void wisDX12DestroyCommandQueue(WisDX12CommandQueue* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisDevice handle.
 * @param self is a pointer to the valid WisDevice instance.
 *
 * */
WISDOM_API void wisDX12DestroyDevice(WisDX12Device* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisAdapterQuery handle.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 *
 * */
WISDOM_API void wisDX12DestroyAdapterQuery(WisDX12AdapterQuery* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisInstance handle.
 * @param self is a pointer to the valid WisInstance instance.
 *
 * */
WISDOM_API void wisDX12DestroyInstance(WisDX12Instance* self);

/**
 * @brief Provided by Wisdom 0.7.0. Creates the WisInstance with extensions, specified in extension array.
 * @param debug_desc points to WisDebugDesc, which defines debug callback and debug layer usage. If `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to WisInstanceExtensionHeader.
 * @param extension_count counts the number of extensions in the  array.
 * @param instance points to WisInstance, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CreateInstance(const WisDebugDesc*              debug_desc,
                                           WisDX12InstanceExtensionHeader** extensions,
                                           size_t                           extension_count,
                                           WisDX12Instance*                 instance);

/**
 * @brief Provided by Wisdom 0.7.0. Queries all the system adapters and allows to iterate through them. The order of adapters depends on `preference`: for MinConsumption - DirectX 12: Integrated, Discrete, External, Software; Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU. For Performance - DirectX 12: External, Discrete, Integrated, Software; Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
 * @param self is a pointer to the valid WisInstance instance.
 * @param preference defines the order in which adapters are listed.
 * @param query points to WisAdapterQuery, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12InstanceQueryAdapters(const WisDX12Instance* self,
                                                  WisAdapterPreference   preference,
                                                  WisDX12AdapterQuery*   query);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the number of adapters present on the system at the time of the query.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @return size is a number of adapters present on the system.
 *
 * */
WISDOM_API size_t wisDX12AdapterQueryGetAdapterCount(const WisDX12AdapterQuery* self);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the description of the adapter at given index.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @param index defines the index of the adapter to get the description for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * @param desc points to WisAdapterDesc, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12AdapterQueryGetAdapterDesc(const WisDX12AdapterQuery* self,
                                                       size_t                     index,
                                                       WisAdapterDesc*            desc);

/**
 * @brief Provided by Wisdom 0.7.0. Checks if the adapter at given index supports presentation to given surface.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @param index defines the index of the adapter to check the support for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * @param surface points to WisSurface to check the presentation support for.
 * @return bool `true` if the adapter supports presentation to the surface, `false` otherwise.
 *
 * */
WISDOM_API bool wisDX12AdapterQueryGetSurfaceSupport(const WisDX12AdapterQuery* self,
                                                     size_t                     index,
                                                     WisDX12SurfaceView         surface);

/**
 * @brief Provided by Wisdom 0.7.0. Creates the device for the adapter at given index.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @param index defines the index of the adapter to create the device for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * @param requirements points to WisDeviceRequirements, which defines required features and properties for device creation.
 * @param device points to WisDevice, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12AdapterQueryCreateDevice(const WisDX12AdapterQuery*       self,
                                                     size_t                           index,
                                                     const WisDX12DeviceRequirements* requirements,
                                                     WisDX12Device*                   device);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command queue of given type.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the command queue to create.
 * @param queue points to WisCommandQueue, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateCommandQueue(const WisDX12Device* self,
                                                     WisCommandQueueType  type,
                                                     WisDX12CommandQueue* queue);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command allocator to allocate command lists with.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the command list this pool is able to allocate.
 * @param allocator points to WisCommandAllocator, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateCommandAllocator(const WisDX12Device*     self,
                                                         WisCommandQueueType      type,
                                                         WisDX12CommandAllocator* allocator);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a fence for GPU-CPU and GPU-GPU synchronization.
 * @param self is a pointer to the valid WisDevice instance.
 * @param initial_value defines the initial value of the fence.
 * @param fence points to WisFence, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateFence(const WisDX12Device* self,
                                              uint64_t             initial_value,
                                              WisDX12Fence*        fence);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a resource allocator for managing GPU resources.
 * @param self is a pointer to the valid WisDevice instance.
 * @param allocator points to WisResourceAllocator, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceGetResourceAllocator(const WisDX12Device*      self,
                                                       WisDX12ResourceAllocator* allocator);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a pipeline layout with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisRootSignatureDesc, which describes the pipeline layout to create.
 * @param layout points to WisRootSignature, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateRootSignature(const WisDX12Device*        self,
                                                      const WisRootSignatureDesc* desc,
                                                      WisDX12RootSignature*       layout);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a descriptor storage with given description.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisDescriptorHeapDesc, which describes the descriptor heap to create.
 * @param heap points to WisDescriptorHeap, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateDescriptorHeap(const WisDX12Device*         self,
                                                       const WisDescriptorHeapDesc* desc,
                                                       WisDX12DescriptorHeap*       heap);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a view storage with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the view heap to create.
 * @param capacity defines the capacity in descriptors of the view heap to create.
 * @param heap points to WisDescriptorHeap, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateViewHeap(const WisDX12Device* self,
                                                 WisViewHeapType      type,
                                                 uint32_t             capacity,
                                                 WisDX12ViewHeap*     heap);

/**
 * @brief Provided by Wisdom 0.7.0. Queries the properties of the device.
 * @param self is a pointer to the valid WisDevice instance.
 * @param properties points to one of the query structs, which is filled with device properties.
 *
 * */
WISDOM_API void wisDX12DeviceQueryProperties(const WisDX12Device* self,
                                             void*                properties);

/**
 * @brief Provided by Wisdom 0.7.0. Waits on multiple fences simultaneously.
 * If wait_all is `WisMutiWaitTypeAll`, waits for all fences to be signaled.
 * Otherwise waits for any fence to be signaled.
 * @param self is a pointer to the valid WisDevice instance.
 * @param fences Array of fence views to wait on.
 * @param fence_values Fence values to wait fences to reach. Array @wis_must have fence_count values.
 * @param fence_count How many fences to wait on.
 * @param wait_for Specifies the kind of wait.
 * All - waits for all fences to be signaled.
 * Any - waits for any fence to be signaled.
 * Default is `WisMutiWaitTypeAll`
 * @param timeout The timeout in nanoseconds. If UINT64_MAX, waits indefinitely.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceWaitForMultipleFences(const WisDX12Device*    self,
                                                        const WisDX12FenceView* fences,
                                                        const uint64_t*         fence_values,
                                                        size_t                  fence_count,
                                                        WisMutiWaitType         wait_for,
                                                        uint64_t                timeout);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a pipeline cache for caching pipeline state objects.
 * @param self is a pointer to the valid WisDevice instance.
 * @param initial_data points to the initial cache data. If `nullptr`, the cache is created empty.
 * @param data_size defines the size of the initial cache data in bytes.
 * @param cache points to WisPipelineCache, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreatePipelineCache(const WisDX12Device*  self,
                                                      const uint8_t*        initial_data,
                                                      size_t                data_size,
                                                      WisDX12PipelineCache* cache);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a shader module from given data.
 * @param self is a pointer to the valid WisDevice instance.
 * @param data Shader bytecode.
 * @param size The size of the shader data in bytes. For SPIR-V @wis_must be multiple of 4.
 * @param shader points to WisShader, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateShader(const WisDX12Device* self,
                                               const uint8_t*       data,
                                               size_t               size,
                                               WisDX12Shader*       shader);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a compute pipeline state object with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisComputePipelineDesc, which describes the compute pipeline to create.
 * @param pipeline points to WisPipeline, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateComputePipeline(const WisDX12Device*              self,
                                                        const WisDX12ComputePipelineDesc* desc,
                                                        WisDX12Pipeline*                  pipeline);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a graphics pipeline state object with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisGraphicsPipelineDesc, which describes the graphics pipeline to create.
 * @param pipeline points to WisPipeline, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateGraphicsPipeline(const WisDX12Device*               self,
                                                         const WisDX12GraphicsPipelineDesc* desc,
                                                         WisDX12Pipeline*                   pipeline);

/**
 * @brief Provided by Wisdom 0.7.0. Checks if the surface format is supported for presentation and returns the supported format.
 * @param self is a pointer to the valid WisDevice instance.
 * @param surface points to WisSurface to check the presentation support for.
 * @param format defines the format to check the presentation support for.
 * @return bool Result of operation.
 *
 * */
WISDOM_API bool wisDX12DeviceGetFormatPresentationSupport(const WisDX12Device* self,
                                                          WisDX12SurfaceView   surface,
                                                          WisDataFormat        format);

/**
 * @brief Provided by Wisdom 0.7.0. Checks if the surface format is supported for presentation and returns the supported format.
 * @param self is a pointer to the valid WisDevice instance.
 * @param surface points to WisSurface to check the presentation support for.
 * @param params Parameters of the surface.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceGetSurfaceParameters(const WisDX12Device*  self,
                                                       WisDX12SurfaceView    surface,
                                                       WisSurfaceParameters* params);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a swapchain for given surface with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param surface points to WisSurface to create the swapchain for. Surface is ref-counted.
 * @param queue points to WisCommandQueue to create the swapchain for. Queue is ref-counted and @wis_must be a graphics queue.
 * @param desc points to WisSwapchainDesc, which describes the swapchain to create.
 * @param swapchain points to WisSwapchain, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DeviceCreateSwapchain(const WisDX12Device*       self,
                                                  const WisDX12Surface*      surface,
                                                  const WisDX12CommandQueue* queue,
                                                  const WisSwapchainDesc*    desc,
                                                  WisDX12Swapchain*          swapchain);

/**
 * @brief Provided by Wisdom 0.7.0. Get the current value of the fence.
 * @param self is a pointer to the valid WisFence instance.
 * @return u64 Value of the fence.
 *
 * */
WISDOM_API uint64_t wisDX12FenceGetCompletedValue(const WisDX12Fence* self);

/**
 * @brief Provided by Wisdom 0.7.0. Wait on CPU for the fence to reach a certain value.
 * @param self is a pointer to the valid WisFence instance.
 * @param value Value to wait for.
 * @param wait_ns The time to wait for the fence to reach the value in nanoseconds. Default is infinite.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12FenceWait(const WisDX12Fence* self,
                                      uint64_t            value,
                                      uint64_t            wait_ns);

/**
 * @brief Provided by Wisdom 0.7.0. Signal the fence from CPU.
 * @param self is a pointer to the valid WisFence instance.
 * @param value Value to signal.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12FenceSignal(const WisDX12Fence* self,
                                        uint64_t            value);

/**
 * @brief Provided by Wisdom 0.7.0. Executes the command lists.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param lists The command lists to execute.
 * @param count The number of command lists to execute.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandQueueSubmit(const WisDX12CommandQueue*    self,
                                               const WisDX12CommandListView* lists,
                                               size_t                        count);

/**
 * @brief Provided by Wisdom 0.7.0. Enqueue the signal to the queue, that gets executed after all the work has been done.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param fence The fence to signal.
 * @param value The value to signal the fence with.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandQueueSignalFence(const WisDX12CommandQueue* self,
                                                    WisDX12FenceView           fence,
                                                    uint64_t                   value);

/**
 * @brief Provided by Wisdom 0.7.0. Enqueues wait operation to the command queue. Queue then waits for the fence to be signalled from CPU or from another queue.
 * Can still be enqueued after the signal.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param fence The fence to wait on.
 * @param value The value to wait the fence to reach.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandQueueWaitFence(const WisDX12CommandQueue* self,
                                                  WisDX12FenceView           fence,
                                                  uint64_t                   value);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a buffer with given descriptor.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 * @param desc points to WisBufferDesc, which describes the buffer to create.
 * @param buffer points to WisBuffer, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12ResourceAllocatorCreateBuffer(const WisDX12ResourceAllocator* self,
                                                          const WisBufferDesc*            desc,
                                                          WisDX12Buffer*                  buffer);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a texture with given descriptor.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 * @param desc points to WisTextureDesc, which describes the texture to create.
 * @param initial_state defines the initial state of the texture. State transition @wis_must be supported, query WisDeviceMemoryProperties to get if the transition is supported. If not, @wis_must be `WisTextureStateUndefined`.
 * @param texture points to WisTexture, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12ResourceAllocatorCreateTexture(const WisDX12ResourceAllocator* self,
                                                           const WisTextureDesc*           desc,
                                                           WisTextureState                 initial_state,
                                                           WisDX12Texture*                 texture);

/**
 * @brief Provided by Wisdom 0.7.0. Maps the buffer memory to CPU accessible address space.
 * @param self is a pointer to the valid WisBuffer instance.
 * @return void points to the pointer, which is filled with the address of the mapped memory on success.
 *
 * */
WISDOM_API void* wisDX12BufferMap(const WisDX12Buffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Unmaps the buffer memory from CPU accessible address space.
 * @param self is a pointer to the valid WisBuffer instance.
 * @return u64 Address of the buffer on GPU.
 *
 * */
WISDOM_API uint64_t wisDX12BufferGetGPUAddress(const WisDX12Buffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Writes data direct to the texture subresource. Texture @wis_must be in `WisTextureStateCommon` and @wis_must_not be a depth texture.
 * @param self is a pointer to the valid WisTexture instance.
 * @param source_data points to the data to write to the texture.
 * @param target_region points to WisTextureRegion, which describes the region of the texture to write to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12TextureWriteSubresource(const WisDX12Texture*   self,
                                                    const void*             source_data,
                                                    const WisTextureRegion* target_region);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @return void CPU descriptor handle for the descriptor heap.
 *
 * */
WISDOM_API void* wisDX12DescriptorHeapGetCPUHandle(const WisDX12DescriptorHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeConstantBuffer` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param data points to WisConstantBufferBinding, which describes the constant buffer descriptors to write.
 * @param index defines the index in the descriptor heap to write the descriptors to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteConstantBuffer(const WisDX12DescriptorHeap*    self,
                                                              const WisConstantBufferBinding* data,
                                                              uint32_t                        index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeBuffer` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param buffer points to WisBuffer to write the descriptor for.
 * @param data points to WisBufferBinding, which describes the shader resource view descriptors to write.
 * @param index defines the index in the descriptor heap to write the descriptors to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteStructuredBuffer(const WisDX12DescriptorHeap* self,
                                                                WisDX12BufferView            buffer,
                                                                const WisBufferBinding*      data,
                                                                uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeRWBuffer` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param buffer points to WisBuffer to write the descriptor for.
 * @param data points to WisBufferBinding, which describes the shader resource view descriptors to write.
 * @param index defines the index in the descriptor heap to write the descriptors to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteRWStructuredBuffer(const WisDX12DescriptorHeap* self,
                                                                  WisDX12BufferView            buffer,
                                                                  const WisBufferBinding*      data,
                                                                  uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeSampler` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param sampler points to WisSamplerDesc, which describes the sampler descriptor to write.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteSampler(const WisDX12DescriptorHeap* self,
                                                       const WisSamplerDesc*        sampler,
                                                       uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param data points to , which describes the texture view to write.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteTexture(const WisDX12DescriptorHeap* self,
                                                       WisDX12TextureView           texture,
                                                       const WisTextureBinding*     data,
                                                       uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a texture view to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param data points to , which describes the texture view to write.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteRWTexture(const WisDX12DescriptorHeap* self,
                                                         WisDX12TextureView           texture,
                                                         const WisTextureBinding*     data,
                                                         uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a raytracing acceleration to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param address GPU address of a raytracing acceleration structure.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12DescriptorHeapWriteAccelerationStructure(const WisDX12DescriptorHeap* self,
                                                                     uint64_t                     address,
                                                                     uint32_t                     index);

/**
 * @brief Provided by Wisdom 0.7.0. Copies descriptors from one heap to another.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param dst_index defines the index in the destination descriptor heap to copy descriptors to.
 * @param src_ptr points to WisDescriptorHeap to copy descriptors from. Source heap @wis_must be CPU Only heap.
 * @param src_index defines the index in the source descriptor heap to copy descriptors from.
 * @param count defines the number of descriptors to copy.
 *
 * */
WISDOM_API void wisDX12DescriptorHeapCopyDescriptors(const WisDX12DescriptorHeap* self,
                                                     uint32_t                     dst_index,
                                                     const void*                  src_ptr,
                                                     uint32_t                     src_index,
                                                     uint32_t                     count);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a render target view to the view heap and returns the CPU descriptor handle for it.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param render_target points to WisRenderTargetDesc, which describes the render target view to write.
 * @param index defines the index in the view heap to write the descriptor to.
 * @return u64 CPU descriptor handle for the view heap.
 *
 * */
WISDOM_API uint64_t wisDX12ViewHeapWriteRenderTarget(const WisDX12ViewHeap*     self,
                                                     WisDX12TextureView         texture,
                                                     const WisRenderTargetDesc* render_target,
                                                     uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a depth stencil view to the view heap and returns the CPU descriptor handle for it.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param render_target points to WisRenderTargetDesc, which describes the render target view to write.
 * @param index defines the index in the view heap to write the descriptor to.
 * @return u64 CPU descriptor handle for the view heap.
 *
 * */
WISDOM_API uint64_t wisDX12ViewHeapWriteDepthStencil(const WisDX12ViewHeap*     self,
                                                     WisDX12TextureView         texture,
                                                     const WisRenderTargetDesc* render_target,
                                                     uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param index defines the index in the view heap to get the descriptor from.
 * @return u64 Address of a view in heap.
 *
 * */
WISDOM_API uint64_t wisDX12ViewHeapGetViewAddress(const WisDX12ViewHeap* self,
                                                  uint32_t               index);

/**
 * @brief Provided by Wisdom 0.7.0. Copies views from one heap to another.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param dst_index defines the index in the destination view heap to copy views to.
 * @param src_ptr points to WisViewHeap to copy views from.
 * @param src_index defines the index in the source view heap to copy views from.
 * @param count defines the number of views to copy.
 *
 * */
WISDOM_API void wisDX12ViewHeapCopyViews(const WisDX12ViewHeap* self,
                                         uint32_t               dst_index,
                                         uint64_t               src_ptr,
                                         uint32_t               src_index,
                                         uint32_t               count);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @return u64 CPU descriptor handle for the view heap.
 *
 * */
WISDOM_API uint64_t wisDX12ViewHeapGetCPUHandle(const WisDX12ViewHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Resets the command allocator, so it can be reused for allocating new command lists.
 * @param self is a pointer to the valid WisCommandAllocator instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandAllocatorReset(const WisDX12CommandAllocator* self);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command list of given type.
 * @param self is a pointer to the valid WisCommandAllocator instance.
 * @param list points to WisCommandList, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandAllocatorCreateCommandList(const WisDX12CommandAllocator* self,
                                                              WisDX12CommandList*            list);

/**
 * @brief Provided by Wisdom 0.7.0. Opens the command list, so commands can be recorded to it.
 * @param self is a pointer to the valid WisCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandListBegin(const WisDX12CommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
 * @param self is a pointer to the valid WisCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12CommandListEnd(const WisDX12CommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Binds descriptor heaps to the command list, so they can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param resource_heap points to WisDescriptorHeap with shader resource views, unordered access views and constant buffer views. If `nullptr`, no resource heap is bound.
 * @param sampler_heap points to WisDescriptorHeap with samplers. If `nullptr`, no sampler heap is bound.
 *
 * */
WISDOM_API void wisDX12CommandListSetDescriptorHeaps(const WisDX12CommandList*    self,
                                                     const WisDX12DescriptorHeap* resource_heap,
                                                     const WisDX12DescriptorHeap* sampler_heap);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the root signature for the command list, so it can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param signature points to WisRootSignature to set.
 * @param pipeline defines the pipeline type to set the root signature for.
 *
 * */
WISDOM_API void wisDX12CommandListSetRootSignature(const WisDX12CommandList* self,
                                                   WisDX12RootSignatureView  signature,
                                                   WisPipelineType           pipeline);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the push constants for the command list, so they can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param data points to , which describes the push constant data to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetPushConstants(const WisDX12CommandList*      self,
                                                   const WisPushConstantDataDesc* data);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the push descriptors for the command list, so they can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param data points to , which describes the push descriptors to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetPushDescriptor(const WisDX12CommandList*        self,
                                                    const WisPushDescriptorDataDesc* data);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the descriptor table offset in descriptor heap for the command list, so it can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param data defines the root parameter index to set the descriptor table for.
 *
 * */
WISDOM_API void wisDX12CommandListSetDescriptorTable(const WisDX12CommandList*         self,
                                                     const WisDescriptorTableDataDesc* data);

/**
 * @brief Provided by Wisdom 0.7.0. Inserts one or more barriers on the current command list.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param barriers points to an array of barriers to insert.
 *
 * */
WISDOM_API void wisDX12CommandListInsertBarriers(const WisDX12CommandList*  self,
                                                 const WisDX12BarrierGroup* barriers);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the pipeline state object for the command list, so it can be used for draw and dispatch calls.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param pipeline points to WisPipeline to set.
 * @param type defines the pipeline type to set the pipeline for.
 *
 * */
WISDOM_API void wisDX12CommandListSetPipeline(const WisDX12CommandList* self,
                                              WisDX12PipelineView       pipeline,
                                              WisPipelineType           type);

/**
 * @brief Provided by Wisdom 0.7.0. Sets multiple viewports.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param viewports The viewports to set.
 * @param count The number of viewports to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetViewports(WisDX12CommandList* self,
                                               const WisViewport*  viewports,
                                               size_t              count);

/**
 * @brief Provided by Wisdom 0.7.0. Sets multiple scissor rects.
 * Each n-th rect corresponds to n-th Viewport set in RSSetViewports if SV_ViewportArrayIndex is used in geometry shader.
 * Otherwise the first is chosen.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param scissor_rects The scissors to set.
 * @param rect_count The number of scissors to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetScissors(WisDX12CommandList* self,
                                              const WisRect*      scissor_rects,
                                              size_t              rect_count);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the primitive topology. Detemines how vertices shall be processed.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param topology The primitive topology to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetPrimitiveTopology(WisDX12CommandList*  self,
                                                       WisPrimitiveTopology topology);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the depth bias. Determines how depth values are modified during rasterization.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param depth_bias The depth bias to set.
 * @param depth_bias_clamp The depth bias clamp to set.
 * @param slope_scaled_depth_bias The slope scaled depth bias to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetDepthBias(WisDX12CommandList* self,
                                               float               depth_bias,
                                               float               depth_bias_clamp,
                                               float               slope_scaled_depth_bias);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the primitive restart value. Determines the index value which is treated as a primitive restart when using indexed draw calls.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param restart_value The primitive restart value to set.
 *
 * */
WISDOM_API void wisDX12CommandListSetPrimitiveRestartValue(WisDX12CommandList*      self,
                                                           WisPrimitiveRestartValue restart_value);

/**
 * @brief Provided by Wisdom 0.7.0. Dispatches compute shader.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param group_count_x The number of groups to dispatch in X dimension.
 * @param group_count_y The number of groups to dispatch in Y dimension. Default is 1.
 * @param group_count_z The number of groups to dispatch in Z dimension. Default is 1.
 *
 * */
WISDOM_API void wisDX12CommandListDispatch(const WisDX12CommandList* self,
                                           uint32_t                  group_count_x,
                                           uint32_t                  group_count_y,
                                           uint32_t                  group_count_z);

/**
 * @brief Provided by Wisdom 0.7.0. Gets the data from the pipeline cache.
 * @param self is a pointer to the valid WisPipelineCache instance.
 * @param data points to the data chunk, which is filled with the data of the cache data on success.
 * @param data_size defines the size of the data chunk in bytes. It @wis_must be greater or equal to the value returned by wisPipelineCacheGetSerializedSize.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12PipelineCacheSerialize(const WisDX12PipelineCache* self,
                                                   uint8_t*                    data,
                                                   size_t                      data_size);

/**
 * @brief Provided by Wisdom 0.7.0. Gets the size of the data in the pipeline cache.
 * @param self is a pointer to the valid WisPipelineCache instance.
 * @return size Size of the data in bytes.
 *
 * */
WISDOM_API size_t wisDX12PipelineCacheGetSerializedSize(const WisDX12PipelineCache* self);

/**
 * @brief Provided by Wisdom 0.7.0. Presents the swapchain image to the screen.
 * @param self is a pointer to the valid WisSwapchain instance.
 * @param flags defines the presentation options.
 * @param rects points to the array of rectangles to present. If `nullptr`, the entire image is presented.
 * @param rect_count defines the number of rectangles to present. Must be 0 if rects is `nullptr`.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisDX12SwapchainPresent(const WisDX12Swapchain* self,
                                             WisPresentFlags         flags,
                                             const WisRect*          rects,
                                             size_t                  rect_count);

/**
 * @brief Provided by Wisdom 0.7.0. Gets the index of the current backbuffer.
 * @param self is a pointer to the valid WisSwapchain instance.
 * @return u32 Index of the current backbuffer.
 *
 * */
WISDOM_API uint32_t wisDX12SwapchainGetCurrentIndex(const WisDX12Swapchain* self);

#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a swapchain, which is a collection of render targets used for presentation.
 *
 * */
WIS_DEFINE_HANDLE(WisVKSwapchain, 8);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU surface, which can be used as a target for rendering and presentation.
 *
 * */
WIS_DEFINE_HANDLE(WisVKSurface, 2);
WIS_DEFINE_HANDLE_VIEW(WisVKSurface, 1);

static inline WisVKSurfaceView wisGetVKSurfaceView(const WisVKSurface* handle)
{
    WisVKSurfaceView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for resource views used in contiguous array.
 *
 * */
WIS_DEFINE_HANDLE(WisVKViewHeap, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU pipeline state object, which encapsulates the state of the GPU pipeline and allows to execute draw and dispatch calls with it.
 *
 * */
WIS_DEFINE_HANDLE(WisVKPipeline, 2);
WIS_DEFINE_HANDLE_VIEW(WisVKPipeline, 1);

static inline WisVKPipelineView wisGetVKPipelineView(const WisVKPipeline* handle)
{
    WisVKPipelineView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU shader module, which contains shader code and allows to create pipeline state objects with it.
 *
 * */
WIS_DEFINE_HANDLE(WisVKShader, 2);
WIS_DEFINE_HANDLE_VIEW(WisVKShader, 1);

static inline WisVKShaderView wisGetVKShaderView(const WisVKShader* handle)
{
    WisVKShaderView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a cache for pipeline state objects, which allows to reuse already created pipelines and speed up pipeline creation.
 *
 * */
WIS_DEFINE_HANDLE(WisVKPipelineCache, 2);
WIS_DEFINE_HANDLE_VIEW(WisVKPipelineCache, 1);

static inline WisVKPipelineCacheView wisGetVKPipelineCacheView(const WisVKPipelineCache* handle)
{
    WisVKPipelineCacheView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU texture resource.
 *
 * */
WIS_DEFINE_HANDLE(WisVKTexture, 3);
WIS_DEFINE_HANDLE_VIEW(WisVKTexture, 1);

static inline WisVKTextureView wisGetVKTextureView(const WisVKTexture* handle)
{
    WisVKTextureView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU buffer resource.
 *
 * */
WIS_DEFINE_HANDLE(WisVKBuffer, 4);
WIS_DEFINE_HANDLE_VIEW(WisVKBuffer, 1);

static inline WisVKBufferView wisGetVKBufferView(const WisVKBuffer* handle)
{
    WisVKBufferView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for descriptors used in contiguous array.
 *
 * */
WIS_DEFINE_HANDLE(WisVKDescriptorHeap, 7);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pipeline layout and a constant data storage, which defines resource bindings for shaders.
 *
 * */
WIS_DEFINE_HANDLE(WisVKRootSignature, 1);
WIS_DEFINE_HANDLE_VIEW(WisVKRootSignature, 1);

static inline WisVKRootSignatureView wisGetVKRootSignatureView(const WisVKRootSignature* handle)
{
    WisVKRootSignatureView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class for allocating and managing GPU resources like buffers and textures.
 *
 * */
WIS_DEFINE_HANDLE(WisVKResourceAllocator, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a fence for GPU-CPU and GPU-GPU synchronization.
 *
 * */
WIS_DEFINE_HANDLE(WisVKFence, 3);
WIS_DEFINE_HANDLE_VIEW(WisVKFence, 1);

static inline WisVKFenceView wisGetVKFenceView(const WisVKFence* handle)
{
    WisVKFenceView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command list for recording GPU commands.
 *
 * */
WIS_DEFINE_HANDLE(WisVKCommandList, 7);
WIS_DEFINE_HANDLE_VIEW(WisVKCommandList, 1);

static inline WisVKCommandListView wisGetVKCommandListView(const WisVKCommandList* handle)
{
    WisVKCommandListView v;
    memcpy(&v, handle, sizeof(v));
    return v;
}

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pool allocator for command lists for recording GPU commands.
 *
 * */
WIS_DEFINE_HANDLE(WisVKCommandAllocator, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command queue for submitting command lists to the GPU.
 *
 * */
WIS_DEFINE_HANDLE(WisVKCommandQueue, 4);

/**
 * @brief Provided by Wisdom 0.7.0. Central class representing logical device.
 *
 * */
WIS_DEFINE_HANDLE(WisVKDevice, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Class that contains a snapshot of adapters that are present on the system.
 *
 * */
WIS_DEFINE_HANDLE(WisVKAdapterQuery, 4);

/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
WIS_DEFINE_HANDLE(WisVKInstance, 3);

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing device extension header. Used in extension development.
 *
 * */
typedef struct WisVKDeviceExtensionHeader {
    const void* opaque; ///< defines opaque member. In @wis_mustnot be changed directly outside extension development.
} WisVKDeviceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Opaque struct, representing instance extension header. Used in extension development.
 *
 * */
typedef struct WisVKInstanceExtensionHeader {
    const void* opaque; ///< defines opaque member. It @wis_mustnot be changed directly outside extension development.
} WisVKInstanceExtensionHeader;

/**
 * @brief Provided by Wisdom 0.7.0. Device requirements. Used to specify required features and properties for device creation.
 *
 * */
typedef struct WisVKDeviceRequirements {
    const WisCommandQueueDesc*   queue_descs; ///< points to an array of WisCommandQueueDesc for which queues will be created during device creation.
    size_t                       queue_desc_count; ///< counts the number of queue descriptions in the wisAdapterQueryCreateDevice queue_descs array.
    WisVKDeviceExtensionHeader** extensions; ///< points to an array of extensions that are to be initialized with pointers to WisDeviceExtensionHeader.
    size_t                       extension_count; ///< counts the number of extensions in the wisAdapterQueryCreateDevice extensions array.
} WisVKDeviceRequirements;

/**
 * @brief Provided by Wisdom 0.7.0. Buffer barrier with the buffer handle.
 *
 * */
typedef struct WisVKBufferBarrier {
    WisBarrierSync      sync_before; ///< Synchronization scope before the barrier.
    WisBarrierSync      sync_after; ///< Synchronization scope after the barrier.
    WisResourceAccess   access_before; ///< Access scope before the barrier.
    WisResourceAccess   access_after; ///< Access scope after the barrier.
    WisVKBufferView     buffer; ///< Buffer view.
    uint64_t            offset; ///< Offset in bytes from the start of the buffer. Default is 0.
    uint64_t            size; ///< Barrier size in bytes. Default is `UINT64_MAX`, which means the whole buffer range.
    WisCommandQueueType queue_type_before; ///< Type of the queue the barrier is executed on before the synchronization point. Used for cross-queue barriers.
    WisCommandQueueType queue_type_after; ///< Type of the queue the barrier is executed on after the synchronization point. Used for cross-queue barriers.
} WisVKBufferBarrier;

/**
 * @brief Provided by Wisdom 0.7.0. Global barrier without resource handles.
 *
 * */
typedef struct WisVKGlobalBarrier {
    WisBarrierSync    sync_before; ///< Synchronization scope before the barrier.
    WisBarrierSync    sync_after; ///< Synchronization scope after the barrier.
    WisResourceAccess access_before; ///< Access scope before the barrier.
    WisResourceAccess access_after; ///< Access scope after the barrier.
} WisVKGlobalBarrier;

/**
 * @brief Provided by Wisdom 0.7.0. Texture barrier with the texture handle.
 *
 * */
typedef struct WisVKTextureBarrier {
    WisBarrierSync      sync_before; ///< Synchronization scope before the barrier.
    WisBarrierSync      sync_after; ///< Synchronization scope after the barrier.
    WisResourceAccess   access_before; ///< Access scope before the barrier.
    WisResourceAccess   access_after; ///< Access scope after the barrier.
    WisTextureState     state_before; ///< Texture state before the barrier.
    WisTextureState     state_after; ///< Texture state after the barrier.
    WisBarrierFlags     flags; ///< Barrier flags. Describe additional options for the barrier.
    WisVKTextureView    texture; ///< Texture view.
    WisSubresourceRange subresource_range; ///< Subresource range for the barrier.
    WisCommandQueueType queue_type_before; ///< Type of the queue the barrier is executed on before the synchronization point. Used for cross-queue barriers.
    WisCommandQueueType queue_type_after; ///< Type of the queue the barrier is executed on after the synchronization point. Used for cross-queue barriers.
} WisVKTextureBarrier;

/**
 * @brief Provided by Wisdom 0.7.0. Barrier group for multiple barriers submission.
 *
 * */
typedef struct WisVKBarrierGroup {
    const WisVKBufferBarrier*  buffer_barriers; ///< Array of buffer barriers.
    size_t                     buffer_barrier_count; ///< Number of buffer barriers in the `WisBarrierGroup::buffer_barriers` array.
    const WisVKTextureBarrier* texture_barriers; ///< Array of texture barriers.
    size_t                     texture_barrier_count; ///< Number of texture barriers in the `WisBarrierGroup::texture_barriers` array.
    const WisVKGlobalBarrier*  global_barriers; ///< Array of global barriers.
    size_t                     global_barrier_count; ///< Number of global barriers in the `WisBarrierGroup::global_barriers` array.
} WisVKBarrierGroup;

/**
 * @brief Provided by Wisdom 0.7.0. Compute pipeline description for WisPipeline creation.
 *
 * */
typedef struct WisVKComputePipelineDesc {
    WisVKRootSignatureView root_signature; ///< Root signature description for the pipeline.
    WisVKShaderView        compute_shader; ///< Compute shader bytecode.
    WisVKPipelineCacheView cache; ///< Pipeline cache data. Used to speed up pipeline creation if available.
    WisPipelineFlags       flags; ///< Pipeline flags. Describe additional options for the pipeline.
} WisVKComputePipelineDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Graphics pipeline description for WisPipeline creation.
 *
 * */
typedef struct WisVKGraphicsPipelineDesc {
    WisVKRootSignatureView     root_signature; ///< Root signature description for the pipeline.
    WisVKShaderView            vertex_shader; ///< Vertex shader bytecode.
    WisVKShaderView            hull_shader; ///< Hull shader bytecode. If not set, the pipeline will be created without a hull shader.
    WisVKShaderView            domain_shader; ///< Domain shader bytecode. If not set, the pipeline will be created without a domain shader.
    WisVKShaderView            geometry_shader; ///< Geometry shader bytecode. If not set, the pipeline will be created without a geometry shader.
    WisVKShaderView            pixel_shader; ///< Pixel shader bytecode. If not set, the pipeline will be created without a pixel shader.
    WisRenderAttachmentsDesc   render_attachments; ///< Render attachments description for the pipeline. Used to create the compatible render pass for the pipeline.
    WisInputLayout             input_layout; ///< Input layout description for the pipeline. If not set, the pipeline will be created without an input layout.
    WisTopologyType            topology_type; ///< Topology type. Default is `WisTopologyTypeTriangle`.
    const WisRasterizerDesc*   rasterizer_desc; ///< Rasterizer description for the pipeline. If not set, the pipeline will be created with default rasterizer state.
    const WisSampleDesc*       sample_desc; ///< Sample description for the pipeline. If not set, the pipeline will be created with default sample state (no multisampling).
    const WisDepthStencilDesc* depth_stencil_desc; ///< Depth stencil description for the pipeline. If not set, the pipeline will be created with depth testing and stencil testing disabled.
    const WisBlendStateDesc*   blend_state_desc; ///< Blend state description for the pipeline. If not set, the pipeline will be created with blending disabled.
    WisVKPipelineCacheView     cache; ///< Pipeline cache data. Used to speed up pipeline creation if available.
    WisPipelineFlags           flags; ///< Pipeline flags. Describe additional options for the pipeline.
} WisVKGraphicsPipelineDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisSwapchain handle.
 * @param self is a pointer to the valid WisSwapchain instance.
 *
 * */
WISDOM_API void wisVKDestroySwapchain(WisVKSwapchain* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisSurface handle.
 * @param self is a pointer to the valid WisSurface instance.
 *
 * */
WISDOM_API void wisVKDestroySurface(WisVKSurface* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisViewHeap handle.
 * @param self is a pointer to the valid WisViewHeap instance.
 *
 * */
WISDOM_API void wisVKDestroyViewHeap(WisVKViewHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisPipeline handle.
 * @param self is a pointer to the valid WisPipeline instance.
 *
 * */
WISDOM_API void wisVKDestroyPipeline(WisVKPipeline* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisShader handle.
 * @param self is a pointer to the valid WisShader instance.
 *
 * */
WISDOM_API void wisVKDestroyShader(WisVKShader* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisPipelineCache handle.
 * @param self is a pointer to the valid WisPipelineCache instance.
 *
 * */
WISDOM_API void wisVKDestroyPipelineCache(WisVKPipelineCache* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisTexture handle.
 * @param self is a pointer to the valid WisTexture instance.
 *
 * */
WISDOM_API void wisVKDestroyTexture(WisVKTexture* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisBuffer handle.
 * @param self is a pointer to the valid WisBuffer instance.
 *
 * */
WISDOM_API void wisVKDestroyBuffer(WisVKBuffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisDescriptorHeap handle.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 *
 * */
WISDOM_API void wisVKDestroyDescriptorHeap(WisVKDescriptorHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisRootSignature handle.
 * @param self is a pointer to the valid WisRootSignature instance.
 *
 * */
WISDOM_API void wisVKDestroyRootSignature(WisVKRootSignature* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisResourceAllocator handle.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 *
 * */
WISDOM_API void wisVKDestroyResourceAllocator(WisVKResourceAllocator* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisFence handle.
 * @param self is a pointer to the valid WisFence instance.
 *
 * */
WISDOM_API void wisVKDestroyFence(WisVKFence* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandList handle.
 * @param self is a pointer to the valid WisCommandList instance.
 *
 * */
WISDOM_API void wisVKDestroyCommandList(WisVKCommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandAllocator handle.
 * @param self is a pointer to the valid WisCommandAllocator instance.
 *
 * */
WISDOM_API void wisVKDestroyCommandAllocator(WisVKCommandAllocator* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisCommandQueue handle.
 * @param self is a pointer to the valid WisCommandQueue instance.
 *
 * */
WISDOM_API void wisVKDestroyCommandQueue(WisVKCommandQueue* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisDevice handle.
 * @param self is a pointer to the valid WisDevice instance.
 *
 * */
WISDOM_API void wisVKDestroyDevice(WisVKDevice* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisAdapterQuery handle.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 *
 * */
WISDOM_API void wisVKDestroyAdapterQuery(WisVKAdapterQuery* self);

/**
 * @brief Provided by Wisdom 0.7.0. Destroys a WisInstance handle.
 * @param self is a pointer to the valid WisInstance instance.
 *
 * */
WISDOM_API void wisVKDestroyInstance(WisVKInstance* self);

/**
 * @brief Provided by Wisdom 0.7.0. Creates the WisInstance with extensions, specified in extension array.
 * @param debug_desc points to WisDebugDesc, which defines debug callback and debug layer usage. If `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to WisInstanceExtensionHeader.
 * @param extension_count counts the number of extensions in the  array.
 * @param instance points to WisInstance, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCreateInstance(const WisDebugDesc*            debug_desc,
                                         WisVKInstanceExtensionHeader** extensions,
                                         size_t                         extension_count,
                                         WisVKInstance*                 instance);

/**
 * @brief Provided by Wisdom 0.7.0. Queries all the system adapters and allows to iterate through them. The order of adapters depends on `preference`: for MinConsumption - DirectX 12: Integrated, Discrete, External, Software; Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU. For Performance - DirectX 12: External, Discrete, Integrated, Software; Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
 * @param self is a pointer to the valid WisInstance instance.
 * @param preference defines the order in which adapters are listed.
 * @param query points to WisAdapterQuery, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKInstanceQueryAdapters(const WisVKInstance* self,
                                                WisAdapterPreference preference,
                                                WisVKAdapterQuery*   query);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the number of adapters present on the system at the time of the query.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @return size is a number of adapters present on the system.
 *
 * */
WISDOM_API size_t wisVKAdapterQueryGetAdapterCount(const WisVKAdapterQuery* self);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the description of the adapter at given index.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @param index defines the index of the adapter to get the description for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * @param desc points to WisAdapterDesc, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKAdapterQueryGetAdapterDesc(const WisVKAdapterQuery* self,
                                                     size_t                   index,
                                                     WisAdapterDesc*          desc);

/**
 * @brief Provided by Wisdom 0.7.0. Checks if the adapter at given index supports presentation to given surface.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @param index defines the index of the adapter to check the support for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * @param surface points to WisSurface to check the presentation support for.
 * @return bool `true` if the adapter supports presentation to the surface, `false` otherwise.
 *
 * */
WISDOM_API bool wisVKAdapterQueryGetSurfaceSupport(const WisVKAdapterQuery* self,
                                                   size_t                   index,
                                                   WisVKSurfaceView         surface);

/**
 * @brief Provided by Wisdom 0.7.0. Creates the device for the adapter at given index.
 * @param self is a pointer to the valid WisAdapterQuery instance.
 * @param index defines the index of the adapter to create the device for. It @wis_must be less than the value returned by wisAdapterQueryGetAdapterCount.
 * @param requirements points to WisDeviceRequirements, which defines required features and properties for device creation.
 * @param device points to WisDevice, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKAdapterQueryCreateDevice(const WisVKAdapterQuery*       self,
                                                   size_t                         index,
                                                   const WisVKDeviceRequirements* requirements,
                                                   WisVKDevice*                   device);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command queue of given type.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the command queue to create.
 * @param queue points to WisCommandQueue, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateCommandQueue(const WisVKDevice*  self,
                                                   WisCommandQueueType type,
                                                   WisVKCommandQueue*  queue);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command allocator to allocate command lists with.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the command list this pool is able to allocate.
 * @param allocator points to WisCommandAllocator, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateCommandAllocator(const WisVKDevice*     self,
                                                       WisCommandQueueType    type,
                                                       WisVKCommandAllocator* allocator);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a fence for GPU-CPU and GPU-GPU synchronization.
 * @param self is a pointer to the valid WisDevice instance.
 * @param initial_value defines the initial value of the fence.
 * @param fence points to WisFence, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateFence(const WisVKDevice* self,
                                            uint64_t           initial_value,
                                            WisVKFence*        fence);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a resource allocator for managing GPU resources.
 * @param self is a pointer to the valid WisDevice instance.
 * @param allocator points to WisResourceAllocator, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceGetResourceAllocator(const WisVKDevice*      self,
                                                     WisVKResourceAllocator* allocator);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a pipeline layout with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisRootSignatureDesc, which describes the pipeline layout to create.
 * @param layout points to WisRootSignature, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateRootSignature(const WisVKDevice*          self,
                                                    const WisRootSignatureDesc* desc,
                                                    WisVKRootSignature*         layout);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a descriptor storage with given description.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisDescriptorHeapDesc, which describes the descriptor heap to create.
 * @param heap points to WisDescriptorHeap, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateDescriptorHeap(const WisVKDevice*           self,
                                                     const WisDescriptorHeapDesc* desc,
                                                     WisVKDescriptorHeap*         heap);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a view storage with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param type defines the type of the view heap to create.
 * @param capacity defines the capacity in descriptors of the view heap to create.
 * @param heap points to WisDescriptorHeap, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateViewHeap(const WisVKDevice* self,
                                               WisViewHeapType    type,
                                               uint32_t           capacity,
                                               WisVKViewHeap*     heap);

/**
 * @brief Provided by Wisdom 0.7.0. Queries the properties of the device.
 * @param self is a pointer to the valid WisDevice instance.
 * @param properties points to one of the query structs, which is filled with device properties.
 *
 * */
WISDOM_API void wisVKDeviceQueryProperties(const WisVKDevice* self,
                                           void*              properties);

/**
 * @brief Provided by Wisdom 0.7.0. Waits on multiple fences simultaneously.
 * If wait_all is `WisMutiWaitTypeAll`, waits for all fences to be signaled.
 * Otherwise waits for any fence to be signaled.
 * @param self is a pointer to the valid WisDevice instance.
 * @param fences Array of fence views to wait on.
 * @param fence_values Fence values to wait fences to reach. Array @wis_must have fence_count values.
 * @param fence_count How many fences to wait on.
 * @param wait_for Specifies the kind of wait.
 * All - waits for all fences to be signaled.
 * Any - waits for any fence to be signaled.
 * Default is `WisMutiWaitTypeAll`
 * @param timeout The timeout in nanoseconds. If UINT64_MAX, waits indefinitely.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceWaitForMultipleFences(const WisVKDevice*    self,
                                                      const WisVKFenceView* fences,
                                                      const uint64_t*       fence_values,
                                                      size_t                fence_count,
                                                      WisMutiWaitType       wait_for,
                                                      uint64_t              timeout);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a pipeline cache for caching pipeline state objects.
 * @param self is a pointer to the valid WisDevice instance.
 * @param initial_data points to the initial cache data. If `nullptr`, the cache is created empty.
 * @param data_size defines the size of the initial cache data in bytes.
 * @param cache points to WisPipelineCache, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreatePipelineCache(const WisVKDevice*  self,
                                                    const uint8_t*      initial_data,
                                                    size_t              data_size,
                                                    WisVKPipelineCache* cache);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a shader module from given data.
 * @param self is a pointer to the valid WisDevice instance.
 * @param data Shader bytecode.
 * @param size The size of the shader data in bytes. For SPIR-V @wis_must be multiple of 4.
 * @param shader points to WisShader, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateShader(const WisVKDevice* self,
                                             const uint8_t*     data,
                                             size_t             size,
                                             WisVKShader*       shader);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a compute pipeline state object with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisComputePipelineDesc, which describes the compute pipeline to create.
 * @param pipeline points to WisPipeline, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateComputePipeline(const WisVKDevice*              self,
                                                      const WisVKComputePipelineDesc* desc,
                                                      WisVKPipeline*                  pipeline);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a graphics pipeline state object with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param desc points to WisGraphicsPipelineDesc, which describes the graphics pipeline to create.
 * @param pipeline points to WisPipeline, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateGraphicsPipeline(const WisVKDevice*               self,
                                                       const WisVKGraphicsPipelineDesc* desc,
                                                       WisVKPipeline*                   pipeline);

/**
 * @brief Provided by Wisdom 0.7.0. Checks if the surface format is supported for presentation and returns the supported format.
 * @param self is a pointer to the valid WisDevice instance.
 * @param surface points to WisSurface to check the presentation support for.
 * @param format defines the format to check the presentation support for.
 * @return bool Result of operation.
 *
 * */
WISDOM_API bool wisVKDeviceGetFormatPresentationSupport(const WisVKDevice* self,
                                                        WisVKSurfaceView   surface,
                                                        WisDataFormat      format);

/**
 * @brief Provided by Wisdom 0.7.0. Checks if the surface format is supported for presentation and returns the supported format.
 * @param self is a pointer to the valid WisDevice instance.
 * @param surface points to WisSurface to check the presentation support for.
 * @param params Parameters of the surface.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceGetSurfaceParameters(const WisVKDevice*    self,
                                                     WisVKSurfaceView      surface,
                                                     WisSurfaceParameters* params);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a swapchain for given surface with given descriptor.
 * @param self is a pointer to the valid WisDevice instance.
 * @param surface points to WisSurface to create the swapchain for. Surface is ref-counted.
 * @param queue points to WisCommandQueue to create the swapchain for. Queue is ref-counted and @wis_must be a graphics queue.
 * @param desc points to WisSwapchainDesc, which describes the swapchain to create.
 * @param swapchain points to WisSwapchain, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDeviceCreateSwapchain(const WisVKDevice*       self,
                                                const WisVKSurface*      surface,
                                                const WisVKCommandQueue* queue,
                                                const WisSwapchainDesc*  desc,
                                                WisVKSwapchain*          swapchain);

/**
 * @brief Provided by Wisdom 0.7.0. Get the current value of the fence.
 * @param self is a pointer to the valid WisFence instance.
 * @return u64 Value of the fence.
 *
 * */
WISDOM_API uint64_t wisVKFenceGetCompletedValue(const WisVKFence* self);

/**
 * @brief Provided by Wisdom 0.7.0. Wait on CPU for the fence to reach a certain value.
 * @param self is a pointer to the valid WisFence instance.
 * @param value Value to wait for.
 * @param wait_ns The time to wait for the fence to reach the value in nanoseconds. Default is infinite.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKFenceWait(const WisVKFence* self,
                                    uint64_t          value,
                                    uint64_t          wait_ns);

/**
 * @brief Provided by Wisdom 0.7.0. Signal the fence from CPU.
 * @param self is a pointer to the valid WisFence instance.
 * @param value Value to signal.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKFenceSignal(const WisVKFence* self,
                                      uint64_t          value);

/**
 * @brief Provided by Wisdom 0.7.0. Executes the command lists.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param lists The command lists to execute.
 * @param count The number of command lists to execute.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCommandQueueSubmit(const WisVKCommandQueue*    self,
                                             const WisVKCommandListView* lists,
                                             size_t                      count);

/**
 * @brief Provided by Wisdom 0.7.0. Enqueue the signal to the queue, that gets executed after all the work has been done.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param fence The fence to signal.
 * @param value The value to signal the fence with.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCommandQueueSignalFence(const WisVKCommandQueue* self,
                                                  WisVKFenceView           fence,
                                                  uint64_t                 value);

/**
 * @brief Provided by Wisdom 0.7.0. Enqueues wait operation to the command queue. Queue then waits for the fence to be signalled from CPU or from another queue.
 * Can still be enqueued after the signal.
 * @param self is a pointer to the valid WisCommandQueue instance.
 * @param fence The fence to wait on.
 * @param value The value to wait the fence to reach.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCommandQueueWaitFence(const WisVKCommandQueue* self,
                                                WisVKFenceView           fence,
                                                uint64_t                 value);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a buffer with given descriptor.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 * @param desc points to WisBufferDesc, which describes the buffer to create.
 * @param buffer points to WisBuffer, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKResourceAllocatorCreateBuffer(const WisVKResourceAllocator* self,
                                                        const WisBufferDesc*          desc,
                                                        WisVKBuffer*                  buffer);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a texture with given descriptor.
 * @param self is a pointer to the valid WisResourceAllocator instance.
 * @param desc points to WisTextureDesc, which describes the texture to create.
 * @param initial_state defines the initial state of the texture. State transition @wis_must be supported, query WisDeviceMemoryProperties to get if the transition is supported. If not, @wis_must be `WisTextureStateUndefined`.
 * @param texture points to WisTexture, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKResourceAllocatorCreateTexture(const WisVKResourceAllocator* self,
                                                         const WisTextureDesc*         desc,
                                                         WisTextureState               initial_state,
                                                         WisVKTexture*                 texture);

/**
 * @brief Provided by Wisdom 0.7.0. Maps the buffer memory to CPU accessible address space.
 * @param self is a pointer to the valid WisBuffer instance.
 * @return void points to the pointer, which is filled with the address of the mapped memory on success.
 *
 * */
WISDOM_API void* wisVKBufferMap(const WisVKBuffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Unmaps the buffer memory from CPU accessible address space.
 * @param self is a pointer to the valid WisBuffer instance.
 * @return u64 Address of the buffer on GPU.
 *
 * */
WISDOM_API uint64_t wisVKBufferGetGPUAddress(const WisVKBuffer* self);

/**
 * @brief Provided by Wisdom 0.7.0. Writes data direct to the texture subresource. Texture @wis_must be in `WisTextureStateCommon` and @wis_must_not be a depth texture.
 * @param self is a pointer to the valid WisTexture instance.
 * @param source_data points to the data to write to the texture.
 * @param target_region points to WisTextureRegion, which describes the region of the texture to write to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKTextureWriteSubresource(const WisVKTexture*     self,
                                                  const void*             source_data,
                                                  const WisTextureRegion* target_region);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @return void CPU descriptor handle for the descriptor heap.
 *
 * */
WISDOM_API void* wisVKDescriptorHeapGetCPUHandle(const WisVKDescriptorHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeConstantBuffer` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param data points to WisConstantBufferBinding, which describes the constant buffer descriptors to write.
 * @param index defines the index in the descriptor heap to write the descriptors to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDescriptorHeapWriteConstantBuffer(const WisVKDescriptorHeap*      self,
                                                            const WisConstantBufferBinding* data,
                                                            uint32_t                        index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeBuffer` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param buffer points to WisBuffer to write the descriptor for.
 * @param data points to WisBufferBinding, which describes the shader resource view descriptors to write.
 * @param index defines the index in the descriptor heap to write the descriptors to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDescriptorHeapWriteStructuredBuffer(const WisVKDescriptorHeap* self,
                                                              WisVKBufferView            buffer,
                                                              const WisBufferBinding*    data,
                                                              uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeRWBuffer` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param buffer points to WisBuffer to write the descriptor for.
 * @param data points to WisBufferBinding, which describes the shader resource view descriptors to write.
 * @param index defines the index in the descriptor heap to write the descriptors to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDescriptorHeapWriteRWStructuredBuffer(const WisVKDescriptorHeap* self,
                                                                WisVKBufferView            buffer,
                                                                const WisBufferBinding*    data,
                                                                uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes `WisDescriptorTypeSampler` descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param sampler points to WisSamplerDesc, which describes the sampler descriptor to write.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDescriptorHeapWriteSampler(const WisVKDescriptorHeap* self,
                                                     const WisSamplerDesc*      sampler,
                                                     uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a descriptor to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param data points to , which describes the texture view to write.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDescriptorHeapWriteTexture(const WisVKDescriptorHeap* self,
                                                     WisVKTextureView           texture,
                                                     const WisTextureBinding*   data,
                                                     uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a texture view to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param data points to , which describes the texture view to write.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDescriptorHeapWriteRWTexture(const WisVKDescriptorHeap* self,
                                                       WisVKTextureView           texture,
                                                       const WisTextureBinding*   data,
                                                       uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a raytracing acceleration to the descriptor heap.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param address GPU address of a raytracing acceleration structure.
 * @param index defines the index in the descriptor heap to write the descriptor to.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKDescriptorHeapWriteAccelerationStructure(const WisVKDescriptorHeap* self,
                                                                   uint64_t                   address,
                                                                   uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Copies descriptors from one heap to another.
 * @param self is a pointer to the valid WisDescriptorHeap instance.
 * @param dst_index defines the index in the destination descriptor heap to copy descriptors to.
 * @param src_ptr points to WisDescriptorHeap to copy descriptors from. Source heap @wis_must be CPU Only heap.
 * @param src_index defines the index in the source descriptor heap to copy descriptors from.
 * @param count defines the number of descriptors to copy.
 *
 * */
WISDOM_API void wisVKDescriptorHeapCopyDescriptors(const WisVKDescriptorHeap* self,
                                                   uint32_t                   dst_index,
                                                   const void*                src_ptr,
                                                   uint32_t                   src_index,
                                                   uint32_t                   count);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a render target view to the view heap and returns the CPU descriptor handle for it.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param render_target points to WisRenderTargetDesc, which describes the render target view to write.
 * @param index defines the index in the view heap to write the descriptor to.
 * @return u64 CPU descriptor handle for the view heap.
 *
 * */
WISDOM_API uint64_t wisVKViewHeapWriteRenderTarget(const WisVKViewHeap*       self,
                                                   WisVKTextureView           texture,
                                                   const WisRenderTargetDesc* render_target,
                                                   uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Writes a depth stencil view to the view heap and returns the CPU descriptor handle for it.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param texture points to WisTexture to write the descriptor for.
 * @param render_target points to WisRenderTargetDesc, which describes the render target view to write.
 * @param index defines the index in the view heap to write the descriptor to.
 * @return u64 CPU descriptor handle for the view heap.
 *
 * */
WISDOM_API uint64_t wisVKViewHeapWriteDepthStencil(const WisVKViewHeap*       self,
                                                   WisVKTextureView           texture,
                                                   const WisRenderTargetDesc* render_target,
                                                   uint32_t                   index);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param index defines the index in the view heap to get the descriptor from.
 * @return u64 Address of a view in heap.
 *
 * */
WISDOM_API uint64_t wisVKViewHeapGetViewAddress(const WisVKViewHeap* self,
                                                uint32_t             index);

/**
 * @brief Provided by Wisdom 0.7.0. Copies views from one heap to another.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @param dst_index defines the index in the destination view heap to copy views to.
 * @param src_ptr points to WisViewHeap to copy views from.
 * @param src_index defines the index in the source view heap to copy views from.
 * @param count defines the number of views to copy.
 *
 * */
WISDOM_API void wisVKViewHeapCopyViews(const WisVKViewHeap* self,
                                       uint32_t             dst_index,
                                       uint64_t             src_ptr,
                                       uint32_t             src_index,
                                       uint32_t             count);

/**
 * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
 * @param self is a pointer to the valid WisViewHeap instance.
 * @return u64 CPU descriptor handle for the view heap.
 *
 * */
WISDOM_API uint64_t wisVKViewHeapGetCPUHandle(const WisVKViewHeap* self);

/**
 * @brief Provided by Wisdom 0.7.0. Resets the command allocator, so it can be reused for allocating new command lists.
 * @param self is a pointer to the valid WisCommandAllocator instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCommandAllocatorReset(const WisVKCommandAllocator* self);

/**
 * @brief Provided by Wisdom 0.7.0. Creates a command list of given type.
 * @param self is a pointer to the valid WisCommandAllocator instance.
 * @param list points to WisCommandList, which is initialized on success.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCommandAllocatorCreateCommandList(const WisVKCommandAllocator* self,
                                                            WisVKCommandList*            list);

/**
 * @brief Provided by Wisdom 0.7.0. Opens the command list, so commands can be recorded to it.
 * @param self is a pointer to the valid WisCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCommandListBegin(const WisVKCommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
 * @param self is a pointer to the valid WisCommandList instance.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKCommandListEnd(const WisVKCommandList* self);

/**
 * @brief Provided by Wisdom 0.7.0. Binds descriptor heaps to the command list, so they can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param resource_heap points to WisDescriptorHeap with shader resource views, unordered access views and constant buffer views. If `nullptr`, no resource heap is bound.
 * @param sampler_heap points to WisDescriptorHeap with samplers. If `nullptr`, no sampler heap is bound.
 *
 * */
WISDOM_API void wisVKCommandListSetDescriptorHeaps(const WisVKCommandList*    self,
                                                   const WisVKDescriptorHeap* resource_heap,
                                                   const WisVKDescriptorHeap* sampler_heap);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the root signature for the command list, so it can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param signature points to WisRootSignature to set.
 * @param pipeline defines the pipeline type to set the root signature for.
 *
 * */
WISDOM_API void wisVKCommandListSetRootSignature(const WisVKCommandList* self,
                                                 WisVKRootSignatureView  signature,
                                                 WisPipelineType         pipeline);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the push constants for the command list, so they can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param data points to , which describes the push constant data to set.
 *
 * */
WISDOM_API void wisVKCommandListSetPushConstants(const WisVKCommandList*        self,
                                                 const WisPushConstantDataDesc* data);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the push descriptors for the command list, so they can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param data points to , which describes the push descriptors to set.
 *
 * */
WISDOM_API void wisVKCommandListSetPushDescriptor(const WisVKCommandList*          self,
                                                  const WisPushDescriptorDataDesc* data);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the descriptor table offset in descriptor heap for the command list, so it can be used for resource binding.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param data defines the root parameter index to set the descriptor table for.
 *
 * */
WISDOM_API void wisVKCommandListSetDescriptorTable(const WisVKCommandList*           self,
                                                   const WisDescriptorTableDataDesc* data);

/**
 * @brief Provided by Wisdom 0.7.0. Inserts one or more barriers on the current command list.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param barriers points to an array of barriers to insert.
 *
 * */
WISDOM_API void wisVKCommandListInsertBarriers(const WisVKCommandList*  self,
                                               const WisVKBarrierGroup* barriers);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the pipeline state object for the command list, so it can be used for draw and dispatch calls.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param pipeline points to WisPipeline to set.
 * @param type defines the pipeline type to set the pipeline for.
 *
 * */
WISDOM_API void wisVKCommandListSetPipeline(const WisVKCommandList* self,
                                            WisVKPipelineView       pipeline,
                                            WisPipelineType         type);

/**
 * @brief Provided by Wisdom 0.7.0. Sets multiple viewports.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param viewports The viewports to set.
 * @param count The number of viewports to set.
 *
 * */
WISDOM_API void wisVKCommandListSetViewports(WisVKCommandList*  self,
                                             const WisViewport* viewports,
                                             size_t             count);

/**
 * @brief Provided by Wisdom 0.7.0. Sets multiple scissor rects.
 * Each n-th rect corresponds to n-th Viewport set in RSSetViewports if SV_ViewportArrayIndex is used in geometry shader.
 * Otherwise the first is chosen.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param scissor_rects The scissors to set.
 * @param rect_count The number of scissors to set.
 *
 * */
WISDOM_API void wisVKCommandListSetScissors(WisVKCommandList* self,
                                            const WisRect*    scissor_rects,
                                            size_t            rect_count);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the primitive topology. Detemines how vertices shall be processed.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param topology The primitive topology to set.
 *
 * */
WISDOM_API void wisVKCommandListSetPrimitiveTopology(WisVKCommandList*    self,
                                                     WisPrimitiveTopology topology);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the depth bias. Determines how depth values are modified during rasterization.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param depth_bias The depth bias to set.
 * @param depth_bias_clamp The depth bias clamp to set.
 * @param slope_scaled_depth_bias The slope scaled depth bias to set.
 *
 * */
WISDOM_API void wisVKCommandListSetDepthBias(WisVKCommandList* self,
                                             float             depth_bias,
                                             float             depth_bias_clamp,
                                             float             slope_scaled_depth_bias);

/**
 * @brief Provided by Wisdom 0.7.0. Sets the primitive restart value. Determines the index value which is treated as a primitive restart when using indexed draw calls.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param restart_value The primitive restart value to set.
 *
 * */
WISDOM_API void wisVKCommandListSetPrimitiveRestartValue(WisVKCommandList*        self,
                                                         WisPrimitiveRestartValue restart_value);

/**
 * @brief Provided by Wisdom 0.7.0. Dispatches compute shader.
 * @param self is a pointer to the valid WisCommandList instance.
 * @param group_count_x The number of groups to dispatch in X dimension.
 * @param group_count_y The number of groups to dispatch in Y dimension. Default is 1.
 * @param group_count_z The number of groups to dispatch in Z dimension. Default is 1.
 *
 * */
WISDOM_API void wisVKCommandListDispatch(const WisVKCommandList* self,
                                         uint32_t                group_count_x,
                                         uint32_t                group_count_y,
                                         uint32_t                group_count_z);

/**
 * @brief Provided by Wisdom 0.7.0. Gets the data from the pipeline cache.
 * @param self is a pointer to the valid WisPipelineCache instance.
 * @param data points to the data chunk, which is filled with the data of the cache data on success.
 * @param data_size defines the size of the data chunk in bytes. It @wis_must be greater or equal to the value returned by wisPipelineCacheGetSerializedSize.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKPipelineCacheSerialize(const WisVKPipelineCache* self,
                                                 uint8_t*                  data,
                                                 size_t                    data_size);

/**
 * @brief Provided by Wisdom 0.7.0. Gets the size of the data in the pipeline cache.
 * @param self is a pointer to the valid WisPipelineCache instance.
 * @return size Size of the data in bytes.
 *
 * */
WISDOM_API size_t wisVKPipelineCacheGetSerializedSize(const WisVKPipelineCache* self);

/**
 * @brief Provided by Wisdom 0.7.0. Presents the swapchain image to the screen.
 * @param self is a pointer to the valid WisSwapchain instance.
 * @param flags defines the presentation options.
 * @param rects points to the array of rectangles to present. If `nullptr`, the entire image is presented.
 * @param rect_count defines the number of rectangles to present. Must be 0 if rects is `nullptr`.
 * @return Result denoting the outcome of operation.
 *
 * */
WISDOM_API WisResult wisVKSwapchainPresent(const WisVKSwapchain* self,
                                           WisPresentFlags       flags,
                                           const WisRect*        rects,
                                           size_t                rect_count);

/**
 * @brief Provided by Wisdom 0.7.0. Gets the index of the current backbuffer.
 * @param self is a pointer to the valid WisSwapchain instance.
 * @return u32 Index of the current backbuffer.
 *
 * */
WISDOM_API uint32_t wisVKSwapchainGetCurrentIndex(const WisVKSwapchain* self);

#endif // WISDOM_VULKAN

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_CORE_C_API_H
