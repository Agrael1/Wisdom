/**
 * @struct WisTextureBinding
 * @ingroup Structures Core
 *
 *
 * @section WisTextureBinding_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct  WisTextureBinding {
 *     WisDataFormat          format;
 *     WisTextureLayout       layout;
 *     WisTextureBindingFlags flags;
 *     WisComponentMapping    component_mapping;
 *     WisSubresourceRange    range;
 * } WisTextureBinding;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  TextureBinding {
 *     wis::DataFormat          format;
 *     wis::TextureLayout       layout;
 *     wis::TextureBindingFlags flags;
 *     wis::ComponentMapping    component_mapping;
 *     wis::SubresourceRange    range;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTextureBinding_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `format` describes the format of the view.
 * - `layout` specifies the layout of the texture. Default is `WisTextureLayoutTexture2D`.
 * - `flags` describes texture binding flags. Describe additional options for the texture binding.
 * - `component_mapping` specifies component mapping for the texture view.
 * - `range` describes subresource description for the texture view.
 * \endcond
 *
 * @section WisTextureBinding_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisTextureBinding_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDescriptorHeapWriteTexture, wisDescriptorHeapWriteRWTexture
 * \endcond
 */
