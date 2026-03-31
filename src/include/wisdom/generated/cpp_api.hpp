// This file is generated. Do not edit directly.
#ifndef WISDOM_CORE_CPP_API_HPP
#define WISDOM_CORE_CPP_API_HPP
#ifndef __cplusplus
#    error C++ is required to include this header.
#endif // __cplusplus

#include <wisdom/bridge/span.hpp>
#include <wisdom/global/definitions.h>
#include <wisdom/global/internal.hpp>

#include "c_api.h"

namespace wis {

//==============================================================
// Enums
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Defines common return status codes. Compare against `wis::Status::Ok` for success.
 *
 * */
enum class Status
{
    Ok = 0,                 ///< Operation succeeded.
    Timeout = 1,            ///< Operation timed out.
    Partial = 2,            ///< Operation partially succeeded.
    InvalidArgument = -1,   ///< One or more arguments, or parts of arguments passed to the function were incorrect.
    OutOfHostMemory = -2,   ///< There is no more host memory available.
    OutOfDeviceMemory = -3, ///< There is no more device memory available.
    DeviceLost = -4,        ///< Device driver was forcefully stopped.
    Occluded = -5,          ///< Swap chain presentation was not visible to the user. Rendering is too fast.
    ValidationFailed = -6,  ///< A validation layer found an error.
    Fail = -7,              ///< Operation expectedly failed.
    Error = -10000,         ///< Operation failed.
};

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
enum class DataFormat
{
    Unknown = 0, ///< Unknown format.
                 /**
                  * @brief
                  * 32 bit per channel RGBA format.
                  * A four-component, 128-bit signed floating-point format that has
                  * a 32-bit R component in bytes 0..3,
                  * a 32-bit G component in bytes 4..7,
                  * a 32-bit B component in bytes 8..11,
                  * a 32-bit A component in bytes 12..15.
                  * */
    RGBA32Float = 2,
    /**
     * @brief
     * 32 bit per channel RGBA format.
     * A four-component, 128-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    RGBA32Uint = 3,
    /**
     * @brief
     * 32 bit per channel RGBA format.
     * A four-component, 128-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    RGBA32Sint = 4,
    /**
     * @brief
     * 32 bit per channel RGB format.
     * A three-component, 96-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    RGB32Float = 6,
    /**
     * @brief
     * 32 bit per channel RGB format.
     * A three-component, 96-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    RGB32Uint = 7,
    /**
     * @brief
     * 32 bit per channel RGB format.
     * A three-component, 96-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    RGB32Sint = 8,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 64-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    RGBA16Float = 10,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 64-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    RGBA16Unorm = 11,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 64-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    RGBA16Uint = 12,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 64-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    RGBA16Snorm = 13,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 64-bit signed integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    RGBA16Sint = 14,
    /**
     * @brief
     * 32 bit per channel RG format.
     * A two-component, 64-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    RG32Float = 16,
    /**
     * @brief
     * 32 bit per channel RG format.
     * A two-component, 64-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    RG32Uint = 17,
    /**
     * @brief
     * 32 bit per channel RG format.
     * A two-component, 64-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    RG32Sint = 18,
    /**
     * @brief
     * 32 bit depth, 8 bit stencil format.
     * A combined depth/stencil format with a 32-bit depth component in bytes 0..3
     * and an 8-bit stencil component in bytes 4..4.
     * */
    D32FloatS8Uint = 20,
    /**
     * @brief
     * 10 bit per channel RGB format with 2 bit alpha.
     * A four-component, 32-bit unsigned normalized format that has
     * a 10-bit R component in bits 0..9,
     * a 10-bit G component in bits 10..19,
     * a 10-bit B component in bits 20..29,
     * a 2-bit A component in bits 30..31.
     * */
    RGB10A2Unorm = 24,
    /**
     * @brief
     * 10 bit per channel RGB format with 2 bit alpha.
     * A four-component, 32-bit unsigned integer format that has
     * a 10-bit R component in bits 0..9,
     * a 10-bit G component in bits 10..19,
     * a 10-bit B component in bits 20..29,
     * a 2-bit A component in bits 30..31.
     * */
    RGB10A2Uint = 25,
    /**
     * @brief
     * 11 bit per channel RG format with 10 bit blue.
     * A three-component, 32-bit signed floating-point format that has
     * a 11-bit R component in bits 0..10,
     * a 11-bit G component in bits 11..21,
     * a 10-bit B component in bits 22..31.
     * */
    RG11B10Float = 26,
    /**
     * @brief
     * 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    RGBA8Unorm = 28,
    /**
     * @brief
     * 8 bit per channel RGBA format. SRGB non-linear color space.
     * A four-component, 32-bit unsigned normalized non-linear SRGB format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    RGBA8UnormSrgb = 29,
    /**
     * @brief
     * 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    RGBA8Uint = 30,
    /**
     * @brief
     * 8 bit per channel RGBA format.
     * A four-component, 32-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    RGBA8Snorm = 31,
    /**
     * @brief
     * 8 bit per channel RGBA format.
     * A four-component, 32-bit signed integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    RGBA8Sint = 32,
    /**
     * @brief
     * 16 bit per channel RG format.
     * A two-component, 32-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    RG16Float = 34,
    /**
     * @brief
     * 16 bit per channel RG format.
     * A two-component, 32-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    RG16Unorm = 35,
    /**
     * @brief
     * 16 bit per channel RG format.
     * A two-component, 32-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    RG16Uint = 36,
    /**
     * @brief
     * 16 bit per channel RG format.
     * A two-component, 32-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    RG16Snorm = 37,
    /**
     * @brief
     * 16 bit per channel RG format.
     * A two-component, 32-bit signed integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    RG16Sint = 38,
    /**
     * @brief
     * 32 bit depth format.
     * A 32-bit depth format supporting 32-bit floating-point depth values.
     * */
    D32Float = 40,
    /**
     * @brief
     * 32 bit per channel R format.
     * A one-component, 32-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3.
     * */
    R32Float = 41,
    /**
     * @brief
     * 32 bit per channel R format.
     * A one-component, 32-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3.
     * */
    R32Uint = 42,
    /**
     * @brief
     * 32 bit per channel R format.
     * A one-component, 32-bit signed integer format that has
     * a 32-bit R component in bytes 0..3.
     * */
    R32Sint = 43,
    /**
     * @brief
     * 24 bit depth, 8 bit stencil format.
     * A combined depth/stencil format with a 24-bit depth component in bytes 0..2
     * and an 8-bit stencil component in bytes 3..3.
     * */
    D24UnormS8Uint = 45,
    /**
     * @brief
     * 8 bit per channel RG format.
     * A two-component, 16-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    RG8Unorm = 49,
    /**
     * @brief
     * 8 bit per channel RG format.
     * A two-component, 16-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    RG8Uint = 50,
    /**
     * @brief
     * 8 bit per channel RG format.
     * A two-component, 16-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    RG8Snorm = 51,
    /**
     * @brief
     * 8 bit per channel RG format.
     * A two-component, 16-bit signed integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    RG8Sint = 52,
    /**
     * @brief
     * 16 bit per channel R format.
     * A one-component, 16-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1.
     * */
    R16Float = 54,
    /**
     * @brief
     * 16 bit depth format.
     * A 16-bit depth format supporting 16-bit unsigned normalized depth values.
     * */
    D16Unorm = 55,
    /**
     * @brief
     * 16 bit per channel R format.
     * A one-component, 16-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1.
     * */
    R16Unorm = 56,
    /**
     * @brief
     * 16 bit per channel R format.
     * A one-component, 16-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1.
     * */
    R16Uint = 57,
    /**
     * @brief
     * 16 bit per channel R format.
     * A one-component, 16-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1.
     * */
    R16Snorm = 58,
    /**
     * @brief
     * 16 bit per channel R format.
     * A one-component, 16-bit signed integer format that has
     * a 16-bit R component in bytes 0..1.
     * */
    R16Sint = 59,
    /**
     * @brief
     * 8 bit per channel R format.
     * A one-component, 8-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0.
     * */
    R8Unorm = 61,
    /**
     * @brief
     * 8 bit per channel R format.
     * A one-component, 8-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0.
     * */
    R8Uint = 62,
    /**
     * @brief
     * 8 bit per channel R format.
     * A one-component, 8-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0.
     * */
    R8Snorm = 63,
    /**
     * @brief
     * 8 bit per channel R format.
     * A one-component, 8-bit signed integer format that has
     * an 8-bit R component in bytes 0..0.
     * */
    R8Sint = 64,
    /**
     * @brief
     * 9 bit per channel RGB format with shared exponent.
     * A three-component, 32-bit unsigned floating-point format that has
     * a 9-bit R component in bits 0..8,
     * a 9-bit G component in bits 9..17,
     * a 9-bit B component in bits 18..26,
     * a shared 5-bit exponent in bits 27..31.
     * */
    RGB9E5UFloat = 67,
    /**
     * @brief
     * BC1/DXT1 compressed format.
     * A four-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGB texel data, and provides 1 bit of alpha.
     * */
    BC1RGBAUnorm = 71,
    /**
     * @brief
     * BC1/DXT1 compressed format.
     * A four-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGB texel data with sRGB nonlinear encoding, and provides 1 bit of alpha.
     * */
    BC1RGBAUnormSrgb = 72,
    /**
     * @brief
     * BC2/DXT3 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    BC2RGBAUnorm = 74,
    /**
     * @brief
     * BC2/DXT3 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    BC2RGBAUnormSrgb = 75,
    /**
     * @brief
     * BC3/DXT5 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values
     * followed by 64 bits encoding RGB values
     * */
    BC3RGBAUnorm = 77,
    /**
     * @brief
     * BC3/DXT5 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    BC3RGBAUnormSrgb = 78,
    /**
     * @brief
     * BC4 compressed format.
     * A single-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized red texel data
     * */
    BC4RUnorm = 80,
    /**
     * @brief
     * BC4 compressed format.
     * A single-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed normalized red texel data
     * */
    BC4RSnorm = 81,
    /**
     * @brief
     * BC5 compressed format.
     * A two-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized red and green texel data
     * */
    BC5RGUnorm = 83,
    /**
     * @brief
     * BC5 compressed format.
     * A two-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed normalized red and green texel data
     * */
    BC5RGSnorm = 84,
    /**
     * @brief
     * 16 bit per channel RGB format.
     * A three-component, 16-bit unsigned normalized format that has
     * a 5-bit B component in bits 0..4,
     * a 6-bit G component in bits 5..10,
     * a 5-bit R component in bits 11..15.
     * */
    B5G6R5Unorm = 85,
    /**
     * @brief
     * 16 bit per channel RGBA format.
     * A four-component, 16-bit unsigned normalized format that has
     * a 5-bit B component in bits 0..4,
     * a 5-bit G component in bits 5..9,
     * a 5-bit R component in bits 10..14,
     * a 1-bit A component in bits 15..15.
     * */
    B5G5R5A1Unorm = 86,
    /**
     * @brief
     * 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned normalized format that has
     * an 8-bit B component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit R component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    BGRA8Unorm = 87,
    /**
     * @brief
     * 8 bit per channel RGBA format. SRGB non-linear color space.
     * A four-component, 32-bit unsigned normalized non-linear SRGB format that has
     * an 8-bit B component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit R component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    BGRA8UnormSrgb = 91,
    /**
     * @brief
     * BC6H compressed format.
     * A three-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned floating-point RGB texel data
     * */
    BC6HUfloat16 = 95,
    /**
     * @brief
     * BC6H compressed format.
     * A three-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed floating-point RGB texel data
     * */
    BC6HSfloat16 = 96,
    /**
     * @brief
     * BC7 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data
     * */
    BC7RGBAUnorm = 98,
    /**
     * @brief
     * BC7 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * */
    BC7RGBAUnormSrgb = 99,
    /**
     * @brief
     * 4 bit per channel RGB format.
     * A four-component, 16-bit unsigned normalized format that has
     * a 4-bit B component in bits 0..3,
     * a 4-bit G component in bits 4..7,
     * a 4-bit R component in bits 8..11,
     * a 4-bit A component in bits 12..15.
     * */
    BGRA4Unorm = 115,
};

/**
 * @brief Provided by Wisdom 0.7.0. Sample rate for multisampling.
 *
 * */
enum class SampleCount
{
    S1 = 1,   ///< 1 sample per pixel.
    S2 = 2,   ///< 2 samples per pixel.
    S4 = 4,   ///< 4 samples per pixel.
    S8 = 8,   ///< 8 samples per pixel.
    S16 = 16, ///< 16 samples per pixel.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture layout. Determines how texture will be accessed.
 *
 * */
enum class TextureLayout
{
    Texture1D = 2,         ///< Texture is 1D array of data. Behaves similarly to Buffer.
    Texture1DArray = 3,    ///< Texture is an array of 1D data.
    Texture2D = 4,         ///< Texture is 2D image, default texture type.
    Texture2DArray = 5,    ///< Texture is an array of 2D images. Can also be used for Cube maps.
    Texture2DMS = 6,       ///< Texture is 2D multisampled image.
    Texture2DMSArray = 7,  ///< Texture is an array of 2D multisampled images.
    Texture3D = 8,         ///< Texture is 3D volume.
    TextureCube = 9,       ///< Texture is a cube map. Behaves similarly to Texture2DArray with 6 layers.
    TextureCubeArray = 10, ///< Texture is an array of cube maps. Behaves similarly to Texture2DArray with 6 layers per
                           ///< cube map.
};

/**
 * @brief Provided by Wisdom 0.7.0. Orders the adapters according to preference using builtin heuristics of underlying
 * APIs. For DirectX 12, this translates directly to `DXGI_GPU_PREFERENCE`. For Vulkan, sorting is based on
 * `VkPhysicalDeviceType` heuristics.
 *
 * */
enum class AdapterPreference
{
    None = 0,           ///< No particular preference, list adapters in system devised order.
    MinConsumption = 1, ///< List the adapters from low power consumption to high. DirectX 12: Integrated, Discrete,
                        ///< External, Software. Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU.
    Performance = 2,    ///< List the adapters from high performance to low. DirectX 12: External, Discrete, Integrated,
                        ///< Software. Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
};

/**
 * @brief Provided by Wisdom 0.7.0. Defines severity levels for logging and debugging messages.
 *
 * */
enum class Severity
{
    Verbose = 0, ///< Verbose level messages, typically used for detailed debugging information.
    Info = 1,    ///< Informational messages that highlight the progress of the application.
    Warning = 2, ///< Potentially harmful situations that warrant attention but do not prevent normal operation.
    Error = 3,   ///< Error events that might still allow the application to continue running.
    Fatal = 4,   ///< Severe error events that will presumably lead the application to abort.
};

/**
 * @brief Provided by Wisdom 0.7.0. Defines types of command queues that can be created on the device. Each type
 * corresponds to a specific kind of workload that the GPU can handle.
 *
 * */
enum class CommandQueueType
{
    Graphics = 0, ///< Command queue for graphics operations, including rendering and compute tasks.
    Compute = 1,  ///< Command queue dedicated to compute operations, optimized for parallel processing tasks.
    Transfer = 2, ///< Command queue for data transfer operations, such as copying resources between buffers and images.
    VideoDecode = 3, ///< Command queue for video decoding operations.
    VideoEncode = 4, ///< Command queue for video encoding operations.
    Count = 5,       ///< Number of command queue types available.
};

/**
 * @brief Provided by Wisdom 0.7.0. Global queue priority. Higher priority queues get more GPU time, but @wis_may cause
 * performance issues if overused.
 *
 * */
enum class CommandQueuePriority
{
    Normal = 0,   ///< Normal queue priority.
    High = 1,     ///< High queue priority.
    Realtime = 2, ///< Global realtime queue priority. Requires special GPU support and @wis_may cause performance
                  ///< issues if used on unsupported hardware.
};

/**
 * @brief Provided by Wisdom 0.7.0. Defines types of pipelines that can be created on the device. Each type corresponds
 * to a specific kind of workload that the GPU can handle.
 *
 * */
enum class PipelineType
{
    Graphics = 0,   ///< Pipeline for graphics operations, including rendering tasks.
    Compute = 1,    ///< Pipeline dedicated to compute operations, optimized for parallel processing tasks.
    RayTracing = 2, ///< Pipeline for ray tracing operations, optimized for ray tracing workloads.
};

/**
 * @brief Provided by Wisdom 0.7.0. Shader stages that can be used in the pipeline. Main use is Root signature and
 * descriptor management. Stages have no granularity, either all or one can be selected.
 *
 * */
enum class ShaderVisibility
{
    All = 0,           ///< All shader stages.
    Vertex = 1,        ///< Vertex shader stage.
    Hull = 2,          ///< Hull/Tessellation control shader stage.
    Domain = 3,        ///< Domain/Tessellation evaluation shader stage.
    Geometry = 4,      ///< Geometry shader stage.
    Pixel = 5,         ///< Pixel/Fragment shader stage.
    Amplification = 6, ///< Amplification shader stage.
    Mesh = 7,          ///< Mesh shader stage.
    Count = 8,         ///< Number of stages.
};

/**
 * @brief Provided by Wisdom 0.7.0. Type of the descriptor in the descriptor table.
 *
 * */
enum class DescriptorType
{
    Sampler = 0,               ///< Descriptor is a sampler.
    ConstantBuffer = 1,        ///< Descriptor is a constant buffer.
    Texture = 2,               ///< Descriptor is a texture.
    RWTexture = 3,             ///< Descriptor is an unordered access read-write texture.
    RWBuffer = 4,              ///< Descriptor is an unordered access read-write buffer.
    Buffer = 5,                ///< Descriptor is a shader resource buffer.
    AccelerationStructure = 6, ///< Descriptor is an acceleration structure.
    Count = 7,                 ///< Descriptor is a mutable type.
};

/**
 * @brief Provided by Wisdom 0.7.0. Comparison function for depth and stencil operations.
 *
 * */
enum class CompareOp
{
    None = 0,         ///< No comparison.
    Never = 1,        ///< Always fail the comparison.
    Less = 2,         ///< Pass the comparison if the source value is less than the destination value.
    Equal = 3,        ///< Pass the comparison if the source value is equal to the destination value.
    LessEqual = 4,    ///< Pass the comparison if the source value is less than or equal to the destination value.
    Greater = 5,      ///< Pass the comparison if the source value is greater than the destination value.
    NotEqual = 6,     ///< Pass the comparison if the source value is not equal to the destination value.
    GreaterEqual = 7, ///< Pass the comparison if the source value is greater than or equal to the destination value.
    Always = 8,       ///< Always pass the comparison.
};

/**
 * @brief Provided by Wisdom 0.7.0. Address mode for texture sampling.
 *
 * */
enum class AddressMode
{
    Repeat = 0,            ///< Repeat the texture.
    MirroredRepeat = 1,    ///< Repeat the texture with mirroring.
    ClampToEdge = 2,       ///< Clamp the texture to the edge.
    ClampToBorder = 3,     ///< Clamp the texture to the border.
    MirrorClampToEdge = 4, ///< Mirror and clamp the texture to the edge.
};

/**
 * @brief Provided by Wisdom 0.7.0. Filtering mode for texture sampling.
 *
 * */
enum class Filter
{
    Point = 0,  ///< Nearest neighbor filtering.
    Linear = 1, ///< Linear filtering.
};

/**
 * @brief Provided by Wisdom 0.7.0. Reduction mode for texture sampling when min and mag filters are different.
 *
 * */
enum class ReductionMode
{
    Standard = 0, ///< Standard filtering behavior.
    Min = 1,      ///< Use the minimum value of the samples for filtering.
    Max = 2,      ///< Use the maximum value of the samples for filtering.
};

/**
 * @brief Provided by Wisdom 0.7.0. Predefined static border colors for samplers. Used when address mode is set to
 * Border.
 *
 * */
enum class StaticBorder
{
    TransparentBlack = 0, ///< Transparent black border color.
    OpaqueBlack = 1,      ///< Opaque black border color.
    OpaqueWhite = 2,      ///< Opaque white border color.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor heap type.
 *
 * */
enum class DescriptorHeapType
{
    Descriptor = 0, ///< Descriptor heap type. Used for all descriptor types, except for samplers.
    Sampler = 1,    ///< Sampler heap type. Used for sampler descriptors.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor memory type. Decides if descriptors are visible and can be bound to GPU.
 *
 * */
enum class DescriptorMemoryType
{
    CpuOnly = 0, ///< Descriptors are only visible to CPU. May be used for copying descriptors to the GPU visible pool.
    ShaderVisible = 1, ///< Descriptors are visible to GPU. Descriptors can be bound to the GPU pipeline directly, but
                       ///< can't be copied from.
};

/**
 * @brief Provided by Wisdom 0.7.0. Component swizzle for texture sampling.
 *
 * */
enum class ComponentSwizzle
{
    Identity = 0, ///< Use the component as is for sampling.
    Red = 1,      ///< Use the red component for sampling.
    Green = 2,    ///< Use the green component for sampling.
    Blue = 3,     ///< Use the blue component for sampling.
    Alpha = 4,    ///< Use the alpha component for sampling.
    Zero = 5,     ///< Use zero for sampling.
    One = 6,      ///< Use one for sampling.
};

/**
 * @brief Provided by Wisdom 0.7.0. Determines the behavior when waiting for multiple fences.
 *
 * */
enum class MultiWaitType
{
    All = 0, ///< All the fences in the batch are triggered.
    Any = 1, ///< At least one of the fences from the batch is triggered.
};

/**
 * @brief Provided by Wisdom 0.7.0. Input classification for vertex buffer data.
 *
 * */
enum class InputClass
{
    PerVertex = 0,   ///< Vertex buffer data is vertex data.
    PerInstance = 1, ///< Vertex buffer data is per instance data.
};

/**
 * @brief Provided by Wisdom 0.7.0. Query type for GPU queries.
 *
 * */
enum class QueryPropertyType
{
    DeviceCommandQueueProperties = 0,   ///< Properties of the device command queues. Expects a
                                        ///< wis::DeviceCommandQueueProperties struct.
    DeviceDescriptorHeapProperties = 1, ///< Properties of the device descriptor heap. Expects a
                                        ///< wis::DeviceDescriptorHeapProperties struct.
    DeviceMemoryProperties = 2,  ///< Properties of the device descriptor heap. Expects a wis::DeviceMemoryProperties
                                 ///< struct.
    DeviceBindingProperties = 3, ///< Properties of the device resource binding. Expects a wis::DeviceBindingProperties
                                 ///< struct.
};

/**
 * @brief Provided by Wisdom 0.7.0. Memory type for resource allocation.
 *
 * */
enum class MemoryType
{
    Default = 0, ///< Default memory type. Alias for `wis::MemoryType::DeviceLocal`
                 /**
                  * @brief
                  * Default memory type.
                  * Local device memory, most efficient for rendering.
                  * */
    DeviceLocal = 0,
    /**
     * @brief
     * Upload memory type.
     * Used for data that is uploaded to the GPU Local memory using copy operations.
     * */
    Upload = 1,
    /**
     * @brief
     * Readback memory type.
     * Used for data that is read back from the GPU Local memory using copy operations.
     * */
    Readback = 2,
    /**
     * @brief
     * GPU upload memory type.
     * Used for data that is directly uploaded to the GPU Local memory using copy operations.
     * Support of this memory @wis_must be queried.
     * */
    GPUUpload = 3,
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture state for resource transitions.
 *
 * */
enum class TextureState
{
    Undefined = -1,            ///< Undefined state.
    Common = 0,                ///< Common state.
    Read = 1,                  ///< General Read state.
    RenderTarget = 2,          ///< Render Target state.
    UnorderedAccess = 3,       ///< Unordered Access state.
    DepthStencilWrite = 4,     ///< Depth Stencil Write state.
    DepthStencilRead = 5,      ///< Depth Stencil Read state.
    DepthWriteStencilRead = 6, ///< Depth Write Stencil Read state.
    StencilWriteDepthRead = 7, ///< Stencil Write Depth Read state.
    ShaderResource = 8,        ///< Shader Resource state.
    CopySrc = 9,               ///< Copy Source state.
    CopyDst = 10,              ///< Copy Destination state.
    Present = 11,              ///< Present swapchain state.
    ShadingRate = 12,          ///< Shading Rate state. Used for Variable Shading Rate.
    VideoDecodeRead = 13,      ///< Video Decode Read state.
    VideoDecodeWrite = 14,     ///< Video Decode Write state.
};

/**
 * @brief Provided by Wisdom 0.7.0. Intermediate shader representation.
 *
 * */
enum class ShaderIntermediate
{
    DXIL = 0,  ///< DirectX Intermediate Language.
    SPIRV = 1, ///< Standard Portable Intermediate Representation for Vulkan.
};

/**
 * @brief Provided by Wisdom 0.7.0. Stencil operation for depth and stencil operations.
 *
 * */
enum class StencilOp
{
    Keep = 1,     ///< Keep the current value.
    Zero = 2,     ///< Set the value to zero.
    Replace = 3,  ///< Replace the value with the reference value.
    IncClamp = 4, ///< Increment the value and clamp to the maximum value.
    DecClamp = 5, ///< Decrement the value and clamp to the minimum value.
    Invert = 6,   ///< Invert the value.
    IncWrap = 7,  ///< Increment the value and wrap to zero when the maximum value is exceeded.
    DecWrap = 8,  ///< Decrement the value and wrap to the maximum value when the minimum value is exceeded.
};

/**
 * @brief Provided by Wisdom 0.7.0. Primitive topology type for wis::GraphicsPipelineDesc.
 *
 * */
enum class TopologyType
{
    Point = 1,    ///< Render points for each vertex.
    Line = 2,     ///< Render lines between vertices.
    Triangle = 3, ///< Render triangles between vertices.
    Patch = 4,    ///< Vertices are interpret as patch list. Used in tesselation process.
};

/**
 * @brief Provided by Wisdom 0.7.0. Fill mode for rasterizer.
 *
 * */
enum class FillMode
{
    Lines = 2, ///< Draw lines between vertices. Wireframe rendering.
    Solid = 3, ///< Fill the area between vertices forming polygons.
};

/**
 * @brief Provided by Wisdom 0.7.0. Cull mode for rasterizer.
 * Triangle culling depends on wis::WindingOrder option.
 *
 * */
enum class CullMode
{
    None = 1,  ///< No culling.
    Front = 2, ///< Cull front-facing triangles.
    Back = 3,  ///< Cull back-facing triangles.
};

/**
 * @brief Provided by Wisdom 0.7.0. Winding order for front-facing triangles.
 *
 * */
enum class WindingOrder
{
    Clockwise = 0,        ///< Front-facing triangles have clockwise winding order.
    CounterClockwise = 1, ///< Front-facing triangles have counter-clockwise winding order.
};

/**
 * @brief Provided by Wisdom 0.7.0. Conservative rasterization mode.
 *
 * */
enum class ConservativeRasterization
{
    Off = 0,          ///< Conservative rasterization is disabled.
    Overestimate = 1, ///< Conservative rasterization is enabled.
};

/**
 * @brief Provided by Wisdom 0.7.0. Line rasterization mode.
 *
 * */
enum class LineRasterization
{
    Default = 0,          ///< Line rasterization is disabled. Lines are not rendered.
    Rectangular = 1,      ///< Lines are rasterized as rectangles.
    AlphaAntialiased = 2, ///< Lines are rasterized as rectangles with anti-aliasing.
};

/**
 * @brief Provided by Wisdom 0.7.0. Blend factor for color blending operations.
 *
 * */
enum class BlendFactor
{
    Zero = 1,            ///< Use zero for blending.
    One = 2,             ///< Use one for blending.
    SrcColor = 3,        ///< Use the source color for blending.
    InvSrcColor = 4,     ///< Use the inverse source color for blending.
    SrcAlpha = 5,        ///< Use the source alpha for blending.
    InvSrcAlpha = 6,     ///< Use the inverse source alpha for blending.
    DestAlpha = 7,       ///< Use the destination alpha for blending.
    InvDestAlpha = 8,    ///< Use the inverse destination alpha for blending.
    DestColor = 9,       ///< Use the destination color for blending.
    InvDestColor = 10,   ///< Use the inverse destination color for blending.
    SrcAlphaSat = 11,    ///< Use the source alpha saturated for blending.
    ConstantColor = 14,  ///< Use a constant blend factor for blending.
    InvBlendFactor = 15, ///< Use the inverse constant blend factor for blending.
    Src1Color = 16,      ///< Use the source color for blending. Dual source blending mode.
    InvSrc1Color = 17,   ///< Use the inverse source color for blending. Dual source blending mode.
    Src1Alpha = 18,      ///< Use the source alpha for blending. Dual source blending mode.
    InvSrc1Alpha = 19,   ///< Use the inverse source alpha for blending. Dual source blending mode.
};

/**
 * @brief Provided by Wisdom 0.7.0. Blend operation for color blending operations.
 *
 * */
enum class BlendOp
{
    Add = 1,         ///< Add the source and destination colors.
    Subtract = 2,    ///< Subtract the source color from the destination color.
    RevSubtract = 3, ///< Subtract the destination color from the source color.
    Min = 4,         ///< Use the minimum of the source and destination colors.
    Max = 5,         ///< Use the maximum of the source and destination colors.
};

/**
 * @brief Provided by Wisdom 0.7.0. Logic operation for color blending operations.
 *
 * */
enum class LogicOp
{
    Clear = 0,        ///< Clear the destination value.
    Set = 1,          ///< Set the destination value.
    Copy = 2,         ///< Copy the source value to the destination.
    CopyInverted = 3, ///< Copy the inverted source value to the destination.
    Noop = 4,         ///< Do not modify the destination value.
    Invert = 5,       ///< Invert the destination value.
    And = 6,          ///< Perform a bitwise AND operation on the source and destination values.
    Nand = 7,         ///< Perform a bitwise NAND operation on the source and destination values.
    Or = 8,           ///< Perform a bitwise OR operation on the source and destination values.
    Nor = 9,          ///< Perform a bitwise NOR operation on the source and destination values.
    Xor = 10,         ///< Perform a bitwise XOR operation on the source and destination values.
    Equiv = 11,       ///< Perform a bitwise equivalent operation on the source and destination values.
    AndReverse = 12,  ///< Perform a bitwise AND operation on the source and inverted destination values.
    AndInverted = 13, ///< Perform a bitwise AND operation on the inverted source and destination values.
    OrReverse = 14,   ///< Perform a bitwise OR operation on the source and inverted destination values.
    OrInverted = 15,  ///< Perform a bitwise OR operation on the inverted source and destination values.
};

/**
 * @brief Provided by Wisdom 0.7.0. Primitive topology for rendering.
 * More info could be found
 * [here](https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies).
 *
 * */
enum class PrimitiveTopology
{
    PointList = 1,         ///< Render points for each vertex.
    LineList = 2,          ///< Render lines between vertices.
    LineStrip = 3,         ///< Render lines between vertices in a strip.
    TriangleList = 4,      ///< Render triangles between vertices.
    TriangleStrip = 5,     ///< Render triangles between vertices in a strip.
    TriangleFan = 6,       ///< Interpret vertex data to form a fan of triangles.
    LineListAdj = 10,      ///< Render lines between vertices with adjacency.
    LineStripAdj = 11,     ///< Render lines between vertices in a strip with adjacency.
    TriangleListAdj = 12,  ///< Render triangles between vertices with adjacency.
    TriangleStripAdj = 13, ///< Render triangles between vertices in a strip with adjacency.
};

/**
 * @brief Provided by Wisdom 0.7.0. Primitive restart value for indexed drawing with primitive restart enabled.
 *
 * */
enum class PrimitiveRestartValue
{
    None = 0,          ///< Primitive restart is disabled. No primitive restart value is used.
    UInt16Max = 65535, ///< Use the maximum value of uint16_t as the primitive restart value.
    UInt32Max = -1,    ///< Use the maximum value of uint32_t as the primitive restart value.
};

/**
 * @brief Provided by Wisdom 0.7.0. Type of the view descriptor heap.
 *
 * */
enum class ViewHeapType
{
    RenderTarget = 0, ///< Descriptor heap for render target views.
    DepthStencil = 1, ///< Descriptor heap for depth stencil views.
};

/**
 * @brief Provided by Wisdom 0.7.0. Swapchain scaling mode.
 *
 * */
enum class SwapchainScaling
{
    None = 0,    ///< No scaling. The swapchain size is equal to the window size.
    Stretch = 1, ///< Stretch scaling. The swapchain size is stretched to the window size.
    Aspect = 2,  ///< Aspect scaling. The swapchain size is scaled to the window size with aspect ratio preserved.
};

/**
 * @brief Provided by Wisdom 0.7.0. Composite alpha flags for swapchain creation.
 *
 * */
enum class CompositeAlpha
{
    Opaque = 0,         ///< The alpha channel, if it exists, is ignored. The image is treated as opaque.
    PreMultiplied = 1,  ///< The alpha channel, if it exists, is respected and used in compositing. The premultiplied
                        ///< alpha format is expected.
    PostMultiplied = 2, ///< The alpha channel, if it exists, is respected and used in compositing. The postmultiplied
                        ///< alpha format is expected.
    Inherit = 3, ///< The alpha channel, if it exists, is respected and used in compositing based on the platform's
                 ///< default behavior.
};

/**
 * @brief Provided by Wisdom 0.7.0. Attachment load operation for render pass.
 *
 * */
enum class LoadOp
{
    Load = 0,     ///< Load the attachment contents.
    Clear = 1,    ///< Clear the attachment contents.
    DontCare = 2, ///< Do not care about the attachment contents.
};

/**
 * @brief Provided by Wisdom 0.7.0. Attachment store operation for render pass.
 *
 * */
enum class StoreOp
{
    Store = 0,    ///< Store the attachment contents.
    DontCare = 1, ///< Do not care about the attachment contents.
};

/**
 * @brief Provided by Wisdom 0.7.0. Flags that describe adapter.
 *
 * */
enum class AdapterFlags : uint32_t
{
    None = 0,             ///< No flags set. Adapter @wis_may be discrete or embedded.
    Remote = (1u << 0),   ///< Adapter is remote. Used for remote rendering.
    Software = (1u << 1), ///< Adapter is software. Uses CPU for software rendering.
};
WISDOM_DEFINE_ENUM_OPERATORS(AdapterFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Flags for descriptor heap creation.
 *
 * */
enum class DescriptorHeapFlags : uint32_t
{
    None = 0,                             ///< No flags set.
    DisallowEmbeddedSamplers = (1u << 1), ///< Heap is used in full for dynamic samplers. There @wis_must_not be any
                                          ///< shader that use embedded samplers that uses that heap. User @wis_may
                                          ///< allocate more samplers in the heap than it would normally be.
};
WISDOM_DEFINE_ENUM_OPERATORS(DescriptorHeapFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Flags for sampler creation.
 *
 * */
enum class SamplerFlags : uint32_t
{
    None = 0,                             ///< No flags set.
    NonNormalizedCoordinates = (1u << 0), ///< Use non-normalized texture coordinates.
};
WISDOM_DEFINE_ENUM_OPERATORS(SamplerFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Buffer usage flags.
 * Determine how the buffer can be used throughout its lifetime.
 *
 * */
enum class BufferUsageFlags : uint32_t
{
    None = 0,                                ///< No flags set. Buffer is not used.
    CopySrc = (1u << 0),                     ///< Buffer is used as a source for copy operations.
    CopyDst = (1u << 1),                     ///< Buffer is used as a destination for copy operations.
    ConstantBuffer = (1u << 2),              ///< Buffer is used as a constant buffer.
    IndexBuffer = (1u << 3),                 ///< Buffer is used as an index buffer.
    VertexBuffer = (1u << 4),                ///< Buffer is used as a vertex buffer or an instance buffer.
    IndirectBuffer = (1u << 5),              ///< Buffer is used as an indirect buffer.
    StorageBuffer = (1u << 6),               ///< Buffer is used as a storage unordered access buffer.
    AccelerationStructureBuffer = (1u << 7), ///< Buffer is used as an acceleration structure buffer.
    AccelerationStructureInput = (1u << 8),  ///< Buffer is used as a read only acceleration instance input buffer.
    ShaderBindingTable = (1u << 9),          ///< Buffer is used as a shader binding table buffer.
};
WISDOM_DEFINE_ENUM_OPERATORS(BufferUsageFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Texture usage flags.
 * Determine how the texture can be used throughout its lifetime.
 *
 * */
enum class TextureUsageFlags : uint32_t
{
    None = 0,                    ///< No flags set. Texture is not used.
    RenderTarget = (1u << 0),    ///< Texture is used as a render target.
    DepthStencil = (1u << 1),    ///< Texture is used as a depth stencil buffer.
    CopySrc = (1u << 2),         ///< Texture is used as a source for copy operations.
    CopyDst = (1u << 3),         ///< Texture is used as a destination for copy operations.
    ShaderResource = (1u << 4),  ///< Texture is used as a shader resource.
    UnorderedAccess = (1u << 5), ///< Texture is used as an unordered access resource.
    HostCopy = (1u << 7),        ///< Texture is used for host copy operations. Works with GPUUpload heap.
};
WISDOM_DEFINE_ENUM_OPERATORS(TextureUsageFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Memory flags.
 * Determine optional properties of the memory allocation.
 *
 * */
enum class MemoryFlags : uint32_t
{
    None = 0, ///< No flags set. Memory is regular.
              /**
               * @brief
               * Memory is dedicated.
               * Used for resources that require dedicated memory.
               * Useful for big resources that are not shared with other resources.
               * E.g. fullscreen textures, big buffers, etc.
               * */
    DedicatedAllocation = (1u << 0),
    /**
     * @brief
     * Memory is mapped.
     * Used in combination with `wis::MemoryType::Upload` or `wis::MemoryType::Readback` to map memory for CPU access.
     * */
    Mapped = (1u << 1),
    /**
     * @brief
     * Memory is exportable.
     * If set, memory can be exported to other processes or APIs.
     * Works only with Device Local memory (`wis::MemoryType::Default`) and only on AllocateXMemory calls.
     * Outside of AllocateXMemory the flag is ignored.
     * */
    Exportable = (1u << 2),
};
WISDOM_DEFINE_ENUM_OPERATORS(MemoryFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Texture creation flags. Reserved for future features.
 *
 * */
enum class TextureFlags : uint32_t
{
    None = 0, ///< No flags set. Texture is regular.
};
WISDOM_DEFINE_ENUM_OPERATORS(TextureFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Texture binding flags, used for extra options.
 *
 * */
enum class TextureBindingFlags : uint32_t
{
    None = 0,                ///< No flags set. Texture view is regular. Implies color read.
    DepthView = (1u << 0),   ///< Texture view is used to read depth. Used for special formats that feature depth and
                             ///< stencil. The bound texture @wis_must be in TODO: specific layout before being used by
                             ///< shader.
    StencilView = (1u << 1), ///< Texture view is used to read stencil. Used for special formats that feature depth and
                             ///< stencil. The bound texture @wis_must be in TODO: specific layout before being used by
                             ///< shader. Cannot be combined with `wis::TextureBindingFlags::DepthView`.
};
WISDOM_DEFINE_ENUM_OPERATORS(TextureBindingFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Barrier synchronization flags for resource barriers.
 *
 * */
enum class BarrierSync : uint32_t
{
    None = 0,                     ///< No synchronization is performed.
    All = (1u << 0),              ///< Synchronize all commands.
    Draw = (1u << 1),             ///< Synchronize draw commands.
    IndexInput = (1u << 2),       ///< Synchronize index input commands.
    VertexShading = (1u << 3),    ///< Synchronize vertex shading commands.
    PixelShading = (1u << 4),     ///< Synchronize pixel shading commands.
    DepthStencil = (1u << 5),     ///< Synchronize depth-stencil commands.
    RenderTarget = (1u << 6),     ///< Synchronize render target commands.
    Compute = (1u << 7),          ///< Synchronize compute commands.
    Raytracing = (1u << 8),       ///< Synchronize ray tracing commands.
    Copy = (1u << 9),             ///< Synchronize copy commands.
    Resolve = (1u << 10),         ///< Synchronize resolve commands.
    ExecuteIndirect = (1u << 11), ///< Synchronize execute indirect commands.
    AllShading = (1u << 12),      ///< Synchronize all shading commands.
    NonPixelShading = (1u << 13), ///< Synchronize non-pixel shading commands.
    ClearUAV = (1u << 14),        ///< Synchronize UAV clear commands.
    VideoDecode = (1u << 15),     ///< Synchronize video decode commands.
    VideoEncode = (1u << 16),     ///< Synchronize video encode commands.
    BuildRTAS = (1u << 17),       ///< Synchronize ray tracing acceleration structure build commands.
    CopyRTAS = (1u << 18),        ///< Synchronize ray tracing acceleration structure copy commands.
};
WISDOM_DEFINE_ENUM_OPERATORS(BarrierSync)

/**
 * @brief Provided by Wisdom 0.7.0. Resource access flags for resource barriers.
 *
 * */
enum class ResourceAccess : uint32_t
{
    Common = 0,                              ///< Common access.
    VertexBuffer = (1u << 0),                ///< Vertex buffer access. Applies only to buffers.
    ConstantBuffer = (1u << 1),              ///< Constant buffer access. Applies only to buffers.
    IndexBuffer = (1u << 2),                 ///< Index buffer access. Applies only to buffers.
    RenderTarget = (1u << 3),                ///< Render target access. Applies only to textures.
    UnorderedAccess = (1u << 4),             ///< Unordered access.
    DepthWrite = (1u << 5),                  ///< Depth write access. Applies only to depth-stencil textures.
    DepthRead = (1u << 6),                   ///< Depth read access. Applies only to depth-stencil textures.
    ShaderResource = (1u << 7),              ///< Shader resource access.
    StreamOutput = (1u << 8),                ///< Stream output access. Applies only to buffers.
    IndirectArgument = (1u << 9),            ///< Indirect argument access.
    CopyDst = (1u << 10),                    ///< Copy destination access.
    CopySrc = (1u << 11),                    ///< Copy source access.
    ConditionalRendering = (1u << 12),       ///< Conditional rendering access.
    AccelerationStructureRead = (1u << 13),  ///< Acceleration structure read access.
    AccelerationStructureWrite = (1u << 14), ///< Acceleration structure write access.
    ShadingRate = (1u << 15),                ///< Shading rate access.
    VideoDecodeRead = (1u << 16),            ///< Video decode read access.
    VideoDecodeWrite = (1u << 17),           ///< Video decode write access.
    ResolveDst = (1u << 18),                 ///< Resolve destination access.
    ResolveSrc = (1u << 19),                 ///< Resolve source access.
    None = (1u << 31),                       ///< No access. Used to indicate no access throughout the pipeline.
};
WISDOM_DEFINE_ENUM_OPERATORS(ResourceAccess)

/**
 * @brief Provided by Wisdom 0.7.0. Barrier flags for resource barriers.
 *
 * */
enum class BarrierFlags : uint32_t
{
    None = 0,                   ///< No flags set. Barrier is regular.
    DiscardContent = (1u << 0), ///< Discard resource content. The content of the resource before the barrier is treated
                                ///< as if resource was not initialized.
    DepthResource = (1u << 1),  ///< Resource is a depth resource. This flag @wis_must be set for all depth resources to
                                ///< make transitions on them.
    StencilResource = (1u << 2), ///< Resource is a stencil resource. This flag @wis_must be set for all stencil
                                 ///< resources to make transitions on them.
    WholeRange = (1u << 3),  ///< Transition whole resource. If not set, the transition is applied only to the specified
                             ///< subresource range. If set, the subresource range is ignored and the transition is
                             ///< applied to all subresources of the resource.
    PlanarImage = (1u << 4), ///< Resource is a planar image. If the flag is not set, plane slices in
                             ///< wis::SubresourceRange are ignored.
};
WISDOM_DEFINE_ENUM_OPERATORS(BarrierFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Pipeline creation flags.
 *
 * */
enum class PipelineFlags : uint32_t
{
    None = 0,                    ///< No flags set. Pipeline is regular.
    FailOnCacheMiss = (1u << 0), ///< Fail pipeline creation if the pipeline cache is missing or incompatible. If not
                                 ///< set, the implementation @wis_may choose to create the pipeline without using the
                                 ///< cache, which @wis_may result in longer creation time.
    EnablePrimitiveRestart = (1u << 1), ///< Enable primitive restart for graphics pipelines. If not set, primitive
                                        ///< restart is disabled and the implementation @wis_may choose to ignore
                                        ///< restart indices in draw calls.
    DynamicDepthBias = (1u << 2),       ///< Enable dynamic depth bias for graphics pipelines. If not set, depth bias is
                                        ///< static and @wis_must be specified at pipeline creation time.
};
WISDOM_DEFINE_ENUM_OPERATORS(PipelineFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Color component flags.
 * Used for color blending operations.
 *
 * */
enum class ColorComponents : uint32_t
{
    None = 0,      ///< No flags set. Color blending is not used.
    R = (1u << 0), ///< Use red component for blending.
    G = (1u << 1), ///< Use green component for blending.
    B = (1u << 2), ///< Use blue component for blending.
    A = (1u << 3), ///< Use alpha component for blending.
    All = 15,      ///< Use all color components for blending.
};
WISDOM_DEFINE_ENUM_OPERATORS(ColorComponents)

/**
 * @brief Provided by Wisdom 0.7.0. Swapchain creation flags.
 *
 * */
enum class SwapchainFlags : uint32_t
{
    None = 0,                 ///< No flags set. Swapchain is regular.
    AllowTearing = (1u << 0), ///< Allow tearing.
    VSync = (1u << 1),  ///< Present with vertical sync. If set, the swapchain is presented with vertical sync pulse.
    Stereo = (1u << 2), ///< Stereo swapchain. If set, the swapchain is created for stereo rendering. If not set, the
                        ///< swapchain is created for mono rendering.
};
WISDOM_DEFINE_ENUM_OPERATORS(SwapchainFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Swapchain creation flags.
 *
 * */
enum class PresentFlags : uint32_t
{
    None = 0,                   ///< No flags set. Swapchain is regular.
    TimeoutOnBlock = (1u << 0), ///< Fail present if the presentation engine is busy. If not set, the implementation
                                ///< @wis_may choose to block until the presentation engine is available.
};
WISDOM_DEFINE_ENUM_OPERATORS(PresentFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Set of flags that affect render pass behavior.
 * More on render pass flags
 * [here](https://learn.microsoft.com/en-us/windows/win32/direct3d12/direct3d-12-render-passes).
 *
 * */
enum class RenderPassFlags : uint32_t
{
    None = 0,                   ///< No flags set. Render pass is regular.
    Suspending = (1u << 1),     ///< Render pass is suspending.
    Resuming = (1u << 2),       ///< Render pass is resuming.
    AllowUAVWrites = (1u << 3), ///< Allow UAV writes. If set, unordered access view (UAV) writes are allowed during the
                                ///< render pass. If not set, UAV writes are not allowed and @wis_may result in
                                ///< undefined behavior if attempted.
};
WISDOM_DEFINE_ENUM_OPERATORS(RenderPassFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Depth stencil select flags.
 * Affect which part of the depth stencil buffer is used.
 *
 * */
enum class DepthStencilFlags : uint32_t
{
    None = 0,                  ///< No flags set. Both attachments are write.
    IgnoreDepth = (1u << 0),   ///< Don't use depth part of the depth stencil buffer.
    IgnoreStencil = (1u << 1), ///< Don't use stencil part of the depth stencil buffer.
    ReadOnlyDepth = (1u << 2), ///< Depth part is read only. Texture @wis_must be in either read state, depending on the
                               ///< format.
    ReadOnlyStencil = (1u << 3), ///< Stencil part is read only. Texture @wis_must be in either read state, depending on
                                 ///< the format.
};
WISDOM_DEFINE_ENUM_OPERATORS(DepthStencilFlags)

//==============================================================
// Delegates
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Defines the debug callback function signature. Used for logging and debugging
 * messages from the graphics API.
 * @param severity indicates message severity level.
 * @param message describes contains the debug message string.
 * @param device defines handle to the device that generated the message. Can be `0` if message is not device specific.
 * @param user_data defines user defined data pointer passed during callback registration.
 *
 * */
using DebugCallback = void (*)(wis::Severity severity, const char* message, std::uint64_t device, void* user_data);

//==============================================================
// Structs
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Main source of communication of operation success.
 *
 * */
struct WIS_NODISCARD Result {
    wis::Status status;         ///< defines operation status. Compare with `wis::Status::Ok`.
    std::int32_t platform_code; ///< indicates platform code from underlying implementation. Is an `HRESULT` for DX12
                                ///< and a `VkResult` for Vulkan.
    const char* error;          ///< specifies contains a human readable error message.
};

/**
 * @brief Provided by Wisdom 0.7.0. Adapter description. Describes hardware driver identification as well as memory
 * limits.
 *
 * */
struct AdapterDesc {
    std::array<char, 256> description; ///< defines adapter description. Contains name of the graphics adapter.
    std::uint32_t vendor_id;           ///< specifies denotes Vendor ID. Can be used to find the correct adapter.
    std::uint32_t device_id; ///< defines denotes Device ID. Together with `wis::AdapterDesc::vendor_id` uniquely
                             ///< identifies the device.
    std::uint64_t dedicated_video_memory; ///< describes measures dedicated video memory in bytes. Used for device local
                                          ///< memory type.
    std::uint64_t shared_system_memory; ///< indicates measures memory that is shared with CPU in bytes. Used for upload
                                        ///< and readback.
    std::uint64_t adapter_id; ///< describes denotes adapter unique ID (LUID). Can be used to find the correct adapter.
    std::array<std::uint8_t, 16>
        adapter_uuid; ///< indicates stores UUID of the adapter, used only with Vulkan API on systems with no LUID.
    wis::AdapterFlags flags; ///< describes adapter flags. Describe the adapter kind.
};

/**
 * @brief Provided by Wisdom 0.7.0. Debugging and logging description. Used to configure debug callback behavior.
 *
 * */
struct DebugDesc {
    bool enable_debug_layer;     ///< describes enables or disables debug layer on both DX12 and VK backends.
    wis::DebugCallback callback; ///< describes the debug callback function.
    void* user_data;             ///< defines user defined data pointer passed to the callback.
};

/**
 * @brief Provided by Wisdom 0.7.0. Command queue description for wis::CommandQueue creation.
 *
 * */
struct CommandQueueDesc {
    wis::CommandQueueType type; ///< specifies the type of the command queue.
    wis::CommandQueuePriority
        priority; ///< specifies command queue priority. Used to set priority of the command queues of the type.
};

/**
 * @brief Provided by Wisdom 0.7.0. Sampler description for  creation.
 *
 * */
struct SamplerDesc {
    wis::Filter min_filter;            ///< describes minification filter.
    wis::Filter mag_filter;            ///< specifies magnification filter.
    wis::Filter mip_filter;            ///< indicates mip level filter.
    wis::ReductionMode reduction_mode; ///< describes reduction mode for min/mag/mip filters.
    bool is_anisotropic;               ///< specifies anisotropic filtering enable.
    std::uint32_t max_anisotropy;      ///< describes max anisotropy level. Min is 1, Max is 16.
    wis::AddressMode address_u;        ///< specifies address mode for U coordinate.
    wis::AddressMode address_v;        ///< indicates address mode for V coordinate.
    wis::AddressMode address_w;        ///< defines address mode for W coordinate.
    float min_lod;                     ///< specifies min LOD value.
    float max_lod;                     ///< defines max LOD value.
    float mip_lod_bias;                ///< specifies mip LOD bias value.
    wis::CompareOp comparison_op;      ///< describes comparison operation for comparison samplers.
    wis::StaticBorder
        static_border_color; ///< indicates static border color. Used if any address mode is set to wis::AddressMode.
    wis::SamplerFlags flags; ///< describes sampler flags. Used to set additional sampler options.
};

/**
 * @brief Provided by Wisdom 0.7.0. Static sampler description for wis::RootSignature creation.
 *
 * */
struct StaticSamplerDesc {
    wis::SamplerDesc sampler;    ///< defines sampler description.
    wis::ShaderVisibility
        visibility;              ///< defines shader stage visibility; it defines the stage where the sampler is used.
    std::uint32_t bind_register; ///< describes bind register number in HLSL.
};

/**
 * @brief Provided by Wisdom 0.7.0. A set of constants that get read directly from root buffer.
 *
 * */
struct PushConstant {
    wis::ShaderVisibility visibility; ///< defines shader stage visibility for the push constant.
    std::uint32_t size_bytes;         ///< describes size of the constant in bytes; it @wis_must be divisible by 4.
    std::uint32_t bind_register;      ///< describes bind register number in HLSL.
    std::uint32_t bind_space;         ///< defines bind space number in HLSL as `register(regN, spaceN)`.
};

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor. Used to push data directly to pipeline.
 *
 * */
struct PushDescriptor {
    wis::ShaderVisibility visibility; ///< defines shader stage visibility for the push descriptor.
    wis::DescriptorType type;         ///< specifies descriptor type. Works only with buffer bindings.
    std::uint32_t bind_register;      ///< describes bind register number in HLSL.
    std::uint32_t bind_space;         ///< defines bind space number in HLSL. `register(regN, spaceN)`
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor table entry for wis::DescriptorTable.
 *
 * */
struct DescriptorTableEntry {
    wis::DescriptorType type;    ///< specifies descriptor type.
    std::uint32_t bind_register; ///< describes bind register number in HLSL.
    std::uint32_t bind_space;    ///< defines bind space number in HLSL. `register(regN, spaceN)`
    std::uint32_t count; ///< describes descriptor count for Array descriptors. UINT32_MAX means unbounded array. 0
                         ///< means single register, same as 1.
    std::uint32_t descriptor_offset; ///< describes offset in descriptors from the heap start. Used for calculating
                                     ///< descriptor indices when binding descriptor tables.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor table for wis::RootSignature creation.
 *
 * */
struct DescriptorTable {
    wis::ShaderVisibility visibility; ///< defines shader stage. Defines the stage where the table is used.
    wis::span<const wis::DescriptorTableEntry> entries; ///< points to an array of descriptor table entries.
};

/**
 * @brief Provided by Wisdom 0.7.0. Pipeline layout description. Defines resource bindings for shaders.
 *
 * */
struct RootSignatureDesc {
    wis::span<const wis::PushConstant>
        push_constants; ///< points to an array of push constants that defines global shader push data.
    wis::span<const wis::PushDescriptor> push_descriptors;   ///< points to an array of wis::PushDescriptor.
    wis::span<const wis::DescriptorTable> descriptor_tables; ///< points to an array of wis::DescriptorTable.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor heap description for wis::DescriptorHeap creation.
 *
 * */
struct DescriptorHeapDesc {
    wis::DescriptorHeapType type;          ///< specifies the type of descriptor heap to create (sampler or descriptor).
    wis::DescriptorMemoryType memory_type; ///< specifies where the descriptor heap will be allocated.
    std::uint32_t descriptor_count;        ///< indicates the amount of descriptors, present in the heap.
    wis::DescriptorHeapFlags flags;        ///< describes describe additional options for the descriptor heap.
};

/**
 * @brief Provided by Wisdom 0.7.0. Component mapping for .
 *
 * */
struct ComponentMapping {
    wis::ComponentSwizzle r; ///< specifies component mapping for Red channel. Default is `wis::ComponentSwizzle::Red`.
    wis::ComponentSwizzle
        g; ///< indicates component mapping for Green channel. Default is `wis::ComponentSwizzle::Green`.
    wis::ComponentSwizzle
        b; ///< specifies component mapping for Blue channel. Default is `wis::ComponentSwizzle::Blue`.
    wis::ComponentSwizzle
        a; ///< describes component mapping for Alpha channel. Default is `wis::ComponentSwizzle::Alpha`.
};

/**
 * @brief Provided by Wisdom 0.7.0. Subresource description for texture data updates and copies.
 *
 * */
struct SubresourceRange {
    std::uint16_t base_mip_level;  ///< indicates mipmap level of the subresource.
    std::uint16_t mip_level_count; ///< describes number of mip levels in the subresource.
    std::uint16_t
        base_array_layer; ///< describes array layer of the subresource. For 3D textures, this defines the depth slice.
    std::uint16_t array_layer_count; ///< indicates number of array layers in the subresource. For 3D textures, this
                                     ///< defines the number of depth slices.
    std::uint16_t plane_slice; ///< indicates base depth slice of the subresource. Used only for 2D textures (YUV).
    std::uint16_t plane_slice_count; ///< indicates number of depth slices in the subresource. Used only for 2D textures
                                     ///< (YUV). Max value is 3.
};

/**
 * @brief Provided by Wisdom 0.7.0. Buffer description for wis::Buffer creation.
 *
 * */
struct BufferDesc {
    std::uint64_t size_bytes;          ///< describes size of the buffer in bytes.
    wis::BufferUsageFlags usage_flags; ///< describes buffer usage flags. Describe how the buffer will be used.
    wis::MemoryType memory_type;       ///< specifies where the buffer will be allocated.
    wis::MemoryFlags memory_flags;     ///< describes the flags of the memory to allocate for the buffer.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture description for wis::Texture creation.
 *
 * */
struct TextureDesc {
    std::uint32_t width;                ///< defines texture width in pixels.
    std::uint32_t height;               ///< describes texture height in pixels.
    std::uint16_t depth_or_array_size;  ///< describes texture depth in pixels. Used only for 3D textures.
    std::uint16_t mip_levels;           ///< defines number of mip levels in the texture.
    wis::DataFormat format;             ///< describes texture format.
    wis::SampleCount sample_count;      ///< specifies number of samples per pixel. Used only for multisampled textures.
    wis::TextureLayout layout;          ///< specifies texture layout. Default is `wis::TextureLayout::Texture2D`.
    wis::TextureUsageFlags usage_flags; ///< describes texture usage flags. Describe how the texture will be used.
    wis::TextureFlags flags;            ///< describes texture flags. Describe additional options for the texture.
    wis::MemoryType memory_type;        ///< specifies where the texture will be allocated.
    wis::MemoryFlags memory_flags;      ///< describes the flags of the memory to allocate for the texture.
};

/**
 * @brief Provided by Wisdom 0.7.0. Push constant set description for wis::CommandList::SetPushConstants.
 *
 * */
struct PushConstantDataDesc {
    wis::PipelineType pipeline; ///< specifies the pipeline type to set the push constants for.
    std::uint32_t root_index;   ///< indicates the root index in the root signature to set the push constants for.
    const void* data;           ///< specifies a pointer to the data to set as push constants.
    std::uint32_t data_size;    ///< defines the size of the data in bytes. It @wis_must be less than or equal to the
                                ///< maximum push constant size defined by the device and 4-byte aligned.
    std::uint32_t push_offset;  ///< specifies the offset in bytes from the start of the push constant root parameter to
                               ///< set the data to. It @wis_must be less than the maximum push constant size defined by
                               ///< the device and 4-byte aligned.
};

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor set description for wis::CommandList::SetPushDescriptor.
 *
 * */
struct PushDescriptorDataDesc {
    wis::PipelineType pipeline; ///< specifies the pipeline type to set the push descriptors for.
    std::uint32_t root_index;   ///< indicates the root index in the root signature to set the push descriptors for.
    wis::DescriptorType descriptor_type; ///< defines the type of the descriptors to push.
    std::uint64_t buffer_address;        ///< indicates buffer device address to push.
};

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor set description for wis::CommandList::SetDescriptorTable.
 *
 * */
struct DescriptorTableDataDesc {
    wis::PipelineType pipeline; ///< specifies the pipeline type to set the push descriptors for.
    std::uint32_t root_index;   ///< indicates the root index in the root signature to set the push descriptors for.
    wis::DescriptorHeapType heap_type; ///< indicates the type of the descriptor heap to bind.
    std::uint32_t heap_offset; ///< defines the offset in descriptors from the start of the heap to set the descriptor
                               ///< table to. Used for calculating descriptor indices when binding descriptor tables.
};

/**
 * @brief Provided by Wisdom 0.7.0. Constant buffer binding description for wis::DescriptorHeap::WriteConstantBuffer.
 *
 * */
struct ConstantBufferBinding {
    std::uint64_t buffer_address; ///< indicates buffer device address to bind.
    std::uint32_t size_bytes;     ///< describes the size of the buffer region to bind in bytes.
};

/**
 * @brief Provided by Wisdom 0.7.0. Buffer binding description for wis::DescriptorHeap::WriteStructuredBuffer.
 *
 * */
struct BufferBinding {
    std::uint64_t array_offset;    ///< describes offset in buffer in structures.
    std::uint32_t stride_bytes;    ///< describes the size of the single structure in buffer.
    std::uint32_t structure_count; ///< describes the number of structures in the buffer region to bind.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture binding description for wis::DescriptorHeap::WriteTexture and
 * wis::DescriptorHeap::WriteRWTexture.
 *
 * */
struct TextureBinding {
    wis::DataFormat format;    ///< describes the format of the view.
    wis::TextureLayout layout; ///< specifies the layout of the texture. Default is `wis::TextureLayout::Texture2D`.
    wis::TextureBindingFlags
        flags; ///< describes texture binding flags. Describe additional options for the texture binding.
    wis::ComponentMapping component_mapping; ///< specifies component mapping for the texture view.
    wis::SubresourceRange range;             ///< describes subresource description for the texture view.
};

/**
 * @brief Provided by Wisdom 0.7.0. Stencil description for wis::DepthStencilDesc.
 *
 * */
struct StencilDesc {
    wis::StencilOp
        fail_op; ///< specifies stencil operation if the stencil test fails. Default is `wis::StencilOp::Keep`.
    wis::StencilOp depth_fail_op; ///< specifies stencil operation if the stencil test passes and the depth test fails.
                                  ///< Default is `wis::StencilOp::Keep`.
    wis::StencilOp
        pass_op; ///< describes stencil operation if the stencil test passes. Default is `wis::StencilOp::Keep`.
    wis::CompareOp stencil_comp; ///< defines stencil comparison function. Default is .
    std::uint8_t read_mask;      ///< indicates stencil read mask. Default is 0xff.
    std::uint8_t write_mask;     ///< specifies stencil write mask. Default is 0xff.
};

/**
 * @brief Provided by Wisdom 0.7.0. Depth stencil description for wis::GraphicsPipelineDesc.
 *
 * */
struct DepthStencilDesc {
    bool depth_enable;              ///< indicates depth test enable. Default is false.
    bool depth_write_enable;        ///< describes depth write enable. Default is false.
    wis::CompareOp depth_comp;      ///< indicates depth comparison function. Default is .
    bool stencil_enable;            ///< defines stencil test enable. Default is false.
    wis::StencilDesc stencil_front; ///< specifies stencil description for front faces.
    wis::StencilDesc stencil_back;  ///< specifies stencil description for back faces.
    bool depth_bound_test;          ///< indicates depth bound test enable. Default is false.
};

/**
 * @brief Provided by Wisdom 0.7.0. Input slot description for wis::InputLayout.
 *
 * */
struct InputBindingDesc {
    std::uint32_t slot;          ///< specifies input slot number; it @wis_must be unique.
    std::uint32_t stride_bytes;  ///< describes stride in bytes. Size of one vertex in the slot.
    wis::InputClass input_class; ///< describes input class. Defines how the data is read (Per vertex or Per instance).
};

/**
 * @brief Provided by Wisdom 0.7.0. Input attribute description for wis::InputLayout.
 *
 * */
struct InputAttributeDesc {
    std::uint32_t binding_index; ///< specifies index into an array of bindings that the attribute is tied to.
    const char*
        semantic_name; ///< defines semantic name of the attribute in HLSL; it @wis_must be unique and null-terminated.
    std::uint32_t semantic_index; ///< indicates semantic index of the attribute in HLSL; it @wis_must be unique.
    std::uint32_t location;       ///< describes location of the attribute in HLSL; it @wis_must be unique.
    wis::DataFormat format;       ///< describes data format of the attribute.
    std::uint32_t offset_bytes;   ///< describes offset in bytes from the beginning of the vertex.
};

/**
 * @brief Provided by Wisdom 0.7.0. Input layout description for wis::GraphicsPipelineDesc.
 *
 * */
struct InputLayout {
    wis::span<const wis::InputBindingDesc>
        bindings; ///< points to an array of input slots used to pick data from multiple vertex arrays.
    wis::span<const wis::InputAttributeDesc> attributes; ///< points to an array of input attributes that describes how
                                                         ///< vertex data is read by the HLSL shader.
};

/**
 * @brief Provided by Wisdom 0.7.0. Rasterizer description for wis::GraphicsPipelineDesc.
 *
 * */
struct RasterizerDesc {
    wis::FillMode fill_mode;       ///< indicates fill mode. Solid or Wireframe. Default is `wis::FillMode::Solid`.
    wis::CullMode cull_mode;       ///< defines cull mode. None, Front, Back. Default is `wis::CullMode::Back`.
    wis::WindingOrder front_face;  ///< indicates front face winding order. Clockwise or CounterClockwise. Default is
                                   ///< `wis::WindingOrder::Clockwise`.
    bool depth_bias_enable;        ///< describes depth bias enable. Default is false.
    float depth_bias;              ///< indicates depth bias. Default is 0.0f.
    float depth_bias_clamp;        ///< indicates depth bias clamp. Default is 0.0f.
    float depth_bias_slope_factor; ///< indicates depth bias slope factor e.g. for shadows. Default is 0.0f.
    bool depth_clip_enable;        ///< specifies depth clip enable. Default is true.
    wis::LineRasterization
        line_rasterization;        ///< specifies line rasterization mode. Default is `wis::LineRasterization::Default`.
    wis::ConservativeRasterization conservative_rasterization; ///< indicates conservative rasterization mode. Default
                                                               ///< is `wis::ConservativeRasterization::Off`.
};

/**
 * @brief Provided by Wisdom 0.7.0. Sample description of Multisampling for wis::GraphicsPipelineDesc.
 *
 * */
struct SampleDesc {
    wis::SampleCount rate;         ///< defines sample rate. Default is `wis::SampleCount::S1`.
    std::uint32_t sample_mask;     ///< describes sample mask. Default is 0xffffffff.
    bool alpha_to_coverage_enable; ///< describes alpha to coverage enable. Default is false.
};

/**
 * @brief Provided by Wisdom 0.7.0. Render attachments description for wis::GraphicsPipelineDesc.
 *
 * */
struct RenderAttachmentsDesc {
    std::array<wis::DataFormat, 8>
        attachment_formats;           ///< defines attachment formats array. Describes the format of the render target.
    std::uint32_t attachments_count;  ///< defines attachment formats count. Max is 8.
    wis::DataFormat depth_attachment; ///< describes depth attachment format. Describes the format of the depth buffer.
    std::uint32_t view_mask; ///< specifies view mask for multiview rendering. Each bit represents a view that can be
                             ///< rendered to with the pipeline. Default is 0, meaning no multiview support.
};

/**
 * @brief Provided by Wisdom 0.7.0. Blend attachment description for wis::BlendStateDesc.
 *
 * */
struct BlendAttachmentDesc {
    bool blend_enable;                ///< indicates blend enable. Default is false.
    wis::BlendFactor src_color_blend; ///< specifies source color blend factor. Default is `wis::BlendFactor::One`.
    wis::BlendFactor
        dst_color_blend;         ///< describes destination color blend factor. Default is `wis::BlendFactor::Zero`.
    wis::BlendOp color_blend_op; ///< describes color blend operation. Default is `wis::BlendOp::Add`.
    wis::BlendFactor src_alpha_blend; ///< describes source alpha blend factor. Default is `wis::BlendFactor::One`.
    wis::BlendFactor dst_alpha_blend; ///< defines destination alpha blend factor. Default is `wis::BlendFactor::Zero`.
    wis::BlendOp alpha_blend_op;      ///< defines alpha blend operation. Default is `wis::BlendOp::Add`.
    wis::ColorComponents color_write_mask; ///< defines color write mask. Default is `wis::ColorComponents::All`.
};

/**
 * @brief Provided by Wisdom 0.7.0. Viewport description for wis::CommandList.
 * Viewport is considered from Top Left corner.
 *
 * */
struct Viewport {
    float x;         ///< defines top left corner x coordinate.
    float y;         ///< describes top left corner y coordinate.
    float width;     ///< defines viewport width.
    float height;    ///< describes viewport height.
    float min_depth; ///< defines minimum depth of the viewport.
    float max_depth; ///< specifies maximum depth of the viewport.
};

/**
 * @brief Provided by Wisdom 0.7.0. Scissor rect for wis::CommandList and present rect.
 *
 * */
struct Rect {
    std::int32_t x;       ///< defines left corner x coordinate.
    std::int32_t y;       ///< describes top corner y coordinate.
    std::uint32_t width;  ///< defines width of the rect.
    std::uint32_t height; ///< describes height of the rect.
};

/**
 * @brief Provided by Wisdom 0.7.0. Blend state description for wis::GraphicsPipelineDesc.
 *
 * */
struct BlendStateDesc {
    bool logic_op_enable;  ///< specifies logic operation enable. Default is false.
    wis::LogicOp logic_op; ///< defines logic operation. Default is `wis::LogicOp::Noop`.
    std::array<wis::BlendAttachmentDesc, 8>
        attachments;       ///< defines blend attachment descriptions. Max Array size is 8.
    std::uint32_t
        attachment_count;  ///< describes blend attachment count. If set as 0 - broadcast to all attachments. Max is 8.
};

/**
 * @brief Provided by Wisdom 0.7.0. Box description for texture copy regions.
 *
 * */
struct Box {
    std::uint32_t x;      ///< defines left corner x coordinate.
    std::uint32_t y;      ///< describes top corner y coordinate.
    std::uint32_t z;      ///< specifies front corner z coordinate. Used only for 3D textures.
    std::uint32_t width;  ///< defines width of the box.
    std::uint32_t height; ///< describes height of the box.
    std::uint32_t depth;  ///< describes depth of the box. Used only for 3D textures.
};

/**
 * @brief Provided by Wisdom 0.7.0. Target subresource description for texture copy regions.
 *
 * */
struct TargetSubresource {
    std::uint16_t mip_level;   ///< describes mipmap level of the target subresource.
    std::uint16_t array_layer; ///< indicates array layer of the target subresource; for 3D textures it @wis_must be 0.
    std::uint16_t plane_slice; ///< indicates depth slice of the target subresource. Used only for 2D textures (YUV).
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture copy region description for texture copy operations.
 *
 * */
struct TextureRegion {
    wis::BarrierFlags
        flags;    ///< describes texture parameters for copy. `wis::BarrierFlags::DiscardContent` is implicit.
    wis::Box box; ///< describes box defining the region to copy.
    wis::TargetSubresource target_subresource; ///< defines target subresource description for the region.
};

/**
 * @brief Provided by Wisdom 0.7.0. Render target description for wis::ViewHeap::WriteRenderTarget and
 * wis::ViewHeap::WriteDepthStencil.
 *
 * */
struct RenderTargetDesc {
    wis::DataFormat format;          ///< describes render target format.
    wis::TextureLayout layout;       ///< specifies render target layout. Default is `wis::TextureLayout::Texture2D`.
    std::uint16_t mip_level;         ///< describes mipmap level of the target subresource.
    std::uint16_t base_array_layer;  ///< describes array layer of the target subresource. For 3D textures, this defines
                                     ///< the base depth slice.
    std::uint16_t array_layer_count; ///< indicates number of array layers in the target subresource. For 3D textures,
                                     ///< this defines the number of depth slices.
    std::uint16_t plane_slice; ///< indicates depth slice of the target subresource. Used only for 2D textures (YUV).
};

/**
 * @brief Provided by Wisdom 0.7.0. Surface parameters for wis::Surface creation.
 *
 * */
struct SurfaceParameters {
    std::uint32_t min_swapchain_images; ///< specifies minimum number of images in the swapchain.
    std::uint32_t max_swapchain_images; ///< defines maximum number of images in the swapchain.
    std::uint32_t
        alpha_modes_supported; ///< specifies bitmask of supported alpha modes for the swapchain. Each bit represents a
                               ///< different alpha mode. Used to determine the supported alpha modes for the swapchain.
    wis::TextureUsageFlags
        texture_usage_flags_supported; ///< specifies bitmask of supported texture usage flags for the swapchain images.
    bool stereo_supported; ///< indicates if stereo rendering is supported. If true, the surface can be used to create a
                           ///< swapchain with stereo support.
};

/**
 * @brief Provided by Wisdom 0.7.0. Swapchain description for wis::Swapchain creation.
 *
 * */
struct SwapchainDesc {
    std::uint32_t width;                        ///< defines swapchain image width in pixels.
    std::uint32_t height;                       ///< describes swapchain image height in pixels.
    std::uint32_t image_count;                  ///< indicates number of images in the swapchain.
    wis::TextureUsageFlags texture_usage_flags; ///< describes texture usage flags for the swapchain images. Describe
                                                ///< how the swapchain images will be used.
    wis::DataFormat format;                     ///< describes swapchain image format.
    wis::SwapchainScaling scaling;              ///< describes swapchain scaling mode.
    wis::SwapchainFlags flags;           ///< describes swapchain flags. Describe additional options for the swapchain.
    wis::CompositeAlpha composite_alpha; ///< defines composite alpha mode. Describe how the alpha channel of the
                                         ///< swapchain images is treated during compositing.
};

/**
 * @brief Provided by Wisdom 0.7.0. Swapchain update description for wis::Swapchain::Update.
 *
 * */
struct SwapchainUpdateDesc {
    std::uint32_t width;       ///< defines new swapchain image width in pixels.
    std::uint32_t height;      ///< describes new swapchain image height in pixels.
    std::uint32_t image_count; ///< indicates number of images in the swapchain.
    wis::DataFormat format;    ///< describes swapchain image format.
    bool vsync; ///< indicates controls vsync; when true, presentation is synchronized to the vertical blanking interval
                ///< to reduce tearing, whereas false can improve frame rate but can introduce tearing.
};

/**
 * @brief Provided by Wisdom 0.7.0. Description for render target.
 *
 * */
struct RenderPassRenderTargetDesc {
    std::uint64_t target;  ///< indicates render target view; it @wis_must reside on wis::ViewHeap.
    wis::LoadOp load_op;   ///< specifies load operation on beginning of render pass. Default is `wis::LoadOp::Load`.
    wis::StoreOp store_op; ///< indicates store operation on end of render pass. Default is `wis::StoreOp::Store`.
    std::array<float, 4> clear_value; ///< indicates clear value for `wis::LoadOp::Clear`.
};

/**
 * @brief Provided by Wisdom 0.7.0. Variant of RenderPassDesc for depth stencil.
 *
 * */
struct RenderPassDepthStencilDesc {
    std::uint64_t target;          ///< indicates depth stencil view.
    wis::LoadOp load_op_depth;     ///< specifies load operation on beginning of render pass for depth. Default is
                                   ///< `wis::LoadOp::Load`.
    wis::LoadOp load_op_stencil;   ///< indicates load operation on beginning of render pass for stencil. Default is
                                   ///< `wis::LoadOp::Load`.
    wis::StoreOp store_op_depth;   ///< indicates store operation on end of render pass for depth. Default is
                                   ///< `wis::StoreOp::Store`.
    wis::StoreOp store_op_stencil; ///< defines store operation on end of render pass for stencil. Default is
                                   ///< `wis::StoreOp::Store`.
    wis::DepthStencilFlags flags;  ///< describes depth stencil select.
    float clear_depth;             ///< indicates clear depth value for `wis::LoadOp::Clear`. Default is 1.0f.
    std::uint8_t clear_stencil;    ///< defines clear stencil value for `wis::LoadOp::Clear`. Default is 0.
};

/**
 * @brief Provided by Wisdom 0.7.0. Render pass description.
 *
 * */
struct RenderPassDesc {
    wis::RenderPassFlags flags;        ///< describes render pass flags.
    std::array<wis::RenderPassRenderTargetDesc, 8>
        render_targets;                ///< describes an array of render target descriptions. Max size is 8.
    std::uint32_t render_target_count; ///< specifies number of render targets in the render pass.
    std::uint32_t view_mask; ///< specifies view mask for multiview rendering. Each bit represents a view that can be
                             ///< rendered to with the render pass. Default is 0, meaning no multiview support.
    wis::RenderPassDepthStencilDesc depth_stencil; ///< specifies depth stencil description; if depth stencil is not
                                                   ///< used, the target field @wis_must be set to 0.
};

/**
 * @brief Provided by Wisdom 0.7.0. Buffer copy region description for buffer copy operations.
 *
 * */
struct BufferCopyRegion {
    std::uint64_t src_offset; ///< specifies source buffer offset in bytes.
    std::uint64_t dst_offset; ///< describes destination buffer offset in bytes.
    std::uint64_t size_bytes; ///< describes size of the region to copy in bytes.
};

/**
 * @brief Provided by Wisdom 0.7.0. Buffer texture copy region description for buffer-texture copy operations.
 *
 * */
struct BufferTextureCopyRegion {
    std::uint64_t buffer_offset;     ///< specifies buffer offset in bytes.
    std::uint32_t buffer_row_length; ///< describes buffer row length in pixels. Used for calculating the offset in the
                                     ///< buffer for each row of the texture.
    std::uint32_t buffer_image_height; ///< describes buffer image height in pixels. Used for calculating the offset in
                                       ///< the buffer for each image of the texture.
    wis::TextureRegion texture_region; ///< describes texture region to copy.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture copy region description for texture copy operations.
 *
 * */
struct TextureCopyRegion {
    wis::TextureRegion src_region; ///< describes source texture region to copy.
    wis::TextureRegion dst_region; ///< describes destination texture region to copy.
};

/**
 * @brief Provided by Wisdom 0.7.0. Query struct header. Used as a header for all query structs.
 *
 * */
struct QueryStructHeader {
    wis::QueryPropertyType
        property_type;   ///< specifies the type of the queried property. Used to determine what struct is passed.
    void* next_in_chain; ///< indicates pointer to the next queried data struct.
};

/**
 * @brief Provided by Wisdom 0.7.0. Device binding properties. Used to query resource binding support and limits.
 *
 * */
struct DeviceBindingProperties {
    wis::QueryPropertyType
        property_type;   ///< specifies the type of the queried property. Used to determine what struct is passed.
                         ///< @wis_must be `wis::QueryPropertyType::DeviceBindingProperties`.
    void* next_in_chain; ///< indicates pointer to the next queried data struct.
    std::uint32_t max_vertex_input_attributes; ///< defines maximum number of vertex input attributes supported by the
                                               ///< device. Used for vertex buffer bindings.
    std::uint32_t max_vertex_input_bindings;   ///< indicates maximum number of vertex input bindings supported by the
                                               ///< device. Used for vertex buffer bindings.
    bool multiple_viewports_supported; ///< indicates if multiple viewports are supported. If true, the device supports
                                       ///< up to 16 viewports and scissor rectangles. If false, only one viewport and
                                       ///< scissor rectangle is supported.
};

/**
 * @brief Provided by Wisdom 0.7.0. Device descriptor heap properties. Used to query descriptor heap support and limits.
 *
 * */
struct DeviceDescriptorHeapProperties {
    wis::QueryPropertyType
        property_type;   ///< specifies the type of the queried property. Used to determine what struct is passed.
                         ///< @wis_must be `wis::QueryPropertyType::DeviceDescriptorHeapProperties`.
    void* next_in_chain; ///< indicates pointer to the next queried data struct.
    std::uint32_t max_descriptor_heap_size; ///< indicates maximum number of descriptors in a single descriptor heap.
    std::uint32_t max_sampler_heap_size;    ///< defines maximum number of samplers in a single descriptor heap.
    std::uint32_t max_sampler_heap_size_with_embedded; ///< defines maximum number of samplers in a single descriptor
                                                       ///< heap, if embedded samplers are used.
    std::uint32_t descriptor_increment_size; ///< describes size of a single descriptor in the descriptor heap. Used for
                                             ///< calculating descriptor offsets.
    std::uint32_t sampler_increment_size;    ///< specifies size of a single sampler in the sampler heap. Used for
                                             ///< calculating sampler offsets.
    std::uint32_t
        render_target_increment_size; ///< defines size of a single render target view descriptor in the descriptor
                                      ///< heap. Used for calculating render target view descriptor offsets.
    std::uint32_t
        depth_stencil_increment_size; ///< defines size of a single depth stencil view descriptor in the descriptor
                                      ///< heap. Used for calculating depth stencil view descriptor offsets.
};

/**
 * @brief Provided by Wisdom 0.7.0. Device memory properties. Used to query memory type support and limits.
 *
 * */
struct DeviceCommandQueueProperties {
    wis::QueryPropertyType
        property_type;    ///< specifies the type of the queried property. @wis_must be wis::QueryPropertyType..
    void* next_in_chain;  ///< indicates pointer to the next queried data struct.
    std::array<bool, 5>
        supported_queues; ///< describes an array of supported queue types. If a queue type is supported, the value is
                          ///< `1`, otherwise `0`. Order of queue types is the same as in wis::CommandQueueType enum.
    bool relaxed_queue_transition; ///< indicates if relaxed queue transition is supported. This feature allows
                                   ///< executing command lists that contain buffers used on different queue types
                                   ///< without explicit resource state transitions when the buffers is used on a
                                   ///< different queue type. It is supported on Windows 10 22H2 and later with WDDM 3.0
                                   ///< or later. On Vulkan it requires `VK_KHR_maintenance9` extension.
    std::array<wis::CommandQueuePriority, 5>
        max_queue_priority; ///< indicates an array of maximum supported priorities for each queue type. If a queue type
                            ///< is not supported, the value is `0`. Order of queue types is the same as in
                            ///< wis::CommandQueueType enum.
};

/**
 * @brief Provided by Wisdom 0.7.0. Device memory properties. Used to query memory type support and limits.
 *
 * */
struct DeviceMemoryProperties {
    wis::QueryPropertyType property_type; ///< specifies the type of the queried property. @wis_must be
                                          ///< `wis::QueryPropertyType::DeviceMemoryProperties`..
    void* next_in_chain;                  ///< indicates pointer to the next queried data struct.
    bool gpu_upload_supported; ///< describes if GPU upload memory type is supported. This memory type allows mapping
                               ///< the memory and writing to it from CPU, while being accessible from GPU. It is
                               ///< usually implemented as write-combined memory on integrated GPUs and as a part of
                               ///< shared system memory on discrete GPUs.
    bool host_image_copy_supported; ///< indicates if host image copy is supported. This feature allows copying data
                                    ///< directly from CPU memory to optimal tiled image layout on GPU, without the need
                                    ///< for an intermediate staging buffer. It is supported on Windows 10 22H2 and
                                    ///< later with WDDM 3.0 or later. On Vulkan it requires `VK_EXT_host_image_copy`
                                    ///< extension.
    std::uint32_t
        supported_initial_transitions; ///< defines bitfield of supported initial resource state transitions for buffers
                                       ///< and textures. If a transition is supported, the corresponding bit is set to
                                       ///< `1`, otherwise `0`. Bit positions are the same as in wis::TextureState enum.
                                       ///< `wis::TextureState::Undefined` is always supported.
};

//==============================================================
// Constants
//==============================================================

/// @brief Provided by Wisdom 0.7.0. Append descriptor table entry after previous table.
static constexpr std::uint32_t DescriptorOffsetAppend = 0xffffffff;

/// @brief Provided by Wisdom 0.7.0. Select all remaining array layers.
static constexpr std::uint32_t RemainingLayers = 0xffffffff;

/// @brief Provided by Wisdom 0.7.0. Select all remaining mip levels.
static constexpr std::uint32_t RemainingMips = 0xffffffff;

/// @brief Provided by Wisdom 0.7.0. Defines the amount of usable space in root signature.
static constexpr std::uint32_t RootSignatureDwords = 64;

/// @brief Provided by Wisdom 0.7.0. Defines the amount of barriers of all types that will not trigger allocation.
static constexpr std::uint32_t TransientMaxBarrierCount = 32;

/// @brief Provided by Wisdom 0.7.0. Defines the amount of planes that can be present on the single (YUV) image.
static constexpr std::uint32_t MaxPlaneCount = 3;

/// @brief Provided by Wisdom 0.7.0. Defines the minimum amount of vertex attributes that @wis_must be supported by the
/// implementation.
static constexpr std::uint32_t MinSupportedInputAttributes = 16;

/// @brief Provided by Wisdom 0.7.0. Defines the minimum amount of vertex bindings that @wis_must be supported by the
/// implementation.
static constexpr std::uint32_t MinSupportedInputBindings = 16;

/// @brief Provided by Wisdom 0.7.0. Defines the maximum amount of render targets that can be bound at once.
static constexpr std::uint32_t MaxRenderTargets = 8;

/// @brief Provided by Wisdom 0.7.0. Defines the maximum amount of viewports that can be bound at once. The same count
/// applies to scissors.
static constexpr std::uint32_t MaxViewports = 16;

/// @brief Provided by Wisdom 0.7.0. Defines the maximum amount of present rectangles and copy regions in the Copy*
/// commands that can be used in a single operation.
static constexpr std::uint32_t MaxCopyRegions = 16;

/// @brief Provided by Wisdom 0.7.0. [internal] Defines the maximum amount of images that can be present in a swapchain
/// within any implementation.
static constexpr std::uint32_t AbsoluteMaxSwapchainImages = 16;

/// @brief Provided by Wisdom 0.7.0. Select whole size of a resource.
static constexpr std::uint64_t WholeSize = 0xffffffffffffffff;

} // namespace wis

#ifdef WISDOM_DX12
#    include <wisdom/dx12/dx12_types.hpp>

namespace wis {
using DX12TextureView = WisDX12TextureView;

using DX12BufferView = WisDX12BufferView;

using DX12SurfaceView = WisDX12SurfaceView;

using DX12PipelineView = WisDX12PipelineView;

using DX12ShaderView = WisDX12ShaderView;

using DX12PipelineCacheView = WisDX12PipelineCacheView;

using DX12RootSignatureView = WisDX12RootSignatureView;

using DX12FenceView = WisDX12FenceView;

using DX12CommandListView = WisDX12CommandListView;

/**
 * @brief Provided by Wisdom 0.7.0. Device requirements. Used to specify required features and properties for device
 * creation.
 *
 * */
struct DX12DeviceRequirements {
    wis::span<const wis::CommandQueueDesc> queue_descs;    ///< points to an array of wis::CommandQueueDesc for which
                                                           ///< queues are created during device creation.
    wis::span<wis::DX12DeviceExtensionHeader*> extensions; ///< points to an array of extensions that are to be
                                                           ///< initialized with pointers to wis::DeviceExtensionHeader.
};

/**
 * @brief Provided by Wisdom 0.7.0. Buffer barrier with the buffer handle.
 *
 * */
struct DX12BufferBarrier {
    wis::BarrierSync sync_before;      ///< indicates synchronization scope before the barrier.
    wis::BarrierSync sync_after;       ///< specifies synchronization scope after the barrier.
    wis::ResourceAccess access_before; ///< defines access scope before the barrier.
    wis::ResourceAccess access_after;  ///< indicates access scope after the barrier.
    wis::DX12BufferView buffer;        ///< specifies buffer view.
    std::uint64_t offset;              ///< indicates offset in bytes from the start of the buffer. Default is 0.
    std::uint64_t
        size; ///< indicates barrier size in bytes. Default is `UINT64_MAX`, which means the whole buffer range.
    wis::CommandQueueType queue_type_before; ///< defines type of the queue the barrier is executed on before the
                                             ///< synchronization point. Used for cross-queue barriers.
    wis::CommandQueueType queue_type_after;  ///< indicates type of the queue the barrier is executed on after the
                                             ///< synchronization point. Used for cross-queue barriers.
};

/**
 * @brief Provided by Wisdom 0.7.0. Global barrier without resource handles.
 *
 * */
struct DX12GlobalBarrier {
    wis::BarrierSync sync_before;      ///< indicates synchronization scope before the barrier.
    wis::BarrierSync sync_after;       ///< specifies synchronization scope after the barrier.
    wis::ResourceAccess access_before; ///< defines access scope before the barrier.
    wis::ResourceAccess access_after;  ///< indicates access scope after the barrier.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture barrier with the texture handle.
 *
 * */
struct DX12TextureBarrier {
    wis::BarrierSync sync_before;            ///< indicates synchronization scope before the barrier.
    wis::BarrierSync sync_after;             ///< specifies synchronization scope after the barrier.
    wis::ResourceAccess access_before;       ///< defines access scope before the barrier.
    wis::ResourceAccess access_after;        ///< indicates access scope after the barrier.
    wis::TextureState state_before;          ///< indicates texture state before the barrier.
    wis::TextureState state_after;           ///< specifies texture state after the barrier.
    wis::BarrierFlags flags;                 ///< describes barrier flags. Describe additional options for the barrier.
    wis::DX12TextureView texture;            ///< describes texture view.
    wis::SubresourceRange subresource_range; ///< specifies subresource range for the barrier.
    wis::CommandQueueType queue_type_before; ///< defines type of the queue the barrier is executed on before the
                                             ///< synchronization point. Used for cross-queue barriers.
    wis::CommandQueueType queue_type_after;  ///< indicates type of the queue the barrier is executed on after the
                                             ///< synchronization point. Used for cross-queue barriers.
};

/**
 * @brief Provided by Wisdom 0.7.0. Barrier group for multiple barriers submission.
 *
 * */
struct DX12BarrierGroup {
    wis::span<const wis::DX12BufferBarrier> buffer_barriers;   ///< points to an array of buffer barriers.
    wis::span<const wis::DX12TextureBarrier> texture_barriers; ///< points to an array of texture barriers.
    wis::span<const wis::DX12GlobalBarrier> global_barriers;   ///< points to an array of global barriers.
};

/**
 * @brief Provided by Wisdom 0.7.0. Compute pipeline description for wis::Pipeline creation.
 *
 * */
struct DX12ComputePipelineDesc {
    wis::DX12RootSignatureView root_signature; ///< describes root signature description for the pipeline.
    wis::DX12ShaderView compute_shader;        ///< indicates compute shader bytecode.
    wis::DX12PipelineCacheView cache; ///< defines pipeline cache data. Used to speed up pipeline creation if available.
    wis::PipelineFlags flags;         ///< describes pipeline flags. Describe additional options for the pipeline.
};

/**
 * @brief Provided by Wisdom 0.7.0. Graphics pipeline description for wis::Pipeline creation.
 *
 * */
struct DX12GraphicsPipelineDesc {
    wis::DX12RootSignatureView root_signature; ///< describes root signature description for the pipeline.
    wis::DX12ShaderView vertex_shader;         ///< defines vertex shader bytecode.
    wis::DX12ShaderView hull_shader;     ///< indicates hull shader bytecode. If not set, the pipeline will be created
                                         ///< without a hull shader.
    wis::DX12ShaderView domain_shader;   ///< specifies domain shader bytecode. If not set, the pipeline will be created
                                         ///< without a domain shader.
    wis::DX12ShaderView geometry_shader; ///< specifies geometry shader bytecode. If not set, the pipeline will be
                                         ///< created without a geometry shader.
    wis::DX12ShaderView pixel_shader;    ///< defines pixel shader bytecode. If not set, the pipeline will be created
                                         ///< without a pixel shader.
    wis::RenderAttachmentsDesc render_attachments; ///< indicates render attachments description for the pipeline. Used
                                                   ///< to create the compatible render pass for the pipeline.
    wis::InputLayout input_layout;   ///< describes input layout description for the pipeline. If not set, the pipeline
                                     ///< will be created without an input layout.
    wis::TopologyType topology_type; ///< specifies topology type. Default is `wis::TopologyType::Triangle`.
    const wis::RasterizerDesc* rasterizer_desc; ///< describes rasterizer description for the pipeline. If not set, the
                                                ///< pipeline will be created with default rasterizer state.
    const wis::SampleDesc* sample_desc; ///< defines sample description for the pipeline. If not set, the pipeline will
                                        ///< be created with default sample state (no multisampling).
    const wis::DepthStencilDesc*
        depth_stencil_desc; ///< defines depth stencil description for the pipeline. If not set, the pipeline will be
                            ///< created with depth testing and stencil testing disabled.
    const wis::BlendStateDesc* blend_state_desc; ///< indicates blend state description for the pipeline. If not set,
                                                 ///< the pipeline will be created with blending disabled.
    wis::DX12PipelineCacheView cache; ///< defines pipeline cache data. Used to speed up pipeline creation if available.
    wis::PipelineFlags flags;         ///< describes pipeline flags. Describe additional options for the pipeline.
};

struct DX12TextureDeleter {
    void operator()(WisDX12Texture* handle) noexcept { ::wisDX12DestroyTexture(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU texture resource.
 *
 * */
class DX12Texture : public wis::impl::Implements<wis::impl::DX12TextureImpl, WisDX12Texture, wis::DX12TextureDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD DX12TextureView GetView() const noexcept
    {
        DX12TextureView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator DX12TextureView() const noexcept { return GetView(); }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes data directly to the texture subresource. Texture @wis_must be in
     * `wis::TextureState::Common` and @wis_must_not be a depth texture.
     * @param source_data specifies a pointer to the data to write to the texture.
     * @param target_region specifies a pointer to wis::TextureRegion, which describes the region of the texture to
     * write to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteSubresource(const void* source_data, const wis::TextureRegion& target_region) const noexcept
    {
        const WisResult wis_result = ::wisDX12TextureWriteSubresource(
            &_impl_storage,
            source_data,
            reinterpret_cast<const WisTextureRegion*>(&target_region)
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
};

struct DX12BufferDeleter {
    void operator()(WisDX12Buffer* handle) noexcept { ::wisDX12DestroyBuffer(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU buffer resource.
 *
 * */
class DX12Buffer : public wis::impl::Implements<wis::impl::DX12BufferImpl, WisDX12Buffer, wis::DX12BufferDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD DX12BufferView GetView() const noexcept
    {
        DX12BufferView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator DX12BufferView() const noexcept { return GetView(); }
    /**
     * @brief Provided by Wisdom 0.7.0. Maps the buffer memory to CPU accessible address space.
     * @return void points to the pointer, which is filled with the address of the mapped memory on success.
     *
     * */
    WIS_NODISCARD inline void* Map() const noexcept { return (::wisDX12BufferMap(&_impl_storage)); }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the GPU virtual address of the buffer.
     * @return u64 Address of the buffer on GPU.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetGPUAddress() const noexcept
    {
        return (::wisDX12BufferGetGPUAddress(&_impl_storage));
    }
};

struct DX12SwapchainDeleter {
    void operator()(WisDX12Swapchain* handle) noexcept { ::wisDX12DestroySwapchain(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a swapchain, which is a collection of render targets used for
 * presentation.
 *
 * */
class DX12Swapchain
    : public wis::impl::Implements<wis::impl::DX12SwapchainImpl, WisDX12Swapchain, wis::DX12SwapchainDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Presents the swapchain image to the screen.
     * @param flags describes the presentation options.
     * @param rects points to an array of rectangles to present. If `nullptr`, the entire image is presented.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Present(wis::PresentFlags flags, wis::span<const wis::Rect> rects) const noexcept
    {
        const WisResult wis_result = ::wisDX12SwapchainPresent(
            &_impl_storage,
            static_cast<WisPresentFlags>(flags),
            reinterpret_cast<const WisRect*>(rects.data()),
            rects.size()
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the index of the current backbuffer. In case of lazy indexing it @wis_may
     * wait for presentation to finish and block.
     * @param out_result denoting the outcome of operation.
     * @return index Index of the current backbuffer.
     *
     * */
    WIS_NODISCARD inline std::uint32_t GetCurrentIndex(wis::Result& out_result) const noexcept
    {
        std::uint32_t index;
        const WisResult
            wis_result = ::wisDX12SwapchainGetCurrentIndex(&_impl_storage, reinterpret_cast<uint32_t*>(&index));
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return index;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Resizes the swapchain buffers. If the swapchain is currently in use, it
     * @wis_must be resized after the GPU finishes using it, so the call @wis_may block until then.
     * @param desc indicates a pointer to wis::SwapchainUpdateDesc, which describes the new swapchain parameters.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Update(const wis::SwapchainUpdateDesc& desc) const noexcept
    {
        const WisResult wis_result = ::wisDX12SwapchainUpdate(
            &_impl_storage,
            reinterpret_cast<const WisSwapchainUpdateDesc*>(&desc)
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the swapchain buffers. The textures are in `wis::TextureState::Common`.
     * @param buffers points to an array of texture views, which are filled with swapchain backbuffers on success.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result GetTextures(wis::span<wis::DX12Texture> buffers) const noexcept
    {
        const WisResult wis_result = ::wisDX12SwapchainGetTextures(
            &_impl_storage,
            reinterpret_cast<WisDX12Texture*>(buffers.data()),
            buffers.size()
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
};

struct DX12SurfaceDeleter {
    void operator()(WisDX12Surface* handle) noexcept { ::wisDX12DestroySurface(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU surface, which can be used as a target for rendering and
 * presentation.
 *
 * */
class DX12Surface : public wis::impl::Implements<wis::impl::DX12SurfaceImpl, WisDX12Surface, wis::DX12SurfaceDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD DX12SurfaceView GetView() const noexcept
    {
        DX12SurfaceView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator DX12SurfaceView() const noexcept { return GetView(); }
};

struct DX12ViewHeapDeleter {
    void operator()(WisDX12ViewHeap* handle) noexcept { ::wisDX12DestroyViewHeap(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for resource views used in contiguous array.
 *
 * */
class DX12ViewHeap
    : public wis::impl::Implements<wis::impl::DX12ViewHeapImpl, WisDX12ViewHeap, wis::DX12ViewHeapDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a render target view to the view heap and returns the CPU descriptor
     * handle for it.
     * @param texture describes a pointer to wis::Texture to write the descriptor for.
     * @param render_target specifies a pointer to wis::RenderTargetDesc, which describes the render target view to
     * write.
     * @param index defines the index in the view heap to write the descriptor to.
     * @return u64 CPU descriptor handle for the view heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t WriteRenderTarget(
        const wis::DX12Texture& texture,
        const wis::RenderTargetDesc& render_target,
        std::uint32_t index
    ) const noexcept
    {
        return (::wisDX12ViewHeapWriteRenderTarget(
            &_impl_storage,
            reinterpret_cast<const WisDX12Texture*>(&texture),
            reinterpret_cast<const WisRenderTargetDesc*>(&render_target),
            index
        ));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a depth stencil view to the view heap and returns the CPU descriptor
     * handle for it.
     * @param texture describes a pointer to wis::Texture to write the descriptor for.
     * @param render_target specifies a pointer to wis::RenderTargetDesc, which describes the depth stencil view to
     * write.
     * @param index defines the index in the view heap to write the descriptor to.
     * @return u64 CPU descriptor handle for the view heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t WriteDepthStencil(
        const wis::DX12Texture& texture,
        const wis::RenderTargetDesc& render_target,
        std::uint32_t index
    ) const noexcept
    {
        return (::wisDX12ViewHeapWriteDepthStencil(
            &_impl_storage,
            reinterpret_cast<const WisDX12Texture*>(&texture),
            reinterpret_cast<const WisRenderTargetDesc*>(&render_target),
            index
        ));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
     * @param index defines the index in the view heap to get the descriptor from.
     * @return u64 Address of a view in heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetViewAddress(std::uint32_t index) const noexcept
    {
        return (::wisDX12ViewHeapGetViewAddress(&_impl_storage, index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies views from one heap to another.
     * @param dst_index specifies the index in the destination view heap to copy views to.
     * @param src_ptr describes a pointer to wis::ViewHeap to copy views from.
     * @param src_index indicates the index in the source view heap to copy views from.
     * @param view_count indicates the number of views to copy.
     *
     * */
    inline void CopyViews(
        std::uint32_t dst_index,
        std::uint64_t src_ptr,
        std::uint32_t src_index,
        std::uint32_t view_count
    ) const noexcept
    {
        ::wisDX12ViewHeapCopyViews(&_impl_storage, dst_index, src_ptr, src_index, view_count);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
     * @return u64 CPU descriptor handle for the view heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetCPUHandle() const noexcept
    {
        return (::wisDX12ViewHeapGetCPUHandle(&_impl_storage));
    }
};

struct DX12PipelineDeleter {
    void operator()(WisDX12Pipeline* handle) noexcept { ::wisDX12DestroyPipeline(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU pipeline state object, which encapsulates the state of the
 * GPU pipeline and allows to execute draw and dispatch calls with it.
 *
 * */
class DX12Pipeline
    : public wis::impl::Implements<wis::impl::DX12PipelineImpl, WisDX12Pipeline, wis::DX12PipelineDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD DX12PipelineView GetView() const noexcept
    {
        DX12PipelineView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator DX12PipelineView() const noexcept { return GetView(); }
};

struct DX12ShaderDeleter {
    void operator()(WisDX12Shader* handle) noexcept { ::wisDX12DestroyShader(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU shader module, which contains shader code and allows to
 * create pipeline state objects with it.
 *
 * */
class DX12Shader : public wis::impl::Implements<wis::impl::DX12ShaderImpl, WisDX12Shader, wis::DX12ShaderDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD DX12ShaderView GetView() const noexcept
    {
        DX12ShaderView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator DX12ShaderView() const noexcept { return GetView(); }
};

struct DX12PipelineCacheDeleter {
    void operator()(WisDX12PipelineCache* handle) noexcept { ::wisDX12DestroyPipelineCache(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a cache for pipeline state objects, which allows to reuse already
 * created pipelines and speed up pipeline creation.
 *
 * */
class DX12PipelineCache
    : public wis::impl::
          Implements<wis::impl::DX12PipelineCacheImpl, WisDX12PipelineCache, wis::DX12PipelineCacheDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD DX12PipelineCacheView GetView() const noexcept
    {
        DX12PipelineCacheView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator DX12PipelineCacheView() const noexcept { return GetView(); }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the data from the pipeline cache.
     * @param data points to an array that is filled with serialized cache data on success.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Serialize(wis::span<std::uint8_t> data) const noexcept
    {
        const WisResult wis_result = ::wisDX12PipelineCacheSerialize(
            &_impl_storage,
            reinterpret_cast<uint8_t*>(data.data()),
            data.size()
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the size of the data in the pipeline cache.
     * @return size Size of the data in bytes.
     *
     * */
    WIS_NODISCARD inline std::size_t GetSerializedSize() const noexcept
    {
        return (::wisDX12PipelineCacheGetSerializedSize(&_impl_storage));
    }
};

struct DX12DescriptorHeapDeleter {
    void operator()(WisDX12DescriptorHeap* handle) noexcept { ::wisDX12DestroyDescriptorHeap(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for descriptors used in contiguous array.
 *
 * */
class DX12DescriptorHeap
    : public wis::impl::
          Implements<wis::impl::DX12DescriptorHeapImpl, WisDX12DescriptorHeap, wis::DX12DescriptorHeapDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the descriptor heap.
     * @return void CPU descriptor handle for the descriptor heap.
     *
     * */
    WIS_NODISCARD inline void* GetCPUHandle() const noexcept
    {
        return (::wisDX12DescriptorHeapGetCPUHandle(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::ConstantBuffer` descriptor to the descriptor heap.
     * @param data specifies a pointer to wis::ConstantBufferBinding, which describes the constant buffer descriptors to
     * write.
     * @param index defines the index in the descriptor heap to write the descriptors to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteConstantBuffer(const wis::ConstantBufferBinding& data, std::uint32_t index) const noexcept
    {
        const WisResult wis_result = ::wisDX12DescriptorHeapWriteConstantBuffer(
            &_impl_storage,
            reinterpret_cast<const WisConstantBufferBinding*>(&data),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::Buffer` descriptor to the descriptor heap.
     * @param buffer specifies a pointer to wis::Buffer to write the descriptor for.
     * @param data specifies a pointer to wis::BufferBinding, which describes the shader resource view descriptors to
     * write.
     * @param index defines the index in the descriptor heap to write the descriptors to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteStructuredBuffer(
        wis::DX12BufferView buffer,
        const wis::BufferBinding& data,
        std::uint32_t index
    ) const noexcept
    {
        const WisResult wis_result = ::wisDX12DescriptorHeapWriteStructuredBuffer(
            &_impl_storage,
            buffer,
            reinterpret_cast<const WisBufferBinding*>(&data),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::RWBuffer` descriptor to the descriptor heap.
     * @param buffer specifies a pointer to wis::Buffer to write the descriptor for.
     * @param data specifies a pointer to wis::BufferBinding, which describes the shader resource view descriptors to
     * write.
     * @param index defines the index in the descriptor heap to write the descriptors to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteRWStructuredBuffer(
        wis::DX12BufferView buffer,
        const wis::BufferBinding& data,
        std::uint32_t index
    ) const noexcept
    {
        const WisResult wis_result = ::wisDX12DescriptorHeapWriteRWStructuredBuffer(
            &_impl_storage,
            buffer,
            reinterpret_cast<const WisBufferBinding*>(&data),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::Sampler` descriptor to the descriptor heap.
     * @param sampler defines a pointer to wis::SamplerDesc, which describes the sampler descriptor to write.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteSampler(const wis::SamplerDesc& sampler, std::uint32_t index) const noexcept
    {
        const WisResult wis_result = ::wisDX12DescriptorHeapWriteSampler(
            &_impl_storage,
            reinterpret_cast<const WisSamplerDesc*>(&sampler),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a descriptor to the descriptor heap.
     * @param texture describes a pointer to wis::Texture to write the descriptor for.
     * @param data specifies a pointer to wis::TextureBinding, which describes the texture view to write.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteTexture(
        wis::DX12TextureView texture,
        const wis::TextureBinding& data,
        std::uint32_t index
    ) const noexcept
    {
        const WisResult wis_result = ::wisDX12DescriptorHeapWriteTexture(
            &_impl_storage,
            texture,
            reinterpret_cast<const WisTextureBinding*>(&data),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a texture view to the descriptor heap.
     * @param texture describes a pointer to wis::Texture to write the descriptor for.
     * @param data specifies a pointer to wis::TextureBinding, which describes the texture view to write.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteRWTexture(
        wis::DX12TextureView texture,
        const wis::TextureBinding& data,
        std::uint32_t index
    ) const noexcept
    {
        const WisResult wis_result = ::wisDX12DescriptorHeapWriteRWTexture(
            &_impl_storage,
            texture,
            reinterpret_cast<const WisTextureBinding*>(&data),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a raytracing acceleration to the descriptor heap.
     * @param address specifies GPU address of a raytracing acceleration structure.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteAccelerationStructure(std::uint64_t address, std::uint32_t index) const noexcept
    {
        const WisResult wis_result = ::wisDX12DescriptorHeapWriteAccelerationStructure(&_impl_storage, address, index);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies descriptors from one heap to another.
     * @param dst_index specifies the index in the destination descriptor heap to copy descriptors to.
     * @param src_ptr describes a pointer to wis::DescriptorHeap to copy descriptors from. Source heap @wis_must be CPU
     * Only heap.
     * @param src_index indicates the index in the source descriptor heap to copy descriptors from.
     * @param descriptor_count indicates the number of descriptors to copy.
     *
     * */
    inline void CopyDescriptors(
        std::uint32_t dst_index,
        const void* src_ptr,
        std::uint32_t src_index,
        std::uint32_t descriptor_count
    ) const noexcept
    {
        ::wisDX12DescriptorHeapCopyDescriptors(&_impl_storage, dst_index, src_ptr, src_index, descriptor_count);
    }
};

struct DX12RootSignatureDeleter {
    void operator()(WisDX12RootSignature* handle) noexcept { ::wisDX12DestroyRootSignature(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pipeline layout and a constant data storage, which defines
 * resource bindings for shaders.
 *
 * */
class DX12RootSignature
    : public wis::impl::
          Implements<wis::impl::DX12RootSignatureImpl, WisDX12RootSignature, wis::DX12RootSignatureDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD DX12RootSignatureView GetView() const noexcept
    {
        DX12RootSignatureView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator DX12RootSignatureView() const noexcept { return GetView(); }
};

struct DX12ResourceAllocatorDeleter {
    void operator()(WisDX12ResourceAllocator* handle) noexcept { ::wisDX12DestroyResourceAllocator(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class for allocating and managing GPU resources like buffers and textures.
 *
 * */
class DX12ResourceAllocator
    : public wis::impl::
          Implements<wis::impl::DX12ResourceAllocatorImpl, WisDX12ResourceAllocator, wis::DX12ResourceAllocatorDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a buffer with given descriptor.
     * @param desc indicates a pointer to wis::BufferDesc, which describes the buffer to create.
     * @param out_result denoting the outcome of operation.
     * @return buffer points to wis::Buffer, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Buffer CreateBuffer(
        const wis::BufferDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12Buffer buffer;
        const WisResult wis_result = ::wisDX12ResourceAllocatorCreateBuffer(
            &_impl_storage,
            reinterpret_cast<const WisBufferDesc*>(&desc),
            buffer.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return buffer;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a texture with given descriptor.
     * @param desc indicates a pointer to wis::TextureDesc, which describes the texture to create.
     * @param out_result denoting the outcome of operation.
     * @return texture points to wis::Texture, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Texture CreateTexture(
        const wis::TextureDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12Texture texture;
        const WisResult wis_result = ::wisDX12ResourceAllocatorCreateTexture(
            &_impl_storage,
            reinterpret_cast<const WisTextureDesc*>(&desc),
            texture.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return texture;
    }
};

struct DX12FenceDeleter {
    void operator()(WisDX12Fence* handle) noexcept { ::wisDX12DestroyFence(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a fence for GPU-CPU and GPU-GPU synchronization.
 *
 * */
class DX12Fence : public wis::impl::Implements<wis::impl::DX12FenceImpl, WisDX12Fence, wis::DX12FenceDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD DX12FenceView GetView() const noexcept
    {
        DX12FenceView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator DX12FenceView() const noexcept { return GetView(); }
    /**
     * @brief Provided by Wisdom 0.7.0. Get the current value of the fence.
     * @return u64 Value of the fence.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetCompletedValue() const noexcept
    {
        return (::wisDX12FenceGetCompletedValue(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Wait on CPU for the fence to reach a certain value.
     * @param value describes value to wait for.
     * @param wait_ns describes the time to wait for the fence to reach the value in nanoseconds. Default is infinite.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Wait(std::uint64_t value, std::uint64_t wait_ns) const noexcept
    {
        const WisResult wis_result = ::wisDX12FenceWait(&_impl_storage, value, wait_ns);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Signal the fence from CPU.
     * @param value describes value to signal.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Signal(std::uint64_t value) const noexcept
    {
        const WisResult wis_result = ::wisDX12FenceSignal(&_impl_storage, value);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
};

struct DX12CommandListDeleter {
    void operator()(WisDX12CommandList* handle) noexcept { ::wisDX12DestroyCommandList(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command list for recording GPU commands.
 *
 * */
class DX12CommandList
    : public wis::impl::Implements<wis::impl::DX12CommandListImpl, WisDX12CommandList, wis::DX12CommandListDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD DX12CommandListView GetView() const noexcept
    {
        DX12CommandListView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator DX12CommandListView() const noexcept { return GetView(); }
    /**
     * @brief Provided by Wisdom 0.7.0. Opens the command list, so commands can be recorded to it.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Begin() const noexcept
    {
        const WisResult wis_result = ::wisDX12CommandListBegin(&_impl_storage);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result End() const noexcept
    {
        const WisResult wis_result = ::wisDX12CommandListEnd(&_impl_storage);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Binds descriptor heaps to the command list, so they can be used for resource
     * binding.
     * @param resource_heap describes a pointer to wis::DescriptorHeap with shader resource views, unordered access
     * views and constant buffer views. If `nullptr`, no resource heap is bound.
     * @param sampler_heap describes a pointer to wis::DescriptorHeap with samplers. If `nullptr`, no sampler heap is
     * bound.
     *
     * */
    inline void SetDescriptorHeaps(
        const wis::DX12DescriptorHeap* resource_heap,
        const wis::DX12DescriptorHeap* sampler_heap
    ) const noexcept
    {
        ::wisDX12CommandListSetDescriptorHeaps(
            &_impl_storage,
            reinterpret_cast<const WisDX12DescriptorHeap*>(resource_heap),
            reinterpret_cast<const WisDX12DescriptorHeap*>(sampler_heap)
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the root signature for the command list, so it can be used for resource
     * binding.
     * @param signature specifies a pointer to wis::RootSignature to set.
     * @param pipeline specifies the pipeline type to set the root signature for.
     *
     * */
    inline void SetRootSignature(wis::DX12RootSignatureView signature, wis::PipelineType pipeline) const noexcept
    {
        ::wisDX12CommandListSetRootSignature(&_impl_storage, signature, static_cast<WisPipelineType>(pipeline));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the push constants for the command list, so they can be used for resource
     * binding.
     * @param data specifies a pointer to , which describes the push constant data to set.
     *
     * */
    inline void SetPushConstants(const wis::PushConstantDataDesc& data) const noexcept
    {
        ::wisDX12CommandListSetPushConstants(&_impl_storage, reinterpret_cast<const WisPushConstantDataDesc*>(&data));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the push descriptors for the command list, so they can be used for resource
     * binding.
     * @param data specifies a pointer to , which describes the push descriptors to set.
     *
     * */
    inline void SetPushDescriptor(const wis::PushDescriptorDataDesc& data) const noexcept
    {
        ::wisDX12CommandListSetPushDescriptor(
            &_impl_storage,
            reinterpret_cast<const WisPushDescriptorDataDesc*>(&data)
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the descriptor table offset in descriptor heap for the command list, so it
     * can be used for resource binding.
     * @param data specifies the root parameter index to set the descriptor table for.
     *
     * */
    inline void SetDescriptorTable(const wis::DescriptorTableDataDesc& data) const noexcept
    {
        ::wisDX12CommandListSetDescriptorTable(
            &_impl_storage,
            reinterpret_cast<const WisDescriptorTableDataDesc*>(&data)
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Inserts one or more barriers on the current command list.
     * @param barriers specifies a pointer to an array of barriers to insert.
     *
     * */
    inline void InsertBarriers(const wis::DX12BarrierGroup& barriers) const noexcept
    {
        ::wisDX12CommandListInsertBarriers(&_impl_storage, reinterpret_cast<const WisDX12BarrierGroup*>(&barriers));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the pipeline state object for the command list, so it can be used for draw
     * and dispatch calls.
     * @param pipeline specifies a pointer to wis::Pipeline to set.
     * @param type specifies the pipeline type to set the pipeline for.
     *
     * */
    inline void SetPipeline(wis::DX12PipelineView pipeline, wis::PipelineType type) const noexcept
    {
        ::wisDX12CommandListSetPipeline(&_impl_storage, pipeline, static_cast<WisPipelineType>(type));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets multiple viewports.
     * @param viewports points to an array of viewports to set.
     *
     * */
    inline void SetViewports(wis::span<const wis::Viewport> viewports) noexcept
    {
        ::wisDX12CommandListSetViewports(
            &_impl_storage,
            reinterpret_cast<const WisViewport*>(viewports.data()),
            viewports.size()
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets multiple scissor rects.
     * Each n-th rect corresponds to n-th Viewport set in RSSetViewports if SV_ViewportArrayIndex is used in geometry
     * shader. Otherwise the first is chosen.
     * @param scissor_rects points to an array of scissor rectangles to set.
     *
     * */
    inline void SetScissors(wis::span<const wis::Rect> scissor_rects) noexcept
    {
        ::wisDX12CommandListSetScissors(
            &_impl_storage,
            reinterpret_cast<const WisRect*>(scissor_rects.data()),
            scissor_rects.size()
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the primitive topology. Determines how vertices shall be processed.
     * @param topology describes primitive topology to set.
     *
     * */
    inline void SetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept
    {
        ::wisDX12CommandListSetPrimitiveTopology(&_impl_storage, static_cast<WisPrimitiveTopology>(topology));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the depth bias. Determines how depth values are modified during
     * rasterization.
     * @param depth_bias indicates depth bias to set.
     * @param depth_bias_clamp indicates depth bias clamp to set.
     * @param slope_scaled_depth_bias defines slope-scaled depth bias to set.
     *
     * */
    inline void SetDepthBias(float depth_bias, float depth_bias_clamp, float slope_scaled_depth_bias) noexcept
    {
        ::wisDX12CommandListSetDepthBias(&_impl_storage, depth_bias, depth_bias_clamp, slope_scaled_depth_bias);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the primitive restart value. Determines the index value which is treated as
     * a primitive restart when using indexed draw calls.
     * @param restart_value describes primitive restart value to set.
     *
     * */
    inline void SetPrimitiveRestartValue(wis::PrimitiveRestartValue restart_value) noexcept
    {
        ::wisDX12CommandListSetPrimitiveRestartValue(
            &_impl_storage,
            static_cast<WisPrimitiveRestartValue>(restart_value)
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Dispatches compute shader.
     * @param group_count_x defines number of groups to dispatch in X dimension.
     * @param group_count_y describes number of groups to dispatch in Y dimension; default is 1.
     * @param group_count_z specifies number of groups to dispatch in Z dimension; default is 1.
     *
     * */
    inline void Dispatch(
        std::uint32_t group_count_x,
        std::uint32_t group_count_y,
        std::uint32_t group_count_z
    ) const noexcept
    {
        ::wisDX12CommandListDispatch(&_impl_storage, group_count_x, group_count_y, group_count_z);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Draws non-indexed geometry.
     * @param vertex_count specifies number of vertices to draw.
     * @param instance_count describes number of instances to draw; default is 1.
     * @param start_vertex indicates index of the first vertex to draw; default is 0.
     * @param start_instance specifies index of the first instance to draw; default is 0.
     *
     * */
    inline void Draw(
        std::uint32_t vertex_count,
        std::uint32_t instance_count,
        std::uint32_t start_vertex,
        std::uint32_t start_instance
    ) const noexcept
    {
        ::wisDX12CommandListDraw(&_impl_storage, vertex_count, instance_count, start_vertex, start_instance);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Draws indexed geometry.
     * @param index_count defines number of indices to draw.
     * @param instance_count describes number of instances to draw; default is 1.
     * @param start_index describes index of the first index to draw; default is 0.
     * @param base_vertex defines value added to each index before reading a vertex from the vertex buffer; default is
     * 0.
     * @param start_instance specifies index of the first instance to draw; default is 0.
     *
     * */
    inline void DrawIndexed(
        std::uint32_t index_count,
        std::uint32_t instance_count,
        std::uint32_t start_index,
        std::int32_t base_vertex,
        std::uint32_t start_instance
    ) const noexcept
    {
        ::wisDX12CommandListDrawIndexed(
            &_impl_storage,
            index_count,
            instance_count,
            start_index,
            base_vertex,
            start_instance
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Begins a render pass with given descriptor.
     * @param desc indicates a pointer to wis::RenderPassDesc, which describes the render pass to begin.
     *
     * */
    inline void BeginRenderPass(const wis::RenderPassDesc& desc) const noexcept
    {
        ::wisDX12CommandListBeginRenderPass(&_impl_storage, reinterpret_cast<const WisRenderPassDesc*>(&desc));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Ends the current render pass.
     *
     * */
    inline void EndRenderPass() const noexcept { ::wisDX12CommandListEndRenderPass(&_impl_storage); }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies regions from one buffer to another.
     * @param dst_buffer defines a pointer to the destination buffer.
     * @param src_buffer describes a pointer to the source buffer.
     * @param regions points to an array of wis::BufferCopyRegion that defines the copy regions.
     *
     * */
    inline void CopyBuffer(
        wis::DX12BufferView dst_buffer,
        wis::DX12BufferView src_buffer,
        wis::span<const wis::BufferCopyRegion> regions
    ) const noexcept
    {
        ::wisDX12CommandListCopyBuffer(
            &_impl_storage,
            dst_buffer,
            src_buffer,
            reinterpret_cast<const WisBufferCopyRegion*>(regions.data()),
            regions.size()
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies regions from a buffer to a texture.
     * @param dst_texture defines a pointer to the destination texture. Texture @wis_must be in
     * `wis::TextureState::CopyDst`.
     * @param src_buffer describes a pointer to the source buffer.
     * @param regions points to an array of wis::BufferTextureCopyRegion that defines the copy regions.
     *
     * */
    inline void CopyBufferToTexture(
        wis::DX12TextureView dst_texture,
        wis::DX12BufferView src_buffer,
        wis::span<const wis::BufferTextureCopyRegion> regions
    ) const noexcept
    {
        ::wisDX12CommandListCopyBufferToTexture(
            &_impl_storage,
            dst_texture,
            src_buffer,
            reinterpret_cast<const WisBufferTextureCopyRegion*>(regions.data()),
            regions.size()
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies regions from a texture to a buffer.
     * @param dst_buffer defines a pointer to the destination buffer.
     * @param src_texture describes a pointer to the source texture. Texture @wis_must be in
     * `wis::TextureState::CopySrc`.
     * @param regions points to an array of wis::BufferTextureCopyRegion that defines the copy regions.
     *
     * */
    inline void CopyTextureToBuffer(
        wis::DX12BufferView dst_buffer,
        wis::DX12TextureView src_texture,
        wis::span<const wis::BufferTextureCopyRegion> regions
    ) const noexcept
    {
        ::wisDX12CommandListCopyTextureToBuffer(
            &_impl_storage,
            dst_buffer,
            src_texture,
            reinterpret_cast<const WisBufferTextureCopyRegion*>(regions.data()),
            regions.size()
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies regions from one texture to another.
     * @param dst_texture defines a pointer to the destination texture. Texture @wis_must be in
     * `wis::TextureState::CopyDst`.
     * @param src_texture describes a pointer to the source texture. Texture @wis_must be in
     * `wis::TextureState::CopySrc`.
     * @param regions points to an array of wis::TextureCopyRegion that defines the copy regions.
     *
     * */
    inline void CopyTexture(
        wis::DX12TextureView dst_texture,
        wis::DX12TextureView src_texture,
        wis::span<const wis::TextureCopyRegion> regions
    ) const noexcept
    {
        ::wisDX12CommandListCopyTexture(
            &_impl_storage,
            dst_texture,
            src_texture,
            reinterpret_cast<const WisTextureCopyRegion*>(regions.data()),
            regions.size()
        );
    }
};

struct DX12CommandAllocatorDeleter {
    void operator()(WisDX12CommandAllocator* handle) noexcept { ::wisDX12DestroyCommandAllocator(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pool allocator for command lists for recording GPU commands.
 *
 * */
class DX12CommandAllocator
    : public wis::impl::
          Implements<wis::impl::DX12CommandAllocatorImpl, WisDX12CommandAllocator, wis::DX12CommandAllocatorDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Resets the command allocator, so it can be reused for allocating new command
     * lists.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Reset() const noexcept
    {
        const WisResult wis_result = ::wisDX12CommandAllocatorReset(&_impl_storage);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a command list of given type.
     * @param out_result denoting the outcome of operation.
     * @return list points to wis::CommandList, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12CommandList CreateCommandList(wis::Result& out_result) const noexcept
    {
        wis::DX12CommandList list;
        const WisResult wis_result = ::wisDX12CommandAllocatorCreateCommandList(&_impl_storage, list.GetStorage());
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return list;
    }
};

struct DX12CommandQueueDeleter {
    void operator()(WisDX12CommandQueue* handle) noexcept { ::wisDX12DestroyCommandQueue(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command queue for submitting command lists to the GPU.
 *
 * */
class DX12CommandQueue
    : public wis::impl::Implements<wis::impl::DX12CommandQueueImpl, WisDX12CommandQueue, wis::DX12CommandQueueDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Executes the command lists.
     * @param lists points to an array of command lists to execute.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Submit(wis::span<const wis::DX12CommandListView> lists) const noexcept
    {
        const WisResult wis_result = ::wisDX12CommandQueueSubmit(
            &_impl_storage,
            reinterpret_cast<const WisDX12CommandListView*>(lists.data()),
            lists.size()
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Enqueue the signal to the queue, that gets executed after all the work has been
     * done.
     * @param fence describes fence to signal.
     * @param value describes value used to signal the fence.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result SignalFence(wis::DX12FenceView fence, std::uint64_t value) const noexcept
    {
        const WisResult wis_result = ::wisDX12CommandQueueSignalFence(&_impl_storage, fence, value);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Enqueues wait operation to the command queue. Queue then waits for the fence to
     * be signalled from CPU or from another queue. Can still be enqueued after the signal.
     * @param fence describes fence to wait on.
     * @param value describes value that the fence is expected to reach.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WaitFence(wis::DX12FenceView fence, std::uint64_t value) const noexcept
    {
        const WisResult wis_result = ::wisDX12CommandQueueWaitFence(&_impl_storage, fence, value);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
};

struct DX12DeviceDeleter {
    void operator()(WisDX12Device* handle) noexcept { ::wisDX12DestroyDevice(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Central class representing logical device.
 *
 * */
class DX12Device : public wis::impl::Implements<wis::impl::DX12DeviceImpl, WisDX12Device, wis::DX12DeviceDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a command queue of given type.
     * @param type specifies the type of the command queue to create.
     * @param out_result denoting the outcome of operation.
     * @return queue points to wis::CommandQueue, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12CommandQueue CreateCommandQueue(
        wis::CommandQueueType type,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12CommandQueue queue;
        const WisResult wis_result = ::wisDX12DeviceCreateCommandQueue(
            &_impl_storage,
            static_cast<WisCommandQueueType>(type),
            queue.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return queue;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a command allocator to allocate command lists with.
     * @param type specifies the type of the command list this pool is able to allocate.
     * @param out_result denoting the outcome of operation.
     * @return allocator points to wis::CommandAllocator, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12CommandAllocator CreateCommandAllocator(
        wis::CommandQueueType type,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12CommandAllocator allocator;
        const WisResult wis_result = ::wisDX12DeviceCreateCommandAllocator(
            &_impl_storage,
            static_cast<WisCommandQueueType>(type),
            allocator.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return allocator;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a fence for GPU-CPU and GPU-GPU synchronization.
     * @param initial_value specifies the initial value of the fence.
     * @param out_result denoting the outcome of operation.
     * @return fence points to wis::Fence, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Fence CreateFence(std::uint64_t initial_value, wis::Result& out_result) const noexcept
    {
        wis::DX12Fence fence;
        const WisResult wis_result = ::wisDX12DeviceCreateFence(&_impl_storage, initial_value, fence.GetStorage());
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return fence;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a resource allocator for managing GPU resources.
     * @param out_result denoting the outcome of operation.
     * @return allocator points to wis::ResourceAllocator, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12ResourceAllocator GetResourceAllocator(wis::Result& out_result) const noexcept
    {
        wis::DX12ResourceAllocator allocator;
        const WisResult wis_result = ::wisDX12DeviceGetResourceAllocator(&_impl_storage, allocator.GetStorage());
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return allocator;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a pipeline layout with given descriptor.
     * @param desc indicates a pointer to wis::RootSignatureDesc, which describes the pipeline layout to create.
     * @param out_result denoting the outcome of operation.
     * @return layout points to wis::RootSignature, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12RootSignature CreateRootSignature(
        const wis::RootSignatureDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12RootSignature layout;
        const WisResult wis_result = ::wisDX12DeviceCreateRootSignature(
            &_impl_storage,
            reinterpret_cast<const WisRootSignatureDesc*>(&desc),
            layout.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return layout;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a descriptor storage with given description.
     * @param desc indicates a pointer to wis::DescriptorHeapDesc, which describes the descriptor heap to create.
     * @param out_result denoting the outcome of operation.
     * @return heap points to wis::DescriptorHeap, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12DescriptorHeap CreateDescriptorHeap(
        const wis::DescriptorHeapDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12DescriptorHeap heap;
        const WisResult wis_result = ::wisDX12DeviceCreateDescriptorHeap(
            &_impl_storage,
            reinterpret_cast<const WisDescriptorHeapDesc*>(&desc),
            heap.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return heap;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a view storage with given descriptor.
     * @param type specifies the type of the view heap to create.
     * @param capacity specifies the capacity in descriptors of the view heap to create.
     * @param out_result denoting the outcome of operation.
     * @return heap points to wis::ViewHeap, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12ViewHeap CreateViewHeap(
        wis::ViewHeapType type,
        std::uint32_t capacity,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12ViewHeap heap;
        const WisResult wis_result = ::wisDX12DeviceCreateViewHeap(
            &_impl_storage,
            static_cast<WisViewHeapType>(type),
            capacity,
            heap.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return heap;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Queries the properties of the device.
     * @param properties describes a pointer to one of the query structs, which is filled with device properties.
     *
     * */
    inline void QueryProperties(void* properties) const noexcept
    {
        ::wisDX12DeviceQueryProperties(&_impl_storage, properties);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Waits on multiple fences simultaneously.
     * If wait_for is `wis::MultiWaitType::All`, waits for all fences to be signaled.
     * Otherwise waits for any fence to be signaled.
     * @param fences defines an array of fence views to wait on.
     * @param fence_values defines fence values to wait for; array @wis_must have fence_count values.
     * @param fence_count describes number of fences to wait on.
     * @param wait_for indicates the kind of wait, where All waits for all fences and Any waits for any fence; default
     * is `wis::MultiWaitType::All`.
     * @param timeout indicates timeout in nanoseconds; if UINT64_MAX, waits indefinitely.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WaitForMultipleFences(
        const wis::DX12FenceView* fences,
        const std::uint64_t* fence_values,
        std::size_t fence_count,
        wis::MultiWaitType wait_for,
        std::uint64_t timeout
    ) const noexcept
    {
        const WisResult wis_result = ::wisDX12DeviceWaitForMultipleFences(
            &_impl_storage,
            fences,
            fence_values,
            fence_count,
            static_cast<WisMultiWaitType>(wait_for),
            timeout
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a pipeline cache for caching pipeline state objects.
     * @param initial_data points to an array of initial cache data bytes. If `nullptr`, the cache is created empty.
     * @param out_result denoting the outcome of operation.
     * @return cache points to wis::PipelineCache, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12PipelineCache CreatePipelineCache(
        wis::span<const std::uint8_t> initial_data,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12PipelineCache cache;
        const WisResult wis_result = ::wisDX12DeviceCreatePipelineCache(
            &_impl_storage,
            reinterpret_cast<const uint8_t*>(initial_data.data()),
            initial_data.size(),
            cache.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return cache;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a shader module from given data.
     * @param data points to an array of shader bytecode.
     * @param out_result denoting the outcome of operation.
     * @return shader points to wis::Shader, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Shader CreateShader(
        wis::span<const std::uint8_t> data,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12Shader shader;
        const WisResult wis_result = ::wisDX12DeviceCreateShader(
            &_impl_storage,
            reinterpret_cast<const uint8_t*>(data.data()),
            data.size(),
            shader.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return shader;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a compute pipeline state object with given descriptor.
     * @param desc indicates a pointer to wis::ComputePipelineDesc, which describes the compute pipeline to create.
     * @param out_result denoting the outcome of operation.
     * @return pipeline points to wis::Pipeline, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Pipeline CreateComputePipeline(
        const wis::DX12ComputePipelineDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12Pipeline pipeline;
        const WisResult wis_result = ::wisDX12DeviceCreateComputePipeline(
            &_impl_storage,
            reinterpret_cast<const WisDX12ComputePipelineDesc*>(&desc),
            pipeline.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return pipeline;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a graphics pipeline state object with given descriptor.
     * @param desc indicates a pointer to wis::GraphicsPipelineDesc, which describes the graphics pipeline to create.
     * @param out_result denoting the outcome of operation.
     * @return pipeline points to wis::Pipeline, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Pipeline CreateGraphicsPipeline(
        const wis::DX12GraphicsPipelineDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12Pipeline pipeline;
        const WisResult wis_result = ::wisDX12DeviceCreateGraphicsPipeline(
            &_impl_storage,
            reinterpret_cast<const WisDX12GraphicsPipelineDesc*>(&desc),
            pipeline.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return pipeline;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Checks if the surface format is supported for presentation and returns the
     * supported format.
     * @param surface describes a pointer to wis::Surface to check the presentation support for.
     * @param format describes the format to check the presentation support for.
     * @return bool Result of operation.
     *
     * */
    WIS_NODISCARD inline bool GetFormatPresentationSupport(
        wis::DX12SurfaceView surface,
        wis::DataFormat format
    ) const noexcept
    {
        return (
            ::wisDX12DeviceGetFormatPresentationSupport(&_impl_storage, surface, static_cast<WisDataFormat>(format))
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets presentation parameters for the specified surface.
     * @param surface describes a pointer to wis::Surface to check the presentation support for.
     * @param out_result denoting the outcome of operation.
     * @return params Parameters of the surface.
     *
     * */
    WIS_NODISCARD inline wis::SurfaceParameters GetSurfaceParameters(
        wis::DX12SurfaceView surface,
        wis::Result& out_result
    ) const noexcept
    {
        wis::SurfaceParameters params;
        const WisResult wis_result = ::wisDX12DeviceGetSurfaceParameters(
            &_impl_storage,
            surface,
            reinterpret_cast<WisSurfaceParameters*>(&params)
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return params;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a swapchain for given surface with given descriptor.
     * @param surface describes a pointer to wis::Surface to create the swapchain for. Surface is ref-counted.
     * @param queue describes a pointer to wis::CommandQueue to create the swapchain for. Queue is ref-counted and
     * @wis_must be a graphics queue.
     * @param desc indicates a pointer to wis::SwapchainDesc, which describes the swapchain to create.
     * @param out_result denoting the outcome of operation.
     * @return swapchain points to wis::Swapchain, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Swapchain CreateSwapchain(
        const wis::DX12Surface& surface,
        const wis::DX12CommandQueue& queue,
        const wis::SwapchainDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12Swapchain swapchain;
        const WisResult wis_result = ::wisDX12DeviceCreateSwapchain(
            &_impl_storage,
            reinterpret_cast<const WisDX12Surface*>(&surface),
            reinterpret_cast<const WisDX12CommandQueue*>(&queue),
            reinterpret_cast<const WisSwapchainDesc*>(&desc),
            swapchain.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return swapchain;
    }
};

struct DX12AdapterQueryDeleter {
    void operator()(WisDX12AdapterQuery* handle) noexcept { ::wisDX12DestroyAdapterQuery(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class that contains a snapshot of adapters that are present on the system.
 *
 * */
class DX12AdapterQuery
    : public wis::impl::Implements<wis::impl::DX12AdapterQueryImpl, WisDX12AdapterQuery, wis::DX12AdapterQueryDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the number of adapters present on the system at the time of the query.
     * @return size is a number of adapters present on the system.
     *
     * */
    WIS_NODISCARD inline std::size_t GetAdapterCount() const noexcept
    {
        return (::wisDX12AdapterQueryGetAdapterCount(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the description of the adapter at given index.
     * @param index defines the index of the adapter to get the description for. It @wis_must be less than the value
     * returned by wis::AdapterQuery::GetAdapterCount.
     * @param out_result denoting the outcome of operation.
     * @return desc points to wis::AdapterDesc, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::AdapterDesc GetAdapterDesc(std::size_t index, wis::Result& out_result) const noexcept
    {
        wis::AdapterDesc desc;
        const WisResult wis_result = ::wisDX12AdapterQueryGetAdapterDesc(
            &_impl_storage,
            index,
            reinterpret_cast<WisAdapterDesc*>(&desc)
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return desc;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Checks if the adapter at given index supports presentation to given surface.
     * @param index defines the index of the adapter to check the support for. It @wis_must be less than the value
     * returned by wis::AdapterQuery::GetAdapterCount.
     * @param surface describes a pointer to wis::Surface to check the presentation support for.
     * @return bool `true` if the adapter supports presentation to the surface, `false` otherwise.
     *
     * */
    WIS_NODISCARD inline bool GetSurfaceSupport(std::size_t index, wis::DX12SurfaceView surface) const noexcept
    {
        return (::wisDX12AdapterQueryGetSurfaceSupport(&_impl_storage, index, surface));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates the device for the adapter at given index.
     * @param index defines the index of the adapter to create the device for. It @wis_must be less than the value
     * returned by wis::AdapterQuery::GetAdapterCount.
     * @param requirements defines a pointer to wis::DeviceRequirements, which defines required features and properties
     * for device creation.
     * @param out_result denoting the outcome of operation.
     * @return device points to wis::Device, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12Device CreateDevice(
        std::size_t index,
        const wis::DX12DeviceRequirements& requirements,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12Device device;
        const WisResult wis_result = ::wisDX12AdapterQueryCreateDevice(
            &_impl_storage,
            index,
            reinterpret_cast<const WisDX12DeviceRequirements*>(&requirements),
            device.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return device;
    }
};

struct DX12InstanceDeleter {
    void operator()(WisDX12Instance* handle) noexcept { ::wisDX12DestroyInstance(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
class DX12Instance
    : public wis::impl::Implements<wis::impl::DX12InstanceImpl, WisDX12Instance, wis::DX12InstanceDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Queries all the system adapters and allows to iterate through them. The order of
     * adapters depends on `preference`: for MinConsumption - DirectX 12: Integrated, Discrete, External, Software;
     * Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU. For Performance - DirectX 12: External, Discrete,
     * Integrated, Software; Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
     * @param preference indicates the order in which adapters are listed.
     * @param out_result denoting the outcome of operation.
     * @return query points to wis::AdapterQuery, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::DX12AdapterQuery QueryAdapters(
        wis::AdapterPreference preference,
        wis::Result& out_result
    ) const noexcept
    {
        wis::DX12AdapterQuery query;
        const WisResult wis_result = ::wisDX12InstanceQueryAdapters(
            &_impl_storage,
            static_cast<WisAdapterPreference>(preference),
            query.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return query;
    }
};

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_desc describes a pointer to wis::DebugDesc, which defines debug callback and debug layer usage. If
 * `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to
 * wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success.
 *
 * */
WIS_NODISCARD inline wis::DX12Instance DX12CreateInstance(
    const wis::DebugDesc* debug_desc,
    wis::span<wis::DX12InstanceExtensionHeader*> extensions,
    wis::Result& out_result
) noexcept
{
    wis::DX12Instance instance;
    const WisResult wis_result = ::wisDX12CreateInstance(
        reinterpret_cast<const WisDebugDesc*>(debug_desc),
        reinterpret_cast<WisDX12InstanceExtensionHeader**>(extensions.data()),
        extensions.size(),
        instance.GetStorage()
    );
    out_result = wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    return instance;
}

} // namespace wis
#endif // WISDOM_DX12

#ifdef WISDOM_VULKAN
#    include <wisdom/vulkan/vk_types.hpp>

namespace wis {
using VKTextureView = WisVKTextureView;

using VKBufferView = WisVKBufferView;

using VKSurfaceView = WisVKSurfaceView;

using VKPipelineView = WisVKPipelineView;

using VKShaderView = WisVKShaderView;

using VKPipelineCacheView = WisVKPipelineCacheView;

using VKRootSignatureView = WisVKRootSignatureView;

using VKFenceView = WisVKFenceView;

using VKCommandListView = WisVKCommandListView;

/**
 * @brief Provided by Wisdom 0.7.0. Device requirements. Used to specify required features and properties for device
 * creation.
 *
 * */
struct VKDeviceRequirements {
    wis::span<const wis::CommandQueueDesc> queue_descs;  ///< points to an array of wis::CommandQueueDesc for which
                                                         ///< queues are created during device creation.
    wis::span<wis::VKDeviceExtensionHeader*> extensions; ///< points to an array of extensions that are to be
                                                         ///< initialized with pointers to wis::DeviceExtensionHeader.
};

/**
 * @brief Provided by Wisdom 0.7.0. Buffer barrier with the buffer handle.
 *
 * */
struct VKBufferBarrier {
    wis::BarrierSync sync_before;      ///< indicates synchronization scope before the barrier.
    wis::BarrierSync sync_after;       ///< specifies synchronization scope after the barrier.
    wis::ResourceAccess access_before; ///< defines access scope before the barrier.
    wis::ResourceAccess access_after;  ///< indicates access scope after the barrier.
    wis::VKBufferView buffer;          ///< specifies buffer view.
    std::uint64_t offset;              ///< indicates offset in bytes from the start of the buffer. Default is 0.
    std::uint64_t
        size; ///< indicates barrier size in bytes. Default is `UINT64_MAX`, which means the whole buffer range.
    wis::CommandQueueType queue_type_before; ///< defines type of the queue the barrier is executed on before the
                                             ///< synchronization point. Used for cross-queue barriers.
    wis::CommandQueueType queue_type_after;  ///< indicates type of the queue the barrier is executed on after the
                                             ///< synchronization point. Used for cross-queue barriers.
};

/**
 * @brief Provided by Wisdom 0.7.0. Global barrier without resource handles.
 *
 * */
struct VKGlobalBarrier {
    wis::BarrierSync sync_before;      ///< indicates synchronization scope before the barrier.
    wis::BarrierSync sync_after;       ///< specifies synchronization scope after the barrier.
    wis::ResourceAccess access_before; ///< defines access scope before the barrier.
    wis::ResourceAccess access_after;  ///< indicates access scope after the barrier.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture barrier with the texture handle.
 *
 * */
struct VKTextureBarrier {
    wis::BarrierSync sync_before;            ///< indicates synchronization scope before the barrier.
    wis::BarrierSync sync_after;             ///< specifies synchronization scope after the barrier.
    wis::ResourceAccess access_before;       ///< defines access scope before the barrier.
    wis::ResourceAccess access_after;        ///< indicates access scope after the barrier.
    wis::TextureState state_before;          ///< indicates texture state before the barrier.
    wis::TextureState state_after;           ///< specifies texture state after the barrier.
    wis::BarrierFlags flags;                 ///< describes barrier flags. Describe additional options for the barrier.
    wis::VKTextureView texture;              ///< describes texture view.
    wis::SubresourceRange subresource_range; ///< specifies subresource range for the barrier.
    wis::CommandQueueType queue_type_before; ///< defines type of the queue the barrier is executed on before the
                                             ///< synchronization point. Used for cross-queue barriers.
    wis::CommandQueueType queue_type_after;  ///< indicates type of the queue the barrier is executed on after the
                                             ///< synchronization point. Used for cross-queue barriers.
};

/**
 * @brief Provided by Wisdom 0.7.0. Barrier group for multiple barriers submission.
 *
 * */
struct VKBarrierGroup {
    wis::span<const wis::VKBufferBarrier> buffer_barriers;   ///< points to an array of buffer barriers.
    wis::span<const wis::VKTextureBarrier> texture_barriers; ///< points to an array of texture barriers.
    wis::span<const wis::VKGlobalBarrier> global_barriers;   ///< points to an array of global barriers.
};

/**
 * @brief Provided by Wisdom 0.7.0. Compute pipeline description for wis::Pipeline creation.
 *
 * */
struct VKComputePipelineDesc {
    wis::VKRootSignatureView root_signature; ///< describes root signature description for the pipeline.
    wis::VKShaderView compute_shader;        ///< indicates compute shader bytecode.
    wis::VKPipelineCacheView cache; ///< defines pipeline cache data. Used to speed up pipeline creation if available.
    wis::PipelineFlags flags;       ///< describes pipeline flags. Describe additional options for the pipeline.
};

/**
 * @brief Provided by Wisdom 0.7.0. Graphics pipeline description for wis::Pipeline creation.
 *
 * */
struct VKGraphicsPipelineDesc {
    wis::VKRootSignatureView root_signature; ///< describes root signature description for the pipeline.
    wis::VKShaderView vertex_shader;         ///< defines vertex shader bytecode.
    wis::VKShaderView hull_shader; ///< indicates hull shader bytecode. If not set, the pipeline will be created without
                                   ///< a hull shader.
    wis::VKShaderView domain_shader;   ///< specifies domain shader bytecode. If not set, the pipeline will be created
                                       ///< without a domain shader.
    wis::VKShaderView geometry_shader; ///< specifies geometry shader bytecode. If not set, the pipeline will be created
                                       ///< without a geometry shader.
    wis::VKShaderView pixel_shader; ///< defines pixel shader bytecode. If not set, the pipeline will be created without
                                    ///< a pixel shader.
    wis::RenderAttachmentsDesc render_attachments; ///< indicates render attachments description for the pipeline. Used
                                                   ///< to create the compatible render pass for the pipeline.
    wis::InputLayout input_layout;   ///< describes input layout description for the pipeline. If not set, the pipeline
                                     ///< will be created without an input layout.
    wis::TopologyType topology_type; ///< specifies topology type. Default is `wis::TopologyType::Triangle`.
    const wis::RasterizerDesc* rasterizer_desc; ///< describes rasterizer description for the pipeline. If not set, the
                                                ///< pipeline will be created with default rasterizer state.
    const wis::SampleDesc* sample_desc; ///< defines sample description for the pipeline. If not set, the pipeline will
                                        ///< be created with default sample state (no multisampling).
    const wis::DepthStencilDesc*
        depth_stencil_desc; ///< defines depth stencil description for the pipeline. If not set, the pipeline will be
                            ///< created with depth testing and stencil testing disabled.
    const wis::BlendStateDesc* blend_state_desc; ///< indicates blend state description for the pipeline. If not set,
                                                 ///< the pipeline will be created with blending disabled.
    wis::VKPipelineCacheView cache; ///< defines pipeline cache data. Used to speed up pipeline creation if available.
    wis::PipelineFlags flags;       ///< describes pipeline flags. Describe additional options for the pipeline.
};

struct VKTextureDeleter {
    void operator()(WisVKTexture* handle) noexcept { ::wisVKDestroyTexture(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU texture resource.
 *
 * */
class VKTexture : public wis::impl::Implements<wis::impl::VKTextureImpl, WisVKTexture, wis::VKTextureDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKTextureView GetView() const noexcept
    {
        VKTextureView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKTextureView() const noexcept { return GetView(); }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes data directly to the texture subresource. Texture @wis_must be in
     * `wis::TextureState::Common` and @wis_must_not be a depth texture.
     * @param source_data specifies a pointer to the data to write to the texture.
     * @param target_region specifies a pointer to wis::TextureRegion, which describes the region of the texture to
     * write to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteSubresource(const void* source_data, const wis::TextureRegion& target_region) const noexcept
    {
        const WisResult wis_result = ::wisVKTextureWriteSubresource(
            &_impl_storage,
            source_data,
            reinterpret_cast<const WisTextureRegion*>(&target_region)
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
};

struct VKBufferDeleter {
    void operator()(WisVKBuffer* handle) noexcept { ::wisVKDestroyBuffer(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU buffer resource.
 *
 * */
class VKBuffer : public wis::impl::Implements<wis::impl::VKBufferImpl, WisVKBuffer, wis::VKBufferDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKBufferView GetView() const noexcept
    {
        VKBufferView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKBufferView() const noexcept { return GetView(); }
    /**
     * @brief Provided by Wisdom 0.7.0. Maps the buffer memory to CPU accessible address space.
     * @return void points to the pointer, which is filled with the address of the mapped memory on success.
     *
     * */
    WIS_NODISCARD inline void* Map() const noexcept { return (::wisVKBufferMap(&_impl_storage)); }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the GPU virtual address of the buffer.
     * @return u64 Address of the buffer on GPU.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetGPUAddress() const noexcept
    {
        return (::wisVKBufferGetGPUAddress(&_impl_storage));
    }
};

struct VKSwapchainDeleter {
    void operator()(WisVKSwapchain* handle) noexcept { ::wisVKDestroySwapchain(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a swapchain, which is a collection of render targets used for
 * presentation.
 *
 * */
class VKSwapchain : public wis::impl::Implements<wis::impl::VKSwapchainImpl, WisVKSwapchain, wis::VKSwapchainDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Presents the swapchain image to the screen.
     * @param flags describes the presentation options.
     * @param rects points to an array of rectangles to present. If `nullptr`, the entire image is presented.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Present(wis::PresentFlags flags, wis::span<const wis::Rect> rects) const noexcept
    {
        const WisResult wis_result = ::wisVKSwapchainPresent(
            &_impl_storage,
            static_cast<WisPresentFlags>(flags),
            reinterpret_cast<const WisRect*>(rects.data()),
            rects.size()
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the index of the current backbuffer. In case of lazy indexing it @wis_may
     * wait for presentation to finish and block.
     * @param out_result denoting the outcome of operation.
     * @return index Index of the current backbuffer.
     *
     * */
    WIS_NODISCARD inline std::uint32_t GetCurrentIndex(wis::Result& out_result) const noexcept
    {
        std::uint32_t index;
        const WisResult
            wis_result = ::wisVKSwapchainGetCurrentIndex(&_impl_storage, reinterpret_cast<uint32_t*>(&index));
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return index;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Resizes the swapchain buffers. If the swapchain is currently in use, it
     * @wis_must be resized after the GPU finishes using it, so the call @wis_may block until then.
     * @param desc indicates a pointer to wis::SwapchainUpdateDesc, which describes the new swapchain parameters.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Update(const wis::SwapchainUpdateDesc& desc) const noexcept
    {
        const WisResult
            wis_result = ::wisVKSwapchainUpdate(&_impl_storage, reinterpret_cast<const WisSwapchainUpdateDesc*>(&desc));
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the swapchain buffers. The textures are in `wis::TextureState::Common`.
     * @param buffers points to an array of texture views, which are filled with swapchain backbuffers on success.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result GetTextures(wis::span<wis::VKTexture> buffers) const noexcept
    {
        const WisResult wis_result = ::wisVKSwapchainGetTextures(
            &_impl_storage,
            reinterpret_cast<WisVKTexture*>(buffers.data()),
            buffers.size()
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
};

struct VKSurfaceDeleter {
    void operator()(WisVKSurface* handle) noexcept { ::wisVKDestroySurface(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU surface, which can be used as a target for rendering and
 * presentation.
 *
 * */
class VKSurface : public wis::impl::Implements<wis::impl::VKSurfaceImpl, WisVKSurface, wis::VKSurfaceDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKSurfaceView GetView() const noexcept
    {
        VKSurfaceView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKSurfaceView() const noexcept { return GetView(); }
};

struct VKViewHeapDeleter {
    void operator()(WisVKViewHeap* handle) noexcept { ::wisVKDestroyViewHeap(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for resource views used in contiguous array.
 *
 * */
class VKViewHeap : public wis::impl::Implements<wis::impl::VKViewHeapImpl, WisVKViewHeap, wis::VKViewHeapDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a render target view to the view heap and returns the CPU descriptor
     * handle for it.
     * @param texture describes a pointer to wis::Texture to write the descriptor for.
     * @param render_target specifies a pointer to wis::RenderTargetDesc, which describes the render target view to
     * write.
     * @param index defines the index in the view heap to write the descriptor to.
     * @return u64 CPU descriptor handle for the view heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t WriteRenderTarget(
        const wis::VKTexture& texture,
        const wis::RenderTargetDesc& render_target,
        std::uint32_t index
    ) const noexcept
    {
        return (::wisVKViewHeapWriteRenderTarget(
            &_impl_storage,
            reinterpret_cast<const WisVKTexture*>(&texture),
            reinterpret_cast<const WisRenderTargetDesc*>(&render_target),
            index
        ));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a depth stencil view to the view heap and returns the CPU descriptor
     * handle for it.
     * @param texture describes a pointer to wis::Texture to write the descriptor for.
     * @param render_target specifies a pointer to wis::RenderTargetDesc, which describes the depth stencil view to
     * write.
     * @param index defines the index in the view heap to write the descriptor to.
     * @return u64 CPU descriptor handle for the view heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t WriteDepthStencil(
        const wis::VKTexture& texture,
        const wis::RenderTargetDesc& render_target,
        std::uint32_t index
    ) const noexcept
    {
        return (::wisVKViewHeapWriteDepthStencil(
            &_impl_storage,
            reinterpret_cast<const WisVKTexture*>(&texture),
            reinterpret_cast<const WisRenderTargetDesc*>(&render_target),
            index
        ));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
     * @param index defines the index in the view heap to get the descriptor from.
     * @return u64 Address of a view in heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetViewAddress(std::uint32_t index) const noexcept
    {
        return (::wisVKViewHeapGetViewAddress(&_impl_storage, index));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies views from one heap to another.
     * @param dst_index specifies the index in the destination view heap to copy views to.
     * @param src_ptr describes a pointer to wis::ViewHeap to copy views from.
     * @param src_index indicates the index in the source view heap to copy views from.
     * @param view_count indicates the number of views to copy.
     *
     * */
    inline void CopyViews(
        std::uint32_t dst_index,
        std::uint64_t src_ptr,
        std::uint32_t src_index,
        std::uint32_t view_count
    ) const noexcept
    {
        ::wisVKViewHeapCopyViews(&_impl_storage, dst_index, src_ptr, src_index, view_count);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the view heap.
     * @return u64 CPU descriptor handle for the view heap.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetCPUHandle() const noexcept
    {
        return (::wisVKViewHeapGetCPUHandle(&_impl_storage));
    }
};

struct VKPipelineDeleter {
    void operator()(WisVKPipeline* handle) noexcept { ::wisVKDestroyPipeline(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU pipeline state object, which encapsulates the state of the
 * GPU pipeline and allows to execute draw and dispatch calls with it.
 *
 * */
class VKPipeline : public wis::impl::Implements<wis::impl::VKPipelineImpl, WisVKPipeline, wis::VKPipelineDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKPipelineView GetView() const noexcept
    {
        VKPipelineView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKPipelineView() const noexcept { return GetView(); }
};

struct VKShaderDeleter {
    void operator()(WisVKShader* handle) noexcept { ::wisVKDestroyShader(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a GPU shader module, which contains shader code and allows to
 * create pipeline state objects with it.
 *
 * */
class VKShader : public wis::impl::Implements<wis::impl::VKShaderImpl, WisVKShader, wis::VKShaderDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKShaderView GetView() const noexcept
    {
        VKShaderView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKShaderView() const noexcept { return GetView(); }
};

struct VKPipelineCacheDeleter {
    void operator()(WisVKPipelineCache* handle) noexcept { ::wisVKDestroyPipelineCache(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a cache for pipeline state objects, which allows to reuse already
 * created pipelines and speed up pipeline creation.
 *
 * */
class VKPipelineCache
    : public wis::impl::Implements<wis::impl::VKPipelineCacheImpl, WisVKPipelineCache, wis::VKPipelineCacheDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKPipelineCacheView GetView() const noexcept
    {
        VKPipelineCacheView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKPipelineCacheView() const noexcept { return GetView(); }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the data from the pipeline cache.
     * @param data points to an array that is filled with serialized cache data on success.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Serialize(wis::span<std::uint8_t> data) const noexcept
    {
        const WisResult wis_result = ::wisVKPipelineCacheSerialize(
            &_impl_storage,
            reinterpret_cast<uint8_t*>(data.data()),
            data.size()
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets the size of the data in the pipeline cache.
     * @return size Size of the data in bytes.
     *
     * */
    WIS_NODISCARD inline std::size_t GetSerializedSize() const noexcept
    {
        return (::wisVKPipelineCacheGetSerializedSize(&_impl_storage));
    }
};

struct VKDescriptorHeapDeleter {
    void operator()(WisVKDescriptorHeap* handle) noexcept { ::wisVKDestroyDescriptorHeap(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a storage for descriptors used in contiguous array.
 *
 * */
class VKDescriptorHeap
    : public wis::impl::Implements<wis::impl::VKDescriptorHeapImpl, WisVKDescriptorHeap, wis::VKDescriptorHeapDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the CPU descriptor handle for the descriptor heap.
     * @return void CPU descriptor handle for the descriptor heap.
     *
     * */
    WIS_NODISCARD inline void* GetCPUHandle() const noexcept
    {
        return (::wisVKDescriptorHeapGetCPUHandle(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::ConstantBuffer` descriptor to the descriptor heap.
     * @param data specifies a pointer to wis::ConstantBufferBinding, which describes the constant buffer descriptors to
     * write.
     * @param index defines the index in the descriptor heap to write the descriptors to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteConstantBuffer(const wis::ConstantBufferBinding& data, std::uint32_t index) const noexcept
    {
        const WisResult wis_result = ::wisVKDescriptorHeapWriteConstantBuffer(
            &_impl_storage,
            reinterpret_cast<const WisConstantBufferBinding*>(&data),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::Buffer` descriptor to the descriptor heap.
     * @param buffer specifies a pointer to wis::Buffer to write the descriptor for.
     * @param data specifies a pointer to wis::BufferBinding, which describes the shader resource view descriptors to
     * write.
     * @param index defines the index in the descriptor heap to write the descriptors to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteStructuredBuffer(
        wis::VKBufferView buffer,
        const wis::BufferBinding& data,
        std::uint32_t index
    ) const noexcept
    {
        const WisResult wis_result = ::wisVKDescriptorHeapWriteStructuredBuffer(
            &_impl_storage,
            buffer,
            reinterpret_cast<const WisBufferBinding*>(&data),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::RWBuffer` descriptor to the descriptor heap.
     * @param buffer specifies a pointer to wis::Buffer to write the descriptor for.
     * @param data specifies a pointer to wis::BufferBinding, which describes the shader resource view descriptors to
     * write.
     * @param index defines the index in the descriptor heap to write the descriptors to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteRWStructuredBuffer(
        wis::VKBufferView buffer,
        const wis::BufferBinding& data,
        std::uint32_t index
    ) const noexcept
    {
        const WisResult wis_result = ::wisVKDescriptorHeapWriteRWStructuredBuffer(
            &_impl_storage,
            buffer,
            reinterpret_cast<const WisBufferBinding*>(&data),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes `wis::DescriptorType::Sampler` descriptor to the descriptor heap.
     * @param sampler defines a pointer to wis::SamplerDesc, which describes the sampler descriptor to write.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteSampler(const wis::SamplerDesc& sampler, std::uint32_t index) const noexcept
    {
        const WisResult wis_result = ::wisVKDescriptorHeapWriteSampler(
            &_impl_storage,
            reinterpret_cast<const WisSamplerDesc*>(&sampler),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a descriptor to the descriptor heap.
     * @param texture describes a pointer to wis::Texture to write the descriptor for.
     * @param data specifies a pointer to wis::TextureBinding, which describes the texture view to write.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteTexture(
        wis::VKTextureView texture,
        const wis::TextureBinding& data,
        std::uint32_t index
    ) const noexcept
    {
        const WisResult wis_result = ::wisVKDescriptorHeapWriteTexture(
            &_impl_storage,
            texture,
            reinterpret_cast<const WisTextureBinding*>(&data),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a texture view to the descriptor heap.
     * @param texture describes a pointer to wis::Texture to write the descriptor for.
     * @param data specifies a pointer to wis::TextureBinding, which describes the texture view to write.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteRWTexture(
        wis::VKTextureView texture,
        const wis::TextureBinding& data,
        std::uint32_t index
    ) const noexcept
    {
        const WisResult wis_result = ::wisVKDescriptorHeapWriteRWTexture(
            &_impl_storage,
            texture,
            reinterpret_cast<const WisTextureBinding*>(&data),
            index
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Writes a raytracing acceleration to the descriptor heap.
     * @param address specifies GPU address of a raytracing acceleration structure.
     * @param index defines the index in the descriptor heap to write the descriptor to.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WriteAccelerationStructure(std::uint64_t address, std::uint32_t index) const noexcept
    {
        const WisResult wis_result = ::wisVKDescriptorHeapWriteAccelerationStructure(&_impl_storage, address, index);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies descriptors from one heap to another.
     * @param dst_index specifies the index in the destination descriptor heap to copy descriptors to.
     * @param src_ptr describes a pointer to wis::DescriptorHeap to copy descriptors from. Source heap @wis_must be CPU
     * Only heap.
     * @param src_index indicates the index in the source descriptor heap to copy descriptors from.
     * @param descriptor_count indicates the number of descriptors to copy.
     *
     * */
    inline void CopyDescriptors(
        std::uint32_t dst_index,
        const void* src_ptr,
        std::uint32_t src_index,
        std::uint32_t descriptor_count
    ) const noexcept
    {
        ::wisVKDescriptorHeapCopyDescriptors(&_impl_storage, dst_index, src_ptr, src_index, descriptor_count);
    }
};

struct VKRootSignatureDeleter {
    void operator()(WisVKRootSignature* handle) noexcept { ::wisVKDestroyRootSignature(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pipeline layout and a constant data storage, which defines
 * resource bindings for shaders.
 *
 * */
class VKRootSignature
    : public wis::impl::Implements<wis::impl::VKRootSignatureImpl, WisVKRootSignature, wis::VKRootSignatureDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKRootSignatureView GetView() const noexcept
    {
        VKRootSignatureView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKRootSignatureView() const noexcept { return GetView(); }
};

struct VKResourceAllocatorDeleter {
    void operator()(WisVKResourceAllocator* handle) noexcept { ::wisVKDestroyResourceAllocator(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class for allocating and managing GPU resources like buffers and textures.
 *
 * */
class VKResourceAllocator
    : public wis::impl::
          Implements<wis::impl::VKResourceAllocatorImpl, WisVKResourceAllocator, wis::VKResourceAllocatorDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a buffer with given descriptor.
     * @param desc indicates a pointer to wis::BufferDesc, which describes the buffer to create.
     * @param out_result denoting the outcome of operation.
     * @return buffer points to wis::Buffer, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKBuffer CreateBuffer(const wis::BufferDesc& desc, wis::Result& out_result) const noexcept
    {
        wis::VKBuffer buffer;
        const WisResult wis_result = ::wisVKResourceAllocatorCreateBuffer(
            &_impl_storage,
            reinterpret_cast<const WisBufferDesc*>(&desc),
            buffer.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return buffer;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a texture with given descriptor.
     * @param desc indicates a pointer to wis::TextureDesc, which describes the texture to create.
     * @param out_result denoting the outcome of operation.
     * @return texture points to wis::Texture, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKTexture CreateTexture(
        const wis::TextureDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKTexture texture;
        const WisResult wis_result = ::wisVKResourceAllocatorCreateTexture(
            &_impl_storage,
            reinterpret_cast<const WisTextureDesc*>(&desc),
            texture.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return texture;
    }
};

struct VKFenceDeleter {
    void operator()(WisVKFence* handle) noexcept { ::wisVKDestroyFence(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a fence for GPU-CPU and GPU-GPU synchronization.
 *
 * */
class VKFence : public wis::impl::Implements<wis::impl::VKFenceImpl, WisVKFence, wis::VKFenceDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKFenceView GetView() const noexcept
    {
        VKFenceView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKFenceView() const noexcept { return GetView(); }
    /**
     * @brief Provided by Wisdom 0.7.0. Get the current value of the fence.
     * @return u64 Value of the fence.
     *
     * */
    WIS_NODISCARD inline std::uint64_t GetCompletedValue() const noexcept
    {
        return (::wisVKFenceGetCompletedValue(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Wait on CPU for the fence to reach a certain value.
     * @param value describes value to wait for.
     * @param wait_ns describes the time to wait for the fence to reach the value in nanoseconds. Default is infinite.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Wait(std::uint64_t value, std::uint64_t wait_ns) const noexcept
    {
        const WisResult wis_result = ::wisVKFenceWait(&_impl_storage, value, wait_ns);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Signal the fence from CPU.
     * @param value describes value to signal.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Signal(std::uint64_t value) const noexcept
    {
        const WisResult wis_result = ::wisVKFenceSignal(&_impl_storage, value);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
};

struct VKCommandListDeleter {
    void operator()(WisVKCommandList* handle) noexcept { ::wisVKDestroyCommandList(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command list for recording GPU commands.
 *
 * */
class VKCommandList
    : public wis::impl::Implements<wis::impl::VKCommandListImpl, WisVKCommandList, wis::VKCommandListDeleter>
{
public:
    using ImplType::ImplType;

public:
    WIS_NODISCARD VKCommandListView GetView() const noexcept
    {
        VKCommandListView v;
        std::memcpy(&v, &_impl_storage, sizeof(v));
        return v;
    }
    WIS_NODISCARD operator VKCommandListView() const noexcept { return GetView(); }
    /**
     * @brief Provided by Wisdom 0.7.0. Opens the command list, so commands can be recorded to it.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Begin() const noexcept
    {
        const WisResult wis_result = ::wisVKCommandListBegin(&_impl_storage);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Closes the command list, so it can be executed on the command queue.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result End() const noexcept
    {
        const WisResult wis_result = ::wisVKCommandListEnd(&_impl_storage);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Binds descriptor heaps to the command list, so they can be used for resource
     * binding.
     * @param resource_heap describes a pointer to wis::DescriptorHeap with shader resource views, unordered access
     * views and constant buffer views. If `nullptr`, no resource heap is bound.
     * @param sampler_heap describes a pointer to wis::DescriptorHeap with samplers. If `nullptr`, no sampler heap is
     * bound.
     *
     * */
    inline void SetDescriptorHeaps(
        const wis::VKDescriptorHeap* resource_heap,
        const wis::VKDescriptorHeap* sampler_heap
    ) const noexcept
    {
        ::wisVKCommandListSetDescriptorHeaps(
            &_impl_storage,
            reinterpret_cast<const WisVKDescriptorHeap*>(resource_heap),
            reinterpret_cast<const WisVKDescriptorHeap*>(sampler_heap)
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the root signature for the command list, so it can be used for resource
     * binding.
     * @param signature specifies a pointer to wis::RootSignature to set.
     * @param pipeline specifies the pipeline type to set the root signature for.
     *
     * */
    inline void SetRootSignature(wis::VKRootSignatureView signature, wis::PipelineType pipeline) const noexcept
    {
        ::wisVKCommandListSetRootSignature(&_impl_storage, signature, static_cast<WisPipelineType>(pipeline));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the push constants for the command list, so they can be used for resource
     * binding.
     * @param data specifies a pointer to , which describes the push constant data to set.
     *
     * */
    inline void SetPushConstants(const wis::PushConstantDataDesc& data) const noexcept
    {
        ::wisVKCommandListSetPushConstants(&_impl_storage, reinterpret_cast<const WisPushConstantDataDesc*>(&data));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the push descriptors for the command list, so they can be used for resource
     * binding.
     * @param data specifies a pointer to , which describes the push descriptors to set.
     *
     * */
    inline void SetPushDescriptor(const wis::PushDescriptorDataDesc& data) const noexcept
    {
        ::wisVKCommandListSetPushDescriptor(&_impl_storage, reinterpret_cast<const WisPushDescriptorDataDesc*>(&data));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the descriptor table offset in descriptor heap for the command list, so it
     * can be used for resource binding.
     * @param data specifies the root parameter index to set the descriptor table for.
     *
     * */
    inline void SetDescriptorTable(const wis::DescriptorTableDataDesc& data) const noexcept
    {
        ::wisVKCommandListSetDescriptorTable(
            &_impl_storage,
            reinterpret_cast<const WisDescriptorTableDataDesc*>(&data)
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Inserts one or more barriers on the current command list.
     * @param barriers specifies a pointer to an array of barriers to insert.
     *
     * */
    inline void InsertBarriers(const wis::VKBarrierGroup& barriers) const noexcept
    {
        ::wisVKCommandListInsertBarriers(&_impl_storage, reinterpret_cast<const WisVKBarrierGroup*>(&barriers));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the pipeline state object for the command list, so it can be used for draw
     * and dispatch calls.
     * @param pipeline specifies a pointer to wis::Pipeline to set.
     * @param type specifies the pipeline type to set the pipeline for.
     *
     * */
    inline void SetPipeline(wis::VKPipelineView pipeline, wis::PipelineType type) const noexcept
    {
        ::wisVKCommandListSetPipeline(&_impl_storage, pipeline, static_cast<WisPipelineType>(type));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets multiple viewports.
     * @param viewports points to an array of viewports to set.
     *
     * */
    inline void SetViewports(wis::span<const wis::Viewport> viewports) noexcept
    {
        ::wisVKCommandListSetViewports(
            &_impl_storage,
            reinterpret_cast<const WisViewport*>(viewports.data()),
            viewports.size()
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets multiple scissor rects.
     * Each n-th rect corresponds to n-th Viewport set in RSSetViewports if SV_ViewportArrayIndex is used in geometry
     * shader. Otherwise the first is chosen.
     * @param scissor_rects points to an array of scissor rectangles to set.
     *
     * */
    inline void SetScissors(wis::span<const wis::Rect> scissor_rects) noexcept
    {
        ::wisVKCommandListSetScissors(
            &_impl_storage,
            reinterpret_cast<const WisRect*>(scissor_rects.data()),
            scissor_rects.size()
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the primitive topology. Determines how vertices shall be processed.
     * @param topology describes primitive topology to set.
     *
     * */
    inline void SetPrimitiveTopology(wis::PrimitiveTopology topology) noexcept
    {
        ::wisVKCommandListSetPrimitiveTopology(&_impl_storage, static_cast<WisPrimitiveTopology>(topology));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the depth bias. Determines how depth values are modified during
     * rasterization.
     * @param depth_bias indicates depth bias to set.
     * @param depth_bias_clamp indicates depth bias clamp to set.
     * @param slope_scaled_depth_bias defines slope-scaled depth bias to set.
     *
     * */
    inline void SetDepthBias(float depth_bias, float depth_bias_clamp, float slope_scaled_depth_bias) noexcept
    {
        ::wisVKCommandListSetDepthBias(&_impl_storage, depth_bias, depth_bias_clamp, slope_scaled_depth_bias);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Sets the primitive restart value. Determines the index value which is treated as
     * a primitive restart when using indexed draw calls.
     * @param restart_value describes primitive restart value to set.
     *
     * */
    inline void SetPrimitiveRestartValue(wis::PrimitiveRestartValue restart_value) noexcept
    {
        ::wisVKCommandListSetPrimitiveRestartValue(
            &_impl_storage,
            static_cast<WisPrimitiveRestartValue>(restart_value)
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Dispatches compute shader.
     * @param group_count_x defines number of groups to dispatch in X dimension.
     * @param group_count_y describes number of groups to dispatch in Y dimension; default is 1.
     * @param group_count_z specifies number of groups to dispatch in Z dimension; default is 1.
     *
     * */
    inline void Dispatch(
        std::uint32_t group_count_x,
        std::uint32_t group_count_y,
        std::uint32_t group_count_z
    ) const noexcept
    {
        ::wisVKCommandListDispatch(&_impl_storage, group_count_x, group_count_y, group_count_z);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Draws non-indexed geometry.
     * @param vertex_count specifies number of vertices to draw.
     * @param instance_count describes number of instances to draw; default is 1.
     * @param start_vertex indicates index of the first vertex to draw; default is 0.
     * @param start_instance specifies index of the first instance to draw; default is 0.
     *
     * */
    inline void Draw(
        std::uint32_t vertex_count,
        std::uint32_t instance_count,
        std::uint32_t start_vertex,
        std::uint32_t start_instance
    ) const noexcept
    {
        ::wisVKCommandListDraw(&_impl_storage, vertex_count, instance_count, start_vertex, start_instance);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Draws indexed geometry.
     * @param index_count defines number of indices to draw.
     * @param instance_count describes number of instances to draw; default is 1.
     * @param start_index describes index of the first index to draw; default is 0.
     * @param base_vertex defines value added to each index before reading a vertex from the vertex buffer; default is
     * 0.
     * @param start_instance specifies index of the first instance to draw; default is 0.
     *
     * */
    inline void DrawIndexed(
        std::uint32_t index_count,
        std::uint32_t instance_count,
        std::uint32_t start_index,
        std::int32_t base_vertex,
        std::uint32_t start_instance
    ) const noexcept
    {
        ::wisVKCommandListDrawIndexed(
            &_impl_storage,
            index_count,
            instance_count,
            start_index,
            base_vertex,
            start_instance
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Begins a render pass with given descriptor.
     * @param desc indicates a pointer to wis::RenderPassDesc, which describes the render pass to begin.
     *
     * */
    inline void BeginRenderPass(const wis::RenderPassDesc& desc) const noexcept
    {
        ::wisVKCommandListBeginRenderPass(&_impl_storage, reinterpret_cast<const WisRenderPassDesc*>(&desc));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Ends the current render pass.
     *
     * */
    inline void EndRenderPass() const noexcept { ::wisVKCommandListEndRenderPass(&_impl_storage); }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies regions from one buffer to another.
     * @param dst_buffer defines a pointer to the destination buffer.
     * @param src_buffer describes a pointer to the source buffer.
     * @param regions points to an array of wis::BufferCopyRegion that defines the copy regions.
     *
     * */
    inline void CopyBuffer(
        wis::VKBufferView dst_buffer,
        wis::VKBufferView src_buffer,
        wis::span<const wis::BufferCopyRegion> regions
    ) const noexcept
    {
        ::wisVKCommandListCopyBuffer(
            &_impl_storage,
            dst_buffer,
            src_buffer,
            reinterpret_cast<const WisBufferCopyRegion*>(regions.data()),
            regions.size()
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies regions from a buffer to a texture.
     * @param dst_texture defines a pointer to the destination texture. Texture @wis_must be in
     * `wis::TextureState::CopyDst`.
     * @param src_buffer describes a pointer to the source buffer.
     * @param regions points to an array of wis::BufferTextureCopyRegion that defines the copy regions.
     *
     * */
    inline void CopyBufferToTexture(
        wis::VKTextureView dst_texture,
        wis::VKBufferView src_buffer,
        wis::span<const wis::BufferTextureCopyRegion> regions
    ) const noexcept
    {
        ::wisVKCommandListCopyBufferToTexture(
            &_impl_storage,
            dst_texture,
            src_buffer,
            reinterpret_cast<const WisBufferTextureCopyRegion*>(regions.data()),
            regions.size()
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies regions from a texture to a buffer.
     * @param dst_buffer defines a pointer to the destination buffer.
     * @param src_texture describes a pointer to the source texture. Texture @wis_must be in
     * `wis::TextureState::CopySrc`.
     * @param regions points to an array of wis::BufferTextureCopyRegion that defines the copy regions.
     *
     * */
    inline void CopyTextureToBuffer(
        wis::VKBufferView dst_buffer,
        wis::VKTextureView src_texture,
        wis::span<const wis::BufferTextureCopyRegion> regions
    ) const noexcept
    {
        ::wisVKCommandListCopyTextureToBuffer(
            &_impl_storage,
            dst_buffer,
            src_texture,
            reinterpret_cast<const WisBufferTextureCopyRegion*>(regions.data()),
            regions.size()
        );
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Copies regions from one texture to another.
     * @param dst_texture defines a pointer to the destination texture. Texture @wis_must be in
     * `wis::TextureState::CopyDst`.
     * @param src_texture describes a pointer to the source texture. Texture @wis_must be in
     * `wis::TextureState::CopySrc`.
     * @param regions points to an array of wis::TextureCopyRegion that defines the copy regions.
     *
     * */
    inline void CopyTexture(
        wis::VKTextureView dst_texture,
        wis::VKTextureView src_texture,
        wis::span<const wis::TextureCopyRegion> regions
    ) const noexcept
    {
        ::wisVKCommandListCopyTexture(
            &_impl_storage,
            dst_texture,
            src_texture,
            reinterpret_cast<const WisTextureCopyRegion*>(regions.data()),
            regions.size()
        );
    }
};

struct VKCommandAllocatorDeleter {
    void operator()(WisVKCommandAllocator* handle) noexcept { ::wisVKDestroyCommandAllocator(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a pool allocator for command lists for recording GPU commands.
 *
 * */
class VKCommandAllocator
    : public wis::impl::
          Implements<wis::impl::VKCommandAllocatorImpl, WisVKCommandAllocator, wis::VKCommandAllocatorDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Resets the command allocator, so it can be reused for allocating new command
     * lists.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Reset() const noexcept
    {
        const WisResult wis_result = ::wisVKCommandAllocatorReset(&_impl_storage);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a command list of given type.
     * @param out_result denoting the outcome of operation.
     * @return list points to wis::CommandList, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKCommandList CreateCommandList(wis::Result& out_result) const noexcept
    {
        wis::VKCommandList list;
        const WisResult wis_result = ::wisVKCommandAllocatorCreateCommandList(&_impl_storage, list.GetStorage());
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return list;
    }
};

struct VKCommandQueueDeleter {
    void operator()(WisVKCommandQueue* handle) noexcept { ::wisVKDestroyCommandQueue(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class representing a command queue for submitting command lists to the GPU.
 *
 * */
class VKCommandQueue
    : public wis::impl::Implements<wis::impl::VKCommandQueueImpl, WisVKCommandQueue, wis::VKCommandQueueDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Executes the command lists.
     * @param lists points to an array of command lists to execute.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result Submit(wis::span<const wis::VKCommandListView> lists) const noexcept
    {
        const WisResult wis_result = ::wisVKCommandQueueSubmit(
            &_impl_storage,
            reinterpret_cast<const WisVKCommandListView*>(lists.data()),
            lists.size()
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Enqueue the signal to the queue, that gets executed after all the work has been
     * done.
     * @param fence describes fence to signal.
     * @param value describes value used to signal the fence.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result SignalFence(wis::VKFenceView fence, std::uint64_t value) const noexcept
    {
        const WisResult wis_result = ::wisVKCommandQueueSignalFence(&_impl_storage, fence, value);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Enqueues wait operation to the command queue. Queue then waits for the fence to
     * be signalled from CPU or from another queue. Can still be enqueued after the signal.
     * @param fence describes fence to wait on.
     * @param value describes value that the fence is expected to reach.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WaitFence(wis::VKFenceView fence, std::uint64_t value) const noexcept
    {
        const WisResult wis_result = ::wisVKCommandQueueWaitFence(&_impl_storage, fence, value);
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
};

struct VKDeviceDeleter {
    void operator()(WisVKDevice* handle) noexcept { ::wisVKDestroyDevice(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Central class representing logical device.
 *
 * */
class VKDevice : public wis::impl::Implements<wis::impl::VKDeviceImpl, WisVKDevice, wis::VKDeviceDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a command queue of given type.
     * @param type specifies the type of the command queue to create.
     * @param out_result denoting the outcome of operation.
     * @return queue points to wis::CommandQueue, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKCommandQueue CreateCommandQueue(
        wis::CommandQueueType type,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKCommandQueue queue;
        const WisResult wis_result = ::wisVKDeviceCreateCommandQueue(
            &_impl_storage,
            static_cast<WisCommandQueueType>(type),
            queue.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return queue;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a command allocator to allocate command lists with.
     * @param type specifies the type of the command list this pool is able to allocate.
     * @param out_result denoting the outcome of operation.
     * @return allocator points to wis::CommandAllocator, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKCommandAllocator CreateCommandAllocator(
        wis::CommandQueueType type,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKCommandAllocator allocator;
        const WisResult wis_result = ::wisVKDeviceCreateCommandAllocator(
            &_impl_storage,
            static_cast<WisCommandQueueType>(type),
            allocator.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return allocator;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a fence for GPU-CPU and GPU-GPU synchronization.
     * @param initial_value specifies the initial value of the fence.
     * @param out_result denoting the outcome of operation.
     * @return fence points to wis::Fence, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKFence CreateFence(std::uint64_t initial_value, wis::Result& out_result) const noexcept
    {
        wis::VKFence fence;
        const WisResult wis_result = ::wisVKDeviceCreateFence(&_impl_storage, initial_value, fence.GetStorage());
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return fence;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a resource allocator for managing GPU resources.
     * @param out_result denoting the outcome of operation.
     * @return allocator points to wis::ResourceAllocator, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKResourceAllocator GetResourceAllocator(wis::Result& out_result) const noexcept
    {
        wis::VKResourceAllocator allocator;
        const WisResult wis_result = ::wisVKDeviceGetResourceAllocator(&_impl_storage, allocator.GetStorage());
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return allocator;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a pipeline layout with given descriptor.
     * @param desc indicates a pointer to wis::RootSignatureDesc, which describes the pipeline layout to create.
     * @param out_result denoting the outcome of operation.
     * @return layout points to wis::RootSignature, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKRootSignature CreateRootSignature(
        const wis::RootSignatureDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKRootSignature layout;
        const WisResult wis_result = ::wisVKDeviceCreateRootSignature(
            &_impl_storage,
            reinterpret_cast<const WisRootSignatureDesc*>(&desc),
            layout.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return layout;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a descriptor storage with given description.
     * @param desc indicates a pointer to wis::DescriptorHeapDesc, which describes the descriptor heap to create.
     * @param out_result denoting the outcome of operation.
     * @return heap points to wis::DescriptorHeap, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKDescriptorHeap CreateDescriptorHeap(
        const wis::DescriptorHeapDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKDescriptorHeap heap;
        const WisResult wis_result = ::wisVKDeviceCreateDescriptorHeap(
            &_impl_storage,
            reinterpret_cast<const WisDescriptorHeapDesc*>(&desc),
            heap.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return heap;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a view storage with given descriptor.
     * @param type specifies the type of the view heap to create.
     * @param capacity specifies the capacity in descriptors of the view heap to create.
     * @param out_result denoting the outcome of operation.
     * @return heap points to wis::ViewHeap, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKViewHeap CreateViewHeap(
        wis::ViewHeapType type,
        std::uint32_t capacity,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKViewHeap heap;
        const WisResult wis_result = ::wisVKDeviceCreateViewHeap(
            &_impl_storage,
            static_cast<WisViewHeapType>(type),
            capacity,
            heap.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return heap;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Queries the properties of the device.
     * @param properties describes a pointer to one of the query structs, which is filled with device properties.
     *
     * */
    inline void QueryProperties(void* properties) const noexcept
    {
        ::wisVKDeviceQueryProperties(&_impl_storage, properties);
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Waits on multiple fences simultaneously.
     * If wait_for is `wis::MultiWaitType::All`, waits for all fences to be signaled.
     * Otherwise waits for any fence to be signaled.
     * @param fences defines an array of fence views to wait on.
     * @param fence_values defines fence values to wait for; array @wis_must have fence_count values.
     * @param fence_count describes number of fences to wait on.
     * @param wait_for indicates the kind of wait, where All waits for all fences and Any waits for any fence; default
     * is `wis::MultiWaitType::All`.
     * @param timeout indicates timeout in nanoseconds; if UINT64_MAX, waits indefinitely.
     * @return Result denoting the outcome of operation.
     *
     * */
    inline wis::Result WaitForMultipleFences(
        const wis::VKFenceView* fences,
        const std::uint64_t* fence_values,
        std::size_t fence_count,
        wis::MultiWaitType wait_for,
        std::uint64_t timeout
    ) const noexcept
    {
        const WisResult wis_result = ::wisVKDeviceWaitForMultipleFences(
            &_impl_storage,
            fences,
            fence_values,
            fence_count,
            static_cast<WisMultiWaitType>(wait_for),
            timeout
        );
        return wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a pipeline cache for caching pipeline state objects.
     * @param initial_data points to an array of initial cache data bytes. If `nullptr`, the cache is created empty.
     * @param out_result denoting the outcome of operation.
     * @return cache points to wis::PipelineCache, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKPipelineCache CreatePipelineCache(
        wis::span<const std::uint8_t> initial_data,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKPipelineCache cache;
        const WisResult wis_result = ::wisVKDeviceCreatePipelineCache(
            &_impl_storage,
            reinterpret_cast<const uint8_t*>(initial_data.data()),
            initial_data.size(),
            cache.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return cache;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a shader module from given data.
     * @param data points to an array of shader bytecode.
     * @param out_result denoting the outcome of operation.
     * @return shader points to wis::Shader, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKShader CreateShader(
        wis::span<const std::uint8_t> data,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKShader shader;
        const WisResult wis_result = ::wisVKDeviceCreateShader(
            &_impl_storage,
            reinterpret_cast<const uint8_t*>(data.data()),
            data.size(),
            shader.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return shader;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a compute pipeline state object with given descriptor.
     * @param desc indicates a pointer to wis::ComputePipelineDesc, which describes the compute pipeline to create.
     * @param out_result denoting the outcome of operation.
     * @return pipeline points to wis::Pipeline, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKPipeline CreateComputePipeline(
        const wis::VKComputePipelineDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKPipeline pipeline;
        const WisResult wis_result = ::wisVKDeviceCreateComputePipeline(
            &_impl_storage,
            reinterpret_cast<const WisVKComputePipelineDesc*>(&desc),
            pipeline.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return pipeline;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a graphics pipeline state object with given descriptor.
     * @param desc indicates a pointer to wis::GraphicsPipelineDesc, which describes the graphics pipeline to create.
     * @param out_result denoting the outcome of operation.
     * @return pipeline points to wis::Pipeline, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKPipeline CreateGraphicsPipeline(
        const wis::VKGraphicsPipelineDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKPipeline pipeline;
        const WisResult wis_result = ::wisVKDeviceCreateGraphicsPipeline(
            &_impl_storage,
            reinterpret_cast<const WisVKGraphicsPipelineDesc*>(&desc),
            pipeline.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return pipeline;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Checks if the surface format is supported for presentation and returns the
     * supported format.
     * @param surface describes a pointer to wis::Surface to check the presentation support for.
     * @param format describes the format to check the presentation support for.
     * @return bool Result of operation.
     *
     * */
    WIS_NODISCARD inline bool GetFormatPresentationSupport(
        wis::VKSurfaceView surface,
        wis::DataFormat format
    ) const noexcept
    {
        return (::wisVKDeviceGetFormatPresentationSupport(&_impl_storage, surface, static_cast<WisDataFormat>(format)));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Gets presentation parameters for the specified surface.
     * @param surface describes a pointer to wis::Surface to check the presentation support for.
     * @param out_result denoting the outcome of operation.
     * @return params Parameters of the surface.
     *
     * */
    WIS_NODISCARD inline wis::SurfaceParameters GetSurfaceParameters(
        wis::VKSurfaceView surface,
        wis::Result& out_result
    ) const noexcept
    {
        wis::SurfaceParameters params;
        const WisResult wis_result = ::wisVKDeviceGetSurfaceParameters(
            &_impl_storage,
            surface,
            reinterpret_cast<WisSurfaceParameters*>(&params)
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return params;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates a swapchain for given surface with given descriptor.
     * @param surface describes a pointer to wis::Surface to create the swapchain for. Surface is ref-counted.
     * @param queue describes a pointer to wis::CommandQueue to create the swapchain for. Queue is ref-counted and
     * @wis_must be a graphics queue.
     * @param desc indicates a pointer to wis::SwapchainDesc, which describes the swapchain to create.
     * @param out_result denoting the outcome of operation.
     * @return swapchain points to wis::Swapchain, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKSwapchain CreateSwapchain(
        const wis::VKSurface& surface,
        const wis::VKCommandQueue& queue,
        const wis::SwapchainDesc& desc,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKSwapchain swapchain;
        const WisResult wis_result = ::wisVKDeviceCreateSwapchain(
            &_impl_storage,
            reinterpret_cast<const WisVKSurface*>(&surface),
            reinterpret_cast<const WisVKCommandQueue*>(&queue),
            reinterpret_cast<const WisSwapchainDesc*>(&desc),
            swapchain.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return swapchain;
    }
};

struct VKAdapterQueryDeleter {
    void operator()(WisVKAdapterQuery* handle) noexcept { ::wisVKDestroyAdapterQuery(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class that contains a snapshot of adapters that are present on the system.
 *
 * */
class VKAdapterQuery
    : public wis::impl::Implements<wis::impl::VKAdapterQueryImpl, WisVKAdapterQuery, wis::VKAdapterQueryDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the number of adapters present on the system at the time of the query.
     * @return size is a number of adapters present on the system.
     *
     * */
    WIS_NODISCARD inline std::size_t GetAdapterCount() const noexcept
    {
        return (::wisVKAdapterQueryGetAdapterCount(&_impl_storage));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Returns the description of the adapter at given index.
     * @param index defines the index of the adapter to get the description for. It @wis_must be less than the value
     * returned by wis::AdapterQuery::GetAdapterCount.
     * @param out_result denoting the outcome of operation.
     * @return desc points to wis::AdapterDesc, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::AdapterDesc GetAdapterDesc(std::size_t index, wis::Result& out_result) const noexcept
    {
        wis::AdapterDesc desc;
        const WisResult wis_result = ::wisVKAdapterQueryGetAdapterDesc(
            &_impl_storage,
            index,
            reinterpret_cast<WisAdapterDesc*>(&desc)
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return desc;
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Checks if the adapter at given index supports presentation to given surface.
     * @param index defines the index of the adapter to check the support for. It @wis_must be less than the value
     * returned by wis::AdapterQuery::GetAdapterCount.
     * @param surface describes a pointer to wis::Surface to check the presentation support for.
     * @return bool `true` if the adapter supports presentation to the surface, `false` otherwise.
     *
     * */
    WIS_NODISCARD inline bool GetSurfaceSupport(std::size_t index, wis::VKSurfaceView surface) const noexcept
    {
        return (::wisVKAdapterQueryGetSurfaceSupport(&_impl_storage, index, surface));
    }
    /**
     * @brief Provided by Wisdom 0.7.0. Creates the device for the adapter at given index.
     * @param index defines the index of the adapter to create the device for. It @wis_must be less than the value
     * returned by wis::AdapterQuery::GetAdapterCount.
     * @param requirements defines a pointer to wis::DeviceRequirements, which defines required features and properties
     * for device creation.
     * @param out_result denoting the outcome of operation.
     * @return device points to wis::Device, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKDevice CreateDevice(
        std::size_t index,
        const wis::VKDeviceRequirements& requirements,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKDevice device;
        const WisResult wis_result = ::wisVKAdapterQueryCreateDevice(
            &_impl_storage,
            index,
            reinterpret_cast<const WisVKDeviceRequirements*>(&requirements),
            device.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return device;
    }
};

struct VKInstanceDeleter {
    void operator()(WisVKInstance* handle) noexcept { ::wisVKDestroyInstance(handle); }
};
/**
 * @brief Provided by Wisdom 0.7.0. Class for creating adapters.
 *
 * */
class VKInstance : public wis::impl::Implements<wis::impl::VKInstanceImpl, WisVKInstance, wis::VKInstanceDeleter>
{
public:
    using ImplType::ImplType;

public:
    /**
     * @brief Provided by Wisdom 0.7.0. Queries all the system adapters and allows to iterate through them. The order of
     * adapters depends on `preference`: for MinConsumption - DirectX 12: Integrated, Discrete, External, Software;
     * Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU. For Performance - DirectX 12: External, Discrete,
     * Integrated, Software; Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
     * @param preference indicates the order in which adapters are listed.
     * @param out_result denoting the outcome of operation.
     * @return query points to wis::AdapterQuery, which is initialized on success.
     *
     * */
    WIS_NODISCARD inline wis::VKAdapterQuery QueryAdapters(
        wis::AdapterPreference preference,
        wis::Result& out_result
    ) const noexcept
    {
        wis::VKAdapterQuery query;
        const WisResult wis_result = ::wisVKInstanceQueryAdapters(
            &_impl_storage,
            static_cast<WisAdapterPreference>(preference),
            query.GetStorage()
        );
        out_result = wis::Result{
            static_cast<wis::Status>(wis_result.status),
            wis_result.platform_code,
            wis_result.error
        };
        return query;
    }
};

/**
 * @brief Provided by Wisdom 0.7.0. Creates the wis::Instance with extensions, specified in extension array.
 * @param debug_desc describes a pointer to wis::DebugDesc, which defines debug callback and debug layer usage. If
 * `nullptr`, debug layer is disabled.
 * @param extensions points to an array of extensions that are to be initialized with pointers to
 * wis::InstanceExtensionHeader.
 * @param out_result denoting the outcome of operation.
 * @return instance points to wis::Instance, which is initialized on success.
 *
 * */
WIS_NODISCARD inline wis::VKInstance VKCreateInstance(
    const wis::DebugDesc* debug_desc,
    wis::span<wis::VKInstanceExtensionHeader*> extensions,
    wis::Result& out_result
) noexcept
{
    wis::VKInstance instance;
    const WisResult wis_result = ::wisVKCreateInstance(
        reinterpret_cast<const WisDebugDesc*>(debug_desc),
        reinterpret_cast<WisVKInstanceExtensionHeader**>(extensions.data()),
        extensions.size(),
        instance.GetStorage()
    );
    out_result = wis::Result{static_cast<wis::Status>(wis_result.status), wis_result.platform_code, wis_result.error};
    return instance;
}

} // namespace wis
#endif // WISDOM_VULKAN

#endif // WISDOM_CORE_CPP_API_HPP
