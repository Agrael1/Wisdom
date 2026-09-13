/**
 * @struct WisDescriptorMemoryType WisDescriptorMemoryType
 * @ingroup Enumerations Core
 *
 * @section WisDescriptorMemoryType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisDescriptorMemoryType {
 *     WisDescriptorMemoryTypeCpuOnly = 0,
 *     WisDescriptorMemoryTypeShaderVisible = 1,
 * } WisDescriptorMemoryType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class DescriptorMemoryType {
 *     CpuOnly = 0,
 *     ShaderVisible = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorMemoryType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Descriptor memory type. Decides if descriptors are visible and can be bound to GPU.
 *
 * \note Translates to  `D3D12_DESCRIPTOR_HEAP_FLAGS` for DirectX 12 implementation.
 *
 * Values:
 * - `WisDescriptorMemoryTypeCpuOnly = 0`: Descriptors are only visible to CPU. May be used for copying descriptors to
 * the GPU visible pool.
 * - `WisDescriptorMemoryTypeShaderVisible = 1`: Descriptors are visible to GPU. Descriptors can be bound to the GPU
 * pipeline directly, but can't be copied from.
 * \endcond
 *
 *
 * @section WisDescriptorMemoryType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisDescriptorHeapDesc
 * \endcond
 */
