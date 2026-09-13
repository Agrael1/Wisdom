/**
 * @struct WisXlibWindowDesc
 * @ingroup Structures Platform
 *
 *
 * @section WisXlibWindowDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisXlibWindowDesc {
 *     void*    display;
 *     uint64_t window;
 * } WisXlibWindowDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  XlibWindowDesc {
 *     void*         display;
 *     std::uint64_t window;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisXlibWindowDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `display` Pointer to the X11 Display. Cast to Display* internally.
 * - `window` The X11 Window ID. Fits standard 32/64-bit window handles.
 * \endcond
 *
 * @section WisXlibWindowDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisXlibWindowDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisXlibExtensionCreateSurface
 * \endcond
 */
