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
 * @brief Provided by Wisdom 0.7.0. Shader stages that can be used in the pipeline. Main use is Root signature and descriptor management. Stages have no granularity, either all or one can be selected.
 *
 * */
typedef enum WisShaderStages {
    WisShaderStagesAll           = 0, ///< All shader stages.
    WisShaderStagesVertex        = 1, ///< Vertex shader stage.
    WisShaderStagesHull          = 2, ///< Hull/Tessellation control shader stage.
    WisShaderStagesDomain        = 3, ///< Domain/Tessellation evaluation shader stage.
    WisShaderStagesGeometry      = 4, ///< Geometry shader stage.
    WisShaderStagesPixel         = 5, ///< Pixel/Fragment shader stage.
    WisShaderStagesAmplification = 6, ///< Amplification shader stage.
    WisShaderStagesMesh          = 7, ///< Mesh shader stage.
    WisShaderStagesCount         = 8, ///< Number of stages.
} WisShaderStages;

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
    WisStaticBorderCustom           = 3, ///< Custom border color defined by the user.
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
 * @brief Provided by Wisdom 0.7.0. Descriptor storage tier. Decides how many descriptors can be allocated in a single heap.
 *
 * */
typedef enum WisDescriptorStorageTier {
    WisDescriptorStorageTierTier1 = 0, ///< Tier 1: VkDescriptorSets and VkDescriptorPools.
    WisDescriptorStorageTierTier2 = 1, ///< Tier 2: Descriptor Buffer with no mutable descriptor type.
    WisDescriptorStorageTierTier3 = 2, ///< Tier 3: Descriptor Buffer with mutable descriptor type.
    WisDescriptorStorageTierTier4 = 3, ///< Tier 4: Descriptor Heap.
} WisDescriptorStorageTier;

/**
 * @brief Provided by Wisdom 0.7.0. Query type for GPU queries.
 *
 * */
typedef enum WisQueryPropertyType {
    WisQueryPropertyTypeDeviceCommandQueueProperties   = 0, ///< Properties of the device command queues. Expects a .
    WisQueryPropertyTypeDeviceDescriptorHeapProperties = 1, ///< Properties of the device descriptor heap. Expects a WisDeviceDescriptorHeapProperties.
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
    uint32_t            max_anisotropy; ///< Max anisotropy level. Max is 16.
    WisAddressMode      address_u; ///< Address mode for U coordinate.
    WisAddressMode      address_v; ///< Address mode for V coordinate.
    WisAddressMode      address_w; ///< Address mode for W coordinate.
    float               min_lod; ///< Min LOD value.
    float               max_lod; ///< Max LOD value.
    float               mip_lod_bias; ///< Mip LOD bias value.
    WisCompareOperation comparison_op; ///< Comparison operation for comparison samplers.
    WisStaticBorder     static_border_color; ///< Static border color. Used if any address mode is set to wis::AddressMode.
    float               border_color[4]; ///< Border color. Used if any address mode is set to wis::AddressMode and static_border_color is set to `WisStaticBorderCustom`.
    WisSamplerFlags     flags; ///< Sampler flags. Used to set additional sampler options.
} WisSamplerDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Static sampler description for WisPipelineLayout creation.
 *
 * */
typedef struct WisStaticSamplerDesc {
    WisSamplerDesc  sampler; ///< Sampler description.
    WisShaderStages stage; ///< Shader stage. Defines the stage where the sampler is used.
    uint32_t        bind_register; ///< Bind register number in HLSL.
} WisStaticSamplerDesc;

/**
 * @brief Provided by Wisdom 0.7.0. A set of constants that get pushed directly to the pipeline. Only one set can be created per shader stage.
 *
 * */
typedef struct WisPushConstant {
    WisShaderStages stage; ///< Shader stage. Defines the stage where the constant is used.
    uint32_t        size_bytes; ///< Size of the constant in bytes. Must be divisible by 4.
    uint32_t        bind_register; ///< Bind register number in HLSL.
    uint32_t        bind_space; ///< Bind space number in HLSL. `register(regN, spaceN)`
} WisPushConstant;

