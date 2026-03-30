/**
 * @struct WisBufferDesc
 * @ingroup Structures
 *
 *
 * @section WisBufferDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisBufferDesc {
 *     uint64_t            size_bytes;
 *     WisBufferUsageFlags usage_flags;
 *     WisMemoryType       memory_type;
 *     WisMemoryFlags      memory_flags;
 * } WisBufferDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  BufferDesc {
 *     std::uint64_t         size_bytes;
 *     wis::BufferUsageFlags usage_flags;
 *     wis::MemoryType       memory_type;
 *     wis::MemoryFlags      memory_flags;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBufferDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `size_bytes` describes size of the buffer in bytes.
 * - `usage_flags` describes buffer usage flags. Describe how the buffer will be used.
 * - `memory_type` specifies where the buffer will be allocated.
 * - `memory_flags` describes the flags of the memory to allocate for the buffer.
 * \endcond
 *
 * @section WisBufferDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBufferDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisResourceAllocatorCreateBuffer
 * \endcond
 */