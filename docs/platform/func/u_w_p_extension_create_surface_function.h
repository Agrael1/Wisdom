/**
 * @struct wisUWPExtensionCreateSurface
 * @ingroup Functions
 *
 *
 * @section wisUWPExtensionCreateSurface_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisUWPExtensionCreateSurface(WisUWPExtension* self,
 *                                        const WisUWPWindowDesc* info,
 *                                        WisSurface*             surface);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKUWPExtensionCreateSurface(WisVKUWPExtension* self,
 *                                          const WisUWPWindowDesc* info,
 *                                          WisVKSurface*           surface);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12UWPExtensionCreateSurface(WisDX12UWPExtension* self,
 *                                            const WisUWPWindowDesc* info,
 *                                            WisDX12Surface*         surface);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::Surface UWPExtension::CreateSurface(const wis::UWPWindowDesc& info,
 *                                                        wis::Result&              out_result) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKSurface VKUWPExtension::CreateSurface(const wis::UWPWindowDesc& info,
 *                                                            wis::Result&              out_result) noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12Surface DX12UWPExtension::CreateSurface(const wis::UWPWindowDesc& info,
 *                                                                wis::Result&              out_result) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisUWPExtensionCreateSurface_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisUWPExtension instance.
 * - `info` UWP windowing data.
 * - `surface` points to WisSurface, initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisUWPExtensionCreateSurface_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisUWPExtensionCreateSurface_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */