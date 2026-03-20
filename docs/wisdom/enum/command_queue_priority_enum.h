/**
 * @struct WisCommandQueuePriority WisCommandQueuePriority
 * @ingroup Enumerations
 *
 * @section WisCommandQueuePriority_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisCommandQueuePriority {
 *     WisCommandQueuePriorityNormal = 0,
 *     WisCommandQueuePriorityHigh = 1,
 *     WisCommandQueuePriorityRealtime = 2,
 * } WisCommandQueuePriority;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class CommandQueuePriority {
 *     Normal = 0,
 *     High = 1,
 *     Realtime = 2,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisCommandQueuePriority_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Global queue priority. Higher priority queues get more GPU time, but @wis_may cause performance issues if overused.
 * 
 * \note Translates to  `D3D12_COMMAND_QUEUE_PRIORITY` for DirectX 12 implementation, and `VkQueueGlobalPriorityEXT` for Vulkan implementation.
 * 
 * Values:
 * - `WisCommandQueuePriorityNormal = 0`: Normal queue priority.
 * - `WisCommandQueuePriorityHigh = 1`: High queue priority.
 * - `WisCommandQueuePriorityRealtime = 2`: Global realtime queue priority. Requires special GPU support and @wis_may cause performance issues if used on unsupported hardware.
 * \endcond
 *
 *
 * @section WisCommandQueuePriority_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisCommandQueueDesc, WisDeviceCommandQueueProperties
 * \endcond
 */