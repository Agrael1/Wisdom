/**
 * @struct WisMemoryType WisMemoryType
 * @ingroup Enumerations Core
 *
 * @section WisMemoryType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisMemoryType {
 *     WisMemoryTypeDefault = 0,
 *     WisMemoryTypeDeviceLocal = 0,
 *     WisMemoryTypeUpload = 1,
 *     WisMemoryTypeReadback = 2,
 *     WisMemoryTypeGPUUpload = 3,
 * } WisMemoryType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class MemoryType {
 *     Default = 0,
 *     DeviceLocal = 0,
 *     Upload = 1,
 *     Readback = 2,
 *     GPUUpload = 3,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisMemoryType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Memory type for resource allocation.
 *
 * \note Translates to  `D3D12_HEAP_TYPE` for DirectX 12 implementation, and `VkMemoryPropertyFlags` for Vulkan implementation.
 *
 * Values:
 * - `WisMemoryTypeDefault = 0`: Default memory type. Alias for `WisMemoryTypeDeviceLocal`
 * - `WisMemoryTypeDeviceLocal = 0`: Default memory type.
 * Local device memory, most efficient for rendering.
 * - `WisMemoryTypeUpload = 1`: Upload memory type.
 * Used for data that is uploaded to the GPU Local memory using copy operations.
 * - `WisMemoryTypeReadback = 2`: Readback memory type.
 * Used for data that is read back from the GPU Local memory using copy operations.
 * - `WisMemoryTypeGPUUpload = 3`: GPU upload memory type.
 * Used for data that is directly uploaded to the GPU Local memory using copy operations.
 * Support of this memory @wis_must be queried.
 * \endcond
 *
 *
 * @section WisMemoryType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBufferDesc, WisTextureDesc
 * \endcond
 */