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
    Ok                = 0, ///< Operation succeded.
    Timeout           = 1, ///< Operation timed out.
    Partial           = 2, ///< Operation partially succeeded.
    InvalidArgument   = -1, ///< One or more arguments, or parts of arguments passed to the function were incorrect.
    OutOfHostMemory   = -2, ///< There is no more host memory available.
    OutOfDeviceMemory = -3, ///< There is no more device memory available.
    DeviceLost        = -4, ///< Device driver was forcefully stopped.
    Occluded          = -5, ///< Swapchain presentation was not visible to the user. Rendering is too fast.
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
};

/**
 * @brief Provided by Wisdom 0.7.0. Flags that describe adapter.
 *
 * */
enum class AdapterFlags : uint32_t {
    None     = 0, ///< No flags set. Adapter @wis_may be descrete or embedded.
    Remote   = (1 << 0), ///< Adapter is remote. Used for remote rendering.
    Software = (1 << 1), ///< Adapter is software. Uses CPU for software rendering.
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
    std::int32_t platform_code; ///< defines platfrom code from underlying implementation. Is an `HRESULT` for DX12 and a `VkResult` for Vulkan.
    const char*  error; ///< contains a human readable error message.
};

/**
 * @brief Provided by Wisdom 0.7.0. Adapter description. Describes hardware driver identificators as well as memory limits.
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
    bool               debug_layer; ///< enables or disables debug layer on both DX12 and VK backends.
    wis::DebugCallback callback; ///< defines the debug callback function.
    void*              user_data; ///< user defined data pointer passed to the callback.
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
};

/**
 * @brief Provided by Wisdom 0.7.0. Pipeline layout description. Defines resource bindings for shaders.
 *
 * */
struct PipelineLayoutDesc {
    wis::span<const wis::PushConstant>   push_constants; ///< points to an array of wis::PushConstant.
    wis::span<const wis::PushDescriptor> push_descriptors; ///< points to an array of wis::PushDescriptor.
    void*                                reserved; ///< reserved for future use. Must be `nullptr`.
    std::size_t                          reserved_size; ///< reserved for future use. Must be `0`.
};

} // namespace wis
#endif // __cplusplus
#endif // WISDOM_CPP_API_HPP
