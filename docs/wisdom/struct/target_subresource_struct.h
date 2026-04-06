/**
 * @struct WisTargetSubresource
 * @ingroup Structures Core
 *
 *
 * @section WisTargetSubresource_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisTargetSubresource {
 *     uint16_t mip_level;
 *     uint16_t array_layer;
 *     uint16_t plane_slice;
 * } WisTargetSubresource;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  TargetSubresource {
 *     std::uint16_t mip_level;
 *     std::uint16_t array_layer;
 *     std::uint16_t plane_slice;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTargetSubresource_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `mip_level` describes mipmap level of the target subresource.
 * - `array_layer` indicates array layer of the target subresource; for 3D textures it @wis_must be 0.
 * - `plane_slice` indicates depth slice of the target subresource. Used only for 2D textures (YUV).
 * \endcond
 *
 * @section WisTargetSubresource_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisTargetSubresource_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisTextureRegion
 * \endcond
 */