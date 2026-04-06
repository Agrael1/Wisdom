/**
 * @struct WisViewHeapFlags WisViewHeapFlags
 * @ingroup Enumerations Core
 *
 * @section WisViewHeapFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisViewHeapFlags {
 *     WisViewHeapFlagsNone = 0,
 *     WisViewHeapFlagsAllowMultisample = (1u << 0),
 * } WisViewHeapFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class ViewHeapFlags : uint32_t {
 *     None = 0,
 *     AllowMultisample = (1u << 0),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisViewHeapFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * View heap creation flags.
 *
 * \note Translates to DirectX 12 as D3D12_DESCRIPTOR_HEAP_FLAGS.
 * Values:
 * - `WisViewHeapFlagsNone = 0`: No flags set. View heap is regular.
 * - `WisViewHeapFlagsAllowMultisample = (1 << 0)`: Shader visible view heap. If set, the view heap is visible to
 * shaders and can be used for descriptor tables. If not set, the view heap is not visible to shaders and cannot be used
 * for descriptor tables. \endcond
 *
 *
 * @section WisViewHeapFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDeviceCreateViewHeap
 * \endcond
 */
