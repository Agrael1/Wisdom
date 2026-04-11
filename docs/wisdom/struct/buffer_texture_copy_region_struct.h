/**
 * @struct WisBufferTextureCopyRegion
 * @ingroup Structures Core
 *
 *
 * @section WisBufferTextureCopyRegion_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisBufferTextureCopyRegion {
 *     uint64_t         buffer_offset;
 *     uint32_t         buffer_row_length;
 *     uint32_t         buffer_image_height;
 *     WisTextureRegion texture_region;
 * } WisBufferTextureCopyRegion;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  BufferTextureCopyRegion {
 *     std::uint64_t      buffer_offset;
 *     std::uint32_t      buffer_row_length;
 *     std::uint32_t      buffer_image_height;
 *     wis::TextureRegion texture_region;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBufferTextureCopyRegion_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `buffer_offset` specifies buffer offset in bytes.
 * - `buffer_row_length` describes buffer row length in pixels. Used for calculating the offset in the buffer for each row of the texture.
 * - `buffer_image_height` describes buffer image height in pixels. Used for calculating the offset in the buffer for each image of the texture.
 * - `texture_region` describes texture region to copy.
 * \endcond
 *
 * @section WisBufferTextureCopyRegion_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBufferTextureCopyRegion_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListCopyBufferToTexture, wisCommandListCopyTextureToBuffer
 * \endcond
 */
