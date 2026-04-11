/**
 * @struct WisConstantBufferBinding
 * @ingroup Structures Core
 *
 *
 * @section WisConstantBufferBinding_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisConstantBufferBinding {
 *     uint64_t buffer_address;
 *     uint32_t size_bytes;
 * } WisConstantBufferBinding;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  ConstantBufferBinding {
 *     std::uint64_t buffer_address;
 *     std::uint32_t size_bytes;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisConstantBufferBinding_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `buffer_address` indicates buffer device address to bind.
 * - `size_bytes` describes the size of the buffer region to bind in bytes.
 * \endcond
 *
 * @section WisConstantBufferBinding_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisConstantBufferBinding_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDescriptorHeapWriteConstantBuffer
 * \endcond
 */
