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
 *     WisViewHeapFlagsAllowVideoTargets = (1u << 0),
 * } WisViewHeapFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class ViewHeapFlags : uint32_t {
 *     None = 0,
 *     AllowMultisample = (1u << 0),
 *     AllowVideoTargets = (1u << 0),
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
 *
 * Values:
 * - `WisViewHeapFlagsNone = 0`: No flags set. View heap is regular.
 * - `WisViewHeapFlagsAllowMultisample = (1 << 0)`: Allows the view heap to be used with multisampled resources. If not
 * set, the view heap does not enable multisample-related usage.
 * - `WisViewHeapFlagsAllowVideoTargets = (1 << 0)`: Allows the view heap to be used with video targets. If not set, the
 * view heap does not enable video target-related usage.
 * \endcond
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
