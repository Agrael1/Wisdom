/**
 * @struct wisFenceSignal
 * @ingroup Functions Core
 *
 *
 * @section wisFenceSignal_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisFenceSignal(const WisFence* self,
 *                          uint64_t        value);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKFenceSignal(const WisVKFence* self,
 *                            uint64_t          value);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12FenceSignal(const WisDX12Fence* self,
 *                              uint64_t            value);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result Fence::Signal(std::uint64_t value) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result VKFence::Signal(std::uint64_t value) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * wis::Result DX12Fence::Signal(std::uint64_t value) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisFenceSignal_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisFence instance.
 * - `value` describes value to signal.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisFenceSignal_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisFenceSignal_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
