// This file is generated. Do not edit directly.
#ifndef WISDOM_CPP_API_HPP
#define WISDOM_CPP_API_HPP
#ifdef __cplusplus
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
 * @brief Provided by Wisdom 0.7.0. Shader stages that can be used in the pipeline. Main use is Root signature and descriptor management. Stages have no granularity, either all or one can be selected.
 *
 * */
enum class ShaderStages {
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
enum class CompareOperation {
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
 * @brief Provided by Wisdom 0.7.0. Predefined static border colors for samplers. Used when address mode is set to Border.
 *
 * */
enum class StaticBorder {
    TransparentBlack = 0, ///< Transparent black border color.
    OpaqueBlack      = 1, ///< Opaque black border color.
    OpaqueWhite      = 2, ///< Opaque white border color.
    Custom           = 3, ///< Custom border color defined by the user.
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
 * @brief Provided by Wisdom 0.7.0. Descriptor storage tier. Decides how many descriptors can be allocated in a single heap.
 *
 * */
enum class DescriptorStorageTier {
    Tier1 = 0, ///< Tier 1: VkDescriptorSets and VkDescriptorPools.
    Tier2 = 1, ///< Tier 2: Descriptor Buffer with no mutable descriptor type.
    Tier3 = 2, ///< Tier 3: Descriptor Buffer with mutable descriptor type.
    Tier4 = 3, ///< Tier 4: Descriptor Heap.
};

/**
 * @brief Provided by Wisdom 0.7.0. Query type for GPU queries.
 *
 * */
enum class QueryPropertyType {
    DeviceCommandQueueProperties   = 0, ///< Properties of the device command queues. Expects a  struct.
    DeviceDescriptorHeapProperties = 1, ///< Properties of the device descriptor heap. Expects a wis::DeviceDescriptorHeapProperties struct.
    DeviceMemoryProperties         = 2, ///< Properties of the device descriptor heap. Expects a wis::DeviceMemoryProperties struct.
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
 * @brief Provided by Wisdom 0.7.0. Flags that describe adapter.
 *
 * */
enum class AdapterFlags : uint32_t {
    None     = 0, ///< No flags set. Adapter @wis_may be discrete or embedded.
    Remote   = (1 << 0), ///< Adapter is remote. Used for remote rendering.
    Software = (1 << 1), ///< Adapter is software. Uses CPU for software rendering.
};

/**
 * @brief Provided by Wisdom 0.7.0. Flags for sampler creation.
 *
 * */
enum class SamplerFlags : uint32_t {
    None                     = 0, ///< No flags set.
    NonNormalizedCoordinates = (1 << 0), ///< Use non-normalized texture coordinates.
};

/**
 * @brief Provided by Wisdom 0.7.0. Buffer usage flags.
 * Determine how the buffer can be used throughout its lifetime.
 *
 * */
enum class BufferUsageFlags : uint32_t {
    None                        = 0, ///< No flags set. Buffer is not used.
    CopySrc                     = (1 << 0), ///< Buffer is used as a source for copy operations.
    CopyDst                     = (1 << 1), ///< Buffer is used as a destination for copy operations.
    ConstantBuffer              = (1 << 2), ///< Buffer is used as a constant buffer.
    IndexBuffer                 = (1 << 3), ///< Buffer is used as an index buffer.
    VertexBuffer                = (1 << 4), ///< Buffer is used as a vertex buffer or an instance buffer.
    IndirectBuffer              = (1 << 5), ///< Buffer is used as an indirect buffer.
    StorageBuffer               = (1 << 6), ///< Buffer is used as a storage unordered access buffer.
    AccelerationStructureBuffer = (1 << 7), ///< Buffer is used as an acceleration structure buffer.
    AccelerationStructureInput  = (1 << 8), ///< Buffer is used as a read only acceleration instance input buffer.
    ShaderBindingTable          = (1 << 9), ///< Buffer is used as a shader binding table buffer.
};

/**
 * @brief Provided by Wisdom 0.7.0. Texture usage flags.
 * Determine how the texture can be used throughout its lifetime.
 *
 * */
enum class TextureUsageFlags : uint32_t {
    None            = 0, ///< No flags set. Texture is not used.
    RenderTarget    = (1 << 0), ///< Texture is used as a render target.
    DepthStencil    = (1 << 1), ///< Texture is used as a depth stencil buffer.
    CopySrc         = (1 << 2), ///< Texture is used as a source for copy operations.
    CopyDst         = (1 << 3), ///< Texture is used as a destination for copy operations.
    ShaderResource  = (1 << 4), ///< Texture is used as a shader resource.
    UnorderedAccess = (1 << 5), ///< Texture is used as an unordered access resource.
    HostCopy        = (1 << 7), ///< Texture is used for host copy operations. Works with GPUUpload heap.
};

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
    DedicatedAllocation = (1 << 0),
    /**
     * @brief
     * Memory is mapped.
     * Used in combination with `wis::MemoryType::Upload` or `wis::MemoryType::Readback` to map memory for CPU access.
     * */
    Mapped = (1 << 1),
    /**
     * @brief
     * Memory is exportable.
     * If set, memory can be exported to other processes or APIs.
     * Works only with Device Local memory (`wis::MemoryType::Default`) and only on AllocateXMemory calls.
     * Outside of AllocateXMemory the flag is ignored.
     * */
    Exportable = (1 << 2),
};

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
    wis::Filter           min_filter; ///< Minification filter.
    wis::Filter           mag_filter; ///< Magnification filter.
    wis::Filter           mip_filter; ///< Mip level filter.
    bool                  is_anisotropic; ///< Anisotropic filtering enable.
    std::uint32_t         max_anisotropy; ///< Max anisotropy level. Max is 16.
    wis::AddressMode      address_u; ///< Address mode for U coordinate.
    wis::AddressMode      address_v; ///< Address mode for V coordinate.
    wis::AddressMode      address_w; ///< Address mode for W coordinate.
    float                 min_lod; ///< Min LOD value.
    float                 max_lod; ///< Max LOD value.
    float                 mip_lod_bias; ///< Mip LOD bias value.
    wis::CompareOperation comparison_op; ///< Comparison operation for comparison samplers.
    wis::StaticBorder     static_border_color; ///< Static border color. Used if any address mode is set to wis::AddressMode.
    std::array<float, 4>  border_color; ///< Border color. Used if any address mode is set to wis::AddressMode and static_border_color is set to `wis::StaticBorder::Custom`.
    wis::SamplerFlags     flags; ///< Sampler flags. Used to set additional sampler options.
};

/**
 * @brief Provided by Wisdom 0.7.0. Static sampler description for wis::PipelineLayout creation.
 *
 * */
struct StaticSamplerDesc {
    wis::SamplerDesc  sampler; ///< Sampler description.
    wis::ShaderStages stage; ///< Shader stage. Defines the stage where the sampler is used.
    std::uint32_t     bind_register; ///< Bind register number in HLSL.
};

/**
 * @brief Provided by Wisdom 0.7.0. A set of constants that get pushed directly to the pipeline. Only one set can be created per shader stage.
 *
 * */
struct PushConstant {
    wis::ShaderStages stage; ///< Shader stage. Defines the stage where the constant is used.
    std::uint32_t     size_bytes; ///< Size of the constant in bytes. Must be divisible by 4.
    std::uint32_t     bind_register; ///< Bind register number in HLSL.
    std::uint32_t     bind_space; ///< Bind space number in HLSL. `register(regN, spaceN)`
};

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor. Used to push data directly to pipeline.
 *
 * */
struct PushDescriptor {
    wis::ShaderStages   stage; ///< Shader stage. Defines the stage where the descriptor is used.
    wis::DescriptorType type; ///< Descriptor type. Works only with buffer bindings.
    std::uint32_t       bind_register; ///< Bind register number in HLSL.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor table entry for wis::DescriptorTable.
 *
 * */
struct DescriptorTableEntry {
    wis::DescriptorType type; ///< Descriptor type.
    std::uint32_t       bind_register; ///< Bind register number in HLSL.
    std::uint32_t       count; ///< Descriptor count for Array descriptors. UINT32_MAX means unbounded array. 0 means single register, same as 1.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor table for wis::PipelineLayout creation.
 *
 * */
struct DescriptorTable {
    wis::DescriptorHeapType                    type; ///< Descriptor heap type. Either Descriptor or Sampler.
    wis::ShaderStages                          stage; ///< Shader stage. Defines the stage where the table is used.
    wis::span<const wis::DescriptorTableEntry> entries; ///< Descriptor table entries array.
    std::uint32_t                              space_overlap; ///< If this value is not zero, bindings from this table can be bound several times in different spaces. Used for descriptor indexing into unbounded arrays of similar types.
};

/**
 * @brief Provided by Wisdom 0.7.0. Pipeline layout description. Defines resource bindings for shaders.
 *
 * */
struct PipelineLayoutDesc {
    wis::span<const wis::PushConstant>      push_constants; ///< points to an array of wis::PushConstant.
    wis::span<const wis::PushDescriptor>    push_descriptors; ///< points to an array of wis::PushDescriptor.
    wis::span<const wis::StaticSamplerDesc> static_samplers; ///< points to an array of wis::StaticSamplerDesc.
    wis::span<const wis::DescriptorTable>   descriptor_tables; ///< points to an array of wis::DescriptorTable.
};

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor heap description for wis::DescriptorHeap creation.
 *
 * */
struct DescriptorHeapDesc {
    wis::DescriptorHeapType   type; ///< indicates the type of descriptor heap to create (sampler or descriptor).
    wis::DescriptorMemoryType memory_type; ///< indicates where the descriptor heap will be allocated.
    std::size_t               descriptor_count; ///< indicates the amount of descriptors, present in the heap.
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
    wis::MemoryType        memory_type; ///< indicates where the texture will be allocated.
    wis::MemoryFlags       memory_flags; ///< The flags of the memory to allocate for the texture.
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
 * @brief Provided by Wisdom 0.7.0. Device descriptor heap properties. Used to query descriptor heap support and limits.
 *
 * */
struct DeviceDescriptorHeapProperties {
    wis::QueryPropertyType property_type; ///< Defines the type of the queried property. Used to determine what struct is passed. @wis_must be `wis::QueryPropertyType::DeviceDescriptorHeapProperties`.
    void*                  next_in_chain; ///< Pointer to the next queried data struct.
    std::size_t            max_descriptor_heap_size; ///< Maximum number of descriptors in a single descriptor heap.
    std::size_t            max_sampler_heap_size; ///< Maximum number of samplers in a single descriptor heap.
    std::size_t            max_sampler_heap_size_with_embedded; ///< Maximum number of samplers in a single descriptor heap, if embedded samplers are used.
    std::size_t            descriptor_increment_size; ///< Size of a single descriptor in the descriptor heap. Used for calculating descriptor offsets.
    std::size_t            sampler_increment_size; ///< Size of a single sampler in the sampler heap. Used for calculating sampler offsets.
};

/**
 * @brief Provided by Wisdom 0.7.0. Device memory properties. Used to query memory type support and limits.
 *
 * */
struct DeviceCommandQueuesProperties {
    wis::QueryPropertyType                   property_type; ///< Defines the type of the queried property. @wis_must be wis::QueryPropertyType..
    void*                                    next_in_chain; ///< Pointer to the next queried data struct.
    std::array<bool, 5>                      supported_queues; ///< Array of supported queue types. If a queue type is supported, the value is `1`, otherwise `0`. Order of queue types is the same as in wis::CommandQueueType enum.
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
};

} // namespace wis
#endif // __cplusplus
#endif // WISDOM_CPP_API_HPP
