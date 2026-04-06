/**
 * @struct WisMultiWaitType WisMultiWaitType
 * @ingroup Enumerations Core
 *
 * @section WisMultiWaitType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisMultiWaitType {
 *     WisMultiWaitTypeAll = 0,
 *     WisMultiWaitTypeAny = 1,
 * } WisMultiWaitType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class MultiWaitType {
 *     All = 0,
 *     Any = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisMultiWaitType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Determines the behavior when waiting for multiple fences.
 *
 * Values:
 * - `WisMultiWaitTypeAll = 0`: All the fences in the batch are triggered.
 * - `WisMultiWaitTypeAny = 1`: At least one of the fences from the batch is triggered.
 * \endcond
 *
 *
 * @section WisMultiWaitType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDeviceWaitForMultipleFences
 * \endcond
 */