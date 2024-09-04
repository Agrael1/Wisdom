// GENERATED
#pragma once
#include "wisdom_exports.h"
#include <stdint.h>
#include <stdbool.h>

/** \mainpage Wisdom API Documentation

<b>Version 0.2.0</b>

Copyright (c) 2024 Ilya Doroshenko. All rights reserved.
License: MIT
See also: [repository on GitHub](https://github.com/Agrael1/Wisdom)
*/

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Shader stages that can be used in the pipeline.
 * Main use is Root signature and descriptor management.
 * Stages have no granularity, either all or one can be selected.
 *
 * Translates to VkShaderStageFlagBits for vk implementation.
 * */
enum WisShaderStages {
    ShaderStagesAll = 0, ///< All shader stages.
    ShaderStagesVertex = 1, ///< Vertex shader stage.
    ShaderStagesHull = 2, ///< Hull/Tessellation control shader stage.
    ShaderStagesDomain = 3, ///< Domain/Tessellation evaluation shader stage.
    ShaderStagesGeometry = 4, ///< Geometry shader stage.
    ShaderStagesPixel = 5, ///< Pixel/Fragment shader stage.
    ShaderStagesAmplification = 6, ///< Amplification shader stage.
    ShaderStagesMesh = 7, ///< Mesh shader stage.
    ShaderStagesCount = 8, ///< Number of stages.
};

/**
 * @brief Represents main error communication source for the API.
 * Compare against StatusOk for success.
 *
 * */
enum WisStatus {
    StatusOk = 0, ///< Operation succeded.
    /**
     * @brief Operation timed out.
     * Counts as a success value in some cases.
     * */
    StatusTimeout = 1,
    /**
     * @brief Operation partially succeeded.
     * Some times it means that core value is initialized,
     * but some functionality may be missing and may require
     * you to check if demanded functionality is in place.
     * */
    StatusPartial = 2,
    StatusError = -1, ///< Operation failed. Check WisResult::error for more details.
    /**
     * @brief One or more arguments, or parts of arguments
     * passed to the function were incorrect. Check WisResult::error for more details.
     * */
    StatusInvalidArgument = -2,
    /**
     * @brief There is no more host memory available.
     * Allocation with malloc or similar call has failed.
     * */
    StatusOutOfMemory = -3,
    /**
     * @brief Device driver was forcefully stopped.
     * Most of the time happens on swapchain presentation.
     * */
    StatusDeviceLost = -4,
    /**
     * @brief Swapchain presentation was not visible to the user.
     * Rendering is too fast.
     * */
    StatusOccluded = -5,
};

/**
 * @brief Determines the behavior when wait for multiple fences is issued.
 *
 * */
enum WisMutiWaitFlags {
    MutiWaitFlagsAll = 0, ///< All the fences in the batch are triggered.
    MutiWaitFlagsAny = 1, ///< At least one of the fences from the batch is triggered.
};

/**
 * @brief Type of the descriptor in the descriptor table.
 *
 * Translates to VkDescriptorType for vk implementation.
 * */
enum WisDescriptorType {
    /**
     * @brief Descriptor is a shader resource view.
     * Used for textures.
     * */
    DescriptorTypeShaderResource = 0,
    DescriptorTypeConstantBuffer = 1, ///< Descriptor is a constant buffer view.
    /**
     * @brief Descriptor is an unordered access view.
     * Used for read/write operations in compute shaders.
     * */
    DescriptorTypeUnorderedAccess = 2,
    /**
     * @brief Descriptor is a sampler.
     * Sampler is used to sample textures in shaders.
     * Stored in separate descriptor table and
     * can't be mixed with other descriptor types
     * */
    DescriptorTypeSampler = 3,
};

/**
 * @brief Type of the queue to create.
 *
 * */
enum WisQueueType {
    QueueTypeGraphics = 0, ///< Queue is used for graphics operations.
    QueueTypeCompute = 2, ///< Queue is used for compute operations.
    QueueTypeCopy = 3, ///< Queue is used for copy operations.
    QueueTypeVideoDecode = 4, ///< Queue is used for video decoding operations.
};

/**
 * @brief Oreders the adapters according to preference
 * using builtin heuristics of underlying APIs.
 *
 * Translates to DXGI_GPU_PREFERENCE for dx implementation.
 * */
enum WisAdapterPreference {
    AdapterPreferenceNone = 0, ///< No particular preference, list adapters in system divised order.
    /**
     * @brief List the adapters from low power consumption to high.
     * Order is as follows: Integrated, Discrete, External, Software.
     * */
    AdapterPreferenceMinConsumption = 1,
    /**
     * @brief List the adapters from high performance to low.
     * Order is as follows: External, Discrete, Integrated, Software.
     * */
    AdapterPreferencePerformance = 2,
};

/**
 * @brief Log message severity.
 * Used with DebugCallback and internal library logging.
 *
 * */
enum WisSeverity {
    SeverityDebug = 0, ///< Message carries debug information.
    SeverityTrace = 1, ///< Message contains trace point (function call stack info).
    SeverityInfo = 2, ///< Message contains general information.
    /**
     * @brief Message contains warning.
     * There is something wrong and it may affect performance or stability of the application.
     * */
    SeverityWarning = 3,
    /**
     * @brief Message contains error.
     * Stability of the application is compromized.
     * */
    SeverityError = 4,
    /**
     * @brief Message contains critical error.
     * The application must be shut down, no further execution.
     * */
    SeverityCritical = 5,
};

/**
 * @brief Input classification for vertex buffer data.
 *
 * */
enum WisInputClass {
    InputClassPerVertex = 0, ///< Vertex buffer data is vertex data.
    InputClassPerInstance = 1, ///< Vertex buffer data is per instance data.
};

