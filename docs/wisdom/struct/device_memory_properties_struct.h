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
 * \endcond
 *
 * @section WisDeviceMemoryProperties_descr Description
 * <hr>
 *
 * `gpu_upload_supported` means that the device has a memory type that is both HOST_VISIBLE and DEVICE_LOCAL. That
 * allows writes to memory directly using CPU mapping.
 * `host_image_copy_supported` means that the device supports copying data directly from CPU memory to optimal tiled
 * image layout on GPU, without the need for an intermediate staging buffer. This can improve performance and reduce
 * memory usage when uploading textures from CPU to GPU.
 *
 * DirectX 12 supports both of these feature simultaneusly. That means that on DirectX 12, if `gpu_upload_supported` is
 * true, then `host_image_copy_supported` will also be true. On Vulkan, these features are independent and may be
 * supported separately. On Vulkan, `host_image_copy_supported` requires the `VK_EXT_host_image_copy` extension, while
 * `gpu_upload_supported` depends on the presence of a memory type that is both HOST_VISIBLE and DEVICE_LOCAL.
 *
 * If `gpu_upload_supported` is true, `WisMemoryTypeGPUUpload` memory type can be used for resource allocation. This
 * memory type allows mapping the memory and writing to it from CPU, while being accessible from GPU.
 *
 * If `host_image_copy_supported` is true, `wisTextureWriteSubresource` function can be used to write texture data
 * directly from CPU memory to optimal tiled image layout on GPU.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDeviceMemoryProperties_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
