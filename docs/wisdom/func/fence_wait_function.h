/**
 * @struct wisFenceWait
 * @ingroup Functions Core
 *
 *
 * @section wisFenceWait_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisFenceWait(const WisFence* self,
 *                        uint64_t        value,
 *                        uint64_t        wait_ns);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKFenceWait(const WisVKFence* self,
 *                          uint64_t          value,
 *                          uint64_t          wait_ns);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12FenceWait(const WisDX12Fence* self,
 *                            uint64_t            value,
 *                            uint64_t            wait_ns);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result Fence::Wait(std::uint64_t value,
 *                         std::uint64_t wait_ns) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * wis::Result VKFence::Wait(std::uint64_t value,
 *                           std::uint64_t wait_ns) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * wis::Result DX12Fence::Wait(std::uint64_t value,
 *                             std::uint64_t wait_ns) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisFenceWait_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisFence instance.
 * - `value` describes value to wait for.
 * - `wait_ns` describes the time to wait for the fence to reach the value in nanoseconds. Default is infinite.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisFenceWait_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisFenceWait_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
