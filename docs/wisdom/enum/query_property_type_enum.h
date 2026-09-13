/**
 * @struct WisQueryPropertyType WisQueryPropertyType
 * @ingroup Enumerations Core
 *
 * @section WisQueryPropertyType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisQueryPropertyType {
 *     WisQueryPropertyTypeDeviceCommandQueueProperties = 0,
 *     WisQueryPropertyTypeDeviceDescriptorHeapProperties = 1,
 *     WisQueryPropertyTypeDeviceMemoryProperties = 2,
 *     WisQueryPropertyTypeDeviceBindingProperties = 3,
 * } WisQueryPropertyType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class QueryPropertyType {
 *     DeviceCommandQueueProperties = 0,
 *     DeviceDescriptorHeapProperties = 1,
 *     DeviceMemoryProperties = 2,
 *     DeviceBindingProperties = 3,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisQueryPropertyType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Query type for GPU queries.
 *
 * Values:
 * - `WisQueryPropertyTypeDeviceCommandQueueProperties = 0`: Properties of the device command queues. Expects a
 * WisDeviceCommandQueueProperties struct.
 * - `WisQueryPropertyTypeDeviceDescriptorHeapProperties = 1`: Properties of the device descriptor heap. Expects a
 * WisDeviceDescriptorHeapProperties struct.
 * - `WisQueryPropertyTypeDeviceMemoryProperties = 2`: Properties of the device descriptor heap. Expects a
 * WisDeviceMemoryProperties struct.
 * - `WisQueryPropertyTypeDeviceBindingProperties = 3`: Properties of the device resource binding. Expects a
 * WisDeviceBindingProperties struct.
 * \endcond
 *
 *
 * @section WisQueryPropertyType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisQueryStructHeader, WisDeviceBindingProperties, WisDeviceDescriptorHeapProperties, WisDeviceCommandQueueProperties,
 * WisDeviceMemoryProperties
 * \endcond
 */
