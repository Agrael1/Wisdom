/**
 * @struct WisTextureState WisTextureState
 * @ingroup Enumerations Core
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
 *     WisTextureStateDepthWriteStencilRead = 6,
 *     WisTextureStateStencilWriteDepthRead = 7,
 *     WisTextureStateShaderResource = 8,
 *     WisTextureStateCopySrc = 9,
 *     WisTextureStateCopyDst = 10,
 *     WisTextureStatePresent = 11,
 *     WisTextureStateShadingRate = 12,
 *     WisTextureStateVideoDecodeRead = 13,
 *     WisTextureStateVideoDecodeWrite = 14,
 *     WisTextureStateResolveDepthStensilDst = 15,
 *     WisTextureStateResolveRenderTargetDst = 16,
 *     WisTextureStateVideoDecodeDPB = 17,
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
 *     DepthWriteStencilRead = 6,
 *     StencilWriteDepthRead = 7,
 *     ShaderResource = 8,
 *     CopySrc = 9,
 *     CopyDst = 10,
 *     Present = 11,
 *     ShadingRate = 12,
 *     VideoDecodeRead = 13,
 *     VideoDecodeWrite = 14,
 *     ResolveDepthStensilDst = 15,
 *     ResolveRenderTargetDst = 16,
 *     VideoDecodeDPB = 17,
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
 * \note Translates to  `D3D12_BARRIER_LAYOUT` for DirectX 12 implementation, and `VkImageLayout` for Vulkan
 * implementation.
 *
 * Values:
 * - `WisTextureStateUndefined = -1`: Undefined state.
 * - `WisTextureStateCommon = 0`: Common state.
 * - `WisTextureStateRead = 1`: General Read state.
 * - `WisTextureStateRenderTarget = 2`: Render Target state.
 * - `WisTextureStateUnorderedAccess = 3`: Unordered Access state.
 * - `WisTextureStateDepthStencilWrite = 4`: Depth Stencil Write state.
 * - `WisTextureStateDepthStencilRead = 5`: Depth Stencil Read state.
 * - `WisTextureStateDepthWriteStencilRead = 6`: Depth Write Stencil Read state.
 * - `WisTextureStateStencilWriteDepthRead = 7`: Stencil Write Depth Read state.
 * - `WisTextureStateShaderResource = 8`: Shader Resource state.
 * - `WisTextureStateCopySrc = 9`: Copy Source state.
 * - `WisTextureStateCopyDst = 10`: Copy Destination state.
 * - `WisTextureStatePresent = 11`: Present swapchain state.
 * - `WisTextureStateShadingRate = 12`: Shading Rate state. Used for Variable Shading Rate.
 * - `WisTextureStateVideoDecodeRead = 13`: Video Decode Read state.
 * - `WisTextureStateVideoDecodeWrite = 14`: Video Decode Write state.
 * - `WisTextureStateResolveDepthStensilDst = 15`: Depth Stencil Resolve Destination state.
 * - `WisTextureStateResolveRenderTargetDst = 16`: Render Target Resolve Destination state.
 * - `WisTextureStateVideoDecodeDPB = 17`: Video Decode DPB (Decoded Picture Buffer) state. Used for reference frame
 * storage during video decoding. Vulkan only, maps to the same video decode read on other APIs.
 * \endcond
 *
 *
 * @section WisTextureState_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisTextureBarrier
 * \endcond
 */