enum WisCullMode {
    CullModeNone = 1,
    CullModeFront = 2,
    CullModeBack = 3,
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
 * Translates to VkFormat for vk implementation.
 * */
enum WisDataFormat {
    DataFormatUnknown = 0, ///< Unknown format.
    /**
     * @brief 32 bit per channel RGBA format.
     * A four-component, 128-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    DataFormatRGBA32Float = 2,
    /**
     * @brief 32 bit per channel RGBA format.
     * A four-component, 128-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    DataFormatRGBA32Uint = 3,
    /**
     * @brief 32 bit per channel RGBA format.
     * A four-component, 128-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11,
     * a 32-bit A component in bytes 12..15.
     * */
    DataFormatRGBA32Sint = 4,
    /**
     * @brief 32 bit per channel RGB format.
     * A three-component, 96-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    DataFormatRGB32Float = 6,
    /**
     * @brief 32 bit per channel RGB format.
     * A three-component, 96-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    DataFormatRGB32Uint = 7,
    /**
     * @brief 32 bit per channel RGB format.
     * A three-component, 96-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7,
     * a 32-bit B component in bytes 8..11.
     * */
    DataFormatRGB32Sint = 8,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 64-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    DataFormatRGBA16Float = 10,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 64-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    DataFormatRGBA16Unorm = 11,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 64-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    DataFormatRGBA16Uint = 12,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 64-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    DataFormatRGBA16Snorm = 13,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 64-bit signed integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3,
     * a 16-bit B component in bytes 4..5,
     * a 16-bit A component in bytes 6..7.
     * */
    DataFormatRGBA16Sint = 14,
    /**
     * @brief 32 bit per channel RG format.
     * A two-component, 64-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    DataFormatRG32Float = 16,
    /**
     * @brief 32 bit per channel RG format.
     * A two-component, 64-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    DataFormatRG32Uint = 17,
    /**
     * @brief 32 bit per channel RG format.
     * A two-component, 64-bit signed integer format that has
     * a 32-bit R component in bytes 0..3,
     * a 32-bit G component in bytes 4..7.
     * */
    DataFormatRG32Sint = 18,
    /**
     * @brief 32 bit depth, 8 bit stencil format.
     * A combined depth/stencil format with a 32-bit depth component in bytes 0..3
     * and an 8-bit stencil component in bytes 4..4.
     * */
    DataFormatD32FloatS8Uint = 20,
    /**
     * @brief 10 bit per channel RGB format with 2 bit alpha.
     * A four-component, 32-bit unsigned normalized format that has
     * a 10-bit R component in bits 0..9,
     * a 10-bit G component in bits 10..19,
     * a 10-bit B component in bits 20..29,
     * a 2-bit A component in bits 30..31.
     * */
    DataFormatRGB10A2Unorm = 24,
    /**
     * @brief 10 bit per channel RGB format with 2 bit alpha.
     * A four-component, 32-bit unsigned integer format that has
     * a 10-bit R component in bits 0..9,
     * a 10-bit G component in bits 10..19,
     * a 10-bit B component in bits 20..29,
     * a 2-bit A component in bits 30..31.
     * */
    DataFormatRGB10A2Uint = 25,
    /**
     * @brief 11 bit per channel RG format with 10 bit blue.
     * A three-component, 32-bit signed floating-point format that has
     * a 11-bit R component in bits 0..10,
     * a 11-bit G component in bits 11..21,
     * a 10-bit B component in bits 22..31.
     * */
    DataFormatRG11B10Float = 26,
    /**
     * @brief 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    DataFormatRGBA8Unorm = 28,
    /**
     * @brief 8 bit per channel RGBA format. SRGB non-linear color space.
     * A four-component, 32-bit unsigned normalized non-linear SRGB format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    DataFormatRGBA8UnormSrgb = 29,
    /**
     * @brief 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    DataFormatRGBA8Uint = 30,
    /**
     * @brief 8 bit per channel RGBA format.
     * A four-component, 32-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    DataFormatRGBA8Snorm = 31,
    /**
     * @brief 8 bit per channel RGBA format.
     * A four-component, 32-bit signed integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit B component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    DataFormatRGBA8Sint = 32,
    /**
     * @brief 16 bit per channel RG format.
     * A two-component, 32-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    DataFormatRG16Float = 34,
    /**
     * @brief 16 bit per channel RG format.
     * A two-component, 32-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    DataFormatRG16Unorm = 35,
    /**
     * @brief 16 bit per channel RG format.
     * A two-component, 32-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    DataFormatRG16Uint = 36,
    /**
     * @brief 16 bit per channel RG format.
     * A two-component, 32-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    DataFormatRG16Snorm = 37,
    /**
     * @brief 16 bit per channel RG format.
     * A two-component, 32-bit signed integer format that has
     * a 16-bit R component in bytes 0..1,
     * a 16-bit G component in bytes 2..3.
     * */
    DataFormatRG16Sint = 38,
    /**
     * @brief 32 bit depth format.
     * A 32-bit depth format supporting 32-bit floating-point depth values.
     * */
    DataFormatD32Float = 40,
    /**
     * @brief 32 bit per channel R format.
     * A one-component, 32-bit signed floating-point format that has
     * a 32-bit R component in bytes 0..3.
     * */
    DataFormatR32Float = 41,
    /**
     * @brief 32 bit per channel R format.
     * A one-component, 32-bit unsigned integer format that has
     * a 32-bit R component in bytes 0..3.
     * */
    DataFormatR32Uint = 42,
    /**
     * @brief 32 bit per channel R format.
     * A one-component, 32-bit signed integer format that has
     * a 32-bit R component in bytes 0..3.
     * */
    DataFormatR32Sint = 43,
    /**
     * @brief 24 bit depth, 8 bit stencil format.
     * A combined depth/stencil format with a 24-bit depth component in bytes 0..2
     * and an 8-bit stencil component in bytes 3..3.
     * */
    DataFormatD24UnormS8Uint = 45,
    /**
     * @brief 8 bit per channel RG format.
     * A two-component, 16-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    DataFormatRG8Unorm = 49,
    /**
     * @brief 8 bit per channel RG format.
     * A two-component, 16-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    DataFormatRG8Uint = 50,
    /**
     * @brief 8 bit per channel RG format.
     * A two-component, 16-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    DataFormatRG8Snorm = 51,
    /**
     * @brief 8 bit per channel RG format.
     * A two-component, 16-bit signed integer format that has
     * an 8-bit R component in bytes 0..0,
     * an 8-bit G component in bytes 1..1.
     * */
    DataFormatRG8Sint = 52,
    /**
     * @brief 16 bit per channel R format.
     * A one-component, 16-bit signed floating-point format that has
     * a 16-bit R component in bytes 0..1.
     * */
    DataFormatR16Float = 54,
    /**
     * @brief 16 bit depth format.
     * A 16-bit depth format supporting 16-bit unsigned normalized depth values.
     * */
    DataFormatD16Unorm = 55,
    /**
     * @brief 16 bit per channel R format.
     * A one-component, 16-bit unsigned normalized format that has
     * a 16-bit R component in bytes 0..1.
     * */
    DataFormatR16Unorm = 56,
    /**
     * @brief 16 bit per channel R format.
     * A one-component, 16-bit unsigned integer format that has
     * a 16-bit R component in bytes 0..1.
     * */
    DataFormatR16Uint = 57,
    /**
     * @brief 16 bit per channel R format.
     * A one-component, 16-bit signed normalized format that has
     * a 16-bit R component in bytes 0..1.
     * */
    DataFormatR16Snorm = 58,
    /**
     * @brief 16 bit per channel R format.
     * A one-component, 16-bit signed integer format that has
     * a 16-bit R component in bytes 0..1.
     * */
    DataFormatR16Sint = 59,
    /**
     * @brief 8 bit per channel R format.
     * A one-component, 8-bit unsigned normalized format that has
     * an 8-bit R component in bytes 0..0.
     * */
    DataFormatR8Unorm = 61,
    /**
     * @brief 8 bit per channel R format.
     * A one-component, 8-bit unsigned integer format that has
     * an 8-bit R component in bytes 0..0.
     * */
    DataFormatR8Uint = 62,
    /**
     * @brief 8 bit per channel R format.
     * A one-component, 8-bit signed normalized format that has
     * an 8-bit R component in bytes 0..0.
     * */
    DataFormatR8Snorm = 63,
    /**
     * @brief 8 bit per channel R format.
     * A one-component, 8-bit signed integer format that has
     * an 8-bit R component in bytes 0..0.
     * */
    DataFormatR8Sint = 64,
    /**
     * @brief 9 bit per channel RGB format with shared exponent.
     * A three-component, 32-bit unsigned floating-point format that has
     * a 9-bit R component in bits 0..8,
     * a 9-bit G component in bits 9..17,
     * a 9-bit B component in bits 18..26,
     * a shared 5-bit exponent in bits 27..31.
     * */
    DataFormatRGB9E5UFloat = 67,
    /**
     * @brief BC1/DXT1 compressed format.
     * A four-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGB texel data, and provides 1 bit of alpha.
     * */
    DataFormatBC1RGBAUnorm = 71,
    /**
     * @brief BC1/DXT1 compressed format.
     * A four-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGB texel data with sRGB nonlinear encoding, and provides 1 bit of alpha.
     * */
    DataFormatBC1RGBAUnormSrgb = 72,
    /**
     * @brief BC2/DXT3 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    DataFormatBC2RGBAUnorm = 74,
    /**
     * @brief BC2/DXT3 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    DataFormatBC2RGBAUnormSrgb = 75,
    /**
     * @brief BC3/DXT5 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with the first 64 bits encoding alpha values
     * followed by 64 bits encoding RGB values
     * */
    DataFormatBC3RGBAUnorm = 77,
    /**
     * @brief BC3/DXT5 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * with the first 64 bits encoding alpha values followed by 64 bits encoding RGB values
     * */
    DataFormatBC3RGBAUnormSrgb = 78,
    /**
     * @brief BC4 compressed format.
     * A single-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized red texel data
     * */
    DataFormatBC4RUnorm = 80,
    /**
     * @brief BC4 compressed format.
     * A single-component, block-compressed format where each 64-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed normalized red texel data
     * */
    DataFormatBC4RSnorm = 81,
    /**
     * @brief BC5 compressed format.
     * A two-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized red and green texel data
     * */
    DataFormatBC5RGUnorm = 83,
    /**
     * @brief BC5 compressed format.
     * A two-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed normalized red and green texel data
     * */
    DataFormatBC5RGSnorm = 84,
    /**
     * @brief 16 bit per channel RGB format.
     * A three-component, 16-bit unsigned normalized format that has
     * a 5-bit B component in bits 0..4,
     * a 6-bit G component in bits 5..10,
     * a 5-bit R component in bits 11..15.
     * */
    DataFormatB5G6R5Unorm = 85,
    /**
     * @brief 16 bit per channel RGBA format.
     * A four-component, 16-bit unsigned normalized format that has
     * a 5-bit B component in bits 0..4,
     * a 5-bit G component in bits 5..9,
     * a 5-bit R component in bits 10..14,
     * a 1-bit A component in bits 15..15.
     * */
    DataFormatB5G5R5A1Unorm = 86,
    /**
     * @brief 8 bit per channel RGBA format.
     * A four-component, 32-bit unsigned normalized format that has
     * an 8-bit B component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit R component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    DataFormatBGRA8Unorm = 87,
    /**
     * @brief 8 bit per channel RGBA format. SRGB non-linear color space.
     * A four-component, 32-bit unsigned normalized non-linear SRGB format that has
     * an 8-bit B component in bytes 0..0,
     * an 8-bit G component in bytes 1..1,
     * an 8-bit R component in bytes 2..2,
     * an 8-bit A component in bytes 3..3.
     * */
    DataFormatBGRA8UnormSrgb = 91,
    /**
     * @brief BC6H compressed format.
     * A three-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned floating-point RGB texel data
     * */
    DataFormatBC6HUfloat16 = 95,
    /**
     * @brief BC6H compressed format.
     * A three-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of signed floating-point RGB texel data
     * */
    DataFormatBC6HSfloat16 = 96,
    /**
     * @brief BC7 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data
     * */
    DataFormatBC7RGBAUnorm = 98,
    /**
     * @brief BC7 compressed format.
     * A four-component, block-compressed format where each 128-bit
     * compressed texel block encodes a 4×4 rectangle
     * of unsigned normalized RGBA texel data with sRGB nonlinear encoding
     * */
    DataFormatBC7RGBAUnormSrgb = 99,
    /**
     * @brief 4 bit per channel RGB format.
     * A four-component, 16-bit unsigned normalized format that has
     * a 4-bit B component in bits 0..3,
     * a 4-bit G component in bits 4..7,
     * a 4-bit R component in bits 8..11,
     * a 4-bit A component in bits 12..15.
     * */
    DataFormatBGRA4Unorm = 115,
};

enum WisFillMode {
    FillModeLines = 2,
    FillModeSolid = 3,
};

enum WisDescriptorMemory {
    DescriptorMemoryCpuOnly = 0,
    DescriptorMemoryShaderVisible = 1,
};

enum WisWindingOrder {
    WindingOrderClockwise = 0,
    WindingOrderCounterClockwise = 1,
};

enum WisSampleRate {
    SampleRateS1 = 1,
    SampleRateS2 = 2,
    SampleRateS4 = 4,
    SampleRateS8 = 8,
    SampleRateS16 = 16,
};

enum WisCompare {
    CompareNone = 0,
    CompareNever = 1,
    CompareLess = 2,
    CompareEqual = 3,
    CompareLessEqual = 4,
    CompareGreater = 5,
    CompareNotEqual = 6,
    CompareGreaterEqual = 7,
    CompareAlways = 8,
};

enum WisStencilOp {
    StencilOpKeep = 1,
    StencilOpZero = 2,
    StencilOpReplace = 3,
    StencilOpIncClamp = 4,
    StencilOpDecClamp = 5,
    StencilOpInvert = 6,
    StencilOpIncWrap = 7,
    StencilOpDecWrap = 8,
};

enum WisBlendFactor {
    BlendFactorZero = 1,
    BlendFactorOne = 2,
    BlendFactorSrcColor = 3,
    BlendFactorInvSrcColor = 4,
    BlendFactorSrcAlpha = 5,
    BlendFactorInvSrcAlpha = 6,
    BlendFactorDestAlpha = 7,
    BlendFactorInvDestAlpha = 8,
    BlendFactorDestColor = 9,
    BlendFactorInvDestColor = 10,
    BlendFactorSrcAlphaSat = 11,
    BlendFactorBlendFactor = 14,
    BlendFactorInvBlendFactor = 15,
    BlendFactorSrc1Color = 16,
    BlendFactorInvSrc1Color = 17,
    BlendFactorSrc1Alpha = 18,
    BlendFactorInvSrc1Alpha = 19,
};

enum WisBlendOp {
    BlendOpAdd = 1,
    BlendOpSubtract = 2,
    BlendOpRevSubtract = 3,
    BlendOpMin = 4,
    BlendOpMax = 5,
};

enum WisSampleCount {
    SampleCountS1 = 1,
    SampleCountS2 = 2,
    SampleCountS4 = 4,
    SampleCountS8 = 8,
    SampleCountS16 = 16,
    SampleCountS32 = 32,
    SampleCountS64 = 64,
};

enum WisLogicOp {
    LogicOpClear = 0,
    LogicOpSet = 1,
    LogicOpCopy = 2,
    LogicOpCopyInverted = 3,
    LogicOpNoop = 4,
    LogicOpInvert = 5,
    LogicOpAnd = 6,
    LogicOpNand = 7,
    LogicOpOr = 8,
    LogicOpNor = 9,
    LogicOpXor = 10,
    LogicOpEquiv = 11,
    LogicOpAndReverse = 12,
    LogicOpAndInverted = 13,
    LogicOpOrReverse = 14,
    LogicOpOrInverted = 15,
};

enum WisMemoryType {
    MemoryTypeDefault = 0,
    MemoryTypeUpload = 1,
    MemoryTypeReadback = 2,
    MemoryTypeGPUUpload = 3,
};

enum WisShaderIntermediate {
    ShaderIntermediateDXIL = 0,
    ShaderIntermediateSPIRV = 1,
};

enum WisTextureState {
    TextureStateUndefined = 4294967295,
    TextureStateCommon = 0,
    TextureStateRead = 1,
    TextureStateRenderTarget = 2,
    TextureStateUnorderedAccess = 3,
    TextureStateDepthStencilWrite = 4,
    TextureStateDepthStencilRead = 5,
    TextureStateShaderResource = 6,
    TextureStateCopySource = 7,
    TextureStateCopyDest = 8,
    TextureStatePresent = 9,
    TextureStateShadingRate = 10,
    TextureStateVideoDecodeRead = 11,
    TextureStateVideoDecodeWrite = 12,
};

enum WisLoadOperation {
    LoadOperationLoad = 0,
    LoadOperationClear = 1,
    LoadOperationDontCare = 2,
};

enum WisTextureLayout {
    TextureLayoutTexture1D = 2,
    TextureLayoutTexture1DArray = 3,
    TextureLayoutTexture2D = 4,
    TextureLayoutTexture2DArray = 5,
    TextureLayoutTexture2DMS = 6,
    TextureLayoutTexture2DMSArray = 7,
    TextureLayoutTexture3D = 8,
};

enum WisDescriptorHeapType {
    DescriptorHeapTypeDescriptor = 0,
    DescriptorHeapTypeSampler = 1,
};

enum WisStoreOperation {
    StoreOperationStore = 0,
    StoreOperationDontCare = 1,
    StoreOperationResolve = 2,
};

enum WisPrimitiveTopology {
    PrimitiveTopologyPointList = 1,
    PrimitiveTopologyLineList = 2,
    PrimitiveTopologyLineStrip = 3,
    PrimitiveTopologyTriangleList = 4,
    PrimitiveTopologyTriangleStrip = 5,
    PrimitiveTopologyTriangleFan = 6,
    PrimitiveTopologyLineListAdj = 10,
    PrimitiveTopologyLineStripAdj = 11,
    PrimitiveTopologyTriangleListAdj = 12,
    PrimitiveTopologyTriangleStripAdj = 13,
};

enum WisTopologyType {
    TopologyTypePoint = 1,
    TopologyTypeLine = 2,
    TopologyTypeTriangle = 3,
    TopologyTypePatch = 4,
};

enum WisFilter {
    FilterPoint = 0,
    FilterLinear = 1,
};

enum WisAddressMode {
    AddressModeRepeat = 0,
    AddressModeMirroredRepeat = 1,
    AddressModeClampToEdge = 2,
    AddressModeClampToBorder = 3,
    AddressModeMirrorClampToEdge = 4,
};

enum WisTextureViewType {
    TextureViewTypeTexture1D = 0,
    TextureViewTypeTexture1DArray = 1,
    TextureViewTypeTexture2D = 2,
    TextureViewTypeTexture2DArray = 3,
    TextureViewTypeTexture2DMS = 4,
    TextureViewTypeTexture2DMSArray = 5,
    TextureViewTypeTexture3D = 6,
    TextureViewTypeTextureCube = 7,
    TextureViewTypeTextureCubeArray = 8,
};

enum WisComponentSwizzle {
    ComponentSwizzleRed = 0,
    ComponentSwizzleGreen = 1,
    ComponentSwizzleBlue = 2,
    ComponentSwizzleAlpha = 3,
    ComponentSwizzleZero = 4,
    ComponentSwizzleOne = 5,
};

//-------------------------------------------------------------------------

enum WisAdapterFlagsBits {
    AdapterFlagsNone = 0x0,
    AdapterFlagsRemote = 1 << 0,
    AdapterFlagsSoftware = 1 << 1,
    AdapterFlagsDX12ACGCompatible = 1 << 2,
    AdapterFlagsDX12SupportsMonitoredFences = 1 << 3,
    AdapterFlagsDX12SupportsNonMonitoredFences = 1 << 4,
    AdapterFlagsDX12KeyedMutexConformance = 1 << 5,
};

enum WisDSSelectBits {
    DSSelectNone = 0x0,
    DSSelectDepth = 1 << 0,
    DSSelectStencil = 1 << 1,
    DSSelectDepthStencil = 0x3,
};

enum WisColorComponentsBits {
    ColorComponentsNone = 0x0,
    ColorComponentsR = 1 << 0,
    ColorComponentsG = 1 << 1,
    ColorComponentsB = 1 << 2,
    ColorComponentsA = 1 << 3,
    ColorComponentsAll = 0xF,
};

enum WisBufferUsageBits {
    BufferUsageNone = 0x0,
    BufferUsageCopySrc = 1 << 0,
    BufferUsageCopyDst = 1 << 1,
    BufferUsageConstantBuffer = 1 << 4,
    BufferUsageIndexBuffer = 1 << 6,
    BufferUsageVertexBuffer = 1 << 7,
};

enum WisMemoryFlagsBits {
    MemoryFlagsNone = 0x0,
    MemoryFlagsDedicatedAllocation = 1 << 0,
    MemoryFlagsMapped = 1 << 1,
};

enum WisRenderPassFlagsBits {
    RenderPassFlagsNone = 0x0,
    RenderPassFlagsSuspending = 1 << 1,
    RenderPassFlagsResuming = 1 << 2,
};

enum WisBarrierSyncBits {
    BarrierSyncNone = 0x0,
    BarrierSyncAll = 1 << 0,
    BarrierSyncDraw = 1 << 1,
    BarrierSyncIndexInput = 1 << 2,
    BarrierSyncVertexShading = 1 << 3,
    BarrierSyncPixelShading = 1 << 4,
    BarrierSyncDepthStencil = 1 << 5,
    BarrierSyncRenderTarget = 1 << 6,
    BarrierSyncCompute = 1 << 7,
    BarrierSyncRaytracing = 1 << 8,
    BarrierSyncCopy = 1 << 9,
    BarrierSyncResolve = 1 << 10,
    BarrierSyncExecuteIndirect = 1 << 11,
    BarrierSyncAllShading = 1 << 12,
    BarrierSyncNonPixelShading = 1 << 13,
    BarrierSyncClearUAV = 1 << 14,
    BarrierSyncVideoDecode = 1 << 15,
    BarrierSyncVideoEncode = 1 << 16,
    BarrierSyncBuildRTAS = 1 << 17,
    BarrierSyncCopyRTAS = 1 << 18,
};

enum WisResourceAccessBits {
    ResourceAccessCommon = 0x0,
    ResourceAccessVertexBuffer = 1 << 0,
    ResourceAccessConstantBuffer = 1 << 1,
    ResourceAccessIndexBuffer = 1 << 2,
    ResourceAccessRenderTarget = 1 << 3,
    ResourceAccessUnorderedAccess = 1 << 4,
    ResourceAccessDepthWrite = 1 << 5,
    ResourceAccessDepthRead = 1 << 6,
    ResourceAccessShaderResource = 1 << 7,
    ResourceAccessStreamOutput = 1 << 8,
    ResourceAccessIndirectArgument = 1 << 9,
    ResourceAccessCopyDest = 1 << 10,
    ResourceAccessCopySource = 1 << 11,
    ResourceAccessConditionalRendering = 1 << 12,
    ResourceAccessAccelerationStrucureRead = 1 << 13,
    ResourceAccessAccelerationStrucureWrite = 1 << 14,
    ResourceAccessShadingRate = 1 << 15,
    ResourceAccessVideoDecodeRead = 1 << 16,
    ResourceAccessVideoDecodeWrite = 1 << 17,
    ResourceAccessPresent = 1 << 18,
    ResourceAccessResolveDest = 1 << 19,
    ResourceAccessResolveSource = 1 << 20,
    ResourceAccessNoAccess = 1 << 31,
};

enum WisTextureUsageBits {
    TextureUsageNone = 0x0,
    TextureUsageRenderTarget = 1 << 0,
    TextureUsageDepthStencil = 1 << 1,
    TextureUsageCopySrc = 1 << 2,
    TextureUsageCopyDst = 1 << 3,
    TextureUsageShaderResource = 1 << 4,
    TextureUsageUnorderedAccess = 1 << 5,
    TextureUsageHostCopy = 1 << 7,
};

//-------------------------------------------------------------------------

typedef struct WisResult WisResult;
typedef struct WisSize2D WisSize2D;
typedef struct WisSize3D WisSize3D;
typedef struct WisBufferRegion WisBufferRegion;
typedef struct WisAdapterDesc WisAdapterDesc;
typedef struct WisInputSlotDesc WisInputSlotDesc;
typedef struct WisInputAttribute WisInputAttribute;
typedef struct WisInputLayout WisInputLayout;
typedef struct WisRasterizerDesc WisRasterizerDesc;
typedef struct WisSampleDesc WisSampleDesc;
typedef struct WisStencilDesc WisStencilDesc;
typedef struct WisDepthStencilDesc WisDepthStencilDesc;
typedef struct WisBlendAttachmentDesc WisBlendAttachmentDesc;
typedef struct WisBlendStateDesc WisBlendStateDesc;
typedef struct WisRenderAttachmentsDesc WisRenderAttachmentsDesc;
typedef struct WisRootConstant WisRootConstant;
typedef struct WisSwapchainDesc WisSwapchainDesc;
typedef struct WisTextureDesc WisTextureDesc;
typedef struct WisAllocationInfo WisAllocationInfo;
typedef struct WisTextureRegion WisTextureRegion;
typedef struct WisBufferTextureCopyRegion WisBufferTextureCopyRegion;
typedef struct WisPushDescriptor WisPushDescriptor;
typedef struct WisSubresourceRange WisSubresourceRange;
typedef struct WisRenderTargetDesc WisRenderTargetDesc;
typedef struct WisViewport WisViewport;
typedef struct WisScissor WisScissor;
typedef struct WisBufferBarrier WisBufferBarrier;
typedef struct WisTextureBarrier WisTextureBarrier;
typedef struct WisDescriptorTableEntry WisDescriptorTableEntry;
typedef struct WisDescriptorTable WisDescriptorTable;
typedef struct WisSamplerDesc WisSamplerDesc;
typedef struct WisComponentMapping WisComponentMapping;
typedef struct WisShaderResourceDesc WisShaderResourceDesc;
typedef enum WisShaderStages WisShaderStages;
typedef enum WisStatus WisStatus;
typedef enum WisMutiWaitFlags WisMutiWaitFlags;
typedef enum WisDescriptorType WisDescriptorType;
typedef enum WisQueueType WisQueueType;
typedef enum WisAdapterPreference WisAdapterPreference;
typedef enum WisSeverity WisSeverity;
typedef enum WisInputClass WisInputClass;
typedef enum WisCullMode WisCullMode;
typedef enum WisDataFormat WisDataFormat;
typedef enum WisFillMode WisFillMode;
typedef enum WisDescriptorMemory WisDescriptorMemory;
typedef enum WisWindingOrder WisWindingOrder;
typedef enum WisSampleRate WisSampleRate;
typedef enum WisCompare WisCompare;
typedef enum WisStencilOp WisStencilOp;
typedef enum WisBlendFactor WisBlendFactor;
typedef enum WisBlendOp WisBlendOp;
typedef enum WisSampleCount WisSampleCount;
typedef enum WisLogicOp WisLogicOp;
typedef enum WisMemoryType WisMemoryType;
typedef enum WisShaderIntermediate WisShaderIntermediate;
typedef enum WisTextureState WisTextureState;
typedef enum WisLoadOperation WisLoadOperation;
typedef enum WisTextureLayout WisTextureLayout;
typedef enum WisDescriptorHeapType WisDescriptorHeapType;
typedef enum WisStoreOperation WisStoreOperation;
typedef enum WisPrimitiveTopology WisPrimitiveTopology;
typedef enum WisTopologyType WisTopologyType;
typedef enum WisFilter WisFilter;
typedef enum WisAddressMode WisAddressMode;
typedef enum WisTextureViewType WisTextureViewType;
typedef enum WisComponentSwizzle WisComponentSwizzle;
typedef enum WisAdapterFlagsBits WisAdapterFlagsBits;
typedef uint32_t WisAdapterFlags;
typedef enum WisDSSelectBits WisDSSelectBits;
typedef uint32_t WisDSSelect;
typedef enum WisColorComponentsBits WisColorComponentsBits;
typedef uint32_t WisColorComponents;
typedef enum WisBufferUsageBits WisBufferUsageBits;
typedef uint32_t WisBufferUsage;
typedef enum WisMemoryFlagsBits WisMemoryFlagsBits;
typedef uint32_t WisMemoryFlags;
typedef enum WisRenderPassFlagsBits WisRenderPassFlagsBits;
typedef uint32_t WisRenderPassFlags;
typedef enum WisBarrierSyncBits WisBarrierSyncBits;
typedef uint32_t WisBarrierSync;
typedef enum WisResourceAccessBits WisResourceAccessBits;
typedef uint32_t WisResourceAccess;
typedef enum WisTextureUsageBits WisTextureUsageBits;
typedef uint32_t WisTextureUsage;

//-------------------------------------------------------------------------

struct WisResult {
    WisStatus status;
    const char* error;
};

struct WisSize2D {
    uint32_t width;
    uint32_t height;
};

struct WisSize3D {
    uint32_t width;
    uint32_t height;
    uint32_t depth_or_layers;
};

struct WisBufferRegion {
    uint64_t src_offset;
    uint64_t dst_offset;
    uint64_t size_bytes;
};

struct WisAdapterDesc {
    const char description[256];
    uint32_t vendor_id;
    uint32_t device_id;
    uint32_t subsys_id;
    uint32_t revision;
    uint64_t dedicated_video_memory;
    uint64_t dedicated_system_memory;
    uint64_t shared_system_memory;
    uint64_t adapter_id;
    WisAdapterFlags flags;
};

struct WisInputSlotDesc {
    uint32_t slot;
    uint32_t stride_bytes;
    WisInputClass input_class;
};

struct WisInputAttribute {
    uint32_t input_slot;
    const char* semantic_name;
    uint32_t semantic_index;
    uint32_t location;
    WisDataFormat format;
    uint32_t offset_bytes;
};

struct WisInputLayout {
    WisInputSlotDesc* slots;
    uint32_t slot_count;
    WisInputAttribute* attributes;
    uint32_t attribute_count;
};

struct WisRasterizerDesc {
    WisFillMode fill_mode;
    WisCullMode cull_mode;
    WisWindingOrder front_face;
    bool depth_bias_enable;
    float depth_bias;
    float depth_bias_clamp;
    float depth_bias_slope_factor;
    bool depth_clip_enable;
};

struct WisSampleDesc {
    WisSampleRate rate;
    float quality;
    uint32_t sample_mask;
};

struct WisStencilDesc {
    WisStencilOp fail_op;
    WisStencilOp depth_fail_op;
    WisStencilOp pass_op;
    WisCompare comparison;
    uint8_t read_mask;
    uint8_t write_mask;
};

struct WisDepthStencilDesc {
    bool depth_enable;
    bool depth_write_enable;
    WisCompare depth_comp;
    bool stencil_enable;
    WisStencilDesc stencil_front;
    WisStencilDesc stencil_back;
    bool depth_bound_test;
};

struct WisBlendAttachmentDesc {
    bool blend_enable;
    WisBlendFactor src_color_blend;
    WisBlendFactor dst_color_blend;
    WisBlendOp color_blend_op;
    WisBlendFactor src_alpha_blend;
    WisBlendFactor dst_alpha_blend;
    WisBlendOp alpha_blend_op;
    WisColorComponents color_write_mask;
};

struct WisBlendStateDesc {
    bool logic_op_enable;
    WisLogicOp logic_op;
    WisBlendAttachmentDesc attachments[8];
    uint32_t attachment_count;
};

struct WisRenderAttachmentsDesc {
    WisDataFormat* attachment_formats;
    uint32_t attachments_count;
    WisDataFormat depth_attachment;
};

struct WisRootConstant {
    WisShaderStages stage;
    uint32_t size_bytes;
};

struct WisSwapchainDesc {
    WisSize2D size;
    WisDataFormat format;
    uint32_t buffer_count;
    bool stereo;
    bool vsync;
};

struct WisTextureDesc {
    WisDataFormat format;
    WisSize3D size;
    uint32_t mip_levels;
    WisTextureLayout layout;
    WisSampleCount sample_count;
    WisTextureUsage usage;
};

struct WisAllocationInfo {
    uint64_t size_bytes;
    uint64_t alignment_bytes;
};

struct WisTextureRegion {
    WisSize3D offset;
    WisSize3D size;
    uint32_t mip;
    uint32_t array_layer;
    WisDataFormat format;
};

struct WisBufferTextureCopyRegion {
    uint64_t buffer_offset;
    WisTextureRegion texture;
};

struct WisPushDescriptor {
    WisShaderStages stage;
    uint32_t bind_register;
    WisDescriptorType type;
    uint32_t reserved;
};

struct WisSubresourceRange {
    uint32_t base_mip_level;
    uint32_t level_count;
    uint32_t base_array_layer;
    uint32_t layer_count;
};

struct WisRenderTargetDesc {
    WisDataFormat format;
    WisTextureLayout layout;
    uint32_t mip;
    uint32_t base_array_layer;
    uint32_t layer_count;
};

struct WisViewport {
    float top_leftx;
    float top_lefty;
    float width;
    float height;
    float min_depth;
    float max_depth;
};

struct WisScissor {
    int32_t left;
    int32_t top;
    int32_t right;
    int32_t bottom;
};

struct WisBufferBarrier {
    WisBarrierSync sync_before;
    WisBarrierSync sync_after;
    WisResourceAccess access_before;
    WisResourceAccess access_after;
    uint64_t offset;
    uint64_t size;
};

struct WisTextureBarrier {
    WisBarrierSync sync_before;
    WisBarrierSync sync_after;
    WisResourceAccess access_before;
    WisResourceAccess access_after;
    WisTextureState state_before;
    WisTextureState state_after;
    WisSubresourceRange subresource_range;
};

struct WisDescriptorTableEntry {
    WisDescriptorType type;
    uint32_t bind_register;
    uint32_t binding;
    uint32_t count;
};

struct WisDescriptorTable {
    WisDescriptorHeapType type;
    WisDescriptorTableEntry* entries;
    uint32_t entry_count;
    WisShaderStages stage;
};

struct WisSamplerDesc {
    WisFilter min_filter;
    WisFilter mag_filter;
    WisFilter mip_filter;
    bool anisotropic;
    uint32_t max_anisotropy;
    WisAddressMode address_u;
    WisAddressMode address_v;
    WisAddressMode address_w;
    float min_lod;
    float max_lod;
    float mip_lod_bias;
    WisCompare comparison_op;
    float border_color[4];
};

struct WisComponentMapping {
    WisComponentSwizzle r;
    WisComponentSwizzle g;
    WisComponentSwizzle b;
    WisComponentSwizzle a;
};

struct WisShaderResourceDesc {
    WisDataFormat format;
    WisTextureViewType view_type;
    WisComponentMapping component_mapping;
    WisSubresourceRange subresource_range;
};

//-------------------------------------------------------------------------

//-------------------------------------------------------------------------

//=================================DELEGATES=================================

typedef void (*DebugCallback)(WisSeverity severity, const char* message, void* user_data);

//-------------------------------------------------------------------------

#ifdef WISDOM_VULKAN
typedef struct VKCommandListView VKCommandListView;
typedef struct VKGraphicsPipelineDesc VKGraphicsPipelineDesc;
typedef struct VKFenceView VKFenceView;
typedef struct VKRenderPassDepthStencilDesc VKRenderPassDepthStencilDesc;
typedef struct VKBufferView VKBufferView;
typedef struct VKTextureView VKTextureView;
typedef struct VKShaderView VKShaderView;
typedef struct VKRenderTargetView VKRenderTargetView;
typedef struct VKRootSignatureView VKRootSignatureView;
typedef struct VKDescriptorBufferView VKDescriptorBufferView;
typedef struct VKBufferBarrier2 VKBufferBarrier2;
typedef struct VKTextureBarrier2 VKTextureBarrier2;
typedef struct VKGraphicsShaderStages VKGraphicsShaderStages;
typedef struct VKRenderPassRenderTargetDesc VKRenderPassRenderTargetDesc;
typedef struct VKRenderPassDesc VKRenderPassDesc;
typedef struct VKVertexBufferBinding VKVertexBufferBinding;

//-------------------------------------------------------------------------

struct VKFenceView {
    void* value;
};

struct VKBufferView {
    void* value;
};

struct VKTextureView {
    void* value;
    uint32_t value2;
    WisSize2D value3;
};

struct VKRenderTargetView {
    void* value;
};

struct VKCommandListView {
    void* value;
};

struct VKShaderView {
    void* value;
};

struct VKRootSignatureView {
    void* value;
};

struct VKDescriptorBufferView {
    uint64_t value1;
    uint32_t value2;
};

struct VKBufferBarrier2 {
    WisBufferBarrier barrier;
    VKBufferView buffer;
};

struct VKTextureBarrier2 {
    WisTextureBarrier barrier;
    VKTextureView texture;
};

struct VKGraphicsShaderStages {
    VKShaderView vertex;
    VKShaderView hull;
    VKShaderView domain;
    VKShaderView geometry;
    VKShaderView pixel;
};

struct VKGraphicsPipelineDesc {
    VKRootSignatureView root_signature;
    WisInputLayout input_layout;
    VKGraphicsShaderStages shaders;
    WisRenderAttachmentsDesc attachments;
    WisRasterizerDesc* rasterizer;
    WisSampleDesc* sample;
    WisBlendStateDesc* blend;
    WisDepthStencilDesc* depth_stencil;
    WisTopologyType topology_type;
};

struct VKRenderPassRenderTargetDesc {
    VKRenderTargetView target;
    WisLoadOperation load_op;
    WisStoreOperation store_op;
    float clear_value[4];
};

struct VKRenderPassDepthStencilDesc {
    VKRenderTargetView target;
    WisLoadOperation load_op_depth;
    WisLoadOperation load_op_stencil;
    WisStoreOperation store_op_depth;
    WisStoreOperation store_op_stencil;
    WisDSSelect depth_stencil_select;
    float clear_depth;
    uint8_t clear_stencil;
};

struct VKRenderPassDesc {
    WisRenderPassFlags flags;
    uint32_t target_count;
    VKRenderPassRenderTargetDesc* targets;
    VKRenderPassDepthStencilDesc* depth_stencil;
};

struct VKVertexBufferBinding {
    VKBufferView buffer;
    uint32_t size;
    uint32_t stride;
};

//-------------------------------------------------------------------------

typedef struct VKCommandQueue_t* VKCommandQueue;
typedef struct VKRootSignature_t* VKRootSignature;
typedef struct VKFactory_t* VKFactory;
typedef struct VKDeviceExtension_t* VKDeviceExtension;
typedef struct VKPipelineState_t* VKPipelineState;
typedef struct VKAdapter_t* VKAdapter;
typedef struct VKDevice_t* VKDevice;
typedef struct VKFence_t* VKFence;
typedef struct VKFactoryExtension_t* VKFactoryExtension;
typedef struct VKResourceAllocator_t* VKResourceAllocator;
typedef struct VKShader_t* VKShader;
typedef struct VKCommandList_t* VKCommandList;
typedef struct VKSwapChain_t* VKSwapChain;
typedef struct VKBuffer_t* VKBuffer;
typedef struct VKTexture_t* VKTexture;
typedef struct VKDebugMessenger_t* VKDebugMessenger;
typedef struct VKRenderTarget_t* VKRenderTarget;
typedef struct VKDescriptorBuffer_t* VKDescriptorBuffer;
typedef struct VKSampler_t* VKSampler;
typedef struct VKMemory_t* VKMemory;
typedef struct VKShaderResource_t* VKShaderResource;

//-------------------------------------------------------------------------

WISDOM_API void VKFactoryDestroy(VKFactory self);
WISDOM_API void VKAdapterDestroy(VKAdapter self);
WISDOM_API void VKDeviceDestroy(VKDevice self);
WISDOM_API void VKFenceDestroy(VKFence self);
WISDOM_API void VKResourceAllocatorDestroy(VKResourceAllocator self);
WISDOM_API void VKRootSignatureDestroy(VKRootSignature self);
WISDOM_API void VKCommandQueueDestroy(VKCommandQueue self);
WISDOM_API void VKShaderDestroy(VKShader self);
WISDOM_API void VKPipelineStateDestroy(VKPipelineState self);
WISDOM_API void VKCommandListDestroy(VKCommandList self);
WISDOM_API void VKBufferDestroy(VKBuffer self);
WISDOM_API void VKTextureDestroy(VKTexture self);
WISDOM_API void VKSwapChainDestroy(VKSwapChain self);
WISDOM_API void VKDebugMessengerDestroy(VKDebugMessenger self);
WISDOM_API void VKRenderTargetDestroy(VKRenderTarget self);
WISDOM_API void VKDescriptorBufferDestroy(VKDescriptorBuffer self);
WISDOM_API void VKSamplerDestroy(VKSampler self);
WISDOM_API void VKMemoryDestroy(VKMemory self);
WISDOM_API void VKShaderResourceDestroy(VKShaderResource self);
WISDOM_API WisResult VKCreateFactory(bool debug_layer, VKFactory* out_factory);
WISDOM_API WisResult VKCreateDevice(VKAdapter adapter, VKDevice* out_device);

//-------------------------------------------------------------------------

WISDOM_API VKFenceView AsVKFenceView(VKFence self);
WISDOM_API VKBufferView AsVKBufferView(VKBuffer self);
WISDOM_API VKTextureView AsVKTextureView(VKTexture self);
WISDOM_API VKRenderTargetView AsVKRenderTargetView(VKRenderTarget self);
WISDOM_API VKCommandListView AsVKCommandListView(VKCommandList self);
WISDOM_API VKShaderView AsVKShaderView(VKShader self);
WISDOM_API VKRootSignatureView AsVKRootSignatureView(VKRootSignature self);
WISDOM_API VKDescriptorBufferView AsVKDescriptorBufferView(VKDescriptorBuffer self);
#endif

#ifdef WISDOM_DX12
typedef struct DX12CommandListView DX12CommandListView;
typedef struct DX12GraphicsPipelineDesc DX12GraphicsPipelineDesc;
typedef struct DX12FenceView DX12FenceView;
typedef struct DX12RenderPassDepthStencilDesc DX12RenderPassDepthStencilDesc;
typedef struct DX12BufferView DX12BufferView;
typedef struct DX12TextureView DX12TextureView;
typedef struct DX12ShaderView DX12ShaderView;
typedef struct DX12RenderTargetView DX12RenderTargetView;
typedef struct DX12RootSignatureView DX12RootSignatureView;
typedef struct DX12DescriptorBufferView DX12DescriptorBufferView;
typedef struct DX12BufferBarrier2 DX12BufferBarrier2;
typedef struct DX12TextureBarrier2 DX12TextureBarrier2;
typedef struct DX12GraphicsShaderStages DX12GraphicsShaderStages;
typedef struct DX12RenderPassRenderTargetDesc DX12RenderPassRenderTargetDesc;
typedef struct DX12RenderPassDesc DX12RenderPassDesc;
typedef struct DX12VertexBufferBinding DX12VertexBufferBinding;

//-------------------------------------------------------------------------

struct DX12FenceView {
    void* value;
};

struct DX12BufferView {
    void* value;
};

struct DX12TextureView {
    void* value;
};

struct DX12RenderTargetView {
    void* value;
};

struct DX12CommandListView {
    void* value;
};

struct DX12ShaderView {
    void* bytecode;
    uint32_t size_bytes;
};

struct DX12RootSignatureView {
    void* value;
};

struct DX12DescriptorBufferView {
    void* value;
};

struct DX12BufferBarrier2 {
    WisBufferBarrier barrier;
    DX12BufferView buffer;
};

struct DX12TextureBarrier2 {
    WisTextureBarrier barrier;
    DX12TextureView texture;
};

struct DX12GraphicsShaderStages {
    DX12ShaderView vertex;
    DX12ShaderView hull;
    DX12ShaderView domain;
    DX12ShaderView geometry;
    DX12ShaderView pixel;
};

struct DX12GraphicsPipelineDesc {
    DX12RootSignatureView root_signature;
    WisInputLayout input_layout;
    DX12GraphicsShaderStages shaders;
    WisRenderAttachmentsDesc attachments;
    WisRasterizerDesc* rasterizer;
    WisSampleDesc* sample;
    WisBlendStateDesc* blend;
    WisDepthStencilDesc* depth_stencil;
    WisTopologyType topology_type;
};

struct DX12RenderPassRenderTargetDesc {
    DX12RenderTargetView target;
    WisLoadOperation load_op;
    WisStoreOperation store_op;
    float clear_value[4];
};

struct DX12RenderPassDepthStencilDesc {
    DX12RenderTargetView target;
    WisLoadOperation load_op_depth;
    WisLoadOperation load_op_stencil;
    WisStoreOperation store_op_depth;
    WisStoreOperation store_op_stencil;
    WisDSSelect depth_stencil_select;
    float clear_depth;
    uint8_t clear_stencil;
};

struct DX12RenderPassDesc {
    WisRenderPassFlags flags;
    uint32_t target_count;
    DX12RenderPassRenderTargetDesc* targets;
    DX12RenderPassDepthStencilDesc* depth_stencil;
};

struct DX12VertexBufferBinding {
    DX12BufferView buffer;
    uint32_t size;
    uint32_t stride;
};

//-------------------------------------------------------------------------

typedef struct DX12CommandQueue_t* DX12CommandQueue;
typedef struct DX12RootSignature_t* DX12RootSignature;
typedef struct DX12Factory_t* DX12Factory;
typedef struct DX12DeviceExtension_t* DX12DeviceExtension;
typedef struct DX12PipelineState_t* DX12PipelineState;
typedef struct DX12Adapter_t* DX12Adapter;
typedef struct DX12Device_t* DX12Device;
typedef struct DX12Fence_t* DX12Fence;
typedef struct DX12FactoryExtension_t* DX12FactoryExtension;
typedef struct DX12ResourceAllocator_t* DX12ResourceAllocator;
typedef struct DX12Shader_t* DX12Shader;
typedef struct DX12CommandList_t* DX12CommandList;
typedef struct DX12SwapChain_t* DX12SwapChain;
typedef struct DX12Buffer_t* DX12Buffer;
typedef struct DX12Texture_t* DX12Texture;
typedef struct DX12DebugMessenger_t* DX12DebugMessenger;
typedef struct DX12RenderTarget_t* DX12RenderTarget;
typedef struct DX12DescriptorBuffer_t* DX12DescriptorBuffer;
typedef struct DX12Sampler_t* DX12Sampler;
typedef struct DX12Memory_t* DX12Memory;
typedef struct DX12ShaderResource_t* DX12ShaderResource;

//-------------------------------------------------------------------------

WISDOM_API void DX12FactoryDestroy(DX12Factory self);
WISDOM_API void DX12AdapterDestroy(DX12Adapter self);
WISDOM_API void DX12DeviceDestroy(DX12Device self);
WISDOM_API void DX12FenceDestroy(DX12Fence self);
WISDOM_API void DX12ResourceAllocatorDestroy(DX12ResourceAllocator self);
WISDOM_API void DX12RootSignatureDestroy(DX12RootSignature self);
WISDOM_API void DX12CommandQueueDestroy(DX12CommandQueue self);
WISDOM_API void DX12ShaderDestroy(DX12Shader self);
WISDOM_API void DX12PipelineStateDestroy(DX12PipelineState self);
WISDOM_API void DX12CommandListDestroy(DX12CommandList self);
WISDOM_API void DX12BufferDestroy(DX12Buffer self);
WISDOM_API void DX12TextureDestroy(DX12Texture self);
WISDOM_API void DX12SwapChainDestroy(DX12SwapChain self);
WISDOM_API void DX12DebugMessengerDestroy(DX12DebugMessenger self);
WISDOM_API void DX12RenderTargetDestroy(DX12RenderTarget self);
WISDOM_API void DX12DescriptorBufferDestroy(DX12DescriptorBuffer self);
WISDOM_API void DX12SamplerDestroy(DX12Sampler self);
WISDOM_API void DX12MemoryDestroy(DX12Memory self);
WISDOM_API void DX12ShaderResourceDestroy(DX12ShaderResource self);
WISDOM_API WisResult DX12CreateFactory(bool debug_layer, DX12Factory* out_factory);
WISDOM_API WisResult DX12CreateDevice(DX12Adapter adapter, DX12Device* out_device);

//-------------------------------------------------------------------------

WISDOM_API DX12FenceView AsDX12FenceView(DX12Fence self);
WISDOM_API DX12BufferView AsDX12BufferView(DX12Buffer self);
WISDOM_API DX12TextureView AsDX12TextureView(DX12Texture self);
WISDOM_API DX12RenderTargetView AsDX12RenderTargetView(DX12RenderTarget self);
WISDOM_API DX12CommandListView AsDX12CommandListView(DX12CommandList self);
WISDOM_API DX12ShaderView AsDX12ShaderView(DX12Shader self);
WISDOM_API DX12RootSignatureView AsDX12RootSignatureView(DX12RootSignature self);
WISDOM_API DX12DescriptorBufferView AsDX12DescriptorBufferView(DX12DescriptorBuffer self);
#endif

#ifdef __cplusplus
}
#endif

//-------------------------------------------------------------------------
