/**
 * @struct WisPushConstantDataDesc
 * @ingroup Structures
 *
 *
 * @section WisPushConstantDataDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisPushConstantDataDesc {
 *     WisPipelineType pipeline;
 *     uint32_t        root_index;
 *     const void*     data;
 *     uint32_t        data_size;
 *     uint32_t        push_offset;
 * } WisPushConstantDataDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  PushConstantDataDesc {
 *     wis::PipelineType pipeline;
 *     std::uint32_t     root_index;
 *     const void*       data;
 *     std::uint32_t     data_size;
 *     std::uint32_t     push_offset;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisPushConstantDataDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `pipeline` specifies the pipeline type to set the push constants for.
 * - `root_index` indicates the root index in the root signature to set the push constants for.
 * - `data` specifies a pointer to the data to set as push constants.
 * - `data_size` defines the size of the data in bytes. It @wis_must be less than or equal to the maximum push constant size defined by the device and 4-byte aligned.
 * - `push_offset` specifies the offset in bytes from the start of the push constant root parameter to set the data to. It @wis_must be less than the maximum push constant size defined by the device and 4-byte aligned.
 * \endcond
 *
 * @section WisPushConstantDataDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisPushConstantDataDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListSetPushConstants
 * \endcond
 */