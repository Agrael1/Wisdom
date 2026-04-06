/**
 * @struct WisBox
 * @ingroup Structures Core
 *
 *
 * @section WisBox_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisBox {
 *     uint32_t x;
 *     uint32_t y;
 *     uint32_t z;
 *     uint32_t width;
 *     uint32_t height;
 *     uint32_t depth;
 * } WisBox;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  Box {
 *     std::uint32_t x;
 *     std::uint32_t y;
 *     std::uint32_t z;
 *     std::uint32_t width;
 *     std::uint32_t height;
 *     std::uint32_t depth;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBox_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `x` defines left corner x coordinate.
 * - `y` describes top corner y coordinate.
 * - `z` specifies front corner z coordinate. Used only for 3D textures.
 * - `width` defines width of the box.
 * - `height` describes height of the box.
 * - `depth` describes depth of the box. Used only for 3D textures.
 * \endcond
 *
 * @section WisBox_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBox_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisTextureRegion
 * \endcond
 */
