/**
 * @struct WisRootSignatureDesc
 * @ingroup Structures Core
 *
 *
 * @section WisRootSignatureDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisRootSignatureDesc {
 *     const WisPushConstant*    push_constants;
 *     size_t                    push_constant_count;
 *     const WisPushDescriptor*  push_descriptors;
 *     size_t                    push_descriptor_count;
 *     const WisDescriptorTable* descriptor_tables;
 *     size_t                    descriptor_table_count;
 * } WisRootSignatureDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  RootSignatureDesc {
 *     wis::span<const wis::PushConstant>    push_constants;
 *     wis::span<const wis::PushDescriptor>  push_descriptors;
 *     wis::span<const wis::DescriptorTable> descriptor_tables;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisRootSignatureDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `push_constants` points to an array of push constants that defines global shader push data.
 * - `push_constant_count` describes the number of the number of push constants in the `WisRootSignatureDesc::push_constants` array.
 * - `push_descriptors` points to an array of WisPushDescriptor.
 * - `push_descriptor_count` specifies the number of the number of push descriptors in the `WisRootSignatureDesc::push_descriptors` array.
 * - `descriptor_tables` points to an array of WisDescriptorTable.
 * - `descriptor_table_count` specifies the number of the number of descriptor tables in the `WisRootSignatureDesc::descriptor_tables` array.
 * \endcond
 *
 * @section WisRootSignatureDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisRootSignatureDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDeviceCreateRootSignature
 * \endcond
 */
