/**
 * @struct WisDescriptorHeapFlags WisDescriptorHeapFlags
 * @ingroup Enumerations Core
 *
 * @section WisDescriptorHeapFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisDescriptorHeapFlags {
 *     WisDescriptorHeapFlagsNone = 0,
 *     WisDescriptorHeapFlagsDisallowEmbeddedSamplers = (1u << 1),
 * } WisDescriptorHeapFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class DescriptorHeapFlags : uint32_t {
 *     None = 0,
 *     DisallowEmbeddedSamplers = (1u << 1),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorHeapFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Flags for descriptor heap creation.
 * 
 * Values:
 * - `WisDescriptorHeapFlagsNone = 0`: No flags set.
 * - `WisDescriptorHeapFlagsDisallowEmbeddedSamplers = (1 << 1)`: Heap is used in full for dynamic samplers. There @wis_mustnot be any shader that use embedded samplers that uses that heap. User @wis_may allocate more samplers in the heap than it would normally be.
 * \endcond
 *
 *
 * @section WisDescriptorHeapFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisDescriptorHeapDesc
 * \endcond
 */
