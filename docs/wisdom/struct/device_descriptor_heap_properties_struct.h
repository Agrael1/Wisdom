/**
 * @struct WisDeviceDescriptorHeapProperties
 * @ingroup Structures Core
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
 *     uint32_t             max_descriptor_heap_size;
 *     uint32_t             max_sampler_heap_size;
 *     uint32_t             max_sampler_heap_size_with_embedded;
 *     uint32_t             descriptor_increment_size;
 *     uint32_t             sampler_increment_size;
 *     uint32_t             render_target_increment_size;
 *     uint32_t             depth_stencil_increment_size;
 *     uint32_t             render_target_with_ms_increment_size;
 *     uint32_t             depth_stencil_with_ms_increment_size;
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
 *     std::uint32_t          max_descriptor_heap_size;
 *     std::uint32_t          max_sampler_heap_size;
 *     std::uint32_t          max_sampler_heap_size_with_embedded;
 *     std::uint32_t          descriptor_increment_size;
 *     std::uint32_t          sampler_increment_size;
 *     std::uint32_t          render_target_increment_size;
 *     std::uint32_t          depth_stencil_increment_size;
 *     std::uint32_t          render_target_with_ms_increment_size;
 *     std::uint32_t          depth_stencil_with_ms_increment_size;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDeviceDescriptorHeapProperties_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `property_type` specifies the type of the queried property. Used to determine what struct is passed. @wis_must be
 * `WisQueryPropertyTypeDeviceDescriptorHeapProperties`.
 * - `next_in_chain` indicates pointer to the next queried data struct.
 * - `max_descriptor_heap_size` indicates maximum number of descriptors in a single descriptor heap.
 * - `max_sampler_heap_size` defines maximum number of samplers in a single descriptor heap.
 * - `max_sampler_heap_size_with_embedded` defines maximum number of samplers in a single descriptor heap, if embedded
 * samplers are used.
 * - `descriptor_increment_size` describes size of a single descriptor in the descriptor heap. Used for calculating
 * descriptor offsets.
 * - `sampler_increment_size` specifies size of a single sampler in the sampler heap. Used for calculating sampler
 * offsets.
 * - `render_target_increment_size` defines size of a single render target view descriptor in the descriptor heap. Used
 * for calculating render target view descriptor offsets.
 * - `depth_stencil_increment_size` defines size of a single depth stencil view descriptor in the descriptor heap. Used
 * for calculating depth stencil view descriptor offsets.
 * - `render_target_with_ms_increment_size` defines size of a single render target view descriptor in the descriptor
 * heap with multisample targets enabled. Used for calculating render target view descriptor offsets.
 * - `depth_stencil_with_ms_increment_size` defines size of a single depth stencil view descriptor in the descriptor
 * heap with multisample targets enabled. Used for calculating depth stencil view descriptor offsets. \endcond
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
