/**
 * @struct wisCreateInstance
 * @ingroup Functions
 *
 * 
 * @section CreateInstance_spec C Specification
 * <hr>
 * 
 * \cond WIS_GEN_CODE
 * General Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 *  WisResult wisCreateInstance(bool                         debug_layer,
 *                              WisInstanceExtensionHeader** extensions,
 *                              size_t                       extension_count,
 *                              WisInstance*                 instance);
 * ```
 * Vulkan Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 *  WisResult wisVKCreateInstance(bool                           debug_layer,
 *                                WisVKInstanceExtensionHeader** extensions,
 *                                size_t                         extension_count,
 *                                WisVKInstance*                 instance);
 * ```
 * DX12 Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 *  WisResult wisDX12CreateInstance(bool                             debug_layer,
 *                                  WisDX12InstanceExtensionHeader** extensions,
 *                                  size_t                           extension_count,
 *                                  WisDX12Instance*                 instance);
 * ```
 * \endcond
 * 
 * @section CreateInstance_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - `debug_layer` defines if the instance is to be created with debug mode.
 * - `extensions` points to an array of extensions that are to be initialized with Instance.
 * - `extension_count` counts the number of extensions in the `extensions` array.
 * - `instance` points to WisInstance, which will be initialized on success (`WisStatusOk`).
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section CreateInstance_descr Description
 * <hr>
 * \cond WIS_GEN_WIS_IDS
 * @validusage_begin
 * @vuid_begin{WIS-wisCreateInstance-extensions-null} If `extension_count` is greater than 0, `extensions` @wis_must be a valid pointer to an array of `extension_count` valid WisInstanceExtensionHeader handles. @vuid_end
 * @vuid_begin{WIS-wisCreateInstance-instance-null} `instance` @wis_mustnot be null. @vuid_end
 * @validusage_end
 * \endcond
 *
 * @section CreateInstance_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */