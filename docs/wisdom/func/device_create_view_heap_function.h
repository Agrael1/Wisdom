/**
 * @struct wisDeviceCreateViewHeap
 * @ingroup Functions
 *
 *
 * @section wisDeviceCreateViewHeap_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceCreateViewHeap(const WisDevice* self,
 *                                   WisViewHeapType  type,
 *                                   uint32_t         capacity,
 *                                   WisViewHeap*     heap);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceCreateViewHeap(const WisVKDevice* self,
 *                                     WisViewHeapType    type,
 *                                     uint32_t           capacity,
 *                                     WisVKViewHeap*     heap);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceCreateViewHeap(const WisDX12Device* self,
 *                                       WisViewHeapType      type,
 *                                       uint32_t             capacity,
 *                                       WisDX12ViewHeap*     heap);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::ViewHeap Device::CreateViewHeap(wis::ViewHeapType type,
 *                                                    std::uint32_t     capacity,
 *                                                    wis::Result&      out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKViewHeap VKDevice::CreateViewHeap(wis::ViewHeapType type,
 *                                                        std::uint32_t     capacity,
 *                                                        wis::Result&      out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12ViewHeap DX12Device::CreateViewHeap(wis::ViewHeapType type,
 *                                                            std::uint32_t     capacity,
 *                                                            wis::Result&      out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateViewHeap_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `type` defines the type of the view heap to create.
 * - `capacity` defines the capacity in descriptors of the view heap to create.
 * - `heap` points to WisDescriptorHeap, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateViewHeap_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateViewHeap_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */