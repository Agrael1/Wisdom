/**
 * @struct WisResolveDesc
 * @ingroup Structures Core
 *
 *
 * @section WisResolveDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisResolveDesc {
 *     uint64_t       resolve_target;
 *     WisResolveMode mode;
 * } WisResolveDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  ResolveDesc {
 *     std::uint64_t    resolve_target;
 *     wis::ResolveMode mode;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisResolveDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `resolve_target` specifies resolve target view for multisampled render target; it @wis_must reside on WisViewHeap which was allocated with allow_multisample. If the render target is multisampled and resolve_target is set to 0. `target` @wis_must also reside on WisViewHeap which was allocated with allow_multisample.
 * - `mode` defines resolve mode. Default is `WisResolveModeAverage`.
 * \endcond
 *
 * @section WisResolveDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisResolveDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRenderPassRenderTargetDesc, WisRenderPassDepthStencilDesc
 * \endcond
 */