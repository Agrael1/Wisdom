/**
 * @struct WisMemoryFlags WisMemoryFlags
 * @ingroup Enumerations
 *
 * @section WisMemoryFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisMemoryFlags {
 *     WisMemoryFlagsNone = 0,
 *     WisMemoryFlagsDedicatedAllocation = (1u << 0),
 *     WisMemoryFlagsMapped = (1u << 1),
 *     WisMemoryFlagsExportable = (1u << 2),
 * } WisMemoryFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class MemoryFlags : uint32_t {
 *     None = 0,
 *     DedicatedAllocation = (1u << 0),
 *     Mapped = (1u << 1),
 *     Exportable = (1u << 2),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisMemoryFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Memory flags.
 * Determine optional properties of the memory allocation.
 * 
 * \note Translates to DirectX 12 as D3D12MA::ALLOCATION_FLAGS, Vulkan as VmaAllocationCreateFlags.
 * Values:
 * - `WisMemoryFlagsNone = 0`: No flags set. Memory is regular.
 * - `WisMemoryFlagsDedicatedAllocation = (1 << 0)`: Memory is dedicated.
 * Used for resources that require dedicated memory.
 * Useful for big resources that are not shared with other resources.
 * E.g. fullscreen textures, big buffers, etc.
 * - `WisMemoryFlagsMapped = (1 << 1)`: Memory is mapped.
 * Used in combination with `WisMemoryTypeUpload` or `WisMemoryTypeReadback` to map memory for CPU access.
 * - `WisMemoryFlagsExportable = (1 << 2)`: Memory is exportable.
 * If set, memory can be exported to other processes or APIs.
 * Works only with Device Local memory (`WisMemoryTypeDefault`) and only on AllocateXMemory calls.
 * Outside of AllocateXMemory the flag is ignored.
 * \endcond
 *
 *
 * @section WisMemoryFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBufferDesc, WisTextureDesc
 * \endcond
 */