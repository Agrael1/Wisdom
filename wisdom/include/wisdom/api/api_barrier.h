#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_common.h>
#endif

WIS_EXPORT namespace wis
{
    /// @brief Texture state layout
    enum class TextureState : uint32_t {
        Undefined = 0xffffffff, // VK_IMAGE_LAYOUT_UNDEFINED
        Common = 0, // VK_IMAGE_LAYOUT_GENERAL
        Read = 1, // VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        RenderTarget = 2, // VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
        DepthWrite = 4, // VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
        DepthRead = 5, // VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL
        ShaderResource = 6, // VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
        CopySrc = 7, // VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL
        CopyDst = 8, // VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
        VRSSource = 11, // VK_IMAGE_LAYOUT_FRAGMENT_SHADING_RATE_ATTACHMENT_OPTIMAL_KHR
        VideoDecodeRead = 12, // VK_IMAGE_LAYOUT_VIDEO_DECODE_SRC_KHR
        VideoDecodeWrite = 13, // VK_IMAGE_LAYOUT_VIDEO_DECODE_DST_KHR
        VideoEncodeRead = 16, // VK_IMAGE_LAYOUT_VIDEO_ENCODE_SRC_KHR
        VideoEncodeWrite = 17, // VK_IMAGE_LAYOUT_VIDEO_ENCODE_DST_KHR
        Present = 100, // VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
    };

    /// @brief Resource access layout
    enum class ResourceAccess : uint32_t {
        Common = 0, // VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT
        VertexBuffer = 0x1, // VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT
        ConstantBuffer = 0x2, // VK_ACCESS_UNIFORM_READ_BIT
        IndexBuffer = 0x4, // VK_ACCESS_INDEX_READ_BIT
        RenderTarget = 0x8, // VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT
        UnorderedAccess = 0x10, // VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT
        DepthWrite = 0x20, // VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT
        DepthRead = 0x40, // VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT
        ShaderResource = 0x80, // VK_ACCESS_SHADER_READ_BIT
        TransformFeedback = 0x100, // VK_ACCESS_TRANSFORM_FEEDBACK_WRITE_BIT_EXT
        IndirectCommandRead = 0x200, // VK_ACCESS_INDIRECT_COMMAND_READ_BIT
        CopySource = 0x400, // VK_ACCESS_TRANSFER_READ_BIT
        CopyDest = 0x800, // VK_ACCESS_TRANSFER_WRITE_BIT
        RayTracingRead = 0x4000, // VK_ACCESS_ACCELERATION_STRUCTURE_READ_BIT_KHR
        RayTracingWrite = 0x8000, // VK_ACCESS_ACCELERATION_STRUCTURE_WRITE_BIT_KHR
        VRSSource = 0x10000, // VK_ACCESS_FRAGMENT_SHADING_RATE_ATTACHMENT_READ_BIT_KHR
        NoAccess = 0x80000000 // VK_ACCESS_NONE
    };

    /// @brief Buffer usage flags, TODO: add more
    enum class BufferFlags : uint32_t {
        None = 0,
        ConstantBuffer = 0x10, // VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
        IndexBuffer = 0x40, // VK_BUFFER_USAGE_INDEX_BUFFER_BIT
        VertexBuffer = 0x80, // VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
    };

    /// @brief Basic texture barrier w/o synchronization
    struct TextureBarrier {
        TextureState state_before; //< State before the barrier
        TextureState state_after; //< State after the barrier
        ResourceAccess access_before; //< Access before the barrier
        ResourceAccess access_after; //< Access after the barrier
        SubresourceRange range{}; //< Subresource range to apply the barrier to
    };

    /// @brief Basic buffer barrier w/o synchronization
    struct BufferBarrier {
        ResourceAccess access_before; //< Access before the barrier
        ResourceAccess access_after; //< Access after the barrier
    };
}
