/**
 * @struct WisTextureDesc
 * @ingroup Structures
 *
 *
 * @section WisTextureDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisTextureDesc {
 *     uint32_t             width;
 *     uint32_t             height;
 *     uint16_t             depth_or_array_size;
 *     uint16_t             mip_levels;
 *     WisDataFormat        format;
 *     WisSampleCount       sample_count;
 *     WisTextureLayout     layout;
 *     WisTextureUsageFlags usage_flags;
 *     WisTextureFlags      flags;
 *     WisMemoryType        memory_type;
 *     WisMemoryFlags       memory_flags;
 * } WisTextureDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  TextureDesc {
 *     std::uint32_t          width;
 *     std::uint32_t          height;
 *     std::uint16_t          depth_or_array_size;
 *     std::uint16_t          mip_levels;
 *     wis::DataFormat        format;
 *     wis::SampleCount       sample_count;
 *     wis::TextureLayout     layout;
 *     wis::TextureUsageFlags usage_flags;
 *     wis::TextureFlags      flags;
 *     wis::MemoryType        memory_type;
 *     wis::MemoryFlags       memory_flags;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTextureDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `width` Texture width in pixels.
 * - `height` Texture height in pixels.
 * - `depth_or_array_size` Texture depth in pixels. Used only for 3D textures.
 * - `mip_levels` Number of mip levels in the texture.
 * - `format` Texture format.
 * - `sample_count` Number of samples per pixel. Used only for multisampled textures.
 * - `layout` Texture layout. Default is `WisTextureLayoutTexture2D`.
 * - `usage_flags` Texture usage flags. Describe how the texture will be used.
 * - `flags` Texture flags. Describe additional options for the texture.
 * - `memory_type` indicates where the texture will be allocated.
 * - `memory_flags` The flags of the memory to allocate for the texture.
 * \endcond
 *
 * @section WisTextureDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisTextureDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see wisResourceAllocatorCreateTexture
 * \endcond
 */