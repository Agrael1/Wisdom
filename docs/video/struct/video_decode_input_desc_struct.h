/**
 * @struct WisVideoDecodeInputDesc
 * @ingroup Structures Video
 *
 *
 * @section WisVideoDecodeInputDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisVideoDecodeInputDesc {
 *     WisBufferView bitstream_buffer;
 *     uint64_t      offset;
 *     uint64_t      size;
 * } WisVideoDecodeInputDesc;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisVKVideoDecodeInputDesc {
 *     WisVKBufferView bitstream_buffer;
 *     uint64_t        offset;
 *     uint64_t        size;
 * } WisVKVideoDecodeInputDesc;
 *
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisDX12VideoDecodeInputDesc {
 *     WisDX12BufferView bitstream_buffer;
 *     uint64_t          offset;
 *     uint64_t          size;
 * } WisDX12VideoDecodeInputDesc;
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct VideoDecodeInputDesc {
 *     wis::BufferView bitstream_buffer;
 *     std::uint64_t   offset;
 *     std::uint64_t   size;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct VKVideoDecodeInputDesc {
 *     wis::VKBufferView bitstream_buffer;
 *     std::uint64_t     offset;
 *     std::uint64_t     size;
 * };
 *
 * // Provided by Wisdom 0.7.1.
 * struct DX12VideoDecodeInputDesc {
 *     wis::DX12BufferView bitstream_buffer;
 *     std::uint64_t       offset;
 *     std::uint64_t       size;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisVideoDecodeInputDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `bitstream_buffer` Input description for a video decode operation that uses a bitstream buffer as input. The buffer
 * view @wis_should contain the compressed video data to be decoded.
 * - `offset` Offset in the buffer where the bistream data is located.
 * - `size` Size of the bitstream data in bytes.
 * \endcond
 *
 * @section WisVideoDecodeInputDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVideoDecodeInputDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisVideoDecodeCommandListDecodeFrame
 * \endcond
 */
