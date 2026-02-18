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

} // namespace wis
#endif // __cplusplus
#endif // WISDOM_CPP_API_HPP