/**
 * @brief Provided by Wisdom 0.7.0. Push descriptor. Used to push data directly to pipeline.
 *
 * */
typedef struct WisPushDescriptor {
    WisShaderStages   stage; ///< Shader stage. Defines the stage where the descriptor is used.
    WisDescriptorType type; ///< Descriptor type. Works only with buffer bindings.
    uint32_t          bind_register; ///< Bind register number in HLSL.
} WisPushDescriptor;

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor table entry for WisDescriptorTable.
 *
 * */
typedef struct WisDescriptorTableEntry {
    WisDescriptorType type; ///< Descriptor type.
    uint32_t          bind_register; ///< Bind register number in HLSL.
    uint32_t          count; ///< Descriptor count for Array descriptors. UINT32_MAX means unbounded array. 0 means single register, same as 1.
} WisDescriptorTableEntry;

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor table for WisPipelineLayout creation.
 *
 * */
typedef struct WisDescriptorTable {
    WisDescriptorHeapType          type; ///< Descriptor heap type. Either Descriptor or Sampler.
    WisShaderStages                stage; ///< Shader stage. Defines the stage where the table is used.
    const WisDescriptorTableEntry* entries; ///< Descriptor table entries array.
    size_t                         entry_count; ///< Descriptor table entries count.
    uint32_t                       space_overlap; ///< If this value is not zero, bindings from this table can be bound several times in different spaces. Used for descriptor indexing into unbounded arrays of similar types.
} WisDescriptorTable;

/**
 * @brief Provided by Wisdom 0.7.0. Pipeline layout description. Defines resource bindings for shaders.
 *
 * */
typedef struct WisPipelineLayoutDesc {
    const WisPushConstant*      push_constants; ///< points to an array of WisPushConstant.
    size_t                      push_constant_count; ///< counts the number of push constants in the `WisPipelineLayoutDesc::push_constants` array.
    const WisPushDescriptor*    push_descriptors; ///< points to an array of WisPushDescriptor.
    size_t                      push_descriptor_count; ///< counts the number of push descriptors in the `WisPipelineLayoutDesc::push_descriptors` array.
    const WisStaticSamplerDesc* static_samplers; ///< points to an array of WisStaticSamplerDesc.
    size_t                      static_sampler_count; ///< counts the number of static samplers in the `WisPipelineLayoutDesc::static_samplers` array.
    const WisDescriptorTable*   descriptor_tables; ///< points to an array of WisDescriptorTable.
    size_t                      descriptor_table_count; ///< counts the number of descriptor tables in the `WisPipelineLayoutDesc::descriptor_tables` array.
} WisPipelineLayoutDesc;

/**
 * @brief Provided by Wisdom 0.7.0. Descriptor heap description for WisDescriptorHeap creation.
 *
 * */
typedef struct WisDescriptorHeapDesc {
    WisDescriptorHeapType   type; ///< indicates the type of descriptor heap to create (sampler or descriptor).
    WisDescriptorMemoryType memory_type; ///< indicates where the descriptor heap will be allocated.
    size_t                  descriptor_count; ///< indicates the amount of descriptors, present in the heap.
} WisDescriptorHeapDesc;

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
    size_t               max_descriptor_heap_size; ///< Maximum number of descriptors in a single descriptor heap.
    size_t               max_sampler_heap_size; ///< Maximum number of samplers in a single descriptor heap.
    size_t               max_sampler_heap_size_with_embedded; ///< Maximum number of samplers in a single descriptor heap, if embedded samplers are used.
    size_t               descriptor_increment_size; ///< Size of a single descriptor in the descriptor heap. Used for calculating descriptor offsets.
    size_t               sampler_increment_size; ///< Size of a single sampler in the sampler heap. Used for calculating sampler offsets.
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

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_API_H
