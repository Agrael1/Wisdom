/**
 * @struct WisXCBWindowDesc
 * @ingroup Structures
 *
 *
 * @section WisXCBWindowDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisXCBWindowDesc {
 *     void*    connection;
 *     uint32_t window;
 * } WisXCBWindowDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  XCBWindowDesc {
 *     void*         connection;
 *     std::uint32_t window;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisXCBWindowDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `connection` Pointer to the XCB connection. Cast to xcb_connection_t* internally.
 * - `window` The X11 Window ID. Fits standard 32/64-bit window handles.
 * \endcond
 *
 * @section WisXCBWindowDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisXCBWindowDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see wisWin32ExtensionCreateSurface
 * \endcond
 */