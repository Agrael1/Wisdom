// GENERATED
#pragma once
#include <array>
#include <cstdint>
#include <functional>

/** \mainpage Wisdom API Documentation

<b>Version 0.5.0</b>

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
struct PushConstant;
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
struct UnorderedAccessDesc;
struct FactoryExtQuery;
struct DeviceExtQuery;
struct TopLevelASBuildDesc;
struct AcceleratedGeometryInput;
struct ASAllocationInfo;
struct DescriptorBindingDesc;
struct ShaderExport;
struct HitGroupDesc;
struct ShaderBindingTableInfo;

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
 * @brief Type of the descriptor in the descriptor table.
 *
 * Translates to D3D12_DESCRIPTOR_RANGE_TYPE for dx implementation.
 * Translates to VkDescriptorType for vk implementation.
 * */
enum class DescriptorType : uint32_t {
    /**
     * @brief Descriptor is a sampler.
     * Sampler is used to sample textures in shaders.
     * Stored in separate descriptor table and
     * can't be mixed with other descriptor types.
     * */
    Sampler = 0,
    ConstantBuffer = 1, ///< Descriptor is a constant buffer.
    Texture = 2, ///< Descriptor is a texture.
    /**
     * @brief Descriptor is an unordered access read-write texture.
     * Used for read/write operations in compute shaders.
     * */
    RWTexture = 3,
    /**
     * @brief Descriptor is an unordered access read-write buffer.
     * Used for read/write operations in compute shaders.
     * */
    RWBuffer = 4,
    /**
     * @brief Descriptor is a shader resource buffer.
     * May be bigger than constant buffers, but slower.
     * */
    Buffer = 5,
    AccelerationStructure = 6, ///< Descriptor is an acceleration structure.
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
 * @brief Type of the geometry in the Acceleration Structure.
 *
 * Translates to VkGeometryTypeKHR for vk implementation.
 * Translates to D3D12_RAYTRACING_GEOMETRY_TYPE for dx implementation.
 * */
enum class ASGeometryType : uint32_t {
    Triangles = 0, ///< Triangles geometry type. Used for triangle meshes.
    AABBs = 1, ///< Axis Aligned Bounding Boxes geometry type. Used for bounding volume hierarchies.
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
 * @brief Shader stages that can be used in the raytracing pipeline.
 *
 * Translates to VkShaderStageFlagBits for vk implementation.
 * */
enum class RaytracingShaderType : uint32_t {
    Raygen = 0, ///< Ray generation shader stage.
    Miss = 1, ///< Miss shader stage.
    ClosestHit = 2, ///< Closest hit shader stage.
    AnyHit = 3, ///< Any hit shader stage.
    Intersection = 4, ///< Intersection shader stage.
    Callable = 5, ///< Callable shader stage.
};

/**
 * @brief Log message severity.
 * Used with wis::DebugCallback and internal library logging.
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
 * @brief Level of the Raytracing Acceleration Structure. Used to create Acceleration structures.
 *
 * */
enum class ASLevel : uint32_t {
    Bottom = 0, ///< Bottom level Acceleration Structure. Contains geometry data.
    Top = 1, ///< Top level Acceleration Structure. Contains instance data.
};

/**
 * @brief Type of the hit group in the raytracing pipeline.
 *
 * Translates to VkRayTracingShaderGroupTypeKHR for vk implementation.
 * Translates to D3D12_HIT_GROUP_TYPE for dx implementation.
 * */
enum class HitGroupType : uint32_t {
    Triangles = 0, ///< Hit group for triangles.
    Procedural = 1, ///< Hit group for procedural geometry.
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
    ConstantColor = 14, ///< Use a constant blend factor for blending.
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
    Default = 0, ///< Default memory type. Alias for wis::MemoryType::DeviceLocal
    /**
     * @brief Default memory type.
     * Local device memory, most efficient for rendering.
     * */
    DeviceLocal = 0,
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
     * Usage with buffers requires allocating memory on GPUUpload heap and mapping the memory.
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
 * @brief Binding index for resources.
 * Used in wis::DescriptorStorage to determine which descriptor type goes where when binding.
 * Same values are used for HLSL side to pick descriptors up.
 * Space 0 and set 0 are reserved for push descriptors and push constants.
 *
 * */
enum class BindingIndex : uint32_t {
    /**
     * @brief No binding index set.Results in [[vk::binding(*,0)]] and register(*).
     * This space is reserved for push constants and push descriptors.
     * */
    None = 0,
    Sampler = 1, ///< Binding index for sampler descriptors. Results in [[vk::binding(0,1)]] and register(s0, space1).
    ConstantBuffer = 2, ///< Binding index for constant buffer descriptors. Results in [[vk::binding(0,2)]] and register(b0, space2).
    Texture = 3, ///< Binding index for texture descriptors. Results in [[vk::binding(0,3)]] and register(t0, space3).
    RWTexture = 4, ///< Binding index for read-write texture descriptors. Results in [[vk::binding(0,4)]] and register(u0, space4).
    RWBuffer = 5, ///< Binding index for read-write buffer descriptors. Results in [[vk::binding(0,5)]] and register(u0, space5).
    /**
     * @brief Binding index for read buffer descriptors. Results in [[vk::binding(0,6)]] and register(t0, space6).
     * Can't be merged with Texture because of Vulkan.
     * */
    Buffer = 6,
    AccelerationStructure = 7, ///< Binding index for acceleration structure descriptors. Results in [[vk::binding(0,7)]] and register(t0, space7).
    Count = 7, ///< Number of binding indices. Used for array sizes.
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
 * @brief Features that device may support.
 * Query by calling with Device::QueryFeatureSupport. Contains core features with optional ones.
 *
 * */
enum class DeviceFeature : uint32_t {
    /**
     * @brief Core Functionality. Supports enhanced barriers. Support for VK and DX12.
     * Used in all barriers to provide more control over synchronization. Without the feature behavior is undefined.
     * To run without this feature for DX12 there are legacy barriers, which can be manually submitted through CommandList internals.
     * Vulkan will not work, as half of current functionality depends on VK_KHR_synchronization2.
     * */
    EnchancedBarriers = 1,
    /**
     * @brief Supports waiting for present to finish. Support for VK, always true for DX12.
     * Unlocks Swapchain::WaitForPresent.
     * */
    WaitForPresent = 2,
    /**
     * @brief Supports advanced index buffer features. Support for VK, always true for DX12.
     * Unlocks CommandList::IASetIndexBuffer2 function. Without the extension behavior is undefined.
     * */
    AdvancedIndexBuffer = 4,
    /**
     * @brief Supports dynamic VSync. Support for VK, always true for DX12.
     * Unlocks Swapchain::Present2 function. Without the extension behavior is the same as Swapchain::Present.
     * */
    DynamicVSync = 5,
    UnusedRenderTargets = 6, ///< Supports unused render targets. Support for VK, always true for DX12.
    PushDescriptors = 7, ///< Supports push descriptors. Support for VK, always true for DX12.
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
enum class ComponentSwizzle : uint32_t {
    Red = 0, ///< Use the red component for sampling.
    Green = 1, ///< Use the green component for sampling.
    Blue = 2, ///< Use the blue component for sampling.
    Alpha = 3, ///< Use the alpha component for sampling.
    Zero = 4, ///< Use zero for sampling.
    One = 5, ///< Use one for sampling.
};

/**
 * @brief Index type for index buffer.
 * Enum values resemble the byte stride of the format.
 *
 * Translates to DXGI_FORMAT for dx implementation.
 * Translates to VkIndexType for vk implementation.
 * */
enum class IndexType : uint32_t {
    UInt16 = 2, ///< 16-bit unsigned integer index type.
    UInt32 = 4, ///< 32-bit unsigned integer index type.
};

/**
 * @brief Factory extension ID.
 * Platform extension values start from 2049
 * 0 is reserved as invalid/custom extension.
 *
 * */
enum class FactoryExtID : uint32_t {
    Custom = 0, ///< Custom provided extension. Default initialization of the extension is done by user.
    DebugExtension = 1,
};

/**
 * @brief Device extension ID.
 * Platform extension values start from 2049
 * 0 is reserved as invalid/custom extension.
 *
 * */
enum class DeviceExtID : uint32_t {
    Custom = 0, ///< Custom provided extension. Default initialization of the extension is done by user.
    DescriptorBufferExtension = 1,
    ExtendedAllocation = 2,
};

/**
 * @brief Flags that describe adapter.
 *
 * */
enum class AdapterFlags {
    None = 0x0, ///< No flags set. Adapter may be descrete or embedded.
    Remote = 1 << 0, ///< Adapter is remote. Used for remote rendering.
    Software = 1 << 1, ///< Adapter is software. Used for software rendering.
};

/**
 * @brief Depth stencil select flags.
 * Affect which part of the depth stencil buffer is used.
 *
 * */
enum class DSSelect {
    None = 0x0, ///< No flags set. Depth stencil buffer is not used.
    Depth = 1 << 0, ///< Use depth part of the depth stencil buffer.
    Stencil = 1 << 1, ///< Use stencil part of the depth stencil buffer.
    DepthStencil = 0x3, ///< Use both depth and stencil parts of the depth stencil buffer.
};

/**
 * @brief Color component flags.
 * Used for color blending operations.
 *
 * */
enum class ColorComponents {
    None = 0x0, ///< No flags set. Color blending is not used.
    R = 1 << 0, ///< Use red component for blending.
    G = 1 << 1, ///< Use green component for blending.
    B = 1 << 2, ///< Use blue component for blending.
    A = 1 << 3, ///< Use alpha component for blending.
    All = 0xF, ///< Use all color components for blending.
};

/**
 * @brief Buffer usage flags.
 * Determine how the buffer can be used throughout its lifetime.
 *
 * Translates to VkBufferUsageFlags for vk implementation.
 * Translates to D3D12_RESOURCE_FLAGS for dx implementation.
 * */
enum class BufferUsage {
    None = 0x0, ///< No flags set. Buffer is not used.
    CopySrc = 1 << 0, ///< Buffer is used as a source for copy operations.
    CopyDst = 1 << 1, ///< Buffer is used as a destination for copy operations.
    ConstantBuffer = 1 << 2, ///< Buffer is used as a constant buffer.
    IndexBuffer = 1 << 3, ///< Buffer is used as an index buffer.
    VertexBuffer = 1 << 4, ///< Buffer is used as a vertex buffer or an instance buffer.
    IndirectBuffer = 1 << 5, ///< Buffer is used as an indirect buffer.
    StorageBuffer = 1 << 6, ///< Buffer is used as a storage unordered access buffer.
    AccelerationStructureBuffer = 1 << 7, ///< Buffer is used as an acceleration structure buffer.
    AccelerationStructureInput = 1 << 8, ///< Buffer is used as a read only acceleration instance input buffer.
    ShaderBindingTable = 1 << 9, ///< Buffer is used as a shader binding table buffer.
};

/**
 * @brief Memory flags.
 * Determine optional properties of the memory allocation.
 *
 * Translates to VmaAllocationCreateFlags for vk implementation.
 * Translates to D3D12MA::ALLOCATION_FLAGS for dx implementation.
 * */
enum class MemoryFlags {
    None = 0x0, ///< No flags set. Memory is regular.
    /**
     * @brief Memory is dedicated.
     * Used for resources that require dedicated memory.
     * Useful for big resources that are not shared with other resources.
     * E.g. fullscreen textures, big buffers, etc.
     * */
    DedicatedAllocation = 1 << 0,
    /**
     * @brief Memory is mapped.
     * Used in combination with wis::MemoryType::Upload or wis::MemoryType::Readback to map memory for CPU access.
     * */
    Mapped = 1 << 1,
    /**
     * @brief Memory is exportable.
     * If set, memory can be exported to other processes or APIs.
     * Works only with Device Local memory (wis::MemoryType::Default) and only on AllocateXMemory calls.
     * Outside of AllocateXMemory the flag is ignored.
     * */
    Exportable = 1 << 2,
};

/**
 * @brief Render pass flags.
 * Set of flags that affect render pass behavior.
 * More on render pass flags [here](https://learn.microsoft.com/en-us/windows/win32/direct3d12/direct3d-12-render-passes).
 *
 * Translates to VkRenderingFlags for vk implementation.
 * Translates to D3D12_RENDER_PASS_FLAGS for dx implementation.
 * */
enum class RenderPassFlags {
    None = 0x0, ///< No flags set. Render pass is regular.
    Suspending = 1 << 1, ///< Render pass is suspending.
    Resuming = 1 << 2, ///< Render pass is resuming.
};

/**
 * @brief Barrier synchronization flags.
 * Used to synchronize resources between different stages of the pipeline.
 *
 * Translates to D3D12_BARRIER_SYNC for dx implementation.
 * Translates to VkPipelineStageFlags2 for vk implementation.
 * */
enum class BarrierSync {
    None = 0x0, ///< No flags set. No synchronization is performed.
    All = 1 << 0, ///< Synchronize all commands.
    Draw = 1 << 1, ///< Synchronize draw commands.
    IndexInput = 1 << 2, ///< Synchronize index input commands.
    VertexShading = 1 << 3, ///< Synchronize vertex shading commands.
    PixelShading = 1 << 4, ///< Synchronize pixel shading commands.
    DepthStencil = 1 << 5, ///< Synchronize depth stencil commands.
    RenderTarget = 1 << 6, ///< Synchronize render target commands.
    Compute = 1 << 7, ///< Synchronize compute commands.
    Raytracing = 1 << 8, ///< Synchronize raytracing commands.
    Copy = 1 << 9, ///< Synchronize copy commands.
    Resolve = 1 << 10, ///< Synchronize resolve commands.
    ExecuteIndirect = 1 << 11, ///< Synchronize execute indirect commands.
    AllShading = 1 << 12, ///< Synchronize all shading commands.
    NonPixelShading = 1 << 13, ///< Synchronize non-pixel shading commands.
    ClearUAV = 1 << 14, ///< Synchronize clear UAV commands.
    VideoDecode = 1 << 15, ///< Synchronize video decode commands.
    VideoEncode = 1 << 16, ///< Synchronize video encode commands.
    BuildRTAS = 1 << 17, ///< Synchronize build raytracing acceleration structure commands.
    CopyRTAS = 1 << 18, ///< Synchronize copy raytracing acceleration structure commands.
};

/**
 * @brief Resource access flags.
 * Determine how resource will be accessed. Used in Barriers.
 *
 * Translates to D3D12_BARRIER_ACCESS for dx implementation.
 * Translates to VkAccessFlags2 for vk implementation.
 * */
enum class ResourceAccess {
    /**
     * @brief Common access.
     * Subresource data must be available for any layout-compatible access after a barrier.
     * */
    Common = 0x0,
    VertexBuffer = 1 << 0, ///< Vertex buffer access. Applies only to buffers.
    ConstantBuffer = 1 << 1, ///< Constant buffer access. Applies only to buffers.
    IndexBuffer = 1 << 2, ///< Index buffer access. Applies only to buffers.
    RenderTarget = 1 << 3, ///< Render target access. Applies only to textures.
    UnorderedAccess = 1 << 4, ///< Unordered access access.
    DepthWrite = 1 << 5, ///< Depth write access. Applies only to DS textures.
    DepthRead = 1 << 6, ///< Depth read access. Applies only to DS textures.
    ShaderResource = 1 << 7, ///< Shader resource access. Applies only to textures.
    StreamOutput = 1 << 8, ///< Stream output access. Applies only to buffers. Reserved for extension.
    IndirectArgument = 1 << 9, ///< Indirect argument access.
    CopyDest = 1 << 10, ///< Copy destination access.
    CopySource = 1 << 11, ///< Copy source access.
    ConditionalRendering = 1 << 12, ///< Conditional rendering access.
    AccelerationStructureRead = 1 << 13, ///< Acceleration structure read access.
    AccelerationStructureWrite = 1 << 14, ///< Acceleration structure write access.
    ShadingRate = 1 << 15, ///< Shading rate access. Used in variable shading rate.
    VideoDecodeRead = 1 << 16, ///< Video decode read access.
    VideoDecodeWrite = 1 << 17, ///< Video decode write access.
    ResolveDest = 1 << 18, ///< Resolve destination access. Used in multisampling.
    ResolveSource = 1 << 19, ///< Resolve source access. Used in multisampling.
    NoAccess = 1 << 31, ///< No access. Used to indicate no access throughout pipeline.
};

/**
 * @brief Texture usage flags.
 * Determine how the texture can be used throughout its lifetime.
 *
 * Translates to D3D12_RESOURCE_FLAGS for dx implementation.
 * Translates to VkImageUsageFlags for vk implementation.
 * */
enum class TextureUsage {
    None = 0x0, ///< No flags set. Texture is not used.
    RenderTarget = 1 << 0, ///< Texture is used as a render target.
    DepthStencil = 1 << 1, ///< Texture is used as a depth stencil buffer.
    CopySrc = 1 << 2, ///< Texture is used as a source for copy operations.
    CopyDst = 1 << 3, ///< Texture is used as a destination for copy operations.
    ShaderResource = 1 << 4, ///< Texture is used as a shader resource.
    UnorderedAccess = 1 << 5, ///< Texture is used as an unordered access resource.
    HostCopy = 1 << 7, ///< Texture is used for host copy operations. Works with ExtendedAllocation extension.
};

/**
 * @brief Fence flags for additional fence features
 *
 * Translates to D3D12_FENCE_FLAGS for dx implementation.
 * */
enum class FenceFlags {
    None = 0x0, ///< No flags set. Fence is regular.
    Shared = 1 << 0, ///< Fence is shared. Used for sharing fences for single physical device.
};

/**
 * @brief Pipeline flags for additional pipeline features
 *
 * Translates to D3D12_PIPELINE_STATE_FLAGS for dx implementation.
 * Translates to VkPipelineCreateFlags for vk implementation.
 * */
enum class PipelineFlags {
    None = 0x0, ///< No flags set. Pipeline is regular.
    /**
     * @brief Pipeline is created to be used with DescriptorBuffer extension.
     * Do not mix DescriptorBuffer and non-DescriptorBuffer pipelines.
     * */
    DescriptorBuffer = 1 << 0,
};

/**
 * @brief Geometry flags for additional geometry features
 *
 * Translates to D3D12_RAYTRACING_GEOMETRY_FLAGS for dx implementation.
 * Translates to VkGeometryFlagsKHR for vk implementation.
 * */
enum class ASGeometryFlags {
    None = 0x0, ///< No flags set. Geometry is regular.
    Opaque = 1 << 0, ///< Geometry is opaque. Used for opaque geometry.
    NoDuplicateAnyHitInvocation = 1 << 1, ///< Geometry has no duplicate any hit invocation.
};

/**
 * @brief Acceleration structure flags for additional acceleration structure features
 *
 * Translates to D3D12_RAYTRACING_ACCELERATION_STRUCTURE_BUILD_FLAGS for dx implementation.
 * Translates to VkBuildAccelerationStructureFlagsKHR for vk implementation.
 * */
enum class AccelerationStructureFlags {
    None = 0x0, ///< No flags set. Acceleration structure is regular.
    AllowUpdate = 1 << 0, ///< Acceleration structure is allowed to be updated.
    AllowCompaction = 1 << 1, ///< Acceleration structure is allowed to be compacted.
    PreferFastTrace = 1 << 2, ///< Acceleration structure is preferred to be fast traced.
    PreferFastBuild = 1 << 3, ///< Acceleration structure is preferred to be fast built.
    MinimizeMemory = 1 << 4, ///< Acceleration structure is minimized for memory usage.
};

/**
 * @brief Instance flags for additional instance features
 *
 * Translates to D3D12_RAYTRACING_INSTANCE_FLAGS for dx implementation.
 * Translates to VkGeometryInstanceFlagsKHR for vk implementation.
 * */
enum class ASInstanceFlags {
    None = 0x0, ///< No flags set. Instance is regular.
    TriangleCullDisable = 1 << 0, ///< Triangle cull is disabled.
    TriangleFrontCounterClockwise = 1 << 1, ///< Triangle front is counter clockwise.
    ForceOpaque = 1 << 2, ///< Force opaque.
    ForceNoOpaque = 1 << 3, ///< Force no opaque.
};

/**
 * @brief Main source of communication of operation success.
 * To check for success compare wis::Result::status with wis::Status::Ok.
 * If there is any error there is  string which is compile-time.
 * It communicates the source of problems even in Release mode.
 * The string contains function name and error message.
 * */
struct Result {
    wis::Status status = wis::Status::Ok; ///< Operation status. Compare with wis::Status::Ok
    const char* error = nullptr; ///< Error message. nullptr or 'Operation Succeeded.' if no error.
};

/**
 * @brief 2D unsigned size.
 * */
struct Size2D {
    uint32_t width;
    uint32_t height;
};

/**
 * @brief 3D unsigned size.
 * */
struct Size3D {
    uint32_t width;
    uint32_t height;
    uint32_t depth_or_layers; ///< Depth for 3D textures, layers for 2D arrays
};

/**
 * @brief Buffer region for copy operations.
 * */
struct BufferRegion {
    uint64_t src_offset = 0; ///< Source offset in bytes. Default is 0.
    uint64_t dst_offset = 0; ///< Destination offset in bytes. Default is 0.
    uint64_t size_bytes; ///< Size of portion of source buffer to take copy from in bytes.
};

/**
 * @brief Adapter description.
 * Describes hardware driver identificators as well as memory limits.
 * */
struct AdapterDesc {
    std::array<const char, 256> description{}; ///< Adapter description. Contains name of the graphics adapter.
    uint32_t vendor_id; ///< Vendor ID. Can be used to find the correct adapter.
    uint32_t device_id; ///< Device ID. Together with wis::AdapterDesc::vendor_id uniquely identifies the device.
    uint32_t subsys_id; ///< Unused
    uint32_t revision; ///< Driver revision. Unused.
    uint64_t dedicated_video_memory; ///< Dedicated video memory in bytes. Used for Default Memory type.
    uint64_t dedicated_system_memory; ///< Dedicated system memory in bytes. Used for Upload and Readback Memory types.
    uint64_t shared_system_memory; ///< Shared system memory in bytes. Used for GPUUpload Memory type.
    uint64_t adapter_id; ///< Adapter unique ID. Can be used to find the correct adapter.
    wis::AdapterFlags flags; ///< Adapter flags. Describe the adapter kind.
};

/**
 * @brief Input slot description for wis::InputLayout.
 * */
struct InputSlotDesc {
    uint32_t slot; ///< Input slot number. Must be unique.
    uint32_t stride_bytes; ///< Stride in bytes. Size of one vertex in the slot.
    wis::InputClass input_class; ///< Input class. Defines how the data is read (Per vertex or Per instance).
};

/**
 * @brief Input attribute description for wis::InputLayout.
 * */
struct InputAttribute {
    uint32_t input_slot; ///< Input slot number. Must be unique.
    const char* semantic_name; ///< Semantic name of the attribute in HLSL. Must be unique.
    uint32_t semantic_index; ///< Semantic index of the attribute in HLSL. Must be unique.
    uint32_t location; ///< Location of the attribute in HLSL. Must be unique.
    wis::DataFormat format; ///< Data format of the attribute.
    uint32_t offset_bytes; ///< Offset in bytes from the beginning of the vertex.
};

/**
 * @brief Input layout description for .
 * */
struct InputLayout {
    const wis::InputSlotDesc* slots; ///< Input slots array. Made to pick up data from several arrays of vertex data.
    uint32_t slot_count; ///< Input slots count. Max number is 16.
    const wis::InputAttribute* attributes; ///< Input attributes array. Describes how the vertex data is read by the HLSL shader.
    uint32_t attribute_count; ///< Input attributes count.
};

/**
 * @brief Rasterizer description for .
 * */
struct RasterizerDesc {
    wis::FillMode fill_mode = wis::FillMode::Solid; ///< Fill mode. Solid or Wireframe. Default is wis::FillMode::Solid.
    wis::CullMode cull_mode = wis::CullMode::Back; ///< Cull mode. None, Front, Back. Default is wis::CullMode::Back.
    wis::WindingOrder front_face = wis::WindingOrder::Clockwise; ///< Front face winding order. Clockwise or CounterClockwise. Default is wis::WindingOrder::Clockwise.
    bool depth_bias_enable = false; ///< Depth bias enable. Default is false.
    float depth_bias = 0.0f; ///< Depth bias. Default is 0.0f.
    float depth_bias_clamp = 0.0f; ///< Depth bias clamp. Default is 0.0f.
    float depth_bias_slope_factor = 0.0f; ///< Depth bias slope factor e.g. for shadows. Default is 0.0f.
    bool depth_clip_enable = true; ///< Depth clip enable. Default is true.
};

/**
 * @brief Sample description of Multisampling for .
 * */
struct SampleDesc {
    wis::SampleRate rate = wis::SampleRate::S1; ///< Sample rate. Default is wis::SampleRate::S1.
    float quality = 0.0f; ///< Sample quality. Default is 0.0f.
    uint32_t sample_mask = 0xffffffff; ///< Sample mask. Default is 0xffffffff.
};

/**
 * @brief Stencil description for wis::DepthStencilDesc.
 * */
struct StencilDesc {
    wis::StencilOp fail_op = wis::StencilOp::Keep; ///< Stencil operation if the stencil test fails. Default is wis::StencilOp::Keep.
    wis::StencilOp depth_fail_op = wis::StencilOp::Keep; ///< Stencil operation if the stencil test passes and the depth test fails. Default is wis::StencilOp::Keep.
    wis::StencilOp pass_op = wis::StencilOp::Keep; ///< Stencil operation if the stencil test passes. Default is wis::StencilOp::Keep.
    wis::Compare comparison = wis::Compare::Always; ///< Stencil comparison function. Default is wis::Compare::Always.
    uint8_t read_mask = 0xff; ///< Stencil read mask. Default is 0xff.
    uint8_t write_mask = 0xff; ///< Stencil write mask. Default is 0xff.
};

/**
 * @brief Depth stencil description for .
 * */
struct DepthStencilDesc {
    bool depth_enable = false; ///< Depth test enable. Default is false.
    bool depth_write_enable = false; ///< Depth write enable. Default is false.
    wis::Compare depth_comp = wis::Compare::Less; ///< Depth comparison function. Default is wis::Compare::Less.
    bool stencil_enable = false; ///< Stencil test enable. Default is false.
    wis::StencilDesc stencil_front = {}; ///< Stencil description for front faces.
    wis::StencilDesc stencil_back = {}; ///< Stencil description for back faces.
    bool depth_bound_test = false; ///< Depth bound test enable. Default is false.
};

/**
 * @brief Blend attachment description for wis::BlendStateDesc.
 * */
struct BlendAttachmentDesc {
    bool blend_enable = false; ///< Blend enable. Default is false.
    wis::BlendFactor src_color_blend = wis::BlendFactor::One; ///< Source color blend factor. Default is wis::BlendFactor::One.
    wis::BlendFactor dst_color_blend = wis::BlendFactor::Zero; ///< Destination color blend factor. Default is wis::BlendFactor::Zero.
    wis::BlendOp color_blend_op = wis::BlendOp::Add; ///< Color blend operation. Default is wis::BlendOp::Add.
    wis::BlendFactor src_alpha_blend = wis::BlendFactor::One; ///< Source alpha blend factor. Default is wis::BlendFactor::One.
    wis::BlendFactor dst_alpha_blend = wis::BlendFactor::Zero; ///< Destination alpha blend factor. Default is wis::BlendFactor::Zero.
    wis::BlendOp alpha_blend_op = wis::BlendOp::Add; ///< Alpha blend operation. Default is wis::BlendOp::Add.
    wis::ColorComponents color_write_mask = wis::ColorComponents::All; ///< Color write mask. Default is wis::ColorComponents::All.
};

/**
 * @brief Blend state description for .
 * */
struct BlendStateDesc {
    bool logic_op_enable = false; ///< Logic operation enable. Default is false.
    wis::LogicOp logic_op = wis::LogicOp::Noop; ///< Logic operation. Default is wis::LogicOp::Noop.
    std::array<wis::BlendAttachmentDesc, 8> attachments{}; ///< Blend attachment descriptions. Max Array size is 8.
    uint32_t attachment_count; ///< Blend attachment count.
};

/**
 * @brief Render attachments description for .
 * */
struct RenderAttachmentsDesc {
    std::array<wis::DataFormat, 8> attachment_formats{}; ///< Attachment formats array. Describes the format of the render target.
    uint32_t attachments_count; ///< Attachment formats count. Max is 8.
    wis::DataFormat depth_attachment; ///< Depth attachment format. Describes the format of the depth buffer.
};

/**
 * @brief A set of constants that get pushed directly to the pipeline.
 * Only one set can be created per shader stage.
 * */
struct PushConstant {
    wis::ShaderStages stage; ///< Shader stage. Defines the stage where the constant is used.
    uint32_t size_bytes; ///< Size of the constant in bytes. Must be divisible by 4.
    uint32_t bind_register; ///< Bind register number in HLSL.
};

/**
 * @brief Swapchain description for wis::SwapChain creation.
 * */
struct SwapchainDesc {
    wis::Size2D size; ///< Swapchain texture size.
    wis::DataFormat format; ///< Swapchain texture format.
    uint32_t buffer_count; ///< Swapchain buffer count.
    bool stereo; ///< Stereo mode enable. If there is no stereo in the system will be ignored.
    bool vsync; ///< VSync enable. Specifies Initial VSync. This value may be changed on per-present bases with wis::DeviceFeature::DynamicVSync.
    bool tearing; ///< Tearing enable. If VSync is disabled, Tearing may be enabled. If System does not allow tearing the flag is ignored.
};

/**
 * @brief Texture description for wis::Texture creation.
 * */
struct TextureDesc {
    wis::DataFormat format; ///< Texture pixel/block format.
    wis::Size3D size; ///< Texture size. Third dimension may be used for array layers or depth layers, depending on wis::TextureDesc::layout.
    uint32_t mip_levels = 1; ///< Mip levels count. Default is 1.
    wis::TextureLayout layout = wis::TextureLayout::Texture2D; ///< Texture layout. Default is wis::TextureLayout::Texture2D.
    wis::SampleRate sample_count = wis::SampleRate::S1; ///< Sample count. Default is wis::SampleRate::S1.
    wis::TextureUsage usage = wis::TextureUsage::None; ///< Texture usage flags.
};

/**
 * @brief Allocation info for Resource Allocation.
 * */
struct AllocationInfo {
    uint64_t size_bytes; ///< Size of the allocation in bytes.
    uint64_t alignment_bytes; ///< Alignment of the allocation in bytes.
};

/**
 * @brief Texture region for copy operations.
 * */
struct TextureRegion {
    /**
     * @brief Offset in the texture in pixels.
     * In BufferToTexture determines offset of destination texture.
     * In TextureToBuffer - offset of source image.
     * */
    wis::Size3D offset;
    wis::Size3D size; ///< Size of the region in pixels.
    uint32_t mip; ///< Mip level of the texture.
    uint32_t array_layer; ///< Array layer of the texture.
    wis::DataFormat format; ///< Format of the texture.
};

/**
 * @brief Buffer to texture copy region.
 * */
struct BufferTextureCopyRegion {
    uint64_t buffer_offset; ///< Buffer offset in bytes.
    wis::TextureRegion texture; ///< Texture region.
};

/**
 * @brief Push descriptor. Used to push data directly to pipeline.
 * */
struct PushDescriptor {
    wis::ShaderStages stage; ///< Shader stage. Defines the stage where the descriptor is used.
    wis::DescriptorType type; ///< Descriptor type. Works only with buffer-like bindings.
};

/**
 * @brief Subresource range for wis::TextureBarrier.
 * */
struct SubresourceRange {
    uint32_t base_mip_level; ///< Base mip level.
    uint32_t level_count; ///< Mip levels count.
    uint32_t base_array_layer; ///< Base array layer.
    uint32_t layer_count; ///< Array layers count.
};

/**
 * @brief Render target description for wis::RenderTarget creation.
 * */
struct RenderTargetDesc {
    wis::DataFormat format; ///< Render target format.
    wis::TextureLayout layout = wis::TextureLayout::Texture2D; ///< Render target layout. Default is wis::TextureLayout::Texture2D.
    uint32_t mip = 0; ///< Mip level of the render target. Default is 0.
    uint32_t base_array_layer = 0; ///< Base array layer of the render target. Default is 0.
    uint32_t layer_count = 1; ///< Array layers count of the render target. Default is 1.
};

/**
 * @brief Viewport description for wis::CommandList.
 * Viewport is considered from Top Left corner.
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
 * @brief Scissor description for wis::CommandList.
 * */
struct Scissor {
    int32_t left; ///< Left corner x coordinate.
    int32_t top; ///< Top corner y coordinate.
    int32_t right; ///< Right corner x coordinate.
    int32_t bottom; ///< Bottom corner y coordinate.
};

/**
 * @brief Buffer barrier for wis::CommandList.
 * */
struct BufferBarrier {
    wis::BarrierSync sync_before; ///< Synchronization before the barrier.
    wis::BarrierSync sync_after; ///< Synchronization after the barrier.
    wis::ResourceAccess access_before; ///< Resource access before the barrier.
    wis::ResourceAccess access_after; ///< Resource access after the barrier.
    uint64_t offset = 0; ///< Offset in the buffer in bytes. Default is 0.
    uint64_t size = UINT64_MAX; ///< Size of the buffer in bytes. Default is UINT64_MAX, which means entire buffer.
};

/**
 * @brief Texture barrier for wis::CommandList.
 * */
struct TextureBarrier {
    wis::BarrierSync sync_before; ///< Synchronization before the barrier.
    wis::BarrierSync sync_after; ///< Synchronization after the barrier.
    wis::ResourceAccess access_before; ///< Resource access before the barrier.
    wis::ResourceAccess access_after; ///< Resource access after the barrier.
    wis::TextureState state_before; ///< Texture state before the barrier.
    wis::TextureState state_after; ///< Texture state after the barrier.
    /**
     * @brief Subresource range of the texture.
     * Zero initialized range means all subresources are selected.
     * */
    wis::SubresourceRange subresource_range;
};

/**
 * @brief Descriptor table entry for wis::DescriptorTable.
 * */
struct DescriptorTableEntry {
    wis::DescriptorType type; ///< Descriptor type.
    uint32_t bind_register; ///< Bind register number in HLSL.
    uint32_t binding; ///< Binding number in HLSL.
    uint32_t count; ///< Descriptor count for Array descriptors. UINT32_MAX means unbounded array.
};

/**
 * @brief Descriptor table for .
 * */
struct DescriptorTable {
    wis::DescriptorHeapType type; ///< Descriptor heap type. Either Descriptor or Sampler.
    const wis::DescriptorTableEntry* entries; ///< Descriptor table entries array.
    uint32_t entry_count; ///< Descriptor table entries count.
    wis::ShaderStages stage; ///< Shader stage. Defines the stage where the table is used.
};

/**
 * @brief Sampler description for wis::Sampler creation.
 * */
struct SamplerDesc {
    wis::Filter min_filter; ///< Minification filter.
    wis::Filter mag_filter; ///< Magnification filter.
    wis::Filter mip_filter; ///< Mip level filter.
    bool anisotropic; ///< Anisotropic filtering enable.
    uint32_t max_anisotropy; ///< Max anisotropy level. Max is 16.
    wis::AddressMode address_u; ///< Address mode for U coordinate.
    wis::AddressMode address_v; ///< Address mode for V coordinate.
    wis::AddressMode address_w; ///< Address mode for W coordinate.
    float min_lod; ///< Min LOD value.
    float max_lod; ///< Max LOD value.
    float mip_lod_bias; ///< Mip LOD bias value.
    wis::Compare comparison_op; ///< Comparison operation for comparison samplers.
    std::array<float, 4> border_color{}; ///< Border color.
};

/**
 * @brief Component mapping for wis::ShaderResourceDesc.
 * */
struct ComponentMapping {
    wis::ComponentSwizzle r = wis::ComponentSwizzle::Red; ///< Component mapping for Red channel. Default is wis::ComponentSwizzle::Red.
    wis::ComponentSwizzle g = wis::ComponentSwizzle::Green; ///< Component mapping for Green channel. Default is wis::ComponentSwizzle::Green.
    wis::ComponentSwizzle b = wis::ComponentSwizzle::Blue; ///< Component mapping for Blue channel. Default is wis::ComponentSwizzle::Blue.
    wis::ComponentSwizzle a = wis::ComponentSwizzle::Alpha; ///< Component mapping for Alpha channel. Default is wis::ComponentSwizzle::Alpha.
};

/**
 * @brief Shader resource description for .
 * */
struct ShaderResourceDesc {
    wis::DataFormat format; ///< Resource format.
    wis::TextureViewType view_type; ///< Resource view type.
    wis::ComponentMapping component_mapping; ///< Component mapping.
    wis::SubresourceRange subresource_range; ///< Subresource range of the resource.
};

/**
 * @brief Unordered access description for RW Texture creation.
 * */
struct UnorderedAccessDesc {
    wis::DataFormat format; ///< Resource format.
    wis::TextureViewType view_type; ///< Resource view type.
    wis::SubresourceRange subresource_range; ///< Subresource range of the resource.
};

/**
 * @brief Struct used to query the extensions for C code.
 * Queried results should not be freed, their lifetime ends with the Factory they were created with.
 * If wis::FactoryExtQuery::extension_id is 0, wis::FactoryExtQuery::result must be populated with already created extension.
 * Otherwise extension is ignored.
 * */
struct FactoryExtQuery {
    wis::FactoryExtID extension_id; ///< Extension ID.
    /**
     * @brief Result of the query.
     * Pointer is populated with the extension with queried ID.
     * If the extension is not supported/failed to initialize the result is NULL.
     * */
    void* result;
};

/**
 * @brief Struct used to query the extensions for C code.
 * Queried results should not be freed, their lifetime ends with the Factory they were created with.
 * If wis::DeviceExtQuery::extension_id is 0, wis::DeviceExtQuery::result must be populated with already created extension.
 * Otherwise extension is ignored.
 * */
struct DeviceExtQuery {
    wis::DeviceExtID extension_id; ///< Extension ID.
    /**
     * @brief Result of the query.
     * Pointer is populated with the extension with queried ID.
     * If the extension is not supported/failed to initialize the result is NULL.
     * */
    void* result;
};

/**
 * @brief Top level acceleration structure build description.
 * */
struct TopLevelASBuildDesc {
    wis::AccelerationStructureFlags flags; ///< Build flags.
    uint32_t instance_count; ///< Instance count.
    uint64_t gpu_address; ///< Address of instances.
    bool indirect; ///< If true Buffer under address contains pointers to the instances, rather than instances themselves.
    /**
     * @brief true means the acceleration structure is being updated.
     * flags must have contained wis::AccelerationStructureFlags::AllowUpdate to perfom updates.
     * */
    bool update;
};

/**
 * @brief Geometry description for bottom-level acceleration structure. Mayy contain AABBs or Triangles.
 * */
struct AcceleratedGeometryInput {
    wis::ASGeometryType geometry_type; ///< Type of the geometry (Triangles/AABB).
    wis::ASGeometryFlags flags; ///< Geometry flags.
    uint64_t vertex_or_aabb_buffer_address; ///< Buffer address of the buffer containing vertex data or AABB data (float [6]) depending on the geometry type.
    uint64_t vertex_or_aabb_buffer_stride; ///< Stride of the vertex buffer in bytes or stride of the AABB buffer in bytes.
    uint64_t index_buffer_address; ///< Buffer address of the buffer containing index data. Unused for wis::ASGeometryType::AABBs.
    uint64_t transform_matrix_address; ///< GPU Buffer address of the containing transform matrix (float [3][4]). Unused for wis::ASGeometryType::AABBs.
    uint32_t vertex_count; ///< Vertex count. Unused for wis::ASGeometryType::AABBs.
    uint32_t triangle_or_aabb_count; ///< For triangles it is equal to (index_count/3) and count for AABBs.
    wis::DataFormat vertex_format; ///< Format of the vertices. Unused for wis::ASGeometryType::AABBs.
    wis::IndexType index_format; ///< Format of the indices. Unused for wis::ASGeometryType::AABBs.
};

/**
 * @brief Acceleration structure allocation info. Used to query sizes for AS build/update buffers.
 * */
struct ASAllocationInfo {
    uint64_t scratch_size; ///< Size of the scratch buffer.
    uint64_t result_size; ///< Size of the result buffer.
    uint64_t update_size; ///< Size of the update buffer.
};

/**
 * @brief Descriptor binding description for RootSignature and Descriptor Storage creation.
 * Description place in array determines binding index that this lane maps to. e.g. bindings[1] means on HLSL side this results in [[vk::binding(0,1)]].
 * All the bindings in Descriptor Storage are unbounded, array of these structures determine the presence and order of the bindings.
 * */
struct DescriptorBindingDesc {
    wis::DescriptorType binding_type; ///< Binding type. Must be unique in array.
    uint32_t binding_space; ///< Binding space number in HLSL.
    /**
     * @brief Number of consecutive spaces this binding occupies.
     * e.g. for binding_space = 1 and space_overlap_count = 3, HLSL binding will be :register(x0,space1), register(x0,space2), register(x0,space3)
     * This is useful for binding multiple resource types to the same register array in HLSL.
     * */
    uint32_t space_overlap_count;
    /**
     * @brief How many bindings should be allocated.
     * Affects only the count of descriptors allocated in the descriptor heap, Root Signature always receives unbounded array with max amount of 4096 registers.
     * */
    uint32_t binding_count;
};

/**
 * @brief Defines export shader functions from a library shader.
 * */
struct ShaderExport {
    const char* entry_point; ///< Entry point of the shader.
    wis::RaytracingShaderType shader_type; ///< Type of the shader.
    uint32_t shader_array_index; ///< Index of the shader in the shader array.
};

/**
 * @brief Hit group description for Raytracing pipeline.
 * */
struct HitGroupDesc {
    /**
     * @brief Type of the hit group.
     * wis::HitGroupType::Triangles - hit group for triangles. Uses closest hit shader and optionally any hit shader for transparency.
     * wis::HitGroupType::Procedural - hit group for procedural geometry. Uses intersection shader and optionally any hit shader for transparency.
     * */
    wis::HitGroupType type;
    uint32_t closest_hit_export_index = UINT32_MAX; ///< Closest hit shader from wis::ShaderExport.
    uint32_t any_hit_export_index = UINT32_MAX; ///< Any hit shader.
    uint32_t intersection_export_index = UINT32_MAX; ///< Intersection shader.
};

/**
 * @brief Shader binding table description for Raytracing pipeline.
 * */
struct ShaderBindingTableInfo {
    uint32_t entry_size; ///< Size/stride of the entry in bytes.
    uint32_t table_start_alignment; ///< Alignment of the table start in bytes.
};

//=================================DELEGATES=================================

/**
 * @brief Debug callback delegate. Used for Library internal log messages.
 * @param severity Severity of the message. Depend on WISDOM_LOG_LEVEL option.
 * @param message Message string.
 * @param user_data User data pointer.
 * */
using DebugCallback = void (*)(wis::Severity severity, const char* message, void* user_data);
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
template<>
struct is_flag_enum<wis::FenceFlags> : public std::true_type {
};
template<>
struct is_flag_enum<wis::PipelineFlags> : public std::true_type {
};
template<>
struct is_flag_enum<wis::ASGeometryFlags> : public std::true_type {
};
template<>
struct is_flag_enum<wis::AccelerationStructureFlags> : public std::true_type {
};
template<>
struct is_flag_enum<wis::ASInstanceFlags> : public std::true_type {
};
//============================== CONSTS ==============================

static inline constexpr Result success{
    wis::Status::Ok, "Operation succeeded"
};

template<typename RetTy>
struct ResultValue {
    Result status;
    RetTy value;

    constexpr ResultValue() noexcept = default;
    constexpr ResultValue(wis::Result status) noexcept
        : status(status)
    {
    }

    template<typename Callable, typename Callee, typename... Args>
        requires std::is_member_function_pointer_v<Callable>
    constexpr ResultValue(Callable&& f, Callee* self, Args&&... args) noexcept
        : value(std::invoke(f, self, status, std::forward<Args>(args)...))
    {
    }
    template<typename Callable, typename... Args>
    constexpr ResultValue(Callable&& f, Args&&... args) noexcept
        : value(f(status, std::forward<Args>(args)...))
    {
    }

    template<typename Callable, typename Callee, typename... Args>
    static constexpr ResultValue<RetTy> from_member_func(Callable&& f, Callee* self, Args&&... args) noexcept
    {
        ResultValue<RetTy> rv;
        rv.value = std::invoke(f, self, rv.status, std::forward<Args>(args)...);
        return rv;
    }
};

template<uint32_t s, typename RetTy>
constexpr decltype(auto) get(ResultValue<RetTy>& rv) noexcept
{
    if constexpr (s == 0) {
        return std::forward<Result>(rv.status);
    } else {
        return std::forward<RetTy>(rv.value);
    }
}

} // namespace wis
