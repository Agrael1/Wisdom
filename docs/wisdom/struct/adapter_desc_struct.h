/**
 * @struct WisAdapterDesc
 * @ingroup Structures
 *
 *
 * @section WisAdapterDesc_spec Specification
 * <hr>
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
 * - `description` Adapter description. Contains name of the graphics adapter.
 * - `vendor_id` denotes Vendor ID. Can be used to find the correct adapter.
 * - `device_id` denotes Device ID. Together with `WisAdapterDesc::vendor_id` uniquely identifies the device.
 * - `dedicated_video_memory` measures dedicated video memory in bytes. Used for device local memory type.
 * - `shared_system_memory` measures memory that is shared with CPU in bytes. Used for upload and readback.
 * - `adapter_id` denotes adapter unique ID (LUID). Can be used to find the correct adapter.
 * - `adapter_uuid` stores UUID of the adapter, used only with Vulkan API on systems with no LUID.
 * - `flags` Adapter flags. Describe the adapter kind.
 * \endcond
 *
 * @section WisAdapterDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisAdapterDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see wisGetAdapterDesc
 * \endcond
 */