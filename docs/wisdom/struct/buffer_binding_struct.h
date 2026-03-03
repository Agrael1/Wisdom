/**
 * @struct WisBufferBinding
 * @ingroup Structures
 *
 *
 * @section WisBufferBinding_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisBufferBinding {
 *     uint64_t array_offset;
 *     uint32_t stride_bytes;
 *     uint32_t structure_count;
 * } WisBufferBinding;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  BufferBinding {
 *     std::uint64_t array_offset;
 *     std::uint32_t stride_bytes;
 *     std::uint32_t structure_count;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBufferBinding_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `array_offset` defines offset in buffer in structures.
 * - `stride_bytes` defines the size of the single structure in buffer.
 * - `structure_count` defines the number of structures in the buffer region to bind.
 * \endcond
 *
 * @section WisBufferBinding_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBufferBinding_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see wisDescriptorHeapWriteStructuredBuffer, wisDescriptorHeapWriteRWStructuredBuffer
 * \endcond
 */