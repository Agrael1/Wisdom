/**
 * @struct WisVertexBufferDesc
 * @ingroup Structures Core
 *
 *
 * @section WisVertexBufferDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVertexBufferDesc {
 *     WisBufferView buffer;
 *     uint32_t      size;
 *     uint32_t      stride;
 *     uint32_t      offset;
 * } WisVertexBufferDesc;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKVertexBufferDesc {
 *     WisVKBufferView buffer;
 *     uint32_t        size;
 *     uint32_t        stride;
 *     uint32_t        offset;
 * } WisVKVertexBufferDesc;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12VertexBufferDesc {
 *     WisDX12BufferView buffer;
 *     uint32_t          size;
 *     uint32_t          stride;
 *     uint32_t          offset;
 * } WisDX12VertexBufferDesc;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VertexBufferDesc {
 *     wis::BufferView buffer;
 *     std::uint32_t   size;
 *     std::uint32_t   stride;
 *     std::uint32_t   offset;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKVertexBufferDesc {
 *     wis::VKBufferView buffer;
 *     std::uint32_t     size;
 *     std::uint32_t     stride;
 *     std::uint32_t     offset;
 * };
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12VertexBufferDesc {
 *     wis::DX12BufferView buffer;
 *     std::uint32_t       size;
 *     std::uint32_t       stride;
 *     std::uint32_t       offset;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisVertexBufferDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `buffer` Vertex Buffer to bind. The buffer view @wis_must have been created with  usage flag.
 * - `size` Size of the buffer in bytes.
 * - `stride` Stride of the buffer in bytes.
 * - `offset` Offset in buffer in bytes. Default is 0.
 * \endcond
 *
 * @section WisVertexBufferDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVertexBufferDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListSetVertexBuffers
 * \endcond
 */