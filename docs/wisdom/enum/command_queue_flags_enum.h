/**
 * @struct WisCommandQueueFlags WisCommandQueueFlags
 * @ingroup Enumerations Core
 *
 * @section WisCommandQueueFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisCommandQueueFlags {
 *     WisCommandQueueFlagsNone = 0,
 *     WisCommandQueueFlagsSynchronized = (1 << 0),
 * } WisCommandQueueFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class CommandQueueFlags : uint32_t {
 *     None = 0,
 *     Synchronized = (1 << 0),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisCommandQueueFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Flags for command queue creation.
 * 
 * Values:
 * - `WisCommandQueueFlagsNone = 0`: No flags set.
 * - `WisCommandQueueFlagsSynchronized = (1 << 0)`: Command queue submission is synchronized.
 * \endcond
 *
 *
 * @section WisCommandQueueFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisCommandQueueDesc
 * \endcond
 */
