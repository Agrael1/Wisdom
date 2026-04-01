/**
 * @struct WisIndexBufferDesc
 * @ingroup Structures
 *
 *
 * @section WisIndexBufferDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisIndexBufferDesc {
 *     WisBufferView buffer;
 *     uint32_t      size;
 *     uint32_t      offset;
 * } WisIndexBufferDesc;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKIndexBufferDesc {
 *     WisVKBufferView buffer;
 *     uint32_t        size;
 *     uint32_t        offset;
 * } WisVKIndexBufferDesc;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12IndexBufferDesc {
 *     WisDX12BufferView buffer;
 *     uint32_t          size;
 *     uint32_t          offset;
 * } WisDX12IndexBufferDesc;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct IndexBufferDesc {
 *     wis::BufferView buffer;
 *     std::uint32_t   size;
 *     std::uint32_t   offset;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKIndexBufferDesc {
 *     wis::VKBufferView buffer;
 *     std::uint32_t     size;
 *     std::uint32_t     offset;
 * };
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12IndexBufferDesc {
 *     wis::DX12BufferView buffer;
 *     std::uint32_t       size;
 *     std::uint32_t       offset;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisIndexBufferDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `buffer` Vertex Buffer to bind. The buffer view @wis_must have been created with  usage flag.
 * - `size` Size of the buffer in bytes.
 * - `offset` Offset in buffer in bytes. Default is 0.
 * \endcond
 *
 * @section WisIndexBufferDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisIndexBufferDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListSetIndexBuffer
 * \endcond
 */