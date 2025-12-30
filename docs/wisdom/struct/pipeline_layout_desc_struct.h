/**
 * @struct WisPipelineLayoutDesc
 * @ingroup Structures
 *
 *
 * @section WisPipelineLayoutDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisPipelineLayoutDesc {
 *     const WisPushConstant*   push_constants;
 *     size_t                   push_constant_count;
 *     const WisPushDescriptor* push_descriptors;
 *     size_t                   push_descriptor_count;
 *     void*                    reserved;
 *     size_t                   reserved_size;
 * } WisPipelineLayoutDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  PipelineLayoutDesc {
 *     wis::span<const wis::PushConstant>   push_constants;
 *     wis::span<const wis::PushDescriptor> push_descriptors;
 *     void*                                reserved;
 *     std::size_t                          reserved_size;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPipelineLayoutDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `push_constants` points to an array of WisPushConstant.
 * - `push_constant_count` counts the number of push constants in the `WisPipelineLayoutDesc::push_constants` array.
 * - `push_descriptors` points to an array of WisPushDescriptor.
 * - `push_descriptor_count` counts the number of push descriptors in the `WisPipelineLayoutDesc::push_descriptors` array.
 * - `reserved` reserved for future use. Must be `nullptr`.
 * - `reserved_size` reserved for future use. Must be `0`.
 * \endcond
 *
 * @section WisPipelineLayoutDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisPipelineLayoutDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see wisDeviceCreatePipelineLayout
 * \endcond
 */