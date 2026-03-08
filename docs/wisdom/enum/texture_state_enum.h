/**
 * @struct WisTextureState WisTextureState
 * @ingroup Enumerations
 *
 * @section WisTextureState_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisTextureState {
 *     WisTextureStateUndefined = -1,
 *     WisTextureStateCommon = 0,
 *     WisTextureStateRead = 1,
 *     WisTextureStateRenderTarget = 2,
 *     WisTextureStateUnorderedAccess = 3,
 *     WisTextureStateDepthStencilWrite = 4,
 *     WisTextureStateDepthStencilRead = 5,
 *     WisTextureStateShaderResource = 6,
 *     WisTextureStateCopySrc = 7,
 *     WisTextureStateCopyDst = 8,
 *     WisTextureStatePresent = 9,
 *     WisTextureStateShadingRate = 10,
 *     WisTextureStateVideoDecodeRead = 11,
 *     WisTextureStateVideoDecodeWrite = 12,
 * } WisTextureState;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class TextureState {
 *     Undefined = -1,
 *     Common = 0,
 *     Read = 1,
 *     RenderTarget = 2,
 *     UnorderedAccess = 3,
 *     DepthStencilWrite = 4,
 *     DepthStencilRead = 5,
 *     ShaderResource = 6,
 *     CopySrc = 7,
 *     CopyDst = 8,
 *     Present = 9,
 *     ShadingRate = 10,
 *     VideoDecodeRead = 11,
 *     VideoDecodeWrite = 12,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTextureState_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Texture state for resource transitions.
 * 
 * \note Translates to  `D3D12_BARRIER_LAYOUT` for DirectX 12 implementation, and `VkImageLayout` for Vulkan implementation.
 * 
 * Values:
 * - `WisTextureStateUndefined = -1`: Undefined state.
 * - `WisTextureStateCommon = 0`: Common state.
 * - `WisTextureStateRead = 1`: General Read state.
 * - `WisTextureStateRenderTarget = 2`: Render Target state.
 * - `WisTextureStateUnorderedAccess = 3`: Unordered Access state.
 * - `WisTextureStateDepthStencilWrite = 4`: Depth Stencil Write state.
 * - `WisTextureStateDepthStencilRead = 5`: Depth Stencil Read state.
 * - `WisTextureStateShaderResource = 6`: Shader Resource state.
 * - `WisTextureStateCopySrc = 7`: Copy Source state.
 * - `WisTextureStateCopyDst = 8`: Copy Destination state.
 * - `WisTextureStatePresent = 9`: Present swapchain state.
 * - `WisTextureStateShadingRate = 10`: Shading Rate state. Used for Variable Shading Rate.
 * - `WisTextureStateVideoDecodeRead = 11`: Video Decode Read state.
 * - `WisTextureStateVideoDecodeWrite = 12`: Video Decode Write state.
 * \endcond
 *
 *
 * @section WisTextureState_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisTextureBarrier, WisTextureBarrier
 * \endcond
 */