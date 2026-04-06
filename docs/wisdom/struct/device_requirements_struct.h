/**
 * @struct WisDeviceRequirements
 * @ingroup Structures Core
 *
 *
 * @section WisDeviceRequirements_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDeviceRequirements {
 *     const WisCommandQueueDesc* queue_descs;
 *     size_t                     queue_desc_count;
 *     WisDeviceExtensionHeader** extensions;
 *     size_t                     extension_count;
 * } WisDeviceRequirements;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKDeviceRequirements {
 *     const WisCommandQueueDesc*   queue_descs;
 *     size_t                       queue_desc_count;
 *     WisVKDeviceExtensionHeader** extensions;
 *     size_t                       extension_count;
 * } WisVKDeviceRequirements;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12DeviceRequirements {
 *     const WisCommandQueueDesc*     queue_descs;
 *     size_t                         queue_desc_count;
 *     WisDX12DeviceExtensionHeader** extensions;
 *     size_t                         extension_count;
 * } WisDX12DeviceRequirements;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct DeviceRequirements {
 *     wis::span<const wis::CommandQueueDesc> queue_descs;
 *     wis::span<wis::DeviceExtensionHeader*> extensions;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKDeviceRequirements {
 *     wis::span<const wis::CommandQueueDesc>   queue_descs;
 *     wis::span<wis::VKDeviceExtensionHeader*> extensions;
 * };
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12DeviceRequirements {
 *     wis::span<const wis::CommandQueueDesc>     queue_descs;
 *     wis::span<wis::DX12DeviceExtensionHeader*> extensions;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisDeviceRequirements_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `queue_descs` points to an array of WisCommandQueueDesc for which queues are created during device creation.
 * - `queue_desc_count` indicates the number of the number of queue descriptions in the wisAdapterQueryCreateDevice queue_descs array.
 * - `extensions` points to an array of extensions that are to be initialized with pointers to WisDeviceExtensionHeader.
 * - `extension_count` describes the number of the number of extensions in the wisAdapterQueryCreateDevice extensions array.
 * \endcond
 *
 * @section WisDeviceRequirements_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDeviceRequirements_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisAdapterQueryCreateDevice
 * \endcond
 */