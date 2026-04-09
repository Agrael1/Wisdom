/**
 * @struct wisWin32ExtensionCreateSurface
 * @ingroup Functions Platform
 *
 *
 * @section wisWin32ExtensionCreateSurface_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisWin32ExtensionCreateSurface(WisWin32Extension* self,
 *                                          const WisWin32WindowDesc* info,
 *                                          WisSurface*               surface);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKWin32ExtensionCreateSurface(WisVKWin32Extension* self,
 *                                            const WisWin32WindowDesc* info,
 *                                            WisVKSurface*             surface);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12Win32ExtensionCreateSurface(WisDX12Win32Extension* self,
 *                                              const WisWin32WindowDesc* info,
 *                                              WisDX12Surface*           surface);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::Surface Win32Extension::CreateSurface(const wis::Win32WindowDesc& info,
 *                                                          wis::Result&                out_result) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKSurface VKWin32Extension::CreateSurface(const wis::Win32WindowDesc& info,
 *                                                              wis::Result&                out_result) noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::DX12Surface DX12Win32Extension::CreateSurface(const wis::Win32WindowDesc& info,
 *                                                                  wis::Result&                out_result) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisWin32ExtensionCreateSurface_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisWin32Extension instance.
 * - `info` Win32 windowing data.
 * - `surface` points to WisSurface, initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisWin32ExtensionCreateSurface_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisWin32ExtensionCreateSurface_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
