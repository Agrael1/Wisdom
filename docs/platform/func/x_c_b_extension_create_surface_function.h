/**
 * @struct wisXCBExtensionCreateSurface
 * @ingroup Functions
 *
 *
 * @section wisXCBExtensionCreateSurface_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisXCBExtensionCreateSurface(WisXCBExtension* self,
 *                                        const WisXCBWindowDesc* info,
 *                                        WisSurface*             surface);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKXCBExtensionCreateSurface(WisVKXCBExtension* self,
 *                                          const WisXCBWindowDesc* info,
 *                                          WisVKSurface*           surface);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12XCBExtensionCreateSurface(WisDX12XCBExtension* self,
 *                                            const WisXCBWindowDesc* info,
 *                                            WisDX12Surface*         surface);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::Surface XCBExtension::CreateSurface(const wis::XCBWindowDesc& info,
 *                                                        wis::Result&              out_result) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKSurface VKXCBExtension::CreateSurface(const wis::XCBWindowDesc& info,
 *                                                            wis::Result&              out_result) noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12Surface DX12XCBExtension::CreateSurface(const wis::XCBWindowDesc& info,
 *                                                                wis::Result&              out_result) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisXCBExtensionCreateSurface_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisXCBExtension instance.
 * - `info` XCB windowing data.
 * - `surface` points to WisSurface, initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisXCBExtensionCreateSurface_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisXCBExtensionCreateSurface_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */