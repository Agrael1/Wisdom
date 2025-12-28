/**
 * @struct wisDeviceCreateResourceAllocator
 * @ingroup Functions
 *
 *
 * @section wisDeviceCreateResourceAllocator_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceCreateResourceAllocator(const WisDevice* self,
 * WisResourceAllocator*allocator);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceCreateResourceAllocator(const WisVKDevice* self,
 * WisVKResourceAllocator*allocator);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceCreateResourceAllocator(const WisDX12Device* self,
 * WisDX12ResourceAllocator*allocator);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::ResourceAllocator Device::CreateResourceAllocator(wis::Result& out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKResourceAllocator VKDevice::CreateResourceAllocator(wis::Result& out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12ResourceAllocator DX12Device::CreateResourceAllocator(wis::Result& out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateResourceAllocator_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `allocator` points to WisResourceAllocator, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateResourceAllocator_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateResourceAllocator_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */