/**
 * @struct wisDeviceCreatePipelineCache
 * @ingroup Functions
 *
 *
 * @section wisDeviceCreatePipelineCache_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceCreatePipelineCache(const WisDevice* self,
 *                                        const uint8_t*   initial_data,
 *                                        size_t           data_size,
 *                                        WisPipelineCache*cache);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceCreatePipelineCache(const WisVKDevice* self,
 *                                          const uint8_t*     initial_data,
 *                                          size_t             data_size,
 *                                          WisVKPipelineCache*cache);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceCreatePipelineCache(const WisDX12Device* self,
 *                                            const uint8_t*       initial_data,
 *                                            size_t               data_size,
 *                                            WisDX12PipelineCache*cache);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::PipelineCache Device::CreatePipelineCache(wis::span<const std::uint8_t> initial_data,
 *                                                              wis::Result&                  out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKPipelineCache VKDevice::CreatePipelineCache(wis::span<const std::uint8_t> initial_data,
 *                                                                  wis::Result&                  out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12PipelineCache DX12Device::CreatePipelineCache(wis::span<const std::uint8_t> initial_data,
 *                                                                      wis::Result&                  out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreatePipelineCache_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `initial_data` points to an array of initial cache data bytes. If `nullptr`, the cache is created empty.
 * - `data_size` defines the size of the initial cache data in bytes.
 * - `cache` points to WisPipelineCache, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreatePipelineCache_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreatePipelineCache_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */