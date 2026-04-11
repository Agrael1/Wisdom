/**
 * @struct wisDestroyInstance
 * @ingroup Functions Core
 *
 *
 * @section wisDestroyInstance_spec Specification
 * <hr>
 *
 * To destroy a Wisdom instance, call:
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisDestroyInstance(WisInstance* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKDestroyInstance(WisVKInstance* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12DestroyInstance(WisDX12Instance* self);
 * ```
 * </details>
 * 
 * \endcond
 *
 * @section wisDestroyInstance_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisInstance instance.
 * \endcond
 *
 * @section wisDestroyInstance_descr Description
 * <hr>
 *
 * In contrast to similar
 * [vkDestroyInstance](https://docs.vulkan.org/refpages/latest/refpages/source/vkDestroyInstance.html) in Vulkan,
 * wisDestroyInstance does not require all the objects to be destroyed prior to its invocation. The resources created
 * under the instance continue to exist after the instance is destroyed, and are cleaned up automatically when their
 * respective handles are destroyed.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisDestroyInstance_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
