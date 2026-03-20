/**
 * @struct WisDeviceCommandQueueProperties
 * @ingroup Structures
 *
 *
 * @section WisDeviceCommandQueueProperties_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisDeviceCommandQueueProperties {
 *     WisQueryPropertyType    property_type;
 *     void*                   next_in_chain;
 *     bool                    supported_queues[5];
 *     bool                    relaxed_queue_transition;
 *     WisCommandQueuePriority max_queue_priority[5];
 * } WisDeviceCommandQueueProperties;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  DeviceCommandQueueProperties {
 *     wis::QueryPropertyType    property_type;
 *     void*                     next_in_chain;
 *     std::array<bool, 5>       supported_queues;
 *     bool                      relaxed_queue_transition;
 *     std::array<wis::CommandQueuePriority, 5> max_queue_priority;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDeviceCommandQueueProperties_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `property_type` Defines the type of the queried property. @wis_must be WisQueryPropertyType..
 * - `next_in_chain` Pointer to the next queried data struct.
 * - `supported_queues` Array of supported queue types. If a queue type is supported, the value is `1`, otherwise `0`. Order of queue types is the same as in WisCommandQueueType enum.
 * - `relaxed_queue_transition` Indicates if relaxed queue transition is supported. This feature allows executing command lists that contain buffers used on different queue types without explicit resource state transitions when the buffers is used on a different queue type. It is supported on Windows 10 22H2 and later with WDDM 3.0 or later. On Vulkan it requires `VK_KHR_maintenance9` extension.
 * - `max_queue_priority` Array of maximum supported priorities for each queue type. If a queue type is not supported, the value is `0`. Order of queue types is the same as in WisCommandQueueType enum.
 * \endcond
 *
 * @section WisDeviceCommandQueueProperties_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDeviceCommandQueueProperties_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */