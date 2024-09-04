// GENERATED
#pragma once
#include <array>
#include <cstdint>

/** \mainpage Wisdom API Documentation

<b>Version 0.2.0</b>

Copyright (c) 2024 Ilya Doroshenko. All rights reserved.
License: MIT
See also: [repository on GitHub](https://github.com/Agrael1/Wisdom)
*/

namespace wis {
struct Result;
struct Size2D;
struct Size3D;
struct BufferRegion;
struct AdapterDesc;
struct InputSlotDesc;
struct InputAttribute;
struct InputLayout;
struct RasterizerDesc;
struct SampleDesc;
struct StencilDesc;
struct DepthStencilDesc;
struct BlendAttachmentDesc;
struct BlendStateDesc;
struct RenderAttachmentsDesc;
struct RootConstant;
struct SwapchainDesc;
struct TextureDesc;
struct AllocationInfo;
struct TextureRegion;
struct BufferTextureCopyRegion;
struct PushDescriptor;
struct SubresourceRange;
struct RenderTargetDesc;
struct Viewport;
struct Scissor;
struct BufferBarrier;
struct TextureBarrier;
struct DescriptorTableEntry;
struct DescriptorTable;
struct SamplerDesc;
struct ComponentMapping;
struct ShaderResourceDesc;

/**
 * @brief Shader stages that can be used in the pipeline.
 * Main use is Root signature and descriptor management.
 * Stages have no granularity, either all or one can be selected.
 *
 * Translates to D3D12_SHADER_VISIBILITY for dx implementation.
 * Translates to VkShaderStageFlagBits for vk implementation.
 * */
enum class ShaderStages : uint32_t {
    All = 0, ///< All shader stages.
    Vertex = 1, ///< Vertex shader stage.
    Hull = 2, ///< Hull/Tessellation control shader stage.
    Domain = 3, ///< Domain/Tessellation evaluation shader stage.
    Geometry = 4, ///< Geometry shader stage.
    Pixel = 5, ///< Pixel/Fragment shader stage.
    Amplification = 6, ///< Amplification shader stage.
    Mesh = 7, ///< Mesh shader stage.
    Count = 8, ///< Number of stages.
};

/**
 * @brief Represents main error communication source for the API.
 * Compare against wis::Status::Ok for success.
 *
 * */
enum class Status : int32_t {
    Ok = 0, ///< Operation succeded.
    /**
     * @brief Operation timed out.
     * Counts as a success value in some cases.
     * */
    Timeout = 1,
    /**
     * @brief Operation partially succeeded.
     * Some times it means that core value is initialized,
     * but some functionality may be missing and may require
     * you to check if demanded functionality is in place.
     * */
    Partial = 2,
    Error = -1, ///< Operation failed. Check wis::Result::error for more details.
    /**
     * @brief One or more arguments, or parts of arguments
     * passed to the function were incorrect. Check wis::Result::error for more details.
     * */
    InvalidArgument = -2,
    /**
     * @brief There is no more host memory available.
     * Allocation with malloc or similar call has failed.
     * */
    OutOfMemory = -3,
    /**
     * @brief Device driver was forcefully stopped.
     * Most of the time happens on swapchain presentation.
     * */
    DeviceLost = -4,
    /**
     * @brief Swapchain presentation was not visible to the user.
     * Rendering is too fast.
     * */
    Occluded = -5,
};

/**
 * @brief Determines the behavior when wait for multiple fences is issued.
 *
 * */
enum class MutiWaitFlags : uint32_t {
    All = 0, ///< All the fences in the batch are triggered.
    Any = 1, ///< At least one of the fences from the batch is triggered.
};

/**
 * @brief Type of the descriptor in the descriptor table.
 *
 * Translates to D3D12_DESCRIPTOR_RANGE_TYPE for dx implementation.
 * Translates to VkDescriptorType for vk implementation.
 * */
enum class DescriptorType : uint32_t {
    /**
     * @brief Descriptor is a shader resource view.
     * Used for textures.
     * */
    ShaderResource = 0,
    ConstantBuffer = 1, ///< Descriptor is a constant buffer view.
    /**
     * @brief Descriptor is an unordered access view.
     * Used for read/write operations in compute shaders.
     * */
    UnorderedAccess = 2,
    /**
     * @brief Descriptor is a sampler.
     * Sampler is used to sample textures in shaders.
     * Stored in separate descriptor table and
     * can't be mixed with other descriptor types
     * */
    Sampler = 3,
};

/**
 * @brief Type of the queue to create.
 *
 * */
enum class QueueType : uint32_t {
    Graphics = 0, ///< Queue is used for graphics operations.
    Compute = 2, ///< Queue is used for compute operations.
    Copy = 3, ///< Queue is used for copy operations.
    VideoDecode = 4, ///< Queue is used for video decoding operations.
};

/**
 * @brief Oreders the adapters according to preference
 * using builtin heuristics of underlying APIs.
 *
 * Translates to DXGI_GPU_PREFERENCE for dx implementation.
 * */
enum class AdapterPreference {
    None = 0, ///< No particular preference, list adapters in system divised order.
    /**
     * @brief List the adapters from low power consumption to high.
     * Order is as follows: Integrated, Discrete, External, Software.
     * */
    MinConsumption = 1,
    /**
     * @brief List the adapters from high performance to low.
     * Order is as follows: External, Discrete, Integrated, Software.
     * */
    Performance = 2,
};

/**
 * @brief Log message severity.
 * Used with DebugCallback and internal library logging.
 *
 * */
enum class Severity {
    Debug = 0, ///< Message carries debug information.
    Trace = 1, ///< Message contains trace point (function call stack info).
    Info = 2, ///< Message contains general information.
    /**
     * @brief Message contains warning.
     * There is something wrong and it may affect performance or stability of the application.
     * */
    Warning = 3,
    /**
     * @brief Message contains error.
     * Stability of the application is compromized.
     * */
    Error = 4,
    /**
     * @brief Message contains critical error.
     * The application must be shut down, no further execution.
     * */
    Critical = 5,
};

/**
 * @brief Input classification for vertex buffer data.
 *
 * */
enum class InputClass : uint32_t {
    PerVertex = 0, ///< Vertex buffer data is vertex data.
    PerInstance = 1, ///< Vertex buffer data is per instance data.
};

/**
 * @brief Cull mode for rasterizer.
 * Triangle culling depends on wis::WindingOrder option.
 *
 * Translates to D3D12_CULL_MODE for dx implementation.
 * Translates to VkCullModeFlags for vk implementation.
 * */
enum class CullMode : uint32_t {
    None = 1, ///< No culling.
    Front = 2, ///< Cull front-facing triangles.
    Back = 3, ///< Cull back-facing triangles.
};

/**
 * @brief Data format for data.
 * Used as a template for data in several cases.
 * Can be used to describe:
 * - Vertex buffer data format
 * - Texture data format
 * - Render target data format
 * - Depth stencil data format
 *
 * Translates to DXGI_FORMAT for dx implementation.
 * Translates to VkFormat for vk implementation.
 * */
enum class DataFormat : uint32_t {
    Unknown = 0, ///< Unknown format.
    /**
     * @brief 32 bit per channel RGBA format.
     * A four-component, 128-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    RGBA32Float = 2,
    /**
     * @brief 32 bit per channel RGBA format.
     * A four-component, 128-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    RGBA32Uint = 3,
    /**
     * @brief 32 bit per channel RGBA format.
     * A four-component, 128-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    RGBA32Sint = 4,
    /**
     * @brief 32 bit per channel RGB format.
     * A three-component, 96-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    RGB32Float = 6,
    /**
     * @brief 32 bit per channel RGB format.
     * A three-component, 96-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    RGB32Uint = 7,
    /**
     * @brief 32 bit per channel RGB format.
     * A three-component, 96-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    RGB32Sint = 8,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 64-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    RGBA16Float = 10,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 64-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    RGBA16Unorm = 11,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 64-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    RGBA16Uint = 12,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 64-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    RGBA16Snorm = 13,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 64-bit signed integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    RGBA16Sint = 14,
    /**
     * @brief 32 bit per channel RG format.
     * A two-component, 64-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    RG32Float = 16,
    /**
     * @brief 32 bit per channel RG format.
     * A two-component, 64-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    RG32Uint = 17,
    /**
     * @brief 32 bit per channel RG format.
     * A two-component, 64-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    RG32Sint = 18,
    /**
     * @brief 32 bit depth, 8 bit stencil format.
     * A combined depth/stencil format with a 32-bit depth component in bytes 0..3
     * and an 8-bit stencil component in bytes 4..4.
     * */
    D32FloatS8Uint = 20,
    /**
     * @brief 10 bit per channel RGB format with 2 bit alpha.
     * A four-component, 32-bit unsigned normalized format that has
     * a 10-bit R component in bits 0..9,
     * a 10-bit G component in bits 10..19,
     * a 10-bit B component in bits 20..29,
     * a 2-bit A component in bits 30..31.
     * */
    RGB10A2Unorm = 24,
    /**
     * @brief 10 bit per channel RGB format with 2 bit alpha.
     * A four-component, 32-bit unsigned integer format that has
     * a 10-bit R component in bits 0..9,
     * a 10-bit G component in bits 10..19,
     * a 10-bit B component in bits 20..29,
     * a 2-bit A component in bits 30..31.
     * */
    RGB10A2Uint = 25,
    /**
     * @brief 11 bit per channel RG format with 10 bit blue.
     * A three-component, 32-bit signed floating-point format that has
     * a 11-bit R component in bits 0..10,
     * a 11-bit G component in bits 11..21,
     * a 10-bit B component in bits 22..31.
     * */
    RG11B10Float = 26,
    /**
     * @brief 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    RGBA8Unorm = 28,
    /**
     * @brief 8 bit per channel RGBA format. SRGB non-linear color space.
     * A four-component, 32-bit unsigned normalized non-linear SRGB format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    RGBA8UnormSrgb = 29,
    /**
     * @brief 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    RGBA8Uint = 30,
    /**
     * @brief 8 bit per channel RGBA format.
     * A four-component, 32-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    RGBA8Snorm = 31,
    /**
     * @brief 8 bit per channel RGBA format.
     * A four-component, 32-bit signed integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    RGBA8Sint = 32,
    /**
     * @brief 16 bit per channel RG format.
     * A two-component, 32-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    RG16Float = 34,
    /**
     * @brief 16 bit per channel RG format.
     * A two-component, 32-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    RG16Unorm = 35,
    /**
     * @brief 16 bit per channel RG format.
     * A two-component, 32-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    RG16Uint = 36,
    /**
     * @brief 16 bit per channel RG format.
     * A two-component, 32-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    RG16Snorm = 37,
    /**
     * @brief 16 bit per channel RG format.
     * A two-component, 32-bit signed integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    RG16Sint = 38,
    /**
     * @brief 32 bit depth format.
     * A 32-bit depth format supporting 32-bit floating-point depth values.
     * */
    D32Float = 40,
    /**
     * @brief 32 bit per channel R format.
     * A one-component, 32-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3.
     * */
    R32Float = 41,
    /**
     * @brief 32 bit per channel R format.
     * A one-component, 32-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3.
     * */
    R32Uint = 42,
    /**
     * @brief 32 bit per channel R format.
     * A one-component, 32-bit signed integer format that has
     * a 32-bit R component in bytes 0..3.
     * */
    R32Sint = 43,
    /**
     * @brief 24 bit depth, 8 bit stencil format.
     * A combined depth/stencil format with a 24-bit depth component in bytes 0..2
     * and an 8-bit stencil component in bytes 3..3.
     * */
    D24UnormS8Uint = 45,
    /**
     * @brief 8 bit per channel RG format.
     * A two-component, 16-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    RG8Unorm = 49,
    /**
     * @brief 8 bit per channel RG format.
     * A two-component, 16-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    RG8Uint = 50,
    /**
     * @brief 8 bit per channel RG format.
     * A two-component, 16-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    RG8Snorm = 51,
    /**
     * @brief 8 bit per channel RG format.
     * A two-component, 16-bit signed integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    RG8Sint = 52,
    /**
     * @brief 16 bit per channel R format.
     * A one-component, 16-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1.
     * */
    R16Float = 54,
    /**
     * @brief 16 bit depth format.
     * A 16-bit depth format supporting 16-bit unsigned normalized depth values.
     * */
    D16Unorm = 55,
    /**
     * @brief 16 bit per channel R format.
     * A one-component, 16-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1.
     * */
    R16Unorm = 56,
    /**
     * @brief 16 bit per channel R format.
     * A one-component, 16-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1.
     * */
    R16Uint = 57,
    /**
     * @brief 16 bit per channel R format.
     * A one-component, 16-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1.
     * */
    R16Snorm = 58,
    /**
     * @brief 16 bit per channel R format.
     * A one-component, 16-bit signed integer format that has
     * a 16-bit R component in bytes 0..1.
     * */
    R16Sint = 59,
    /**
     * @brief 8 bit per channel R format.
     * A one-component, 8-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0.
     * */
    R8Unorm = 61,
    /**
     * @brief 8 bit per channel R format.
     * A one-component, 8-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0.
     * */
    R8Uint = 62,
    /**
     * @brief 8 bit per channel R format.
     * A one-component, 8-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0.
     * */
    R8Snorm = 63,
    /**
     * @brief 8 bit per channel R format.
     * A one-component, 8-bit signed integer format that has
     * an 8-bit R component in bytes 0..0.
     * */
    R8Sint = 64,
    /**
     * @brief 9 bit per channel RGB format with shared exponent.
     * A three-component, 32-bit unsigned floating-point format that has
     * a 9-bit R component in bits 0..8,
     * a 9-bit G component in bits 9..17,
     * a 9-bit B component in bits 18..26,
     * a shared 5-bit exponent in bits 27..31.
     * */
    RGB9E5UFloat = 67,
    /**
     * @brief BC1/DXT1 compressed format.
     * A four-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGB texel data, and provides 1 bit of alpha.
     * */
    BC1RGBAUnorm = 71,
    /**
     * @brief BC1/DXT1 compressed format.
     * A four-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGB texel data with sRGB nonlinear encoding, and provides 1 bit of alpha.
     * */
    BC1RGBAUnormSrgb = 72,
    /**
     * @brief BC2/DXT3 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    BC2RGBAUnorm = 74,
    /**
     * @brief BC2/DXT3 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    BC2RGBAUnormSrgb = 75,
    /**
     * @brief BC3/DXT5 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values
     * followed by 64 bits encoding RGB values
     * */
    BC3RGBAUnorm = 77,
    /**
     * @brief BC3/DXT5 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    BC3RGBAUnormSrgb = 78,
    /**
     * @brief BC4 compressed format.
     * A single-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized red texel data
     * */
    BC4RUnorm = 80,
    /**
     * @brief BC4 compressed format.
     * A single-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed normalized red texel data
     * */
    BC4RSnorm = 81,
    /**
     * @brief BC5 compressed format.
     * A two-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized red and green texel data
     * */
    BC5RGUnorm = 83,
    /**
     * @brief BC5 compressed format.
     * A two-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed normalized red and green texel data
     * */
    BC5RGSnorm = 84,
    /**
     * @brief 16 bit per channel RGB format.
     * A three-component, 16-bit unsigned normalized format that has
     * a 5-bit B component in bits 0..4,
     * a 6-bit G component in bits 5..10,
     * a 5-bit R component in bits 11..15.
     * */
    B5G6R5Unorm = 85,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 16-bit unsigned normalized format that has
     * a 5-bit B component in bits 0..4,
     * a 5-bit G component in bits 5..9,
     * a 5-bit R component in bits 10..14,
     * a 1-bit A component in bits 15..15.
     * */
    B5G5R5A1Unorm = 86,
    /**
     * @brief 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned normalized format that has
     * an 8-bit B component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit R component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    BGRA8Unorm = 87,
    /**
     * @brief 8 bit per channel RGBA format. SRGB non-linear color space.
     * A four-component, 32-bit unsigned normalized non-linear SRGB format that has
     * an 8-bit B component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit R component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    BGRA8UnormSrgb = 91,
    /**
     * @brief BC6H compressed format.
     * A three-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned floating-point RGB texel data
     * */
    BC6HUfloat16 = 95,
    /**
     * @brief BC6H compressed format.
     * A three-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed floating-point RGB texel data
     * */
    BC6HSfloat16 = 96,
    /**
     * @brief BC7 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data
     * */
    BC7RGBAUnorm = 98,
    /**
     * @brief BC7 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * */
    BC7RGBAUnormSrgb = 99,
    /**
     * @brief 4 bit per channel RGB format.
     * A four-component, 16-bit unsigned normalized format that has
     * a 4-bit B component in bits 0..3,
     * a 4-bit G component in bits 4..7,
     * a 4-bit R component in bits 8..11,
     * a 4-bit A component in bits 12..15.
     * */
    BGRA4Unorm = 115,
};

/**
 * @brief Fill mode for rasterizer.
 *
 * Translates to D3D12_FILL_MODE for dx implementation.
 * Translates to VkPolygonMode for vk implementation.
 * */
enum class FillMode : uint32_t {
    Lines = 2, ///< Draw lines between vertices. Wireframe rendering.
    Solid = 3, ///< Fill the area between vertices forming polygons.
};

/**
 * @brief Descriptor memory type.
 * Decides if descriptors are visible and can be bound to GPU.
 *
 * Translates to D3D12_DESCRIPTOR_HEAP_FLAGS for dx implementation.
 * */
enum class DescriptorMemory : uint32_t {
    /**
     * @brief Descriptors are only visible to CPU.
     * May be used for copying descriptors to the GPU visible pool.
     * */
    CpuOnly = 0,
    /**
     * @brief Descriptors are visible to GPU.
     * Descriptors can be bound to the GPU pipeline directly.
     * */
    ShaderVisible = 1,
};

/**
 * @brief Winding order for front-facing triangles.
 *
 * Translates to BOOL for dx implementation.
 * Translates to VkFrontFace for vk implementation.
 * */
enum class WindingOrder : uint32_t {
    Clockwise = 0, ///< Front-facing triangles have clockwise winding order.
    CounterClockwise = 1, ///< Front-facing triangles have counter-clockwise winding order.
};

/**
 * @brief Sample rate for multisampling.
 *
 * Translates to uint32_t for dx implementation.
 * Translates to VkSampleCountFlagBits for vk implementation.
 * */
enum class SampleRate : uint32_t {
    S1 = 1, ///< 1 sample per pixel.
    S2 = 2, ///< 2 samples per pixel.
    S4 = 4, ///< 4 samples per pixel.
    S8 = 8, ///< 8 samples per pixel.
    S16 = 16, ///< 16 samples per pixel.
};

/**
 * @brief Comparison function for depth and stencil operations.
 *
 * Translates to D3D12_COMPARISON_FUNC for dx implementation.
 * Translates to VkCompareOp for vk implementation.
 * */
enum class Compare : uint32_t {
    None = 0, ///< No comparison.
    Never = 1, ///< Always fail the comparison.
    Less = 2, ///< Pass the comparison if the source value is less than the destination value.
    Equal = 3, ///< Pass the comparison if the source value is equal to the destination value.
    LessEqual = 4, ///< Pass the comparison if the source value is less than or equal to the destination value.
    Greater = 5, ///< Pass the comparison if the source value is greater than the destination value.
    NotEqual = 6, ///< Pass the comparison if the source value is not equal to the destination value.
    GreaterEqual = 7, ///< Pass the comparison if the source value is greater than or equal to the destination value.
    Always = 8, ///< Always pass the comparison.
};

/**
 * @brief Stencil operation for depth and stencil operations.
 *
 * Translates to D3D12_STENCIL_OP for dx implementation.
 * Translates to VkStencilOp for vk implementation.
 * */
enum class StencilOp : uint32_t {
    Keep = 1, ///< Keep the current value.
    Zero = 2, ///< Set the value to zero.
    Replace = 3, ///< Replace the value with the reference value.
    IncClamp = 4, ///< Increment the value and clamp to the maximum value.
    DecClamp = 5, ///< Decrement the value and clamp to the minimum value.
    Invert = 6, ///< Invert the value.
    IncWrap = 7, ///< Increment the value and wrap to zero when the maximum value is exceeded.
    DecWrap = 8, ///< Decrement the value and wrap to the maximum value when the minimum value is exceeded.
};

/**
 * @brief Blend factor for color blending operations.
 *
 * Translates to D3D12_BLEND for dx implementation.
 * Translates to VkBlendFactor for vk implementation.
 * */
enum class BlendFactor : uint32_t {
    Zero = 1, ///< Use zero for blending.
    One = 2, ///< Use one for blending.
    SrcColor = 3, ///< Use the source color for blending.
    InvSrcColor = 4, ///< Use the inverse source color for blending.
    SrcAlpha = 5, ///< Use the source alpha for blending.
    InvSrcAlpha = 6, ///< Use the inverse source alpha for blending.
    DestAlpha = 7, ///< Use the destination alpha for blending.
    InvDestAlpha = 8, ///< Use the inverse destination alpha for blending.
    DestColor = 9, ///< Use the destination color for blending.
    InvDestColor = 10, ///< Use the inverse destination color for blending.
    SrcAlphaSat = 11, ///< Use the source alpha saturated for blending.
    BlendFactor = 14, ///< Use a constant blend factor for blending.
    InvBlendFactor = 15, ///< Use the inverse constant blend factor for blending.
    Src1Color = 16, ///< Use the source color for blending. Dual source blending mode.
    InvSrc1Color = 17, ///< Use the inverse source color for blending. Dual source blending mode.
    Src1Alpha = 18, ///< Use the source alpha for blending. Dual source blending mode.
    InvSrc1Alpha = 19, ///< Use the inverse source alpha for blending. Dual source blending mode.
};

/**
 * @brief Blend operation for color blending operations.
 *
 * Translates to D3D12_BLEND_OP for dx implementation.
 * Translates to VkBlendOp for vk implementation.
 * */
enum class BlendOp : uint32_t {
    Add = 1, ///< Add the source and destination colors.
    Subtract = 2, ///< Subtract the source color from the destination color.
    RevSubtract = 3, ///< Subtract the destination color from the source color.
    Min = 4, ///< Use the minimum of the source and destination colors.
    Max = 5, ///< Use the maximum of the source and destination colors.
};

/**
 * @brief Logic operation for color blending operations.
 *
 * Translates to D3D12_LOGIC_OP for dx implementation.
 * Translates to VkLogicOp for vk implementation.
 * */
enum class LogicOp : uint32_t {
    Clear = 0, ///< Clear the destination value.
    Set = 1, ///< Set the destination value.
    Copy = 2, ///< Copy the source value to the destination.
    CopyInverted = 3, ///< Copy the inverted source value to the destination.
    Noop = 4, ///< Do not modify the destination value.
    Invert = 5, ///< Invert the destination value.
    And = 6, ///< Perform a bitwise AND operation on the source and destination values.
    Nand = 7, ///< Perform a bitwise NAND operation on the source and destination values.
    Or = 8, ///< Perform a bitwise OR operation on the source and destination values.
    Nor = 9, ///< Perform a bitwise NOR operation on the source and destination values.
    Xor = 10, ///< Perform a bitwise XOR operation on the source and destination values.
    Equiv = 11, ///< Perform a bitwise equivalent operation on the source and destination values.
    AndReverse = 12, ///< Perform a bitwise AND operation on the source and inverted destination values.
    AndInverted = 13, ///< Perform a bitwise AND operation on the inverted source and destination values.
    OrReverse = 14, ///< Perform a bitwise OR operation on the source and inverted destination values.
    OrInverted = 15, ///< Perform a bitwise OR operation on the inverted source and destination values.
};

/**
 * @brief Memory type for resource allocation.
 *
 * Translates to D3D12_HEAP_TYPE for dx implementation.
 * Translates to VkMemoryPropertyFlags for vk implementation.
 * */
enum class MemoryType : uint32_t {
    /**
     * @brief Default memory type.
     * Local device memory, most efficient for rendering.
     * */
    Default = 0,
    /**
     * @brief Upload memory type.
     * Used for data that is uploaded to the GPU Local memory using copy operations.
     * */
    Upload = 1,
    /**
     * @brief Readback memory type.
     * Used for data that is read back from the GPU Local memory using copy operations.
     * */
    Readback = 2,
    /**
     * @brief GPU upload memory type.
     * Used for data that is directly uploaded to the GPU Local memory using copy operations.
     * Can be used only with Extended Allocation extension enabled and supported.
     * Usage outside of the Extended Allocation is undefined behavior.
     * */
    GPUUpload = 3,
};

/**
 * @brief Intermediate shader representation.
 *
 * */
enum class ShaderIntermediate : uint32_t {
    DXIL = 0, ///< DirectX Intermediate Language.
    SPIRV = 1, ///< Standard Portable Intermediate Representation for Vulkan.
};

/**
 * @brief Texture state for resource transitions.
 *
 * Translates to D3D12_BARRIER_LAYOUT for dx implementation.
 * Translates to VkImageLayout for vk implementation.
 * */
enum class TextureState : uint32_t {
    Undefined = 4294967295, ///< Undefined state.
    Common = 0, ///< Common state.
    Read = 1, ///< General Read state.
    RenderTarget = 2, ///< Render Target state.
    UnorderedAccess = 3, ///< Unordered Access state.
    DepthStencilWrite = 4, ///< Depth Stencil Write state.
    DepthStencilRead = 5, ///< Depth Stencil Read state.
    ShaderResource = 6, ///< Shader Resource state.
    CopySource = 7, ///< Copy Source state.
    CopyDest = 8, ///< Copy Destination state.
    Present = 9, ///< Present swapchain state.
    ShadingRate = 10, ///< Shading Rate state. Used for Variable Shading Rate.
    VideoDecodeRead = 11, ///< Video Decode Read state.
    VideoDecodeWrite = 12, ///< Video Decode Write state.
};

/**
 * @brief Attachment load operation for render pass.
 *
 * Translates to VkAttachmentLoadOp for vk implementation.
 * Translates to D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE for dx implementation.
 * */
enum class LoadOperation : uint32_t {
    Load = 0, ///< Load the attachment contents.
    Clear = 1, ///< Clear the attachment contents.
    DontCare = 2, ///< Do not care about the attachment contents.
};

/**
 * @brief Texture layout. Determines how texture will be accessed.
 *
 * */
enum class TextureLayout : uint32_t {
    Texture1D = 2, ///< Texture is 1D array of data. Behaves similarly to Buffer.
    Texture1DArray = 3, ///< Texture is an array of 1D data.
    Texture2D = 4, ///< Texture is 2D image, default texture type.
    Texture2DArray = 5, ///< Texture is an array of 2D images. Can also be used for Cube maps.
    Texture2DMS = 6, ///< Texture is 2D multisampled image.
    Texture2DMSArray = 7, ///< Texture is an array of 2D multisampled images.
    Texture3D = 8, ///< Texture is 3D volume.
};

/**
 * @brief Descriptor heap type.
 *
 * Translates to D3D12_DESCRIPTOR_HEAP_TYPE for dx implementation.
 * */
enum class DescriptorHeapType : uint32_t {
    /**
     * @brief Descriptor heap type.
     * Used for all descriptor types, except for samplers.
     * */
    Descriptor = 0,
    /**
     * @brief Sampler heap type.
     * Used for sampler descriptors.
     * */
    Sampler = 1,
};

/**
 * @brief Attachment store operation for render pass.
 *
 * Translates to VkAttachmentStoreOp for vk implementation.
 * Translates to D3D12_RENDER_PASS_ENDING_ACCESS_TYPE for dx implementation.
 * */
enum class StoreOperation : uint32_t {
    Store = 0, ///< Store the attachment contents.
    DontCare = 1, ///< Do not care about the attachment contents.
    Resolve = 2, ///< Resolve the attachment contents. Used for multisampling attachments.
};

/**
 * @brief Primitive topology for rendering.
 * More info could be found [here](https://learn.microsoft.com/en-us/windows/win32/direct3d11/d3d10-graphics-programming-guide-primitive-topologies).
 *
 * Translates to D3D_PRIMITIVE_TOPOLOGY for dx implementation.
 * Translates to VkPrimitiveTopology for vk implementation.
 * */
enum class PrimitiveTopology : uint32_t {
    PointList = 1, ///< Render points for each vertex.
    LineList = 2, ///< Render lines between vertices.
    LineStrip = 3, ///< Render lines between vertices in a strip.
    TriangleList = 4, ///< Render triangles between vertices.
    TriangleStrip = 5, ///< Render triangles between vertices in a strip.
    TriangleFan = 6, ///< Interpret vertex data to form a fan of triangles.
    LineListAdj = 10, ///< Render lines between vertices with adjacency.
    LineStripAdj = 11, ///< Render lines between vertices in a strip with adjacency.
    TriangleListAdj = 12, ///< Render triangles between vertices with adjacency.
    TriangleStripAdj = 13, ///< Render triangles between vertices in a strip with adjacency.
};

/**
 * @brief Primitive topology type for rendering.
 *
 * Translates to D3D12_PRIMITIVE_TOPOLOGY_TYPE for dx implementation.
 * Translates to VkPrimitiveTopology for vk implementation.
 * */
enum class TopologyType : uint32_t {
    Point = 1, ///< Render points for each vertex.
    Line = 2, ///< Render lines between vertices.
    Triangle = 3, ///< Render triangles between vertices.
    Patch = 4, ///< Vertices are interpret as patch list. Used in tesselation process.
};

/**
 * @brief Filtering mode for texture sampling.
 *
 * Translates to VkFilter for vk implementation.
 * Translates to D3D12_FILTER_TYPE for dx implementation.
 * */
enum class Filter : uint32_t {
    Point = 0, ///< Nearest neighbor filtering.
    Linear = 1, ///< Linear filtering.
};

/**
 * @brief Address mode for texture sampling.
 *
 * Translates to VkSamplerAddressMode for vk implementation.
 * Translates to D3D12_TEXTURE_ADDRESS_MODE for dx implementation.
 * */
enum class AddressMode : uint32_t {
    Repeat = 0, ///< Repeat the texture.
    MirroredRepeat = 1, ///< Repeat the texture with mirroring.
    ClampToEdge = 2, ///< Clamp the texture to the edge.
    ClampToBorder = 3, ///< Clamp the texture to the border.
    MirrorClampToEdge = 4, ///< Mirror and clamp the texture to the edge.
};

/**
 * @brief Texture view type.
 *
 * Translates to VkImageViewType for vk implementation.
 * Translates to D3D12_SRV_DIMENSION for dx implementation.
 * */
enum class TextureViewType : uint32_t {
    Texture1D = 0, ///< Texture is 1D array of data. Behaves similarly to Buffer.
    Texture1DArray = 1, ///< Texture is an array of 1D data.
    Texture2D = 2, ///< Texture is 2D image, default texture type.
    Texture2DArray = 3, ///< Texture is an array of 2D images.
    Texture2DMS = 4, ///< Texture is 2D multisampled image.
    Texture2DMSArray = 5, ///< Texture is an array of 2D multisampled images.
    Texture3D = 6, ///< Texture is 3D volume.
    TextureCube = 7, ///< Texture is a cube map.
    TextureCubeArray = 8, ///< Texture is an array of cube maps.
};

/**
 * @brief Component swizzle for texture sampling.
 *
 * Translates to D3D12_SHADER_COMPONENT_MAPPING for dx implementation.
 * Translates to VkComponentSwizzle for vk implementation.
 * */
enum class ComponentSwizzle : uint8_t {
    Red = 0, ///< Use the red component for sampling.
    Green = 1, ///< Use the green component for sampling.
    Blue = 2, ///< Use the blue component for sampling.
    Alpha = 3, ///< Use the alpha component for sampling.
    Zero = 4, ///< Use zero for sampling.
    One = 5, ///< Use one for sampling.
};

enum class AdapterFlags {
    None = 0x0,
    Remote = 1 << 0,
    Software = 1 << 1,
    DX12ACGCompatible = 1 << 2,
    DX12SupportsMonitoredFences = 1 << 3,
    DX12SupportsNonMonitoredFences = 1 << 4,
    DX12KeyedMutexConformance = 1 << 5,
};

enum class DSSelect {
    None = 0x0,
    Depth = 1 << 0,
    Stencil = 1 << 1,
    DepthStencil = 0x3,
};

enum class ColorComponents {
    None = 0x0,
    R = 1 << 0,
    G = 1 << 1,
    B = 1 << 2,
    A = 1 << 3,
    All = 0xF,
};

enum class BufferUsage {
    None = 0x0,
    CopySrc = 1 << 0,
    CopyDst = 1 << 1,
    ConstantBuffer = 1 << 4,
    IndexBuffer = 1 << 6,
    VertexBuffer = 1 << 7,
};

enum class MemoryFlags {
    None = 0x0,
    DedicatedAllocation = 1 << 0,
    Mapped = 1 << 1,
};

enum class RenderPassFlags {
    None = 0x0,
    Suspending = 1 << 1,
    Resuming = 1 << 2,
};

enum class BarrierSync {
    None = 0x0,
    All = 1 << 0,
    Draw = 1 << 1,
    IndexInput = 1 << 2,
    VertexShading = 1 << 3,
    PixelShading = 1 << 4,
    DepthStencil = 1 << 5,
    RenderTarget = 1 << 6,
    Compute = 1 << 7,
    Raytracing = 1 << 8,
    Copy = 1 << 9,
    Resolve = 1 << 10,
    ExecuteIndirect = 1 << 11,
    AllShading = 1 << 12,
    NonPixelShading = 1 << 13,
    ClearUAV = 1 << 14,
    VideoDecode = 1 << 15,
    VideoEncode = 1 << 16,
    BuildRTAS = 1 << 17,
    CopyRTAS = 1 << 18,
};

enum class ResourceAccess {
    Common = 0x0,
    VertexBuffer = 1 << 0,
    ConstantBuffer = 1 << 1,
    IndexBuffer = 1 << 2,
    RenderTarget = 1 << 3,
    UnorderedAccess = 1 << 4,
    DepthWrite = 1 << 5,
    DepthRead = 1 << 6,
    ShaderResource = 1 << 7,
    StreamOutput = 1 << 8,
    IndirectArgument = 1 << 9,
    CopyDest = 1 << 10,
    CopySource = 1 << 11,
    ConditionalRendering = 1 << 12,
    AccelerationStrucureRead = 1 << 13,
    AccelerationStrucureWrite = 1 << 14,
    ShadingRate = 1 << 15,
    VideoDecodeRead = 1 << 16,
    VideoDecodeWrite = 1 << 17,
    Present = 1 << 18,
    ResolveDest = 1 << 19,
    ResolveSource = 1 << 20,
    NoAccess = 1 << 31,
};

enum class TextureUsage {
    None = 0x0,
    RenderTarget = 1 << 0,
    DepthStencil = 1 << 1,
    CopySrc = 1 << 2,
    CopyDst = 1 << 3,
    ShaderResource = 1 << 4,
    UnorderedAccess = 1 << 5,
    HostCopy = 1 << 7,
};

struct Result {
    wis::Status status = wis::Status::Ok;
    const char* error = nullptr;
};

struct Size2D {
    uint32_t width;
    uint32_t height;
};

struct Size3D {
    uint32_t width;
    uint32_t height;
    uint32_t depth_or_layers;
};

struct BufferRegion {
    uint64_t src_offset = 0;
    uint64_t dst_offset = 0;
    uint64_t size_bytes;
};

struct AdapterDesc {
    std::array<const char, 256> description{};
    uint32_t vendor_id;
    uint32_t device_id;
    uint32_t subsys_id;
    uint32_t revision;
    uint64_t dedicated_video_memory;
    uint64_t dedicated_system_memory;
    uint64_t shared_system_memory;
    uint64_t adapter_id;
    wis::AdapterFlags flags;
};

struct InputSlotDesc {
    uint32_t slot;
    uint32_t stride_bytes;
    wis::InputClass input_class;
};

struct InputAttribute {
    uint32_t input_slot;
    const char* semantic_name;
    uint32_t semantic_index;
    uint32_t location;
    wis::DataFormat format;
    uint32_t offset_bytes;
};

struct InputLayout {
    wis::InputSlotDesc* slots;
    uint32_t slot_count;
    wis::InputAttribute* attributes;
    uint32_t attribute_count;
};

struct RasterizerDesc {
    wis::FillMode fill_mode = wis::FillMode::Solid;
    wis::CullMode cull_mode = wis::CullMode::Back;
    wis::WindingOrder front_face = wis::WindingOrder::Clockwise;
    bool depth_bias_enable = false;
    float depth_bias = 0.0f;
    float depth_bias_clamp = 0.0f;
    float depth_bias_slope_factor = 0.0f;
    bool depth_clip_enable = true;
};

struct SampleDesc {
    wis::SampleRate rate = wis::SampleRate::S1;
    float quality = 0.0f;
    uint32_t sample_mask = 0xffffffff;
};

struct StencilDesc {
    wis::StencilOp fail_op = wis::StencilOp::Keep;
    wis::StencilOp depth_fail_op = wis::StencilOp::Keep;
    wis::StencilOp pass_op = wis::StencilOp::Keep;
    wis::Compare comparison = wis::Compare::Always;
    uint8_t read_mask = 0xff;
    uint8_t write_mask = 0xff;
};

struct DepthStencilDesc {
    bool depth_enable = false;
    bool depth_write_enable = false;
    wis::Compare depth_comp = wis::Compare::Less;
    bool stencil_enable = false;
    wis::StencilDesc stencil_front = {};
    wis::StencilDesc stencil_back = {};
    bool depth_bound_test = false;
};

struct BlendAttachmentDesc {
    bool blend_enable = false;
    wis::BlendFactor src_color_blend = wis::BlendFactor::One;
    wis::BlendFactor dst_color_blend = wis::BlendFactor::Zero;
    wis::BlendOp color_blend_op = wis::BlendOp::Add;
    wis::BlendFactor src_alpha_blend = wis::BlendFactor::One;
    wis::BlendFactor dst_alpha_blend = wis::BlendFactor::Zero;
    wis::BlendOp alpha_blend_op = wis::BlendOp::Add;
    wis::ColorComponents color_write_mask = wis::ColorComponents::All;
};

struct BlendStateDesc {
    bool logic_op_enable = false;
    wis::LogicOp logic_op = wis::LogicOp::Noop;
    std::array<wis::BlendAttachmentDesc, 8> attachments{};
    uint32_t attachment_count;
};

struct RenderAttachmentsDesc {
    wis::DataFormat* attachment_formats;
    uint32_t attachments_count;
    wis::DataFormat depth_attachment;
};

struct RootConstant {
    wis::ShaderStages stage;
    uint32_t size_bytes;
};

struct SwapchainDesc {
    wis::Size2D size;
    wis::DataFormat format;
    uint32_t buffer_count;
    bool stereo;
    bool vsync;
};

struct TextureDesc {
    wis::DataFormat format;
    wis::Size3D size;
    uint32_t mip_levels;
    wis::TextureLayout layout = wis::TextureLayout::Texture2D;
    wis::SampleRate sample_count = wis::SampleRate::S1;
    wis::TextureUsage usage = wis::TextureUsage::None;
};

struct AllocationInfo {
    uint64_t size_bytes;
    uint64_t alignment_bytes;
};

struct TextureRegion {
    wis::Size3D offset;
    wis::Size3D size;
    uint32_t mip;
    uint32_t array_layer;
    wis::DataFormat format;
};

struct BufferTextureCopyRegion {
    uint64_t buffer_offset;
    wis::TextureRegion texture;
};

struct PushDescriptor {
    wis::ShaderStages stage;
    uint32_t bind_register;
    wis::DescriptorType type;
    uint32_t reserved;
};

struct SubresourceRange {
    uint32_t base_mip_level;
    uint32_t level_count;
    uint32_t base_array_layer;
    uint32_t layer_count;
};

struct RenderTargetDesc {
    wis::DataFormat format;
    wis::TextureLayout layout;
    uint32_t mip;
    uint32_t base_array_layer;
    uint32_t layer_count;
};

struct Viewport {
    float top_leftx;
    float top_lefty;
    float width;
    float height;
    float min_depth;
    float max_depth;
};

struct Scissor {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};

struct BufferBarrier {
    wis::BarrierSync sync_before;
    wis::BarrierSync sync_after;
    wis::ResourceAccess access_before;
    wis::ResourceAccess access_after;
    uint64_t offset = 0;
    uint64_t size = UINT64_MAX;
};

struct TextureBarrier {
    wis::BarrierSync sync_before;
    wis::BarrierSync sync_after;
    wis::ResourceAccess access_before;
    wis::ResourceAccess access_after;
    wis::TextureState state_before;
    wis::TextureState state_after;
    wis::SubresourceRange subresource_range;
};

struct DescriptorTableEntry {
    wis::DescriptorType type;
    uint32_t bind_register;
    uint32_t binding;
    uint32_t count;
};

struct DescriptorTable {
    wis::DescriptorHeapType type;
    wis::DescriptorTableEntry* entries;
    uint32_t entry_count;
    wis::ShaderStages stage;
};

struct SamplerDesc {
    wis::Filter min_filter;
    wis::Filter mag_filter;
    wis::Filter mip_filter;
    bool anisotropic;
    uint32_t max_anisotropy;
    wis::AddressMode address_u;
    wis::AddressMode address_v;
    wis::AddressMode address_w;
    float min_lod;
    float max_lod;
    float mip_lod_bias;
    wis::Compare comparison_op;
    std::array<float, 4> border_color{};
};

struct ComponentMapping {
    wis::ComponentSwizzle r = wis::ComponentSwizzle::Red;
    wis::ComponentSwizzle g = wis::ComponentSwizzle::Green;
    wis::ComponentSwizzle b = wis::ComponentSwizzle::Blue;
    wis::ComponentSwizzle a = wis::ComponentSwizzle::Alpha;
};

struct ShaderResourceDesc {
    wis::DataFormat format;
    wis::TextureViewType view_type;
    wis::ComponentMapping component_mapping;
    wis::SubresourceRange subresource_range;
};

//=================================DELEGATES=================================

typedef void (*DebugCallback)(wis::Severity severity, const char* message, void* user_data);
//==============================TYPE TRAITS==============================

template<typename T>
struct is_flag_enum : public std::false_type {
};
template<>
struct is_flag_enum<wis::AdapterFlags> : public std::true_type {
};
template<>
struct is_flag_enum<wis::DSSelect> : public std::true_type {
};
template<>
struct is_flag_enum<wis::ColorComponents> : public std::true_type {
};
template<>
struct is_flag_enum<wis::BufferUsage> : public std::true_type {
};
template<>
struct is_flag_enum<wis::MemoryFlags> : public std::true_type {
};
template<>
struct is_flag_enum<wis::RenderPassFlags> : public std::true_type {
};
template<>
struct is_flag_enum<wis::BarrierSync> : public std::true_type {
};
template<>
struct is_flag_enum<wis::ResourceAccess> : public std::true_type {
};
template<>
struct is_flag_enum<wis::TextureUsage> : public std::true_type {
};
//============================== CONSTS ==============================

static inline constexpr Result success{
    wis::Status::Ok, "Operation succeeded"
};

template<typename RetTy>
struct ResultValue {
    Result status;
    RetTy value;

    constexpr ResultValue(RetTy value) noexcept
        : status(success), value(std::move(value)) { }
    constexpr ResultValue(Result status) noexcept
        : status(status) { }
    constexpr ResultValue(Result status, RetTy value) noexcept
        : status(status), value(std::move(value)) { }
};

template<uint32_t s, typename RetTy>
constexpr decltype(auto) get(ResultValue<RetTy>& rv) noexcept
{
    if constexpr (s == 0)
        return std::forward<Result>(rv.status);
    else
        return std::forward<RetTy>(rv.value);
}

} // namespace wis
