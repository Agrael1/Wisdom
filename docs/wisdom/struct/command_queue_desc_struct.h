/**
 * @struct WisCommandQueueDesc
 * @ingroup Structures
 *
 *
 * @section WisCommandQueueDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisCommandQueueDesc {
 *     WisCommandQueueType     type;
 *     WisCommandQueueFlags    flags;
 *     WisCommandQueuePriority priority;
 * } WisCommandQueueDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  CommandQueueDesc {
 *     wis::CommandQueueType     type;
 *     wis::CommandQueueFlags    flags;
 *     wis::CommandQueuePriority priority;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisCommandQueueDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `type` defines the type of the command queue.
 * - `flags` defines command queue flags. Used to set additional options for command queue creation.
 * - `priority` defines the global priority of the command queue.
 * \endcond
 *
 * @section WisCommandQueueDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisCommandQueueDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see WisDeviceRequirements
 * \endcond
 */