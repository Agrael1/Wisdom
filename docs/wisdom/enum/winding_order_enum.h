/**
 * @struct WisWindingOrder WisWindingOrder
 * @ingroup Enumerations
 *
 * @section WisWindingOrder_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisWindingOrder {
 *     WisWindingOrderClockwise = 0,
 *     WisWindingOrderCounterClockwise = 1,
 * } WisWindingOrder;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class WindingOrder {
 *     Clockwise = 0,
 *     CounterClockwise = 1,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisWindingOrder_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Winding order for front-facing triangles.
 * 
 * \note Translates to  `BOOL` for DirectX 12 implementation, and `VkFrontFace` for Vulkan implementation.
 * 
 * Values:
 * - `WisWindingOrderClockwise = 0`: Front-facing triangles have clockwise winding order.
 * - `WisWindingOrderCounterClockwise = 1`: Front-facing triangles have counter-clockwise winding order.
 * \endcond
 *
 *
 * @section WisWindingOrder_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRasterizerDesc
 * \endcond
 */