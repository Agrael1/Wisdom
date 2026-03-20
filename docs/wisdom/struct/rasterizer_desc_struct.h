/**
 * @struct WisRasterizerDesc
 * @ingroup Structures
 *
 *
 * @section WisRasterizerDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisRasterizerDesc {
 *     WisFillMode                  fill_mode;
 *     WisCullMode                  cull_mode;
 *     WisWindingOrder              front_face;
 *     bool                         depth_bias_enable;
 *     float                        depth_bias;
 *     float                        depth_bias_clamp;
 *     float                        depth_bias_slope_factor;
 *     bool                         depth_clip_enable;
 *     WisLineRasterization         line_rasterization;
 *     WisConservativeRasterization conservative_rasterization;
 * } WisRasterizerDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  RasterizerDesc {
 *     wis::FillMode                  fill_mode;
 *     wis::CullMode                  cull_mode;
 *     wis::WindingOrder              front_face;
 *     bool                           depth_bias_enable;
 *     float                          depth_bias;
 *     float                          depth_bias_clamp;
 *     float                          depth_bias_slope_factor;
 *     bool                           depth_clip_enable;
 *     wis::LineRasterization         line_rasterization;
 *     wis::ConservativeRasterization conservative_rasterization;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisRasterizerDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `fill_mode` Fill mode. Solid or Wireframe. Default is `WisFillModeSolid`.
 * - `cull_mode` Cull mode. None, Front, Back. Default is `WisCullModeBack`.
 * - `front_face` Front face winding order. Clockwise or CounterClockwise. Default is `WisWindingOrderClockwise`.
 * - `depth_bias_enable` Depth bias enable. Default is false.
 * - `depth_bias` Depth bias. Default is 0.0f.
 * - `depth_bias_clamp` Depth bias clamp. Default is 0.0f.
 * - `depth_bias_slope_factor` Depth bias slope factor e.g. for shadows. Default is 0.0f.
 * - `depth_clip_enable` Depth clip enable. Default is true.
 * - `line_rasterization` Line rasterization mode. Default is `WisLineRasterizationDefault`.
 * - `conservative_rasterization` Conservative rasterization mode. Default is `WisConservativeRasterizationOff`.
 * \endcond
 *
 * @section WisRasterizerDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisRasterizerDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisGraphicsPipelineDesc
 * \endcond
 */