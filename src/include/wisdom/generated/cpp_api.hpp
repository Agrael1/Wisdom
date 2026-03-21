// This file is generated. Do not edit directly.
#ifndef WISDOM_CORE_CPP_API_HPP
#define WISDOM_CORE_CPP_API_HPP
#ifndef __cplusplus
#error C++ is required to include this header.
#endif // __cplusplus

#include <wisdom/global/definitions.h>
#include <wisdom/bridge/span.hpp>

namespace wis {

//==============================================================
// Enums
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Defines common return status codes. Compare against `wis::Status::Ok` for success.
 *
 * */
enum class Status {
    Ok                = 0, ///< Operation succeeded.
    Timeout           = 1, ///< Operation timed out.
    Partial           = 2, ///< Operation partially succeeded.
    InvalidArgument   = -1, ///< One or more arguments, or parts of arguments passed to the function were incorrect.
    OutOfHostMemory   = -2, ///< There is no more host memory available.
    OutOfDeviceMemory = -3, ///< There is no more device memory available.
    DeviceLost        = -4, ///< Device driver was forcefully stopped.
    Occluded          = -5, ///< Swap chain presentation was not visible to the user. Rendering is too fast.
    ValidationFailed  = -6, ///< A validation layer found an error.
    Fail              = -7, ///< Operation expectedly failed.
    Error             = -10000, ///< Operation failed.
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
enum class DataFormat {
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
enum class SampleCount {
    S1  = 1, ///< 1 sample per pixel.
    S2  = 2, ///< 2 samples per pixel.
    S4  = 4, ///< 4 samples per pixel.
    S8  = 8, ///< 8 samples per pixel.
    S16 = 16, ///< 16 samples per pixel.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture layout. Determines how texture will be accessed.
 *
 * */
enum class TextureLayout {
    Texture1D        = 2, ///< Texture is 1D array of data. Behaves similarly to Buffer.
    Texture1DArray   = 3, ///< Texture is an array of 1D data.
    Texture2D        = 4, ///< Texture is 2D image, default texture type.
    Texture2DArray   = 5, ///< Texture is an array of 2D images. Can also be used for Cube maps.
    Texture2DMS      = 6, ///< Texture is 2D multisampled image.
    Texture2DMSArray = 7, ///< Texture is an array of 2D multisampled images.
    Texture3D        = 8, ///< Texture is 3D volume.
    TextureCube      = 9, ///< Texture is a cube map. Behaves similarly to Texture2DArray with 6 layers.
    TextureCubeArray = 10, ///< Texture is an array of cube maps. Behaves similarly to Texture2DArray with 6 layers per cube map.
};

/**
 * @brief Provided by Wisdom 0.7.0. Orders the adapters according to preference using builtin heuristics of underlying APIs. For DirectX 12, this translates directly to `DXGI_GPU_PREFERENCE`. For Vulkan, sorting is based on `VkPhysicalDeviceType` heuristics.
 *
 * */
enum class AdapterPreference {
    None           = 0, ///< No particular preference, list adapters in system devised order.
    MinConsumption = 1, ///< List the adapters from low power consumption to high. DirectX 12: Integrated, Discrete, External, Software. Vulkan: Integrated GPU, Discrete GPU, Virtual GPU, CPU.
    Performance    = 2, ///< List the adapters from high performance to low. DirectX 12: External, Discrete, Integrated, Software. Vulkan: Discrete GPU, Integrated GPU, Virtual GPU, CPU.
};

/**
 * @brief Provided by Wisdom 0.7.0. Defines severity levels for logging and debugging messages.
 *
 * */
enum class Severity {
    Verbose = 0, ///< Verbose level messages, typically used for detailed debugging information.
    Info    = 1, ///< Informational messages that highlight the progress of the application.
    Warning = 2, ///< Potentially harmful situations that warrant attention but do not prevent normal operation.
    Error   = 3, ///< Error events that might still allow the application to continue running.
    Fatal   = 4, ///< Severe error events that will presumably lead the application to abort.
};

/**
 * @brief Provided by Wisdom 0.7.0. Defines types of command queues that can be created on the device. Each type corresponds to a specific kind of workload that the GPU can handle.
 *
 * */
enum class CommandQueueType {
    Graphics    = 0, ///< Command queue for graphics operations, including rendering and compute tasks.
    Compute     = 1, ///< Command queue dedicated to compute operations, optimized for parallel processing tasks.
    Transfer    = 2, ///< Command queue for data transfer operations, such as copying resources between buffers and images.
    VideoDecode = 3, ///< Command queue for video decoding operations.
    VideoEncode = 4, ///< Command queue for video encoding operations.
    Count       = 5, ///< Number of command queue types available.
};

/**
 * @brief Provided by Wisdom 0.7.0. Global queue priority. Higher priority queues get more GPU time, but @wis_may cause performance issues if overused.
 *
 * */
enum class CommandQueuePriority {
    Normal   = 0, ///< Normal queue priority.
    High     = 1, ///< High queue priority.
    Realtime = 2, ///< Global realtime queue priority. Requires special GPU support and @wis_may cause performance issues if used on unsupported hardware.
};

/**
 * @brief Provided by Wisdom 0.7.0. Defines types of pipelines that can be created on the device. Each type corresponds to a specific kind of workload that the GPU can handle.
 *
 * */
enum class PipelineType {
    Graphics   = 0, ///< Pipeline for graphics operations, including rendering tasks.
    Compute    = 1, ///< Pipeline dedicated to compute operations, optimized for parallel processing tasks.
    RayTracing = 2, ///< Pipeline for ray tracing operations, optimized for ray tracing workloads.
};

/**
 * @brief Provided by Wisdom 0.7.0. Shader stages that can be used in the pipeline. Main use is Root signature and descriptor management. Stages have no granularity, either all or one can be selected.
 *
 * */
enum class ShaderVisibility {
    All           = 0, ///< All shader stages.
    Vertex        = 1, ///< Vertex shader stage.
    Hull          = 2, ///< Hull/Tessellation control shader stage.
    Domain        = 3, ///< Domain/Tessellation evaluation shader stage.
    Geometry      = 4, ///< Geometry shader stage.
    Pixel         = 5, ///< Pixel/Fragment shader stage.
    Amplification = 6, ///< Amplification shader stage.
    Mesh          = 7, ///< Mesh shader stage.
    Count         = 8, ///< Number of stages.
};

/**
 * @brief Provided by Wisdom 0.7.0. Type of the descriptor in the descriptor table.
 *
 * */
enum class DescriptorType {
    Sampler               = 0, ///< Descriptor is a sampler.
    ConstantBuffer        = 1, ///< Descriptor is a constant buffer.
    Texture               = 2, ///< Descriptor is a texture.
    RWTexture             = 3, ///< Descriptor is an unordered access read-write texture.
    RWBuffer              = 4, ///< Descriptor is an unordered access read-write buffer.
    Buffer                = 5, ///< Descriptor is a shader resource buffer.
    AccelerationStructure = 6, ///< Descriptor is an acceleration structure.
    Count                 = 7, ///< Descriptor is a mutable type.
};

/**
 * @brief Provided by Wisdom 0.7.0. Comparison function for depth and stencil operations.
 *
 * */
enum class CompareOp {
    None         = 0, ///< No comparison.
    Never        = 1, ///< Always fail the comparison.
    Less         = 2, ///< Pass the comparison if the source value is less than the destination value.
    Equal        = 3, ///< Pass the comparison if the source value is equal to the destination value.
    LessEqual    = 4, ///< Pass the comparison if the source value is less than or equal to the destination value.
    Greater      = 5, ///< Pass the comparison if the source value is greater than the destination value.
    NotEqual     = 6, ///< Pass the comparison if the source value is not equal to the destination value.
    GreaterEqual = 7, ///< Pass the comparison if the source value is greater than or equal to the destination value.
    Always       = 8, ///< Always pass the comparison.
};

/**
 * @brief Provided by Wisdom 0.7.0. Address mode for texture sampling.
 *
 * */
enum class AddressMode {
    Repeat            = 0, ///< Repeat the texture.
    MirroredRepeat    = 1, ///< Repeat the texture with mirroring.
    ClampToEdge       = 2, ///< Clamp the texture to the edge.
    ClampToBorder     = 3, ///< Clamp the texture to the border.
    MirrorClampToEdge = 4, ///< Mirror and clamp the texture to the edge.
};

/**
 * @brief Provided by Wisdom 0.7.0. Filtering mode for texture sampling.
 *
 * */
enum class Filter {
    Point  = 0, ///< Nearest neighbor filtering.
    Linear = 1, ///< Linear filtering.
};

/**
 * @brief Provided by Wisdom 0.7.0. Reduction mode for texture sampling when min and mag filters are different.
 *
 * */
enum class ReductionMode {
    Standard = 0, ///< Standard filtering behavior.
    Min      = 1, ///< Use the minimum value of the samples for filtering.
    Max      = 2, ///< Use the maximum value of the samples for filtering.
};

/**
 * @brief Provided by Wisdom 0.7.0. Predefined static border colors for samplers. Used when address mode is set to Border.
 *
 * */
enum class StaticBorder {
    TransparentBlack = 0, ///< Transparent black border color.
    OpaqueBlack      = 1, ///< Opaque black border color.
    OpaqueWhite      = 2, ///< Opaque white border color.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor heap type.
 *
 * */
enum class DescriptorHeapType {
    Descriptor = 0, ///< Descriptor heap type. Used for all descriptor types, except for samplers.
    Sampler    = 1, ///< Sampler heap type. Used for sampler descriptors.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor memory type. Decides if descriptors are visible and can be bound to GPU.
 *
 * */
enum class DescriptorMemoryType {
    CpuOnly       = 0, ///< Descriptors are only visible to CPU. May be used for copying descriptors to the GPU visible pool.
    ShaderVisible = 1, ///< Descriptors are visible to GPU. Descriptors can be bound to the GPU pipeline directly, but can't be copied from.
};

/**
 * @brief Provided by Wisdom 0.7.0. Component swizzle for texture sampling.
 *
 * */
enum class ComponentSwizzle {
    Identity = 0, ///< Use the component as is for sampling.
    Red      = 1, ///< Use the red component for sampling.
    Green    = 2, ///< Use the green component for sampling.
    Blue     = 3, ///< Use the blue component for sampling.
    Alpha    = 4, ///< Use the alpha component for sampling.
    Zero     = 5, ///< Use zero for sampling.
    One      = 6, ///< Use one for sampling.
};

/**
 * @brief Provided by Wisdom 0.7.0. Determines the behavior when wait for multiple fences is issued.
 *
 * */
enum class MutiWaitType {
    All = 0, ///< All the fences in the batch are triggered.
    Any = 1, ///< At least one of the fences from the batch is triggered.
};

/**
 * @brief Provided by Wisdom 0.7.0. Input classification for vertex buffer data.
 *
 * */
enum class InputClass {
    PerVertex   = 0, ///< Vertex buffer data is vertex data.
    PerInstance = 1, ///< Vertex buffer data is per instance data.
};

/**
 * @brief Provided by Wisdom 0.7.0. Query type for GPU queries.
 *
 * */
enum class QueryPropertyType {
    DeviceCommandQueueProperties   = 0, ///< Properties of the device command queues. Expects a wis::DeviceCommandQueueProperties struct.
    DeviceDescriptorHeapProperties = 1, ///< Properties of the device descriptor heap. Expects a wis::DeviceDescriptorHeapProperties struct.
    DeviceMemoryProperties         = 2, ///< Properties of the device descriptor heap. Expects a wis::DeviceMemoryProperties struct.
    DeviceBindingProperties        = 3, ///< Properties of the device resource binding. Expects a wis::DeviceBindingProperties struct.
};

/**
 * @brief Provided by Wisdom 0.7.0. Memory type for resource allocation.
 *
 * */
enum class MemoryType {
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
enum class TextureState {
    Undefined         = -1, ///< Undefined state.
    Common            = 0, ///< Common state.
    Read              = 1, ///< General Read state.
    RenderTarget      = 2, ///< Render Target state.
    UnorderedAccess   = 3, ///< Unordered Access state.
    DepthStencilWrite = 4, ///< Depth Stencil Write state.
    DepthStencilRead  = 5, ///< Depth Stencil Read state.
    ShaderResource    = 6, ///< Shader Resource state.
    CopySrc           = 7, ///< Copy Source state.
    CopyDst           = 8, ///< Copy Destination state.
    Present           = 9, ///< Present swapchain state.
    ShadingRate       = 10, ///< Shading Rate state. Used for Variable Shading Rate.
    VideoDecodeRead   = 11, ///< Video Decode Read state.
    VideoDecodeWrite  = 12, ///< Video Decode Write state.
};

/**
 * @brief Provided by Wisdom 0.7.0. Intermediate shader representation.
 *
 * */
enum class ShaderIntermediate {
    DXIL  = 0, ///< DirectX Intermediate Language.
    SPIRV = 1, ///< Standard Portable Intermediate Representation for Vulkan.
};

/**
 * @brief Provided by Wisdom 0.7.0. Stencil operation for depth and stencil operations.
 *
 * */
enum class StencilOp {
    Keep     = 1, ///< Keep the current value.
    Zero     = 2, ///< Set the value to zero.
    Replace  = 3, ///< Replace the value with the reference value.
    IncClamp = 4, ///< Increment the value and clamp to the maximum value.
    DecClamp = 5, ///< Decrement the value and clamp to the minimum value.
    Invert   = 6, ///< Invert the value.
    IncWrap  = 7, ///< Increment the value and wrap to zero when the maximum value is exceeded.
    DecWrap  = 8, ///< Decrement the value and wrap to the maximum value when the minimum value is exceeded.
};

/**
 * @brief Provided by Wisdom 0.7.0. Primitive topology type for wis::GraphicsPipelineDesc.
 *
 * */
enum class TopologyType {
    Point    = 1, ///< Render points for each vertex.
    Line     = 2, ///< Render lines between vertices.
    Triangle = 3, ///< Render triangles between vertices.
    Patch    = 4, ///< Vertices are interpret as patch list. Used in tesselation process.
};

/**
 * @brief Provided by Wisdom 0.7.0. Fill mode for rasterizer.
 *
 * */
enum class FillMode {
    Lines = 2, ///< Draw lines between vertices. Wireframe rendering.
    Solid = 3, ///< Fill the area between vertices forming polygons.
};

/**
 * @brief Provided by Wisdom 0.7.0. Cull mode for rasterizer.
 * Triangle culling depends on wis::WindingOrder option.
 *
 * */
enum class CullMode {
    None  = 1, ///< No culling.
    Front = 2, ///< Cull front-facing triangles.
    Back  = 3, ///< Cull back-facing triangles.
};

/**
 * @brief Provided by Wisdom 0.7.0. Winding order for front-facing triangles.
 *
 * */
enum class WindingOrder {
    Clockwise        = 0, ///< Front-facing triangles have clockwise winding order.
    CounterClockwise = 1, ///< Front-facing triangles have counter-clockwise winding order.
};

/**
 * @brief Provided by Wisdom 0.7.0. Conservative rasterization mode.
 *
 * */
enum class ConservativeRasterization {
    Off          = 0, ///< Conservative rasterization is disabled.
    Overestimate = 1, ///< Conservative rasterization is enabled.
};

/**
 * @brief Provided by Wisdom 0.7.0. Line rasterization mode.
 *
 * */
enum class LineRasterization {
    Default          = 0, ///< Line rasterization is disabled. Lines are not rendered.
    Rectangular      = 1, ///< Lines are rasterized as rectangles.
    AlphaAntialiased = 2, ///< Lines are rasterized as rectangles with anti-aliasing.
};

/**
 * @brief Provided by Wisdom 0.7.0. Blend factor for color blending operations.
 *
 * */
enum class BlendFactor {
    Zero           = 1, ///< Use zero for blending.
    One            = 2, ///< Use one for blending.
    SrcColor       = 3, ///< Use the source color for blending.
    InvSrcColor    = 4, ///< Use the inverse source color for blending.
    SrcAlpha       = 5, ///< Use the source alpha for blending.
    InvSrcAlpha    = 6, ///< Use the inverse source alpha for blending.
    DestAlpha      = 7, ///< Use the destination alpha for blending.
    InvDestAlpha   = 8, ///< Use the inverse destination alpha for blending.
    DestColor      = 9, ///< Use the destination color for blending.
    InvDestColor   = 10, ///< Use the inverse destination color for blending.
    SrcAlphaSat    = 11, ///< Use the source alpha saturated for blending.
    ConstantColor  = 14, ///< Use a constant blend factor for blending.
    InvBlendFactor = 15, ///< Use the inverse constant blend factor for blending.
    Src1Color      = 16, ///< Use the source color for blending. Dual source blending mode.
    InvSrc1Color   = 17, ///< Use the inverse source color for blending. Dual source blending mode.
    Src1Alpha      = 18, ///< Use the source alpha for blending. Dual source blending mode.
    InvSrc1Alpha   = 19, ///< Use the inverse source alpha for blending. Dual source blending mode.
};

/**
 * @brief Provided by Wisdom 0.7.0. Blend operation for color blending operations.
 *
 * */
enum class BlendOp {
    Add         = 1, ///< Add the source and destination colors.
    Subtract    = 2, ///< Subtract the source color from the destination color.
    RevSubtract = 3, ///< Subtract the destination color from the source color.
    Min         = 4, ///< Use the minimum of the source and destination colors.
    Max         = 5, ///< Use the maximum of the source and destination colors.
};

/**
 * @brief Provided by Wisdom 0.7.0. Logic operation for color blending operations.
 *
 * */
enum class LogicOp {
    Clear        = 0, ///< Clear the destination value.
    Set          = 1, ///< Set the destination value.
    Copy         = 2, ///< Copy the source value to the destination.
    CopyInverted = 3, ///< Copy the inverted source value to the destination.
    Noop         = 4, ///< Do not modify the destination value.
    Invert       = 5, ///< Invert the destination value.
    And          = 6, ///< Perform a bitwise AND operation on the source and destination values.
    Nand         = 7, ///< Perform a bitwise NAND operation on the source and destination values.
    Or           = 8, ///< Perform a bitwise OR operation on the source and destination values.
    Nor          = 9, ///< Perform a bitwise NOR operation on the source and destination values.
    Xor          = 10, ///< Perform a bitwise XOR operation on the source and destination values.
    Equiv        = 11, ///< Perform a bitwise equivalent operation on the source and destination values.
    AndReverse   = 12, ///< Perform a bitwise AND operation on the source and inverted destination values.
    AndInverted  = 13, ///< Perform a bitwise AND operation on the inverted source and destination values.
    OrReverse    = 14, ///< Perform a bitwise OR operation on the source and inverted destination values.
    OrInverted   = 15, ///< Perform a bitwise OR operation on the inverted source and destination values.
};

/**
 * @brief Provided by Wisdom 0.7.0. Primitive topology for rendering.
 * More info could be found [here](https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies).
 *
 * */
enum class PrimitiveTopology {
    PointList        = 1, ///< Render points for each vertex.
    LineList         = 2, ///< Render lines between vertices.
    LineStrip        = 3, ///< Render lines between vertices in a strip.
    TriangleList     = 4, ///< Render triangles between vertices.
    TriangleStrip    = 5, ///< Render triangles between vertices in a strip.
    TriangleFan      = 6, ///< Interpret vertex data to form a fan of triangles.
    LineListAdj      = 10, ///< Render lines between vertices with adjacency.
    LineStripAdj     = 11, ///< Render lines between vertices in a strip with adjacency.
    TriangleListAdj  = 12, ///< Render triangles between vertices with adjacency.
    TriangleStripAdj = 13, ///< Render triangles between vertices in a strip with adjacency.
};

/**
 * @brief Provided by Wisdom 0.7.0. Primitive restart value for indexed drawing with primitive restart enabled.
 *
 * */
enum class PrimitiveRestartValue {
    None      = 0, ///< Primitive restart is disabled. No primitive restart value is used.
    UInt16Max = 65535, ///< Use the maximum value of uint16_t as the primitive restart value.
    UInt32Max = -1, ///< Use the maximum value of uint32_t as the primitive restart value.
};

/**
 * @brief Provided by Wisdom 0.7.0. Type of the view descriptor heap.
 *
 * */
enum class ViewHeapType {
    RenderTarget = 0, ///< Descriptor heap for render target views.
    DepthStencil = 1, ///< Descriptor heap for depth stencil views.
};

/**
 * @brief Provided by Wisdom 0.7.0. Flags that describe adapter.
 *
 * */
enum class AdapterFlags : uint32_t {
    None     = 0, ///< No flags set. Adapter @wis_may be discrete or embedded.
    Remote   = (1u << 0), ///< Adapter is remote. Used for remote rendering.
    Software = (1u << 1), ///< Adapter is software. Uses CPU for software rendering.
};
WISDOM_DEFINE_ENUM_OPERATORS(AdapterFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Flags for descriptor heap creation.
 *
 * */
enum class DescriptorHeapFlags : uint32_t {
    None                     = 0, ///< No flags set.
    DisallowEmbeddedSamplers = (1u << 1), ///< Heap is used in full for dynamic samplers. There @wis_must_not be any shader that use embedded samplers that uses that heap. User @wis_may allocate more samplers in the heap than it would normally be.
};
WISDOM_DEFINE_ENUM_OPERATORS(DescriptorHeapFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Flags for sampler creation.
 *
 * */
enum class SamplerFlags : uint32_t {
    None                     = 0, ///< No flags set.
    NonNormalizedCoordinates = (1u << 0), ///< Use non-normalized texture coordinates.
};
WISDOM_DEFINE_ENUM_OPERATORS(SamplerFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Buffer usage flags.
 * Determine how the buffer can be used throughout its lifetime.
 *
 * */
enum class BufferUsageFlags : uint32_t {
    None                        = 0, ///< No flags set. Buffer is not used.
    CopySrc                     = (1u << 0), ///< Buffer is used as a source for copy operations.
    CopyDst                     = (1u << 1), ///< Buffer is used as a destination for copy operations.
    ConstantBuffer              = (1u << 2), ///< Buffer is used as a constant buffer.
    IndexBuffer                 = (1u << 3), ///< Buffer is used as an index buffer.
    VertexBuffer                = (1u << 4), ///< Buffer is used as a vertex buffer or an instance buffer.
    IndirectBuffer              = (1u << 5), ///< Buffer is used as an indirect buffer.
    StorageBuffer               = (1u << 6), ///< Buffer is used as a storage unordered access buffer.
    AccelerationStructureBuffer = (1u << 7), ///< Buffer is used as an acceleration structure buffer.
    AccelerationStructureInput  = (1u << 8), ///< Buffer is used as a read only acceleration instance input buffer.
    ShaderBindingTable          = (1u << 9), ///< Buffer is used as a shader binding table buffer.
};
WISDOM_DEFINE_ENUM_OPERATORS(BufferUsageFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Texture usage flags.
 * Determine how the texture can be used throughout its lifetime.
 *
 * */
enum class TextureUsageFlags : uint32_t {
    None            = 0, ///< No flags set. Texture is not used.
    RenderTarget    = (1u << 0), ///< Texture is used as a render target.
    DepthStencil    = (1u << 1), ///< Texture is used as a depth stencil buffer.
    CopySrc         = (1u << 2), ///< Texture is used as a source for copy operations.
    CopyDst         = (1u << 3), ///< Texture is used as a destination for copy operations.
    ShaderResource  = (1u << 4), ///< Texture is used as a shader resource.
    UnorderedAccess = (1u << 5), ///< Texture is used as an unordered access resource.
    HostCopy        = (1u << 7), ///< Texture is used for host copy operations. Works with GPUUpload heap.
};
WISDOM_DEFINE_ENUM_OPERATORS(TextureUsageFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Memory flags.
 * Determine optional properties of the memory allocation.
 *
 * */
enum class MemoryFlags : uint32_t {
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
enum class TextureFlags : uint32_t {
    None = 0, ///< No flags set. Texture is regular.
};
WISDOM_DEFINE_ENUM_OPERATORS(TextureFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Texture binding flags, used for extra options.
 *
 * */
enum class TextureBindingFlags : uint32_t {
    None        = 0, ///< No flags set. Texture view is regular. Implies color read.
    DepthView   = (1u << 0), ///< Texture view is used to read depth. Used for special formats that feature depth and stencil. The bound texture @wis_must be in TODO: specific layout before being used by shader.
    StencilView = (1u << 1), ///< Texture view is used to read stencil. Used for special formats that feature depth and stencil. The bound texture @wis_must be in TODO: specific layout before being used by shader. Cannot be combined with `wis::TextureBindingFlags::DepthView`.
};
WISDOM_DEFINE_ENUM_OPERATORS(TextureBindingFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Barrier synchronization flags for resource barriers.
 *
 * */
enum class BarrierSync : uint32_t {
    None            = 0, ///< No synchronization is performed.
    All             = (1u << 0), ///< Synchronize all commands.
    Draw            = (1u << 1), ///< Synchronize draw commands.
    IndexInput      = (1u << 2), ///< Synchronize index input commands.
    VertexShading   = (1u << 3), ///< Synchronize vertex shading commands.
    PixelShading    = (1u << 4), ///< Synchronize pixel shading commands.
    DepthStencil    = (1u << 5), ///< Synchronize depth-stencil commands.
    RenderTarget    = (1u << 6), ///< Synchronize render target commands.
    Compute         = (1u << 7), ///< Synchronize compute commands.
    Raytracing      = (1u << 8), ///< Synchronize ray tracing commands.
    Copy            = (1u << 9), ///< Synchronize copy commands.
    Resolve         = (1u << 10), ///< Synchronize resolve commands.
    ExecuteIndirect = (1u << 11), ///< Synchronize execute indirect commands.
    AllShading      = (1u << 12), ///< Synchronize all shading commands.
    NonPixelShading = (1u << 13), ///< Synchronize non-pixel shading commands.
    ClearUAV        = (1u << 14), ///< Synchronize UAV clear commands.
    VideoDecode     = (1u << 15), ///< Synchronize video decode commands.
    VideoEncode     = (1u << 16), ///< Synchronize video encode commands.
    BuildRTAS       = (1u << 17), ///< Synchronize ray tracing acceleration structure build commands.
    CopyRTAS        = (1u << 18), ///< Synchronize ray tracing acceleration structure copy commands.
};
WISDOM_DEFINE_ENUM_OPERATORS(BarrierSync)

/**
 * @brief Provided by Wisdom 0.7.0. Resource access flags for resource barriers.
 *
 * */
enum class ResourceAccess : uint32_t {
    Common                     = 0, ///< Common access.
    VertexBuffer               = (1u << 0), ///< Vertex buffer access. Applies only to buffers.
    ConstantBuffer             = (1u << 1), ///< Constant buffer access. Applies only to buffers.
    IndexBuffer                = (1u << 2), ///< Index buffer access. Applies only to buffers.
    RenderTarget               = (1u << 3), ///< Render target access. Applies only to textures.
    UnorderedAccess            = (1u << 4), ///< Unordered access.
    DepthWrite                 = (1u << 5), ///< Depth write access. Applies only to depth-stencil textures.
    DepthRead                  = (1u << 6), ///< Depth read access. Applies only to depth-stencil textures.
    ShaderResource             = (1u << 7), ///< Shader resource access.
    StreamOutput               = (1u << 8), ///< Stream output access. Applies only to buffers.
    IndirectArgument           = (1u << 9), ///< Indirect argument access.
    CopyDst                    = (1u << 10), ///< Copy destination access.
    CopySrc                    = (1u << 11), ///< Copy source access.
    ConditionalRendering       = (1u << 12), ///< Conditional rendering access.
    AccelerationStructureRead  = (1u << 13), ///< Acceleration structure read access.
    AccelerationStructureWrite = (1u << 14), ///< Acceleration structure write access.
    ShadingRate                = (1u << 15), ///< Shading rate access.
    VideoDecodeRead            = (1u << 16), ///< Video decode read access.
    VideoDecodeWrite           = (1u << 17), ///< Video decode write access.
    ResolveDst                 = (1u << 18), ///< Resolve destination access.
    ResolveSrc                 = (1u << 19), ///< Resolve source access.
    None                       = (1u << 31), ///< No access. Used to indicate no access throughout the pipeline.
};
WISDOM_DEFINE_ENUM_OPERATORS(ResourceAccess)

/**
 * @brief Provided by Wisdom 0.7.0. Barrier flags for resource barriers.
 *
 * */
enum class BarrierFlags : uint32_t {
    None            = 0, ///< No flags set. Barrier is regular.
    DiscardContent  = (1u << 0), ///< Discard resource content. The content of the resource before the barrier is treated as if resource was not initialized.
    DepthResource   = (1u << 1), ///< Resource is a depth resource. This flag @wis_must be set for all depth resources to make transitions on them.
    StencilResource = (1u << 2), ///< Resource is a stencil resource. This flag @wis_must be set for all stencil resources to make transitions on them. If resource has format `wis::DataFormat::D24UnormS8Uint` both `wis::BarrierFlags::DepthResource` and `wis::BarrierFlags::StencilResource` @wis_must be set.
    WholeRange      = (1u << 3), ///< Transition whole resource. If not set, the transition is applied only to the specified subresource range. If set, the subresource range is ignored and the transition is applied to all subresources of the resource.
    PlanarImage     = (1u << 4), ///< Resource is a planar image. If the flag is not set, plane slices in wis::SubresourceRange are ignored.
};
WISDOM_DEFINE_ENUM_OPERATORS(BarrierFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Pipeline creation flags.
 *
 * */
enum class PipelineFlags : uint32_t {
    None                   = 0, ///< No flags set. Pipeline is regular.
    FailOnCacheMiss        = (1u << 0), ///< Fail pipeline creation if the pipeline cache is missing or incompatible. If not set, the implementation @wis_may choose to create the pipeline without using the cache, which @wis_may result in longer creation time.
    EnablePrimitiveRestart = (1u << 1), ///< Enable primitive restart for graphics pipelines. If not set, primitive restart is disabled and the implementation @wis_may choose to ignore restart indices in draw calls.
    DynamicDepthBias       = (1u << 2), ///< Enable dynamic depth bias for graphics pipelines. If not set, depth bias is static and @wis_must be specified at pipeline creation time.
};
WISDOM_DEFINE_ENUM_OPERATORS(PipelineFlags)

/**
 * @brief Provided by Wisdom 0.7.0. Color component flags.
 * Used for color blending operations.
 *
 * */
enum class ColorComponents : uint32_t {
    None = 0, ///< No flags set. Color blending is not used.
    R    = (1u << 0), ///< Use red component for blending.
    G    = (1u << 1), ///< Use green component for blending.
    B    = (1u << 2), ///< Use blue component for blending.
    A    = (1u << 3), ///< Use alpha component for blending.
    All  = 15, ///< Use all color components for blending.
};
WISDOM_DEFINE_ENUM_OPERATORS(ColorComponents)

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
using DebugCallback = void (*)(wis::Severity severity, const char* message, std::uint64_t device, void* user_data);

//==============================================================
// Structs
//==============================================================

/**
 * @brief Provided by Wisdom 0.7.0. Main source of communication of operation success.
 *
 * */
struct WIS_NODISCARD Result {
    wis::Status  status; ///< defines operation status. Compare with `wis::Status::Ok`.
    std::int32_t platform_code; ///< defines platform code from underlying implementation. Is an `HRESULT` for DX12 and a `VkResult` for Vulkan.
    const char*  error; ///< contains a human readable error message.
};

/**
 * @brief Provided by Wisdom 0.7.0. Adapter description. Describes hardware driver identification as well as memory limits.
 *
 * */
struct AdapterDesc {
    std::array<char, 256>        description; ///< Adapter description. Contains name of the graphics adapter.
    std::uint32_t                vendor_id; ///< denotes Vendor ID. Can be used to find the correct adapter.
    std::uint32_t                device_id; ///< denotes Device ID. Together with `wis::AdapterDesc::vendor_id` uniquely identifies the device.
    std::uint64_t                dedicated_video_memory; ///< measures dedicated video memory in bytes. Used for device local memory type.
    std::uint64_t                shared_system_memory; ///< measures memory that is shared with CPU in bytes. Used for upload and readback.
    std::uint64_t                adapter_id; ///< denotes adapter unique ID (LUID). Can be used to find the correct adapter.
    std::array<std::uint8_t, 16> adapter_uuid; ///< stores UUID of the adapter, used only with Vulkan API on systems with no LUID.
    wis::AdapterFlags            flags; ///< Adapter flags. Describe the adapter kind.
};

/**
 * @brief Provided by Wisdom 0.7.0. Debugging and logging description. Used to configure debug callback behavior.
 *
 * */
struct DebugDesc {
    bool               enable_debug_layer; ///< enables or disables debug layer on both DX12 and VK backends.
    wis::DebugCallback callback; ///< defines the debug callback function.
    void*              user_data; ///< user defined data pointer passed to the callback.
};

/**
 * @brief Provided by Wisdom 0.7.0. Command queue description for wis::CommandQueue creation.
 *
 * */
struct CommandQueueDesc {
    wis::CommandQueueType     type; ///< defines the type of the command queue.
    wis::CommandQueuePriority priority; ///< defines command queue priority. Used to set priority of the command queues of the type.
};

/**
 * @brief Provided by Wisdom 0.7.0. Sampler description for  creation.
 *
 * */
struct SamplerDesc {
    wis::Filter        min_filter; ///< Minification filter.
    wis::Filter        mag_filter; ///< Magnification filter.
    wis::Filter        mip_filter; ///< Mip level filter.
    wis::ReductionMode reduction_mode; ///< Reduction mode for min/mag/mip filters.
    bool               is_anisotropic; ///< Anisotropic filtering enable.
    std::uint32_t      max_anisotropy; ///< Max anisotropy level. Min is 1, Max is 16.
    wis::AddressMode   address_u; ///< Address mode for U coordinate.
    wis::AddressMode   address_v; ///< Address mode for V coordinate.
    wis::AddressMode   address_w; ///< Address mode for W coordinate.
    float              min_lod; ///< Min LOD value.
    float              max_lod; ///< Max LOD value.
    float              mip_lod_bias; ///< Mip LOD bias value.
    wis::CompareOp     comparison_op; ///< Comparison operation for comparison samplers.
    wis::StaticBorder  static_border_color; ///< Static border color. Used if any address mode is set to wis::AddressMode.
    wis::SamplerFlags  flags; ///< Sampler flags. Used to set additional sampler options.
};

/**
 * @brief Provided by Wisdom 0.7.0. Static sampler description for  creation.
 *
 * */
struct StaticSamplerDesc {
    wis::SamplerDesc      sampler; ///< Sampler description.
    wis::ShaderVisibility visibility; ///< Shader stage visibility. Defines the stage where the sampler is used.
    std::uint32_t         bind_register; ///< Bind register number in HLSL.
};

/**
 * @brief Provided by Wisdom 0.7.0. A set of constants that get read directly from root buffer.
 *
 * */
struct PushConstant {
    wis::ShaderVisibility visibility; ///< Shader stage visibility for the push constant.
    std::uint32_t         size_bytes; ///< Size of the constant in bytes. Must be divisible by 4.
    std::uint32_t         bind_register; ///< Bind register number in HLSL.
    std::uint32_t         bind_space; ///< Bind space number in HLSL. `register(regN, spaceN)`
};

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor. Used to push data directly to pipeline.
 *
 * */
struct PushDescriptor {
    wis::ShaderVisibility visibility; ///< Shader stage visibility for the push descriptor.
    wis::DescriptorType   type; ///< Descriptor type. Works only with buffer bindings.
    std::uint32_t         bind_register; ///< Bind register number in HLSL.
    std::uint32_t         bind_space; ///< Bind space number in HLSL. `register(regN, spaceN)`
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor table entry for wis::DescriptorTable.
 *
 * */
struct DescriptorTableEntry {
    wis::DescriptorType type; ///< Descriptor type.
    std::uint32_t       bind_register; ///< Bind register number in HLSL.
    std::uint32_t       bind_space; ///< Bind space number in HLSL. `register(regN, spaceN)`
    std::uint32_t       count; ///< Descriptor count for Array descriptors. UINT32_MAX means unbounded array. 0 means single register, same as 1.
    std::uint32_t       descriptor_offset; ///< Offset in descriptors from the heap start. Used for calculating descriptor indices when binding descriptor tables.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor table for  creation.
 *
 * */
struct DescriptorTable {
    wis::ShaderVisibility                      visibility; ///< Shader stage. Defines the stage where the table is used.
    wis::span<const wis::DescriptorTableEntry> entries; ///< Descriptor table entries array.
};

/**
 * @brief Provided by Wisdom 0.7.0. Pipeline layout description. Defines resource bindings for shaders.
 *
 * */
struct RootSignatureDesc {
    wis::span<const wis::PushConstant>    push_constants; ///< describes the global shader push data.
    wis::span<const wis::PushDescriptor>  push_descriptors; ///< points to an array of wis::PushDescriptor.
    wis::span<const wis::DescriptorTable> descriptor_tables; ///< points to an array of wis::DescriptorTable.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor heap description for wis::DescriptorHeap creation.
 *
 * */
struct DescriptorHeapDesc {
    wis::DescriptorHeapType   type; ///< indicates the type of descriptor heap to create (sampler or descriptor).
    wis::DescriptorMemoryType memory_type; ///< indicates where the descriptor heap will be allocated.
    std::uint32_t             descriptor_count; ///< indicates the amount of descriptors, present in the heap.
    wis::DescriptorHeapFlags  flags; ///< describe additional options for the descriptor heap.
};

/**
 * @brief Provided by Wisdom 0.7.0. Component mapping for .
 *
 * */
struct ComponentMapping {
    wis::ComponentSwizzle r; ///< Component mapping for Red channel. Default is `wis::ComponentSwizzle::Red`.
    wis::ComponentSwizzle g; ///< Component mapping for Green channel. Default is `wis::ComponentSwizzle::Green`.
    wis::ComponentSwizzle b; ///< Component mapping for Blue channel. Default is `wis::ComponentSwizzle::Blue`.
    wis::ComponentSwizzle a; ///< Component mapping for Alpha channel. Default is `wis::ComponentSwizzle::Alpha`.
};

/**
 * @brief Provided by Wisdom 0.7.0. Subresource description for texture data updates and copies.
 *
 * */
struct SubresourceRange {
    std::uint16_t base_mip_level; ///< Mipmap level of the subresource.
    std::uint16_t mip_level_count; ///< Number of mip levels in the subresource.
    std::uint16_t base_array_layer; ///< Array layer of the subresource. For 3D textures, this defines the depth slice.
    std::uint16_t array_layer_count; ///< Number of array layers in the subresource. For 3D textures, this defines the number of depth slices.
    std::uint16_t plane_slice; ///< Base depth slice of the subresource. Used only for 2D textures (YUV).
    std::uint16_t plane_slice_count; ///< Number of depth slices in the subresource. Used only for 2D textures (YUV). Max value is 3.
};

/**
 * @brief Provided by Wisdom 0.7.0. Buffer description for wis::Buffer creation.
 *
 * */
struct BufferDesc {
    std::uint64_t         size_bytes; ///< Size of the buffer in bytes.
    wis::BufferUsageFlags usage_flags; ///< Buffer usage flags. Describe how the buffer will be used.
    wis::MemoryType       memory_type; ///< indicates where the buffer will be allocated.
    wis::MemoryFlags      memory_flags; ///< The flags of the memory to allocate for the buffer.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture description for wis::Texture creation.
 *
 * */
struct TextureDesc {
    std::uint32_t          width; ///< Texture width in pixels.
    std::uint32_t          height; ///< Texture height in pixels.
    std::uint16_t          depth_or_array_size; ///< Texture depth in pixels. Used only for 3D textures.
    std::uint16_t          mip_levels; ///< Number of mip levels in the texture.
    wis::DataFormat        format; ///< Texture format.
    wis::SampleCount       sample_count; ///< Number of samples per pixel. Used only for multisampled textures.
    wis::TextureLayout     layout; ///< Texture layout. Default is `wis::TextureLayout::Texture2D`.
    wis::TextureUsageFlags usage_flags; ///< Texture usage flags. Describe how the texture will be used.
    wis::TextureFlags      flags; ///< Texture flags. Describe additional options for the texture.
    wis::MemoryType        memory_type; ///< indicates where the texture will be allocated.
    wis::MemoryFlags       memory_flags; ///< The flags of the memory to allocate for the texture.
};

/**
 * @brief Provided by Wisdom 0.7.0. Push constant set description for wis::CommandList::SetPushConstants.
 *
 * */
struct PushConstantDataDesc {
    wis::PipelineType pipeline; ///< defines the pipeline type to set the push constants for.
    std::uint32_t     root_index; ///< defines the root index in the root signature to set the push constants for.
    const void*       data; ///< points to the data to set as push constants.
    std::uint32_t     data_size; ///< defines the size of the data in bytes. It @wis_must be less than or equal to the maximum push constant size defined by the device and 4-byte aligned.
    std::uint32_t     push_offset; ///< defines the offset in bytes from the start of the push constant root parameter to set the data to. It @wis_must be less than the maximum push constant size defined by the device and 4-byte aligned.
};

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor set description for wis::CommandList::SetPushDescriptor.
 *
 * */
struct PushDescriptorDataDesc {
    wis::PipelineType   pipeline; ///< defines the pipeline type to set the push descriptors for.
    std::uint32_t       root_index; ///< defines the root index in the root signature to set the push descriptors for.
    wis::DescriptorType descriptor_type; ///< defines the type of the descriptors to push.
    std::uint64_t       buffer_address; ///< defines buffer device address to push.
};

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor set description for wis::CommandList::SetDescriptorTable.
 *
 * */
struct DescriptorTableDataDesc {
    wis::PipelineType       pipeline; ///< defines the pipeline type to set the push descriptors for.
    std::uint32_t           root_index; ///< defines the root index in the root signature to set the push descriptors for.
    wis::DescriptorHeapType heap_type; ///< defines the type of the descriptor heap to bind.
    std::uint32_t           heap_offset; ///< defines the offset in descriptors from the start of the heap to set the descriptor table to. Used for calculating descriptor indices when binding descriptor tables.
};

/**
 * @brief Provided by Wisdom 0.7.0. Constant buffer binding description for wis::DescriptorHeap::WriteConstantBuffer and .
 *
 * */
struct ConstantBufferBinding {
    std::uint64_t buffer_address; ///< defines buffer device address to bind.
    std::uint32_t size_bytes; ///< defines the size of the buffer region to bind in bytes.
};

/**
 * @brief Provided by Wisdom 0.7.0. Buffer binding description for wis::DescriptorHeap::WriteStructuredBuffer.
 *
 * */
struct BufferBinding {
    std::uint64_t array_offset; ///< defines offset in buffer in structures.
    std::uint32_t stride_bytes; ///< defines the size of the single structure in buffer.
    std::uint32_t structure_count; ///< defines the number of structures in the buffer region to bind.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture binding description for wis::DescriptorHeap::WriteTexture and wis::DescriptorHeap::WriteRWTexture.
 *
 * */
struct TextureBinding {
    wis::DataFormat          format; ///< defines the format of the view.
    wis::TextureLayout       layout; ///< defines the layout of the texture. Default is `wis::TextureLayout::Texture2D`.
    wis::TextureBindingFlags flags; ///< Texture binding flags. Describe additional options for the texture binding.
    wis::ComponentMapping    component_mapping; ///< Component mapping for the texture view.
    wis::SubresourceRange    range; ///< Subresource description for the texture view.
};

/**
 * @brief Provided by Wisdom 0.7.0. Stencil description for wis::DepthStencilDesc.
 *
 * */
struct StencilDesc {
    wis::StencilOp fail_op; ///< Stencil operation if the stencil test fails. Default is `wis::StencilOp::Keep`.
    wis::StencilOp depth_fail_op; ///< Stencil operation if the stencil test passes and the depth test fails. Default is `wis::StencilOp::Keep`.
    wis::StencilOp pass_op; ///< Stencil operation if the stencil test passes. Default is `wis::StencilOp::Keep`.
    wis::CompareOp stencil_comp; ///< Stencil comparison function. Default is .
    std::uint8_t   read_mask; ///< Stencil read mask. Default is 0xff.
    std::uint8_t   write_mask; ///< Stencil write mask. Default is 0xff.
};

/**
 * @brief Provided by Wisdom 0.7.0. Depth stencil description for wis::GraphicsPipelineDesc.
 *
 * */
struct DepthStencilDesc {
    bool             depth_enable; ///< Depth test enable. Default is false.
    bool             depth_write_enable; ///< Depth write enable. Default is false.
    wis::CompareOp   depth_comp; ///< Depth comparison function. Default is .
    bool             stencil_enable; ///< Stencil test enable. Default is false.
    wis::StencilDesc stencil_front; ///< Stencil description for front faces.
    wis::StencilDesc stencil_back; ///< Stencil description for back faces.
    bool             depth_bound_test; ///< Depth bound test enable. Default is false.
};

/**
 * @brief Provided by Wisdom 0.7.0. Input slot description for wis::InputLayout.
 *
 * */
struct InputBindingDesc {
    std::uint32_t   slot; ///< Input slot number. Must be unique.
    std::uint32_t   stride_bytes; ///< Stride in bytes. Size of one vertex in the slot.
    wis::InputClass input_class; ///< Input class. Defines how the data is read (Per vertex or Per instance).
};

/**
 * @brief Provided by Wisdom 0.7.0. Input attribute description for wis::InputLayout.
 *
 * */
struct InputAttributeDesc {
    std::uint32_t   binding_index; ///< Index into an array of bindings that the attribute is tied to.
    const char*     semantic_name; ///< Semantic name of the attribute in HLSL. Must be unique and null terminated.
    std::uint32_t   semantic_index; ///< Semantic index of the attribute in HLSL. Must be unique.
    std::uint32_t   location; ///< Location of the attribute in HLSL. Must be unique.
    wis::DataFormat format; ///< Data format of the attribute.
    std::uint32_t   offset_bytes; ///< Offset in bytes from the beginning of the vertex.
};

/**
 * @brief Provided by Wisdom 0.7.0. Input layout description for wis::GraphicsPipelineDesc.
 *
 * */
struct InputLayout {
    wis::span<const wis::InputBindingDesc>   bindings; ///< Input slots array. Made to pick up data from several arrays of vertex data.
    wis::span<const wis::InputAttributeDesc> attributes; ///< Input attributes array. Describes how the vertex data is read by the HLSL shader.
};

/**
 * @brief Provided by Wisdom 0.7.0. Rasterizer description for wis::GraphicsPipelineDesc.
 *
 * */
struct RasterizerDesc {
    wis::FillMode                  fill_mode; ///< Fill mode. Solid or Wireframe. Default is `wis::FillMode::Solid`.
    wis::CullMode                  cull_mode; ///< Cull mode. None, Front, Back. Default is `wis::CullMode::Back`.
    wis::WindingOrder              front_face; ///< Front face winding order. Clockwise or CounterClockwise. Default is `wis::WindingOrder::Clockwise`.
    bool                           depth_bias_enable; ///< Depth bias enable. Default is false.
    float                          depth_bias; ///< Depth bias. Default is 0.0f.
    float                          depth_bias_clamp; ///< Depth bias clamp. Default is 0.0f.
    float                          depth_bias_slope_factor; ///< Depth bias slope factor e.g. for shadows. Default is 0.0f.
    bool                           depth_clip_enable; ///< Depth clip enable. Default is true.
    wis::LineRasterization         line_rasterization; ///< Line rasterization mode. Default is `wis::LineRasterization::Default`.
    wis::ConservativeRasterization conservative_rasterization; ///< Conservative rasterization mode. Default is `wis::ConservativeRasterization::Off`.
};

/**
 * @brief Provided by Wisdom 0.7.0. Sample description of Multisampling for wis::GraphicsPipelineDesc.
 *
 * */
struct SampleDesc {
    wis::SampleCount rate; ///< Sample rate. Default is `wis::SampleCount::S1`.
    std::uint32_t    sample_mask; ///< Sample mask. Default is 0xffffffff.
    bool             alpha_to_coverage_enable; ///< Alpha to coverage enable. Default is false.
};

/**
 * @brief Provided by Wisdom 0.7.0. Render attachments description for wis::GraphicsPipelineDesc.
 *
 * */
struct RenderAttachmentsDesc {
    std::array<wis::DataFormat, 8> attachment_formats; ///< Attachment formats array. Describes the format of the render target.
    std::uint32_t                  attachments_count; ///< Attachment formats count. Max is 8.
    wis::DataFormat                depth_attachment; ///< Depth attachment format. Describes the format of the depth buffer.
    std::uint32_t                  view_mask; ///< View mask for multiview rendering. Each bit represents a view that can be rendered to with the pipeline. Default is 0, meaning no multiview support.
};

/**
 * @brief Provided by Wisdom 0.7.0. Blend attachment description for wis::BlendStateDesc.
 *
 * */
struct BlendAttachmentDesc {
    bool                 blend_enable; ///< Blend enable. Default is false.
    wis::BlendFactor     src_color_blend; ///< Source color blend factor. Default is `wis::BlendFactor::One`.
    wis::BlendFactor     dst_color_blend; ///< Destination color blend factor. Default is `wis::BlendFactor::Zero`.
    wis::BlendOp         color_blend_op; ///< Color blend operation. Default is `wis::BlendOp::Add`.
    wis::BlendFactor     src_alpha_blend; ///< Source alpha blend factor. Default is `wis::BlendFactor::One`.
    wis::BlendFactor     dst_alpha_blend; ///< Destination alpha blend factor. Default is `wis::BlendFactor::Zero`.
    wis::BlendOp         alpha_blend_op; ///< Alpha blend operation. Default is `wis::BlendOp::Add`.
    wis::ColorComponents color_write_mask; ///< Color write mask. Default is `wis::ColorComponents::All`.
};

/**
 * @brief Provided by Wisdom 0.7.0. Viewport description for wis::CommandList.
 * Viewport is considered from Top Left corner.
 *
 * */
struct Viewport {
    float top_leftx; ///< Top left corner x coordinate.
    float top_lefty; ///< Top left corner y coordinate.
    float width; ///< Viewport width.
    float height; ///< Viewport height.
    float min_depth; ///< Minimum depth of the viewport.
    float max_depth; ///< Maximum depth of the viewport.
};

/**
 * @brief Provided by Wisdom 0.7.0. Scissor description for wis::CommandList.
 *
 * */
struct Scissor {
    std::int32_t left; ///< Left corner x coordinate.
    std::int32_t top; ///< Top corner y coordinate.
    std::int32_t right; ///< Right corner x coordinate.
    std::int32_t bottom; ///< Bottom corner y coordinate.
};

/**
 * @brief Provided by Wisdom 0.7.0. Blend state description for wis::GraphicsPipelineDesc.
 *
 * */
struct BlendStateDesc {
    bool                                    logic_op_enable; ///< Logic operation enable. Default is false.
    wis::LogicOp                            logic_op; ///< Logic operation. Default is `wis::LogicOp::Noop`.
    std::array<wis::BlendAttachmentDesc, 8> attachments; ///< Blend attachment descriptions. Max Array size is 8.
    std::uint32_t                           attachment_count; ///< Blend attachment count. If set as 0 - broadcast to all attachments. Max is 8.
};

/**
 * @brief Provided by Wisdom 0.7.0. Box description for texture copy regions.
 *
 * */
struct Box {
    std::uint32_t x; ///< Left corner x coordinate.
    std::uint32_t y; ///< Top corner y coordinate.
    std::uint32_t z; ///< Front corner z coordinate. Used only for 3D textures.
    std::uint32_t width; ///< Width of the box.
    std::uint32_t height; ///< Height of the box.
    std::uint32_t depth; ///< Depth of the box. Used only for 3D textures.
};

/**
 * @brief Provided by Wisdom 0.7.0. Target subresource description for texture copy regions.
 *
 * */
struct TargetSubresource {
    std::uint16_t mip_level; ///< Mipmap level of the target subresource.
    std::uint16_t array_layer; ///< Array layer of the target subresource. For 3D textures @wis_must be 0.
    std::uint16_t plane_slice; ///< Depth slice of the target subresource. Used only for 2D textures (YUV).
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture copy region description for texture copy operations.
 *
 * */
struct TextureRegion {
    wis::Box               box; ///< Box defining the region to copy.
    wis::TargetSubresource target_subresource; ///< Target subresource description for the region.
};

/**
 * @brief Provided by Wisdom 0.7.0. Render target description for wis::ViewHeap::WriteRenderTarget and wis::ViewHeap::WriteDepthStencil.
 *
 * */
struct RenderTargetDesc {
    wis::DataFormat    format; ///< Render target format.
    wis::TextureLayout layout; ///< Render target layout. Default is `wis::TextureLayout::Texture2D`.
    std::uint16_t      mip_level; ///< Mipmap level of the target subresource.
    std::uint16_t      base_array_layer; ///< Array layer of the target subresource. For 3D textures, this defines the base depth slice.
    std::uint16_t      array_layer_count; ///< Number of array layers in the target subresource. For 3D textures, this defines the number of depth slices.
    std::uint16_t      plane_slice; ///< Depth slice of the target subresource. Used only for 2D textures (YUV).
};

/**
 * @brief Provided by Wisdom 0.7.0. Query struct header. Used as a header for all query structs.
 *
 * */
struct QueryStructHeader {
    wis::QueryPropertyType property_type; ///< Defines the type of the queried property. Used to determine what struct is passed.
    void*                  next_in_chain; ///< Pointer to the next queried data struct.
};

/**
 * @brief Provided by Wisdom 0.7.0. Device binding properties. Used to query resource binding support and limits.
 *
 * */
struct DeviceBindingProperties {
    wis::QueryPropertyType property_type; ///< Defines the type of the queried property. Used to determine what struct is passed. @wis_must be `wis::QueryPropertyType::DeviceBindingProperties`.
    void*                  next_in_chain; ///< Pointer to the next queried data struct.
    std::uint32_t          max_vertex_input_attributes; ///< Maximum number of vertex input attributes supported by the device. Used for vertex buffer bindings.
    std::uint32_t          max_vertex_input_bindings; ///< Maximum number of vertex input bindings supported by the device. Used for vertex buffer bindings.
    bool                   multiple_viewports_supported; ///< Indicates if multiple viewports are supported. If true, the device supports up to 16 viewports and scissor rectangles. If false, only one viewport and scissor rectangle is supported.
};

/**
 * @brief Provided by Wisdom 0.7.0. Device descriptor heap properties. Used to query descriptor heap support and limits.
 *
 * */
struct DeviceDescriptorHeapProperties {
    wis::QueryPropertyType property_type; ///< Defines the type of the queried property. Used to determine what struct is passed. @wis_must be `wis::QueryPropertyType::DeviceDescriptorHeapProperties`.
    void*                  next_in_chain; ///< Pointer to the next queried data struct.
    std::uint32_t          max_descriptor_heap_size; ///< Maximum number of descriptors in a single descriptor heap.
    std::uint32_t          max_sampler_heap_size; ///< Maximum number of samplers in a single descriptor heap.
    std::uint32_t          max_sampler_heap_size_with_embedded; ///< Maximum number of samplers in a single descriptor heap, if embedded samplers are used.
    std::uint32_t          descriptor_increment_size; ///< Size of a single descriptor in the descriptor heap. Used for calculating descriptor offsets.
    std::uint32_t          sampler_increment_size; ///< Size of a single sampler in the sampler heap. Used for calculating sampler offsets.
};

/**
 * @brief Provided by Wisdom 0.7.0. Device memory properties. Used to query memory type support and limits.
 *
 * */
struct DeviceCommandQueueProperties {
    wis::QueryPropertyType                   property_type; ///< Defines the type of the queried property. @wis_must be wis::QueryPropertyType..
    void*                                    next_in_chain; ///< Pointer to the next queried data struct.
    std::array<bool, 5>                      supported_queues; ///< Array of supported queue types. If a queue type is supported, the value is `1`, otherwise `0`. Order of queue types is the same as in wis::CommandQueueType enum.
    bool                                     relaxed_queue_transition; ///< Indicates if relaxed queue transition is supported. This feature allows executing command lists that contain buffers used on different queue types without explicit resource state transitions when the buffers is used on a different queue type. It is supported on Windows 10 22H2 and later with WDDM 3.0 or later. On Vulkan it requires `VK_KHR_maintenance9` extension.
    std::array<wis::CommandQueuePriority, 5> max_queue_priority; ///< Array of maximum supported priorities for each queue type. If a queue type is not supported, the value is `0`. Order of queue types is the same as in wis::CommandQueueType enum.
};

/**
 * @brief Provided by Wisdom 0.7.0. Device memory properties. Used to query memory type support and limits.
 *
 * */
struct DeviceMemoryProperties {
    wis::QueryPropertyType property_type; ///< Defines the type of the queried property. @wis_must be `wis::QueryPropertyType::DeviceMemoryProperties`..
    void*                  next_in_chain; ///< Pointer to the next queried data struct.
    bool                   gpu_upload_supported; ///< Indicates if GPU upload memory type is supported. This memory type allows mapping the memory and writing to it from CPU, while being accessible from GPU. It is usually implemented as write-combined memory on integrated GPUs and as a part of shared system memory on discrete GPUs.
    bool                   host_image_copy_supported; ///< Indicates if host image copy is supported. This feature allows copying data directly from CPU memory to optimal tiled image layout on GPU, without the need for an intermediate staging buffer. It is supported on Windows 10 22H2 and later with WDDM 3.0 or later. On Vulkan it requires `VK_EXT_host_image_copy` extension.
    std::uint32_t          supported_initial_transitions; ///< Bitfield of supported initial resource state transitions for buffers and textures. If a transition is supported, the corresponding bit is set to `1`, otherwise `0`. Bit positions are the same as in wis::TextureState enum. `wis::TextureState::Undefined` is always supported.
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

/// @brief Provided by Wisdom 0.7.0. Defines the minimum amount of vertex attributes that @wis_must be supported by the implementation.
static constexpr std::uint32_t MinSupportedInputAttributes = 16;

/// @brief Provided by Wisdom 0.7.0. Defines the minimum amount of vertex bindings that @wis_must be supported by the implementation.
static constexpr std::uint32_t MinSupportedInputBindings = 16;

/// @brief Provided by Wisdom 0.7.0. Defines the maximum amount of render targets that can be bound at once.
static constexpr std::uint32_t MaxRenderTargets = 8;

/// @brief Provided by Wisdom 0.7.0. Defines the maximum amount of viewports that can be bound at once. The same count applies to scissors.
static constexpr std::uint32_t MaxViewports = 16;

/// @brief Provided by Wisdom 0.7.0. Select whole size of a resource.
static constexpr std::uint64_t WholeSize = 0xffffffffffffffff;

} // namespace wis
#endif // WISDOM_CORE_CPP_API_HPP
