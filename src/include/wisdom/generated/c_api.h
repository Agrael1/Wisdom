// This file is generated. Do not edit directly.
#ifndef WISDOM_C_API_H
#define WISDOM_C_API_H
#include <wisdom/global/definitions.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

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
typedef enum WisCompareOperation {
    WisCompareOperationNone         = 0, ///< No comparison.
    WisCompareOperationNever        = 1, ///< Always fail the comparison.
    WisCompareOperationLess         = 2, ///< Pass the comparison if the source value is less than the destination value.
    WisCompareOperationEqual        = 3, ///< Pass the comparison if the source value is equal to the destination value.
    WisCompareOperationLessEqual    = 4, ///< Pass the comparison if the source value is less than or equal to the destination value.
    WisCompareOperationGreater      = 5, ///< Pass the comparison if the source value is greater than the destination value.
    WisCompareOperationNotEqual     = 6, ///< Pass the comparison if the source value is not equal to the destination value.
    WisCompareOperationGreaterEqual = 7, ///< Pass the comparison if the source value is greater than or equal to the destination value.
    WisCompareOperationAlways       = 8, ///< Always pass the comparison.
} WisCompareOperation;

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
 * @brief Provided by Wisdom 0.7.0. Query type for GPU queries.
 *
 * */
typedef enum WisQueryPropertyType {
    WisQueryPropertyTypeDeviceCommandQueueProperties   = 0, ///< Properties of the device command queues. Expects a  struct.
    WisQueryPropertyTypeDeviceDescriptorHeapProperties = 1, ///< Properties of the device descriptor heap. Expects a WisDeviceDescriptorHeapProperties struct.
    WisQueryPropertyTypeDeviceMemoryProperties         = 2, ///< Properties of the device descriptor heap. Expects a WisDeviceMemoryProperties struct.
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
 * @brief Provided by Wisdom 0.7.0. Flags that describe adapter.
 *
 * */
typedef enum WisAdapterFlags {
    WisAdapterFlagsNone     = 0, ///< No flags set. Adapter @wis_may be discrete or embedded.
    WisAdapterFlagsRemote   = (1 << 0), ///< Adapter is remote. Used for remote rendering.
    WisAdapterFlagsSoftware = (1 << 1), ///< Adapter is software. Uses CPU for software rendering.
} WisAdapterFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Flags for descriptor heap creation.
 *
 * */
typedef enum WisDescriptorHeapFlags {
    WisDescriptorHeapFlagsNone                     = 0, ///< No flags set.
    WisDescriptorHeapFlagsDisallowEmbeddedSamplers = (1 << 1), ///< Heap is used in full for dynamic samplers. There @wis_must_not be any shader that use embedded samplers that uses that heap. User @wis_may allocate more samplers in the heap than it would normally be.
} WisDescriptorHeapFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Flags for sampler creation.
 *
 * */
typedef enum WisSamplerFlags {
    WisSamplerFlagsNone                     = 0, ///< No flags set.
    WisSamplerFlagsNonNormalizedCoordinates = (1 << 0), ///< Use non-normalized texture coordinates.
} WisSamplerFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Buffer usage flags.
 * Determine how the buffer can be used throughout its lifetime.
 *
 * */
typedef enum WisBufferUsageFlags {
    WisBufferUsageFlagsNone                        = 0, ///< No flags set. Buffer is not used.
    WisBufferUsageFlagsCopySrc                     = (1 << 0), ///< Buffer is used as a source for copy operations.
    WisBufferUsageFlagsCopyDst                     = (1 << 1), ///< Buffer is used as a destination for copy operations.
    WisBufferUsageFlagsConstantBuffer              = (1 << 2), ///< Buffer is used as a constant buffer.
    WisBufferUsageFlagsIndexBuffer                 = (1 << 3), ///< Buffer is used as an index buffer.
    WisBufferUsageFlagsVertexBuffer                = (1 << 4), ///< Buffer is used as a vertex buffer or an instance buffer.
    WisBufferUsageFlagsIndirectBuffer              = (1 << 5), ///< Buffer is used as an indirect buffer.
    WisBufferUsageFlagsStorageBuffer               = (1 << 6), ///< Buffer is used as a storage unordered access buffer.
    WisBufferUsageFlagsAccelerationStructureBuffer = (1 << 7), ///< Buffer is used as an acceleration structure buffer.
    WisBufferUsageFlagsAccelerationStructureInput  = (1 << 8), ///< Buffer is used as a read only acceleration instance input buffer.
    WisBufferUsageFlagsShaderBindingTable          = (1 << 9), ///< Buffer is used as a shader binding table buffer.
} WisBufferUsageFlags;

/**
 * @brief Provided by Wisdom 0.7.0. Texture usage flags.
 * Determine how the texture can be used throughout its lifetime.
 *
 * */
typedef enum WisTextureUsageFlags {
    WisTextureUsageFlagsNone            = 0, ///< No flags set. Texture is not used.
    WisTextureUsageFlagsRenderTarget    = (1 << 0), ///< Texture is used as a render target.
    WisTextureUsageFlagsDepthStencil    = (1 << 1), ///< Texture is used as a depth stencil buffer.
    WisTextureUsageFlagsCopySrc         = (1 << 2), ///< Texture is used as a source for copy operations.
    WisTextureUsageFlagsCopyDst         = (1 << 3), ///< Texture is used as a destination for copy operations.
    WisTextureUsageFlagsShaderResource  = (1 << 4), ///< Texture is used as a shader resource.
    WisTextureUsageFlagsUnorderedAccess = (1 << 5), ///< Texture is used as an unordered access resource.
    WisTextureUsageFlagsHostCopy        = (1 << 7), ///< Texture is used for host copy operations. Works with GPUUpload heap.
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
    WisMemoryFlagsDedicatedAllocation = (1 << 0),
    /**
     * @brief
     * Memory is mapped.
     * Used in combination with `WisMemoryTypeUpload` or `WisMemoryTypeReadback` to map memory for CPU access.
     * */
    WisMemoryFlagsMapped = (1 << 1),
    /**
     * @brief
     * Memory is exportable.
     * If set, memory can be exported to other processes or APIs.
     * Works only with Device Local memory (`WisMemoryTypeDefault`) and only on AllocateXMemory calls.
     * Outside of AllocateXMemory the flag is ignored.
     * */
    WisMemoryFlagsExportable = (1 << 2),
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
    WisTextureBindingFlagsDepthView   = (1 << 0), ///< Texture view is used to read depth. Used for special formats that feature depth and stencil. The bound texture @wis_must be in TODO: specific layout before being used by shader.
    WisTextureBindingFlagsStencilView = (1 << 1), ///< Texture view is used to read stencil. Used for special formats that feature depth and stencil. The bound texture @wis_must be in TODO: specific layout before being used by shader. Cannot be combined with `WisTextureBindingFlags::DepthView`.
} WisTextureBindingFlags;

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
    WisFilter           min_filter; ///< Minification filter.
    WisFilter           mag_filter; ///< Magnification filter.
    WisFilter           mip_filter; ///< Mip level filter.
    bool                is_anisotropic; ///< Anisotropic filtering enable.
    uint32_t            max_anisotropy; ///< Max anisotropy level. Min is 1, Max is 16.
    WisAddressMode      address_u; ///< Address mode for U coordinate.
    WisAddressMode      address_v; ///< Address mode for V coordinate.
    WisAddressMode      address_w; ///< Address mode for W coordinate.
    float               min_lod; ///< Min LOD value.
    float               max_lod; ///< Max LOD value.
    float               mip_lod_bias; ///< Mip LOD bias value.
    WisCompareOperation comparison_op; ///< Comparison operation for comparison samplers.
    WisStaticBorder     static_border_color; ///< Static border color. Used if any address mode is set to wis::AddressMode.
    WisSamplerFlags     flags; ///< Sampler flags. Used to set additional sampler options.
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
    uint16_t plane_slice; ///< Base depth slice of the subresource. Used only for 2D textures (YUV). Max value is 3.
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
 * @brief Provided by Wisdom 0.7.0. Push constant set description for WisCommandList.
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
 * @brief Provided by Wisdom 0.7.0. Push descriptor set description for WisCommandList.
 *
 * */
typedef struct WisPushDescriptorDataDesc {
    WisPipelineType   pipeline; ///< defines the pipeline type to set the push descriptors for.
    uint32_t          root_index; ///< defines the root index in the root signature to set the push descriptors for.
    WisDescriptorType descriptor_type; ///< defines the type of the descriptors to push.
    uint64_t          buffer_address; ///< defines buffer device address to push.
} WisPushDescriptorDataDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor set description for WisCommandList.
 *
 * */
typedef struct WisDescriptorTableDataDesc {
    WisPipelineType       pipeline; ///< defines the pipeline type to set the push descriptors for.
    uint32_t              root_index; ///< defines the root index in the root signature to set the push descriptors for.
    WisDescriptorHeapType heap_type; ///< defines the type of the descriptor heap to bind.
    uint32_t              heap_offset; ///< defines the offset in descriptors from the start of the heap to set the descriptor table to. Used for calculating descriptor indices when binding descriptor tables.
} WisDescriptorTableDataDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Constant buffer binding description for WisDescriptorHeap and WisDescriptorHeap.
 *
 * */
typedef struct WisConstantBufferBinding {
    uint64_t buffer_address; ///< defines buffer device address to bind.
    uint32_t size_bytes; ///< defines the size of the buffer region to bind in bytes.
} WisConstantBufferBinding;

/**
 * @brief Provided by Wisdom 0.7.0. Buffer binding description for WisDescriptorHeap.
 *
 * */
typedef struct WisBufferBinding {
    uint64_t array_offset; ///< defines offset in buffer in structures.
    uint32_t stride_bytes; ///< defines the size of the single structure in buffer.
    uint32_t structure_count; ///< defines the number of structures in the buffer region to bind.
} WisBufferBinding;

/**
 * @brief Provided by Wisdom 0.7.0. Texture binding description for WisDescriptorHeap and WisDescriptorHeap.
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
 * @brief Provided by Wisdom 0.7.0. Query struct header. Used as a header for all query structs.
 *
 * */
typedef struct WisQueryStructHeader {
    WisQueryPropertyType property_type; ///< Defines the type of the queried property. Used to determine what struct is passed.
    void*                next_in_chain; ///< Pointer to the next queried data struct.
} WisQueryStructHeader;

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
typedef struct WisDeviceCommandQueuesProperties {
    WisQueryPropertyType    property_type; ///< Defines the type of the queried property. @wis_must be wis::QueryPropertyType..
    void*                   next_in_chain; ///< Pointer to the next queried data struct.
    bool                    supported_queues[5]; ///< Array of supported queue types. If a queue type is supported, the value is `1`, otherwise `0`. Order of queue types is the same as in wis::CommandQueueType enum.
    WisCommandQueuePriority max_queue_priority[5]; ///< Array of maximum supported priorities for each queue type. If a queue type is not supported, the value is `0`. Order of queue types is the same as in wis::CommandQueueType enum.
} WisDeviceCommandQueuesProperties;

/**
 * @brief Provided by Wisdom 0.7.0. Device memory properties. Used to query memory type support and limits.
 *
 * */
typedef struct WisDeviceMemoryProperties {
    WisQueryPropertyType property_type; ///< Defines the type of the queried property. @wis_must be `WisQueryPropertyTypeDeviceMemoryProperties`..
    void*                next_in_chain; ///< Pointer to the next queried data struct.
    bool                 gpu_upload_supported; ///< Indicates if GPU upload memory type is supported. This memory type allows mapping the memory and writing to it from CPU, while being accessible from GPU. It is usually implemented as write-combined memory on integrated GPUs and as a part of shared system memory on discrete GPUs.
    bool                 host_image_copy_supported; ///< Indicates if host image copy is supported. This feature allows copying data directly from CPU memory to optimal tiled image layout on GPU, without the need for an intermediate staging buffer. It is supported on Windows 10 22H2 and later with WDDM 3.0 or later. On Vulkan it requires `VK_EXT_host_image_copy` extension.
} WisDeviceMemoryProperties;

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_API_H
