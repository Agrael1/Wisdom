/**
 * @struct WisDeviceRequirements
 * @ingroup Structures
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
 *     WisCommandQueueDesc**      queue_descs;
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
 *     WisCommandQueueDesc**        queue_descs;
 *     size_t                       queue_desc_count;
 *     WisVKDeviceExtensionHeader** extensions;
 *     size_t                       extension_count;
 * } WisVKDeviceRequirements;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12DeviceRequirements {
 *     WisCommandQueueDesc**          queue_descs;
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
 *     wis::span<wis::CommandQueueDesc*>      queue_descs;
 *     std::size_t                            queue_desc_count;
 *     wis::span<wis::DeviceExtensionHeader*> extensions;
 *     std::size_t                            extension_count;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKDeviceRequirements {
 *     wis::span<wis::CommandQueueDesc*>        queue_descs;
 *     std::size_t                              queue_desc_count;
 *     wis::span<wis::VKDeviceExtensionHeader*> extensions;
 *     std::size_t                              extension_count;
 * };
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12DeviceRequirements {
 *     wis::span<wis::CommandQueueDesc*>          queue_descs;
 *     std::size_t                                queue_desc_count;
 *     wis::span<wis::DX12DeviceExtensionHeader*> extensions;
 *     std::size_t                                extension_count;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisDeviceRequirements_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `queue_descs` points to an array of WisCommandQueueDesc for which queues will be created during device creation.
 * - `queue_desc_count` counts the number of queue descriptions in the wisAdapterQueryCreateDevice array.
 * - `extensions` points to an array of extensions that are to be initialized with pointers to WisDeviceExtensionHeader.
 * - `extension_count` counts the number of extensions in the wisAdapterQueryCreateDevice array.
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
 * @see wisAdapterQueryCreateDevice
 * \endcond
 */