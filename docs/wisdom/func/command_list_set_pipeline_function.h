/**
 * @struct wisCommandListSetPipeline
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListSetPipeline_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListSetPipeline(const WisCommandList* self,
 *                                WisPipelineView       pipeline,
 *                                WisPipelineType       type);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListSetPipeline(const WisVKCommandList* self,
 *                                  WisVKPipelineView       pipeline,
 *                                  WisPipelineType         type);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListSetPipeline(const WisDX12CommandList* self,
 *                                    WisDX12PipelineView       pipeline,
 *                                    WisPipelineType           type);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::SetPipeline(wis::PipelineView pipeline,
 *                               wis::PipelineType type) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::SetPipeline(wis::VKPipelineView pipeline,
 *                                 wis::PipelineType   type) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::SetPipeline(wis::DX12PipelineView pipeline,
 *                                   wis::PipelineType     type) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListSetPipeline_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `pipeline` specifies a pointer to WisPipeline to set.
 * - `type` specifies the pipeline type to set the pipeline for.
 * \endcond
 *
 * @section wisCommandListSetPipeline_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListSetPipeline_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */