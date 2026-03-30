/**
 * @struct wisCommandQueueWaitFence
 * @ingroup Functions
 *
 *
 * @section wisCommandQueueWaitFence_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisCommandQueueWaitFence(const WisCommandQueue* self,
 *                                    WisFenceView           fence,
 *                                    uint64_t               value);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKCommandQueueWaitFence(const WisVKCommandQueue* self,
 *                                      WisVKFenceView           fence,
 *                                      uint64_t                 value);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12CommandQueueWaitFence(const WisDX12CommandQueue* self,
 *                                        WisDX12FenceView           fence,
 *                                        uint64_t                   value);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result CommandQueue::WaitFence(wis::FenceView fence,
 *                                     std::uint64_t  value) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKCommandQueue::WaitFence(wis::VKFenceView fence,
 *                                       std::uint64_t    value) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12CommandQueue::WaitFence(wis::DX12FenceView fence,
 *                                         std::uint64_t      value) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandQueueWaitFence_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandQueue instance.
 * - `fence` describes fence to wait on.
 * - `value` describes value that the fence is expected to reach.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisCommandQueueWaitFence_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandQueueWaitFence_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */