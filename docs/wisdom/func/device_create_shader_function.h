/**
 * @struct wisDeviceCreateShader
 * @ingroup Functions Core
 *
 *
 * @section wisDeviceCreateShader_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDeviceCreateShader(const WisDevice* self,
 *                                 const uint8_t*   data,
 *                                 size_t           size,
 *                                 WisShader*       shader);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKDeviceCreateShader(const WisVKDevice* self,
 *                                   const uint8_t*     data,
 *                                   size_t             size,
 *                                   WisVKShader*       shader);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12DeviceCreateShader(const WisDX12Device* self,
 *                                     const uint8_t*       data,
 *                                     size_t               size,
 *                                     WisDX12Shader*       shader);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::Shader Device::CreateShader(wis::span<const std::uint8_t> data,
 *                                                wis::Result&                  out_result) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKShader VKDevice::CreateShader(wis::span<const std::uint8_t> data,
 *                                                    wis::Result&                  out_result) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12Shader DX12Device::CreateShader(wis::span<const std::uint8_t> data,
 *                                                        wis::Result&                  out_result) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisDeviceCreateShader_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisDevice instance.
 * - `data` points to an array of shader bytecode.
 * - `size` indicates size of shader data in bytes; for SPIR-V it @wis_must be a multiple of 4.
 * - `shader` points to WisShader, which is initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisDeviceCreateShader_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDeviceCreateShader_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
