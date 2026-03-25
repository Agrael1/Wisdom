/**
 * @struct WisViewport
 * @ingroup Structures
 *
 *
 * @section WisViewport_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisViewport {
 *     float x;
 *     float y;
 *     float width;
 *     float height;
 *     float min_depth;
 *     float max_depth;
 * } WisViewport;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  Viewport {
 *     float x;
 *     float y;
 *     float width;
 *     float height;
 *     float min_depth;
 *     float max_depth;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisViewport_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `x` Top left corner x coordinate.
 * - `y` Top left corner y coordinate.
 * - `width` Viewport width.
 * - `height` Viewport height.
 * - `min_depth` Minimum depth of the viewport.
 * - `max_depth` Maximum depth of the viewport.
 * \endcond
 *
 * @section WisViewport_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisViewport_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListSetViewports
 * \endcond
 */