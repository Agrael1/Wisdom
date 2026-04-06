/**
 * @struct WisWaylandWindowDesc
 * @ingroup Structures Platform
 *
 *
 * @section WisWaylandWindowDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisWaylandWindowDesc {
 *     void* display;
 *     void* surface;
 * } WisWaylandWindowDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  WaylandWindowDesc {
 *     void* display;
 *     void* surface;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisWaylandWindowDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `display` Pointer to the Wayland display. Cast to wl_display* internally.
 * - `surface` Pointer to the Wayland surface. Cast to wl_surface* internally.
 * \endcond
 *
 * @section WisWaylandWindowDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisWaylandWindowDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisWaylandExtensionCreateSurface
 * \endcond
 */