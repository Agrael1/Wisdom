/**
 * @struct WisDeviceMemoryProperties
 * @ingroup Structures Core
 *
 * Structure describing memory properties of the device.
 * Pass it to `wisDeviceQueryProperties` either directly or chained to another query structure for it to be filled.
 *
 * @section WisDeviceMemoryProperties_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisDeviceMemoryProperties {
 *     WisQueryPropertyType property_type;
 *     void*                next_in_chain;
 *     bool                 gpu_upload_supported;
 *     bool                 host_image_copy_supported;
 *     uint64_t             gpu_upload_heap_budget;
 * } WisDeviceMemoryProperties;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  DeviceMemoryProperties {
 *     wis::QueryPropertyType property_type;
 *     void*                  next_in_chain;
 *     bool                   gpu_upload_supported;
 *     bool                   host_image_copy_supported;
 *     std::uint64_t          gpu_upload_heap_budget;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDeviceMemoryProperties_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `property_type` specifies the type of the queried property. @wis_must be
 * `WisQueryPropertyTypeDeviceMemoryProperties`..
 * - `next_in_chain` indicates pointer to the next queried data struct.
 * - `gpu_upload_supported` describes if GPU upload memory type is supported. This memory type allows mapping the memory
 * and writing to it from CPU, while being accessible from GPU. It is usually implemented as write-combined memory on
 * integrated GPUs and as a part of shared system memory on discrete GPUs.
 * - `host_image_copy_supported` indicates if host image copy is supported. This feature allows copying data directly
 * from CPU memory to optimal tiled image layout on GPU, without the need for an intermediate staging buffer. It is
 * supported on Windows 10 22H2 and later with WDDM 3.0 or later. On Vulkan it requires `VK_EXT_host_image_copy`
 * extension.
 * - `gpu_upload_heap_budget` specifies heap budget for GPU upload memory type in bytes. This is an approximate value of
 * how much memory of this type can be allocated, and it can change over time depending on the system state.
 * \endcond
 *
 * @section WisDeviceMemoryProperties_descr Description
 * <hr>
 *
 * `gpu_upload_heap_budget` is a metric of how much memory of the GPU upload type can be allocated, but it is not a hard
 * limit. It is possible that allocations of this type may fail even if the total allocated memory is below this budget,
 * due to fragmentation or other factors. Conversely, it may be possible to allocate more memory than this budget in
 * some cases. This value should be used as a guideline for how much memory of this type to allocate, rather than a
 * strict limit.
 *
 * `gpu_upload_heap_budget` has 3 possible states:
 * - If the device does not support GPU upload memory type, this value will be 0.
 * - If the device supports GPU upload memory, but the ReBAR is not enabled, this value will be 0 on DirectX 12 and a
 * small value on Vulkan (e.g., 256MB), representing the portion of shared system memory that is accessible to the GPU.
 * The value is exposed even if ReBAR is not enabled, because in some cases allocation such a small amount may be
 * beneficial, because it may be faster to access than regular Upload Heap.
 * - If the device supports GPU upload memory and ReBAR is enabled, this value will be either equal to the size of
 * dedicated video memory or a nearby value, depending on how the system allocates memory for the GPU upload type. This
 * means that whole video memory is accessible by CPU and uploads can be done without staging buffers.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDeviceMemoryProperties_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
