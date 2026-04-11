/**
 * @struct wisXCBExtensionCreateSurface
 * @ingroup Functions Platform
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
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKSurface VKXCBExtension::CreateSurface(const wis::XCBWindowDesc& info,
 *                                                            wis::Result&              out_result) noexcept;
 * }
 * ```
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
