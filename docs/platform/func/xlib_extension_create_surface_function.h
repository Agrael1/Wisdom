/**
 * @struct wisXlibExtensionCreateSurface
 * @ingroup Functions Platform
 *
 *
 * @section wisXlibExtensionCreateSurface_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisXlibExtensionCreateSurface(WisXlibExtension* self,
 *                                         const WisXlibWindowDesc* info,
 *                                         WisSurface*              surface);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKXlibExtensionCreateSurface(WisVKXlibExtension* self,
 *                                           const WisXlibWindowDesc* info,
 *                                           WisVKSurface*            surface);
 * 
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD wis::VKSurface VKXlibExtension::CreateSurface(const wis::XlibWindowDesc& info,
 *                                                             wis::Result&               out_result) noexcept;
 * }
 * ```
 * \endcond
 *
 * @section wisXlibExtensionCreateSurface_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisXlibExtension instance.
 * - `info` Xlib windowing data.
 * - `surface` points to WisSurface, initialized on success.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisXlibExtensionCreateSurface_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisXlibExtensionCreateSurface_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
