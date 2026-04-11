/**
 * @struct WisSubresourceRange
 * @ingroup Structures Core
 *
 *
 * @section WisSubresourceRange_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisSubresourceRange {
 *     uint16_t base_mip_level;
 *     uint16_t mip_level_count;
 *     uint16_t base_array_layer;
 *     uint16_t array_layer_count;
 *     uint16_t plane_slice;
 *     uint16_t plane_slice_count;
 * } WisSubresourceRange;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  SubresourceRange {
 *     std::uint16_t base_mip_level;
 *     std::uint16_t mip_level_count;
 *     std::uint16_t base_array_layer;
 *     std::uint16_t array_layer_count;
 *     std::uint16_t plane_slice;
 *     std::uint16_t plane_slice_count;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSubresourceRange_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `base_mip_level` indicates mipmap level of the subresource.
 * - `mip_level_count` describes number of mip levels in the subresource.
 * - `base_array_layer` describes array layer of the subresource. For 3D textures, this defines the depth slice.
 * - `array_layer_count` indicates number of array layers in the subresource. For 3D textures, this defines the number
 * of depth slices.
 * - `plane_slice` indicates base depth slice of the subresource. Used only for 2D textures (YUV).
 * - `plane_slice_count` indicates number of depth slices in the subresource. Used only for 2D textures (YUV). Max value
 * is 3. \endcond
 *
 * @section WisSubresourceRange_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisSubresourceRange_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisTextureBinding, WisTextureBarrier
 * \endcond
 */
