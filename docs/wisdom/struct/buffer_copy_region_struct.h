/**
 * @struct WisBufferCopyRegion
 * @ingroup Structures
 *
 *
 * @section WisBufferCopyRegion_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisBufferCopyRegion {
 *     uint64_t src_offset;
 *     uint64_t dst_offset;
 *     uint64_t size_bytes;
 * } WisBufferCopyRegion;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  BufferCopyRegion {
 *     std::uint64_t src_offset;
 *     std::uint64_t dst_offset;
 *     std::uint64_t size_bytes;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBufferCopyRegion_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `src_offset` specifies source buffer offset in bytes.
 * - `dst_offset` describes destination buffer offset in bytes.
 * - `size_bytes` describes size of the region to copy in bytes.
 * \endcond
 *
 * @section WisBufferCopyRegion_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBufferCopyRegion_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListCopyBuffer
 * \endcond
 */