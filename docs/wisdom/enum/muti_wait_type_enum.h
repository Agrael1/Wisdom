/**
 * @struct WisMutiWaitType WisMutiWaitType
 * @ingroup Enumerations
 *
 * @section WisMutiWaitType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisMutiWaitType {
 *     WisMutiWaitTypeAll = 0,
 *     WisMutiWaitTypeAny = 1,
 * } WisMutiWaitType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class MutiWaitType {
 *     All = 0,
 *     Any = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisMutiWaitType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Determines the behavior when wait for multiple fences is issued.
 * 
 * Values:
 * - `WisMutiWaitTypeAll = 0`: All the fences in the batch are triggered.
 * - `WisMutiWaitTypeAny = 1`: At least one of the fences from the batch is triggered.
 * \endcond
 *
 *
 * @section WisMutiWaitType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDeviceWaitForMultipleFences
 * \endcond
 */