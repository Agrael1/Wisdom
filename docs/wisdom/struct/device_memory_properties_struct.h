/**
 * @struct WisDeviceMemoryProperties
 * @ingroup Structures Core
 *
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
 *     uint32_t             supported_initial_transitions;
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
 *     std::uint32_t          supported_initial_transitions;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDeviceMemoryProperties_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `property_type` specifies the type of the queried property. @wis_must be `WisQueryPropertyTypeDeviceMemoryProperties`..
 * - `next_in_chain` indicates pointer to the next queried data struct.
 * - `gpu_upload_supported` describes if GPU upload memory type is supported. This memory type allows mapping the memory and writing to it from CPU, while being accessible from GPU. It is usually implemented as write-combined memory on integrated GPUs and as a part of shared system memory on discrete GPUs.
 * - `host_image_copy_supported` indicates if host image copy is supported. This feature allows copying data directly from CPU memory to optimal tiled image layout on GPU, without the need for an intermediate staging buffer. It is supported on Windows 10 22H2 and later with WDDM 3.0 or later. On Vulkan it requires `VK_EXT_host_image_copy` extension.
 * - `supported_initial_transitions` defines bitfield of supported initial resource state transitions for buffers and textures. If a transition is supported, the corresponding bit is set to `1`, otherwise `0`. Bit positions are the same as in WisTextureState enum. `WisTextureStateUndefined` is always supported.
 * \endcond
 *
 * @section WisDeviceMemoryProperties_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDeviceMemoryProperties_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
