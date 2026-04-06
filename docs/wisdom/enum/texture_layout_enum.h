/**
 * @struct WisTextureLayout WisTextureLayout
 * @ingroup Enumerations Core
 *
 * @section WisTextureLayout_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisTextureLayout {
 *     WisTextureLayoutTexture1D = 2,
 *     WisTextureLayoutTexture1DArray = 3,
 *     WisTextureLayoutTexture2D = 4,
 *     WisTextureLayoutTexture2DArray = 5,
 *     WisTextureLayoutTexture2DMS = 6,
 *     WisTextureLayoutTexture2DMSArray = 7,
 *     WisTextureLayoutTexture3D = 8,
 *     WisTextureLayoutTextureCube = 9,
 *     WisTextureLayoutTextureCubeArray = 10,
 * } WisTextureLayout;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class TextureLayout {
 *     Texture1D = 2,
 *     Texture1DArray = 3,
 *     Texture2D = 4,
 *     Texture2DArray = 5,
 *     Texture2DMS = 6,
 *     Texture2DMSArray = 7,
 *     Texture3D = 8,
 *     TextureCube = 9,
 *     TextureCubeArray = 10,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTextureLayout_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Texture layout. Determines how texture will be accessed.
 * 
 * Values:
 * - `WisTextureLayoutTexture1D = 2`: Texture is 1D array of data. Behaves similarly to Buffer.
 * - `WisTextureLayoutTexture1DArray = 3`: Texture is an array of 1D data.
 * - `WisTextureLayoutTexture2D = 4`: Texture is 2D image, default texture type.
 * - `WisTextureLayoutTexture2DArray = 5`: Texture is an array of 2D images. Can also be used for Cube maps.
 * - `WisTextureLayoutTexture2DMS = 6`: Texture is 2D multisampled image.
 * - `WisTextureLayoutTexture2DMSArray = 7`: Texture is an array of 2D multisampled images.
 * - `WisTextureLayoutTexture3D = 8`: Texture is 3D volume.
 * - `WisTextureLayoutTextureCube = 9`: Texture is a cube map. Behaves similarly to Texture2DArray with 6 layers.
 * - `WisTextureLayoutTextureCubeArray = 10`: Texture is an array of cube maps. Behaves similarly to Texture2DArray with 6 layers per cube map.
 * \endcond
 *
 *
 * @section WisTextureLayout_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisTextureDesc, WisTextureBinding, WisRenderTargetDesc
 * \endcond
 */