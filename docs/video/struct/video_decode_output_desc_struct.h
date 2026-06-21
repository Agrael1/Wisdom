/**
 * @struct WisVideoDecodeOutputDesc
 * @ingroup Structures Video
 *
 *
 * @section WisVideoDecodeOutputDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisVideoDecodeOutputDesc {
 *     WisTextureView output_texture;
 *     WisDataFormat  format;
 *     uint32_t       subresource;
 * } WisVideoDecodeOutputDesc;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisVKVideoDecodeOutputDesc {
 *     WisVKTextureView output_texture;
 *     WisDataFormat    format;
 *     uint32_t         subresource;
 * } WisVKVideoDecodeOutputDesc;
 *
 * // Provided by Wisdom 0.7.1.
 * typedef struct WisDX12VideoDecodeOutputDesc {
 *     WisDX12TextureView output_texture;
 *     WisDataFormat      format;
 *     uint32_t           subresource;
 * } WisDX12VideoDecodeOutputDesc;
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct VideoDecodeOutputDesc
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct VKVideoDecodeOutputDesc ;
 *
 * // Provided by Wisdom 0.7.1.
 * struct DX12VideoDecodeOutputDesc
 * ```
 * </details>
 *
 * \endcond
 *
 * @section WisVideoDecodeOutputDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - `output_texture` The texture that will receive the decoded video frame.
 * - `format` The data format of the output texture.
 * - `subresource` The subresource index of the texture to decode into.
 *
 * \endcond
 *
 * @section WisVideoDecodeOutputDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisVideoDecodeOutputDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
