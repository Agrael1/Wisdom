/**
 * @struct wisInitWin32Extension
 * @ingroup Functions Platform
 *
 *
 * @section wisInitWin32Extension_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisInitWin32Extension(WisWin32Extension* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKInitWin32Extension(WisVKWin32Extension* self);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12InitWin32Extension(WisDX12Win32Extension* self);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * Win32Extension::Win32Extension() noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * VKWin32Extension::VKWin32Extension() noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * DX12Win32Extension::DX12Win32Extension() noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisInitWin32Extension_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` is a pointer to uninitialized WisWin32Extension instance memory. It will be initialized by this
 * function.
 * **note** The corresponding destroy function is `wisDestroyWin32Extension`.
 * \endcond
 *
 * @section wisInitWin32Extension_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisInitWin32Extension_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
