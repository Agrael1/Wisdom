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
    WisStatusOk                = 0, ///< Operation succeded.
    WisStatusTimeout           = 1, ///< Operation timed out.
    WisStatusPartial           = 2, ///< Operation partially succeeded.
    WisStatusInvalidArgument   = -1, ///< One or more arguments, or parts of arguments passed to the function were incorrect.
    WisStatusOutOfHostMemory   = -2, ///< There is no more host memory available.
    WisStatusOutOfDeviceMemory = -3, ///< There is no more device memory available.
    WisStatusDeviceLost        = -4, ///< Device driver was forcefully stopped.
    WisStatusOccluded          = -5, ///< Swapchain presentation was not visible to the user. Rendering is too fast.
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
 * @brief Provided by Wisdom 0.7.0. Flags that describe adapter.
 *
 * */
typedef enum WisAdapterFlags {
    WisAdapterFlagsNone     = 0, ///< No flags set. Adapter @wis_may be descrete or embedded.
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
    int32_t     platform_code; ///< defines platfrom code from underlying implementation. Is an `HRESULT` for DX12 and a `VkResult` for Vulkan.
    const char* error; ///< contains a human readable error message.
} WisResult;

/**
 * @brief Provided by Wisdom 0.7.0. Adapter description. Describes hardware driver identificators as well as memory limits.
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
 * @brief Provided by Wisdom 0.7.0. Sampler description for WisSampler creation.
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

#ifdef __cplusplus
}
#endif // __cplusplus
#endif // WISDOM_C_API_H
