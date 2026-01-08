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
 *     const WisPushConstant*      push_constants;
 *     size_t                      push_constant_count;
 *     const WisPushDescriptor*    push_descriptors;
 *     size_t                      push_descriptor_count;
 *     const WisStaticSamplerDesc* static_samplers;
 *     size_t                      static_sampler_count;
 *     const WisDescriptorTable*   descriptor_tables;
 *     size_t                      descriptor_table_count;
 * } WisPipelineLayoutDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  PipelineLayoutDesc {
 *     wis::span<const wis::PushConstant>      push_constants;
 *     wis::span<const wis::PushDescriptor>    push_descriptors;
 *     wis::span<const wis::StaticSamplerDesc> static_samplers;
 *     wis::span<const wis::DescriptorTable>   descriptor_tables;
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
 * - `static_samplers` points to an array of WisStaticSamplerDesc.
 * - `static_sampler_count` counts the number of static samplers in the `WisPipelineLayoutDesc::static_samplers` array.
 * - `descriptor_tables` points to an array of WisDescriptorTable.
 * - `descriptor_table_count` counts the number of descriptor tables in the `WisPipelineLayoutDesc::descriptor_tables` array.
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