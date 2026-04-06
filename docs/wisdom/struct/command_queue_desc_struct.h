/**
 * @struct WisCommandQueueDesc
 * @ingroup Structures Core
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
 *     wis::CommandQueuePriority priority;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisCommandQueueDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `type` specifies the type of the command queue.
 * - `priority` specifies command queue priority. Used to set priority of the command queues of the type.
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
 * @see Structs:
 * WisDeviceRequirements
 * \endcond
 */