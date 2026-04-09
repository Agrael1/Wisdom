/**
 * @struct WisDescriptorTableDataDesc
 * @ingroup Structures Core
 *
 *
 * @section WisDescriptorTableDataDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisDescriptorTableDataDesc {
 *     WisPipelineType       pipeline;
 *     uint32_t              root_index;
 *     WisDescriptorHeapType heap_type;
 *     uint32_t              heap_offset;
 * } WisDescriptorTableDataDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  DescriptorTableDataDesc {
 *     wis::PipelineType       pipeline;
 *     std::uint32_t           root_index;
 *     wis::DescriptorHeapType heap_type;
 *     std::uint32_t           heap_offset;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorTableDataDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `pipeline` specifies the pipeline type to set the push descriptors for.
 * - `root_index` indicates the root index in the root signature to set the push descriptors for.
 * - `heap_type` indicates the type of the descriptor heap to bind.
 * - `heap_offset` defines the offset in descriptors from the start of the heap to set the descriptor table to. Used for calculating descriptor indices when binding descriptor tables.
 * \endcond
 *
 * @section WisDescriptorTableDataDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDescriptorTableDataDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListSetDescriptorTable
 * \endcond
 */
