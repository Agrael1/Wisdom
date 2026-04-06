/**
 * @struct wisWaylandExtensionCreateSurface
 * @ingroup Functions Platform
 *
 *
 * @section wisWaylandExtensionCreateSurface_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisWaylandExtensionCreateSurface(WisWaylandExtension* self,
 *                                            const WisWaylandWindowDesc* info,
 *                                            WisSurface*                 surface);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKWaylandExtensionCreateSurface(WisVKWaylandExtension* self,
 *                                              const WisWaylandWindowDesc* info,
 *                                              WisVKSurface*               surface);
 *
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKSurface VKWaylandExtension::CreateSurface(const wis::WaylandWindowDesc& info,
 *                                                                wis::Result&                  out_result) noexcept;
 * }
 * ```
 * \endcond
 *
 * @section wisWaylandExtensionCreateSurface_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisWaylandExtension instance.
 * - `info` Wayland windowing data.
 * - `surface` points to WisSurface, initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisWaylandExtensionCreateSurface_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisWaylandExtensionCreateSurface_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */