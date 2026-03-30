/**
 * @struct WisAdapterDesc
 * @ingroup Structures
 *
 *
 * @section WisAdapterDesc_spec Specification
 * <hr>
 * 
 * Structure describing a graphics adapter present in the system:
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisAdapterDesc {
 *     char            description[256];
 *     uint32_t        vendor_id;
 *     uint32_t        device_id;
 *     uint64_t        dedicated_video_memory;
 *     uint64_t        shared_system_memory;
 *     uint64_t        adapter_id;
 *     uint8_t         adapter_uuid[16];
 *     WisAdapterFlags flags;
 * } WisAdapterDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  AdapterDesc {
 *     std::array<char, 256> description;
 *     std::uint32_t     vendor_id;
 *     std::uint32_t     device_id;
 *     std::uint64_t     dedicated_video_memory;
 *     std::uint64_t     shared_system_memory;
 *     std::uint64_t     adapter_id;
 *     std::array<std::uint8_t, 16> adapter_uuid;
 *     wis::AdapterFlags flags;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisAdapterDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `description` defines adapter description. Contains name of the graphics adapter.
 * - `vendor_id` specifies denotes Vendor ID. Can be used to find the correct adapter.
 * - `device_id` defines denotes Device ID. Together with `WisAdapterDesc::vendor_id` uniquely identifies the device.
 * - `dedicated_video_memory` describes measures dedicated video memory in bytes. Used for device local memory type.
 * - `shared_system_memory` indicates measures memory that is shared with CPU in bytes. Used for upload and readback.
 * - `adapter_id` describes denotes adapter unique ID (LUID). Can be used to find the correct adapter.
 * - `adapter_uuid` indicates stores UUID of the adapter, used only with Vulkan API on systems with no LUID.
 * - `flags` describes adapter flags. Describe the adapter kind.
 * \endcond
 *
 * @section WisAdapterDesc_descr Description
 * <hr>
 * 
 * `description` provides a human-readable name for the adapter, which @wis_may help in identifying it among multiple adapters in the system. 
 * The string is null-terminated and with length 256 characters.
 * 
 * `vendor_id` and `device_id` are identifiers assigned by the hardware manufacturer. They @wis_can be used to look up more detailed information about the adapter from vendor databases.
 * 
 * `dedicated_video_memory` indicates the amount of memory that is exclusively available to the GPU for high-performance operations. This memory is typically faster and more efficient for graphics processing tasks.
 * `shared_system_memory` indicates the amount of system memory that @wis_can be used by the GPU when dedicated video memory is insufficient. This memory is shared with the CPU and @wis_may have higher latency compared to dedicated video memory.
 * Both `dedicated_video_memory` and `shared_system_memory` are reported in bytes. Applications @wis_may use this information to make decisions about resource allocation and performance optimizations.
 * The values @wis_may not be identical across different implementations, because underlying graphics APIs expose varying levels of detail about adapter memory.
 * 
 * `adapter_id` provides a unique identifier for the adapter within the system. This ID @wis_can be used to select or reference the same adapter in other API calls or configurations.
 * It matches the LUID provided by Vulkan and DirectX 12 APIs. On systems where LUID is not available (e.g., some Linux systems with Vulkan), `adapter_uuid` is used instead to uniquely identify the adapter.
 * 
 * `flags` describe the type and capabilities of the adapter. They provide information about whether the adapter is integrated, discrete, software-based, or external. This information @wis_can help applications make decisions about which adapter to use based on performance and power consumption considerations.
 * 
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisAdapterDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisAdapterQueryGetAdapterDesc
 * \endcond
 */