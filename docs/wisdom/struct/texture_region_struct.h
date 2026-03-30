/**
 * @struct WisTextureRegion
 * @ingroup Structures
 *
 *
 * @section WisTextureRegion_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisTextureRegion {
 *     WisBox               box;
 *     WisTargetSubresource target_subresource;
 * } WisTextureRegion;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  TextureRegion {
 *     wis::Box               box;
 *     wis::TargetSubresource target_subresource;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTextureRegion_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `box` describes box defining the region to copy.
 * - `target_subresource` defines target subresource description for the region.
 * \endcond
 *
 * @section WisTextureRegion_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisTextureRegion_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisTextureWriteSubresource
 * \endcond
 */