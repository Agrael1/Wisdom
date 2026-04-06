/**
 * @struct WisSubresource
 * @ingroup Structures Core
 *
 *
 * @section WisSubresource_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisSubresource {
 *     uint16_t base_mip_level;
 *     uint16_t mip_level_count;
 *     uint16_t base_array_layer;
 *     uint16_t array_layer_count;
 * } WisSubresource;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  Subresource 
 * ```
 * 
 * \endcond
 *
 * @section WisSubresource_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - `base_mip_level` Mipmap level of the subresource.
 * - `mip_level_count` Number of mip levels in the subresource.
 * - `base_array_layer` Array layer of the subresource. For 3D textures, this defines the depth slice.
 * - `array_layer_count` Number of array layers in the subresource. For 3D textures, this defines the number of depth slices.
 * 
 * \endcond
 *
 * @section WisSubresource_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisSubresource_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
