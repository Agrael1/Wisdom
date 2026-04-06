/**
 * @struct WisInputClass WisInputClass
 * @ingroup Enumerations Core
 *
 * @section WisInputClass_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisInputClass {
 *     WisInputClassPerVertex = 0,
 *     WisInputClassPerInstance = 1,
 * } WisInputClass;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class InputClass {
 *     PerVertex = 0,
 *     PerInstance = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisInputClass_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Input classification for vertex buffer data.
 *
 * Values:
 * - `WisInputClassPerVertex = 0`: Vertex buffer data is vertex data.
 * - `WisInputClassPerInstance = 1`: Vertex buffer data is per instance data.
 * \endcond
 *
 *
 * @section WisInputClass_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisInputBindingDesc
 * \endcond
 */