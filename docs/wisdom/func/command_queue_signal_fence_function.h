/**
 * @struct wisCommandQueueSignalFence
 * @ingroup Functions Core
 *
 *
 * @section wisCommandQueueSignalFence_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisCommandQueueSignalFence(const WisCommandQueue* self,
 *                                      WisFenceView           fence,
 *                                      uint64_t               value);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKCommandQueueSignalFence(const WisVKCommandQueue* self,
 *                                        WisVKFenceView           fence,
 *                                        uint64_t                 value);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12CommandQueueSignalFence(const WisDX12CommandQueue* self,
 *                                          WisDX12FenceView           fence,
 *                                          uint64_t                   value);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result CommandQueue::SignalFence(wis::FenceView fence,
 *                                       std::uint64_t  value) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result VKCommandQueue::SignalFence(wis::VKFenceView fence,
 *                                         std::uint64_t    value) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * wis::Result DX12CommandQueue::SignalFence(wis::DX12FenceView fence,
 *                                           std::uint64_t      value) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandQueueSignalFence_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandQueue instance.
 * - `fence` describes fence to signal.
 * - `value` describes value used to signal the fence.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisCommandQueueSignalFence_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandQueueSignalFence_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */