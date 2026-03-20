/**
 * @struct WisWin32WindowDesc
 * @ingroup Structures
 *
 *
 * @section WisWin32WindowDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisWin32WindowDesc {
 *     void* hinstance;
 *     void* hwnd;
 * } WisWin32WindowDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  Win32WindowDesc {
 *     void* hinstance;
 *     void* hwnd;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisWin32WindowDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `hinstance` HINSTANCE of the window. Cast to HINSTANCE internally.
 * - `hwnd` HWND of the window. Cast to HWND internally.
 * \endcond
 *
 * @section WisWin32WindowDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisWin32WindowDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisWin32ExtensionCreateSurface
 * \endcond
 */