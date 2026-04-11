/**
 * @struct wisCreateInstance
 * @ingroup Functions Core
 *
 *
 * @section CreateInstance_spec Specification
 * <hr>
 *
 * To create a Wisdom instance, call:
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisCreateInstance(const WisDebugDesc*          debug_desc,
 *                             WisInstanceExtensionHeader** extensions,
 *                             size_t                       extension_count,
 *                             WisInstance*                 instance);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * WisResult wisVKCreateInstance(const WisDebugDesc*            debug_desc,
 *                               WisVKInstanceExtensionHeader** extensions,
 *                               size_t                         extension_count,
 *                               WisVKInstance*                 instance);
 *
 * // Provided by Wisdom 0.7.0.
 * WisResult wisDX12CreateInstance(const WisDebugDesc*              debug_desc,
 *                                 WisDX12InstanceExtensionHeader** extensions,
 *                                 size_t                           extension_count,
 *                                 WisDX12Instance*                 instance);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::Instance CreateInstance(const wis::DebugDesc*                    debug_desc,
 *                                            wis::span<wis::InstanceExtensionHeader*> extensions,
 *                                            wis::Result&                             out_result) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::VKInstance VKCreateInstance(const wis::DebugDesc*                      debug_desc,
 *                                                wis::span<wis::VKInstanceExtensionHeader*> extensions,
 *                                                wis::Result&                               out_result) noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD wis::DX12Instance DX12CreateInstance(const wis::DebugDesc*                        debug_desc,
 *                                                    wis::span<wis::DX12InstanceExtensionHeader*> extensions,
 *                                                    wis::Result&                                 out_result) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section CreateInstance_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - `debug_desc` describes a pointer to WisDebugDesc, which defines debug callback and debug layer usage. If `nullptr`, debug layer is disabled.
 * - `extensions` points to an array of extensions that are to be initialized with pointers to WisInstanceExtensionHeader.
 * - `extension_count` describes the number of the number of extensions in the  array.
 * - `instance` points to WisInstance, which is initialized on success.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section CreateInstance_descr Description
 * <hr>
 *
 * `debug_layer` enables additional validation and debugging features in the underlying graphics API. Works only for
 * DirectX 12 implementation, but reserved for future internal validation layers.
 *
 * `debug_layer` @wis_may have performance implications, and is recommended to be used only during development and
 * debugging phases. `extensions` allows the user to specify additional functionality or features to be enabled in the
 * created instance. Each extension is represented by a `WisInstanceExtensionHeader`, which @wis_must be properly
 * initialized before passing to this function. The creation procedure varies depending on the underlying graphics API.
 * Vulkan implementation tries to create a `VkInstance` with requested extensions enabled, passing necessary layers and
 * extensions to the [vkCreateInstance](https://docs.vulkan.org/refpages/latest/refpages/source/vkCreateInstance.html).
 *
 * The `extensions` @wis_may have overlapping required layers and extensions, which are deduplicated during instance
 * creation. After successful instance creation, the extensions are finalized, allowing them to set up any additional
 * state or resources needed for their functionality.
 *
 * Extensions are provided with extensive information during their initialization, and should check for compatibility
 * with the requested instance configuration. If any extension fails to initialize, the instance is still created, but
 * the return code is set to `WisStatusPartial`, indicating that not all requested features were successfully enabled.
 * Extensions @wis_should provide information about their initialization state through their own mechanisms, such as
 * success functions.
 *
 *
 * \note Vulkan instance is always created with following extensions enabled:
 *      "VK_KHR_surface",
 *      "VK_EXT_surface_maintenance1",
 *      "VK_KHR_get_surface_capabilities2",
 *      "VK_KHR_get_physical_device_properties2"
 *
 * \cond WIS_GEN_WIS_IDS
 * @validusage_begin
 * @vuid_begin{WIS-WisCreateInstance-extensions-null} If  extension_count is greater than 0,  extensions @wis_must be a valid pointer to an array of  extension_count valid WisInstanceExtensionHeader handles. @vuid_end
 * @vuid_begin{WIS-WisCreateInstance-instance-null}  instance @wis_mustnot be null. @vuid_end
 * @validusage_end
 * \endcond
 *
 * @section CreateInstance_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
