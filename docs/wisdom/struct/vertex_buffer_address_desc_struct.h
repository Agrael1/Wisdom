/**
 * @struct WisVertexBufferAddressDesc
 * @ingroup Structures
 *
 *
 * @section WisVertexBufferAddressDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisVertexBufferAddressDesc {
 *     uint64_t buffer;
 *     uint32_t size;
 *     uint32_t stride;
 * } WisVertexBufferAddressDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  VertexBufferAddressDesc {
 *     std::uint64_t buffer;
 *     std::uint32_t size;
 *     std::uint32_t stride;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisVertexBufferAddressDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `buffer` Buffer address.
 * - `size` Size of the buffer in bytes.
 * - `stride` Stride of the buffer in bytes.
 * \endcond
 *
 * @section WisVertexBufferAddressDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVertexBufferAddressDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListSetVertexBuffers2
 * \endcond
 */