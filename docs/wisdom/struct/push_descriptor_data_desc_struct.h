/**
 * @struct WisPushDescriptorDataDesc
 * @ingroup Structures
 *
 *
 * @section WisPushDescriptorDataDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisPushDescriptorDataDesc {
 *     WisPipelineType   pipeline;
 *     uint32_t          root_index;
 *     WisDescriptorType descriptor_type;
 *     uint64_t          buffer_address;
 * } WisPushDescriptorDataDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  PushDescriptorDataDesc {
 *     wis::PipelineType   pipeline;
 *     std::uint32_t       root_index;
 *     wis::DescriptorType descriptor_type;
 *     std::uint64_t       buffer_address;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPushDescriptorDataDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `pipeline` defines the pipeline type to set the push descriptors for.
 * - `root_index` defines the root index in the root signature to set the push descriptors for.
 * - `descriptor_type` defines the type of the descriptors to push.
 * - `buffer_address` defines buffer device address to push.
 * \endcond
 *
 * @section WisPushDescriptorDataDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisPushDescriptorDataDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see wisCommandListSetPushDescriptor
 * \endcond
 */