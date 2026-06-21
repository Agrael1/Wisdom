/**
 * @struct wisVideoDecodeCommandListInsertBarriers
 * @ingroup Functions Video
 *
 *
 * @section wisVideoDecodeCommandListInsertBarriers_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisVideoDecodeCommandListInsertBarriers(const WisVideoDecodeCommandList* self,
 *                                              const WisBarrierGroup*           barriers);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * void wisVKVideoDecodeCommandListInsertBarriers(const WisVKVideoDecodeCommandList* self,
 *                                                const WisVKBarrierGroup*           barriers);
 *
 * // Provided by Wisdom 0.7.1.
 * void wisDX12VideoDecodeCommandListInsertBarriers(const WisDX12VideoDecodeCommandList* self,
 *                                                  const WisDX12BarrierGroup*           barriers);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * void VideoDecodeCommandList::InsertBarriers(const wis::BarrierGroup& barriers) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * void VKVideoDecodeCommandList::InsertBarriers(const wis::VKBarrierGroup& barriers) const noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * void DX12VideoDecodeCommandList::InsertBarriers(const wis::DX12BarrierGroup& barriers) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisVideoDecodeCommandListInsertBarriers_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisVideoDecodeCommandList instance.
 * - `barriers` specifies a pointer to an array of barriers to insert.
 * \endcond
 *
 * @section wisVideoDecodeCommandListInsertBarriers_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisVideoDecodeCommandListInsertBarriers_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
