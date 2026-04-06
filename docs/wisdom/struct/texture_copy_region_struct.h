/**
 * @struct WisTextureCopyRegion
 * @ingroup Structures Core
 *
 *
 * @section WisTextureCopyRegion_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisTextureCopyRegion {
 *     WisTextureRegion src_region;
 *     WisTextureRegion dst_region;
 * } WisTextureCopyRegion;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  TextureCopyRegion {
 *     wis::TextureRegion src_region;
 *     wis::TextureRegion dst_region;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTextureCopyRegion_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `src_region` describes source texture region to copy.
 * - `dst_region` describes destination texture region to copy.
 * \endcond
 *
 * @section WisTextureCopyRegion_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisTextureCopyRegion_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListCopyTexture
 * \endcond
 */