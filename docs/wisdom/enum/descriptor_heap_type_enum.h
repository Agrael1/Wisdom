/**
 * @struct WisDescriptorHeapType WisDescriptorHeapType
 * @ingroup Enumerations
 *
 * @section WisDescriptorHeapType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisDescriptorHeapType {
 *     WisDescriptorHeapTypeDescriptor = 0,
 *     WisDescriptorHeapTypeSampler = 1,
 * } WisDescriptorHeapType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class DescriptorHeapType {
 *     Descriptor = 0,
 *     Sampler = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorHeapType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Descriptor heap type.
 * 
 * \note Translates to  `D3D12_DESCRIPTOR_HEAP_TYPE` for DirectX 12 implementation.
 * 
 * Values:
 * - `WisDescriptorHeapTypeDescriptor = 0`: Descriptor heap type. Used for all descriptor types, except for samplers.
 * - `WisDescriptorHeapTypeSampler = 1`: Sampler heap type. Used for sampler descriptors.
 * \endcond
 *
 *
 * @section WisDescriptorHeapType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisDescriptorHeapDesc, WisDescriptorTableDataDesc
 * \endcond
 */