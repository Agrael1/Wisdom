/**
 * @struct wisDeviceGetResourceAllocator
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceGetResourceAllocator_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDeviceGetResourceAllocator(const WisDevice* self,
 * WisResourceAllocator*allocator);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKDeviceGetResourceAllocator(const WisVKDevice* self,
 * WisVKResourceAllocator*allocator);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12DeviceGetResourceAllocator(const WisDX12Device* self,
 * WisDX12ResourceAllocator*allocator);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::ResourceAllocator Device::GetResourceAllocator(wis::Result& out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKResourceAllocator VKDevice::GetResourceAllocator(wis::Result& out_result) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DX12ResourceAllocator DX12Device::GetResourceAllocator(wis::Result& out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceGetResourceAllocator_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `allocator` points to WisResourceAllocator, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceGetResourceAllocator_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceGetResourceAllocator_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */