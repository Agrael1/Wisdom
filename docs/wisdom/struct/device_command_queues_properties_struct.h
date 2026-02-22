/**
 * @struct WisDeviceCommandQueuesProperties
 * @ingroup Structures
 *
 *
 * @section WisDeviceCommandQueuesProperties_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisDeviceCommandQueuesProperties {
 *     WisQueryPropertyType    property_type;
 *     void*                   next_in_chain;
 *     bool                    supported_queues[5];
 *     WisCommandQueuePriority max_queue_priority[5];
 * } WisDeviceCommandQueuesProperties;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  DeviceCommandQueuesProperties {
 *     wis::QueryPropertyType    property_type;
 *     void*                     next_in_chain;
 *     std::array<bool, 5>       supported_queues;
 *     std::array<wis::CommandQueuePriority, 5> max_queue_priority;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDeviceCommandQueuesProperties_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `property_type` Defines the type of the queried property. @wis_must be wis::QueryPropertyType..
 * - `next_in_chain` Pointer to the next queried data struct.
 * - `supported_queues` Array of supported queue types. If a queue type is supported, the value is `1`, otherwise `0`. Order of queue types is the same as in wis::CommandQueueType enum.
 * - `max_queue_priority` Array of maximum supported priorities for each queue type. If a queue type is not supported, the value is `0`. Order of queue types is the same as in wis::CommandQueueType enum.
 * \endcond
 *
 * @section WisDeviceCommandQueuesProperties_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDeviceCommandQueuesProperties_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */