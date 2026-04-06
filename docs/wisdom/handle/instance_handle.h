/**
 * @struct WisInstance
 * @ingroup Handles Core
 *
 *
 * @section WisInstance_spec Specification
 * <hr>
 *
 * There is no global state in Wisdom. All per-application state is stored in an instance of `WisInstance`.
 * Depending on the underlying graphics API, the instance may hold different resources.
 * Most notably, in Vulkan, the instance holds a `VkInstance` handle, while in DX12, it may hold a `IDXGIFactory` or similar object.
 * Vulkan implementation also carries global function pointers needed to call Vulkan functions.
 *
 * \cond WIS_GEN_CODE
 *  Vulkan Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WIS_DEFINE_HANDLE(WisVKInstance,3);
 * ```
 *  DX12 Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WIS_DEFINE_HANDLE(WisDX12Instance,2);
 * ```
 * \endcond
 *
 * @section WisInstance_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDestroyInstance, wisCreateInstance, wisInstanceQueryAdapters
 * \endcond
 */
