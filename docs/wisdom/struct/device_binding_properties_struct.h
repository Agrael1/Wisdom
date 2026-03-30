/**
 * @struct WisDeviceBindingProperties
 * @ingroup Structures
 *
 *
 * @section WisDeviceBindingProperties_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisDeviceBindingProperties {
 *     WisQueryPropertyType property_type;
 *     void*                next_in_chain;
 *     uint32_t             max_vertex_input_attributes;
 *     uint32_t             max_vertex_input_bindings;
 *     bool                 multiple_viewports_supported;
 * } WisDeviceBindingProperties;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  DeviceBindingProperties {
 *     wis::QueryPropertyType property_type;
 *     void*                  next_in_chain;
 *     std::uint32_t          max_vertex_input_attributes;
 *     std::uint32_t          max_vertex_input_bindings;
 *     bool                   multiple_viewports_supported;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDeviceBindingProperties_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `property_type` specifies the type of the queried property. Used to determine what struct is passed. @wis_must be `WisQueryPropertyTypeDeviceBindingProperties`.
 * - `next_in_chain` indicates pointer to the next queried data struct.
 * - `max_vertex_input_attributes` defines maximum number of vertex input attributes supported by the device. Used for vertex buffer bindings.
 * - `max_vertex_input_bindings` indicates maximum number of vertex input bindings supported by the device. Used for vertex buffer bindings.
 * - `multiple_viewports_supported` indicates if multiple viewports are supported. If true, the device supports up to 16 viewports and scissor rectangles. If false, only one viewport and scissor rectangle is supported.
 * \endcond
 *
 * @section WisDeviceBindingProperties_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDeviceBindingProperties_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */