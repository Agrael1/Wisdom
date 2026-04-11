/**
 * @struct WisRenderTargetDesc
 * @ingroup Structures Core
 *
 *
 * @section WisRenderTargetDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisRenderTargetDesc {
 *     WisDataFormat    format;
 *     WisTextureLayout layout;
 *     uint16_t         mip_level;
 *     uint16_t         base_array_layer;
 *     uint16_t         array_layer_count;
 *     uint16_t         plane_slice;
 * } WisRenderTargetDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  RenderTargetDesc {
 *     wis::DataFormat    format;
 *     wis::TextureLayout layout;
 *     std::uint16_t      mip_level;
 *     std::uint16_t      base_array_layer;
 *     std::uint16_t      array_layer_count;
 *     std::uint16_t      plane_slice;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisRenderTargetDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `format` describes render target format.
 * - `layout` specifies render target layout. Default is `WisTextureLayoutTexture2D`.
 * - `mip_level` describes mipmap level of the target subresource.
 * - `base_array_layer` describes array layer of the target subresource. For 3D textures, this defines the base depth slice.
 * - `array_layer_count` indicates number of array layers in the target subresource. For 3D textures, this defines the number of depth slices.
 * - `plane_slice` indicates depth slice of the target subresource. Used only for 2D textures (YUV).
 * \endcond
 *
 * @section WisRenderTargetDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisRenderTargetDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisViewHeapWriteRenderTarget, wisViewHeapWriteDepthStencil
 * \endcond
 */
