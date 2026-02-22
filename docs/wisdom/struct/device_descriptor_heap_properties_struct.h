/**
 * @struct WisDeviceDescriptorHeapProperties
 * @ingroup Structures
 *
 *
 * @section WisDeviceDescriptorHeapProperties_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisDeviceDescriptorHeapProperties {
 *     WisQueryPropertyType property_type;
 *     void*                next_in_chain;
 *     size_t               max_descriptor_heap_size;
 *     size_t               max_sampler_heap_size;
 *     size_t               max_sampler_heap_size_with_embedded;
 *     size_t               descriptor_increment_size;
 *     size_t               sampler_increment_size;
 * } WisDeviceDescriptorHeapProperties;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  DeviceDescriptorHeapProperties {
 *     wis::QueryPropertyType property_type;
 *     void*                  next_in_chain;
 *     std::size_t            max_descriptor_heap_size;
 *     std::size_t            max_sampler_heap_size;
 *     std::size_t            max_sampler_heap_size_with_embedded;
 *     std::size_t            descriptor_increment_size;
 *     std::size_t            sampler_increment_size;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDeviceDescriptorHeapProperties_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `property_type` Defines the type of the queried property. Used to determine what struct is passed. @wis_must be `WisQueryPropertyTypeDeviceDescriptorHeapProperties`.
 * - `next_in_chain` Pointer to the next queried data struct.
 * - `max_descriptor_heap_size` Maximum number of descriptors in a single descriptor heap.
 * - `max_sampler_heap_size` Maximum number of samplers in a single descriptor heap.
 * - `max_sampler_heap_size_with_embedded` Maximum number of samplers in a single descriptor heap, if embedded samplers are used.
 * - `descriptor_increment_size` Size of a single descriptor in the descriptor heap. Used for calculating descriptor offsets.
 * - `sampler_increment_size` Size of a single sampler in the sampler heap. Used for calculating sampler offsets.
 * \endcond
 *
 * @section WisDeviceDescriptorHeapProperties_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDeviceDescriptorHeapProperties_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */