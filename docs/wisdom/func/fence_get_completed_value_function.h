/**
 * @struct wisFenceGetCompletedValue
 * @ingroup Functions Core
 *
 *
 * @section wisFenceGetCompletedValue_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisFenceGetCompletedValue(const WisFence* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisVKFenceGetCompletedValue(const WisVKFence* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisDX12FenceGetCompletedValue(const WisDX12Fence* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t Fence::GetCompletedValue() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t VKFence::GetCompletedValue() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t DX12Fence::GetCompletedValue() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisFenceGetCompletedValue_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisFence instance.
 * 
 * - **return** Value of the fence.
 * \endcond
 *
 * @section wisFenceGetCompletedValue_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisFenceGetCompletedValue_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */