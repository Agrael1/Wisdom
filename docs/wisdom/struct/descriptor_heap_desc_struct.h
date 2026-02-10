/**
 * @struct WisDescriptorHeapDesc
 * @ingroup Structures
 *
 *
 * @section WisDescriptorHeapDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisDescriptorHeapDesc {
 *     WisDescriptorHeapType   type;
 *     WisDescriptorMemoryType memory_type;
 *     size_t                  descriptor_count;
 * } WisDescriptorHeapDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  DescriptorHeapDesc {
 *     wis::DescriptorHeapType   type;
 *     wis::DescriptorMemoryType memory_type;
 *     std::size_t               descriptor_count;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorHeapDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `type` indicates the type of descriptor heap to create (sampler or descriptor).
 * - `memory_type` indicates where the descriptor heap will be allocated.
 * - `descriptor_count` indicates the amount of descriptors, present in the heap.
 * \endcond
 *
 * @section WisDescriptorHeapDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDescriptorHeapDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see wisDeviceCreateDescriptorHeap
 * \endcond
 */