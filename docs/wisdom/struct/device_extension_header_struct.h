/**
 * @struct WisDeviceExtensionHeader
 * @ingroup Structures
 *
 *
 * @section WisDeviceExtensionHeader_spec Specification
 * <hr>
 * 
 * An opaque structure used to extend device functionality. Serves as a type marker for device extensions.
 * Defined only for C API. C++ API uses special polymorphic classes for instance extensions.
 * 
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDeviceExtensionHeader {
 *     const void* opaque;
 * } WisDeviceExtensionHeader;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKDeviceExtensionHeader {
 *     const void* opaque;
 * } WisVKDeviceExtensionHeader;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12DeviceExtensionHeader {
 *     const void* opaque;
 * } WisDX12DeviceExtensionHeader;
 * ```
 * </details>
 * 
 * \endcond
 *
 * @section WisDeviceExtensionHeader_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `opaque` defines opaque member. In @wis_mustnot be changed directly outside extension development.
 * \endcond
 *
 * @section WisDeviceExtensionHeader_descr Description
 * <hr>
 * 
 * This structure is always present in every device extension and @wis_must be initialized with extension-specific initialization function before being passed to `wisCreateDevice`.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDeviceExtensionHeader_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see WisDeviceRequirements
 * \endcond
 */
