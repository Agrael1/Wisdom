/**
 * @struct wisCommandListSetRootSignature
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListSetRootSignature_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListSetRootSignature(const WisCommandList* self,
 *                                     WisRootSignatureView  signature,
 *                                     WisPipelineType       pipeline);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListSetRootSignature(const WisVKCommandList* self,
 *                                       WisVKRootSignatureView  signature,
 *                                       WisPipelineType         pipeline);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListSetRootSignature(const WisDX12CommandList* self,
 *                                         WisDX12RootSignatureView  signature,
 *                                         WisPipelineType           pipeline);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::SetRootSignature(wis::RootSignatureView signature,
 *                                    wis::PipelineType      pipeline) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::SetRootSignature(wis::VKRootSignatureView signature,
 *                                      wis::PipelineType        pipeline) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::SetRootSignature(wis::DX12RootSignatureView signature,
 *                                        wis::PipelineType          pipeline) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetRootSignature_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `signature` specifies a pointer to WisRootSignature to set.
 * - `pipeline` specifies the pipeline type to set the root signature for.
 * \endcond
 *
 * @section wisCommandListSetRootSignature_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetRootSignature_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */