#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
typedef enum Result {
    Result_Success = 0,
    Result_Failure = 1,
} Result;

typedef struct wisAllocator {
    void* user_data;
    void* (*allocate)(void* user_data, uint32_t size, uint32_t alignment);
    void (*free)(void* user_data, void* ptr);
} wisAllocator;

/// @brief Adapter flags used to describe the adapter properties
typedef enum wisAdapterFlags {
    AdapterFlags_None = 0, //< Adapter is a hardware adapter
    AdapterFlags_Remote = 1, //< Adapter is a remote adapter
    AdapterFlags_Software = 2, //< Adapter is a software adapter
    AdapterFlags_ACGCompatible = 4, //< Adapter is Active Code Guard compatible (DX 12.1 only)
} wisAdapterFlags;

/// @brief Adapter description, contains information about the adapter capabilities
typedef struct wisAdapterDesc {
    char description[256]; //< Adapter description
    uint32_t vendor_id; //< Adapter vendor id
    uint32_t device_id; //< Adapter device id
    uint32_t subsys_id; //< Adapter subsystem id (DX12)/ api version (Vulkan)
    uint32_t revision; //< Adapter revision (DX12)/ driver version (Vulkan)

    uint64_t dedicated_video_memory; //< Dedicated video memory
    uint64_t dedicated_system_memory; //< Dedicated system memory (DX12 only)
    uint64_t shared_system_memory; //< Shared system memory
    uint64_t adapter_id; //< Adapter id(LUID)

    wisAdapterFlags flags; //< Adapter flags describing the adapter properties
} wisAdapterDesc;

/// @brief Enumeration specifying Adapter query ordering
typedef enum wisAdapterPreference {
    AdapterPreference_None, //< Default order (defined by system)
    AdapterPreference_MinConsumption, //< Order:Integrated, Discrete
    AdapterPreference_Performance //< Order: Discrete, Integrated
} wisAdapterPreference;

/// @brief Version info, the same as in Vulkan
typedef struct wisVersion {
    uint32_t major;
    uint32_t minor;
    uint32_t patch;
} wisVersion;

/// @brief Application info, used only for debug purposes in Vulkan
typedef struct wisApplicationInfo {
    const char* application_name;
    const char* engine_name;
    wisVersion app_version;
    wisVersion engine_version;
} wisApplicationInfo;

typedef enum wisShaderType {
    ShaderType_Unknown,
    ShaderType_Vertex,
    ShaderType_Pixel,
    ShaderType_Geometry,
    ShaderType_Hull,
    ShaderType_Domain,
    ShaderType_Amplification,
    ShaderType_Mesh,
    ShaderType_Compute
} wisShaderType;

/// @brief Buffer usage flags, TODO: add more
typedef enum wisBufferFlags {
    BufferFlags_None = 0,
    BufferFlags_CopySource = 0x1, // VK_BUFFER_USAGE_TRANSFER_SRC_BIT
    BufferFlags_CopyDest = 0x2, // VK_BUFFER_USAGE_TRANSFER_DST_BIT
    BufferFlags_ConstantBuffer = 0x10, // VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT
    BufferFlags_IndexBuffer = 0x40, // VK_BUFFER_USAGE_INDEX_BUFFER_BIT
    BufferFlags_VertexBuffer = 0x80, // VK_BUFFER_USAGE_VERTEX_BUFFER_BIT
} wisBufferFlags;

typedef enum wisTextureFlags {
    TextureFlags_None = 0,
    TextureFlags_CopySource = 0x1, // VK_BUFFER_USAGE_TRANSFER_SRC_BIT
    TextureFlags_CopyDest = 0x2, // VK_BUFFER_USAGE_TRANSFER_DST_BIT
    TextureFlags_ShaderResource = 0x4, // VK_IMAGE_USAGE_SAMPLED_BIT
    TextureFlags_RenderTarget = 0x10, // VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT
    TextureFlags_DepthStencil = 0x20, // VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT
} wisTextureFlags;

#ifdef __cplusplus
}
#endif
