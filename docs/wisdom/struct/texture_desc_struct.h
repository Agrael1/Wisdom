/**
 * @struct WisTextureDesc
 * @ingroup Structures Core
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
 *     const WisDataFormat* cast_formats;
 *     size_t               cast_format_count;
 * } WisTextureDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  TextureDesc {
 *     std::uint32_t                    width;
 *     std::uint32_t                    height;
 *     std::uint16_t                    depth_or_array_size;
 *     std::uint16_t                    mip_levels;
 *     wis::DataFormat                  format;
 *     wis::SampleCount                 sample_count;
 *     wis::TextureLayout               layout;
 *     wis::TextureUsageFlags           usage_flags;
 *     wis::TextureFlags                flags;
 *     wis::MemoryType                  memory_type;
 *     wis::MemoryFlags                 memory_flags;
 *     wis::span<const wis::DataFormat> cast_formats;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTextureDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `width` defines texture width in pixels.
 * - `height` describes texture height in pixels.
 * - `depth_or_array_size` describes texture depth in pixels. Used only for 3D textures.
 * - `mip_levels` defines number of mip levels in the texture.
 * - `format` describes texture format.
 * - `sample_count` specifies number of samples per pixel. Used only for multisampled textures.
 * - `layout` specifies texture layout. Default is `WisTextureLayoutTexture2D`.
 * - `usage_flags` describes texture usage flags. Describe how the texture will be used.
 * - `flags` describes texture flags. Describe additional options for the texture.
 * - `memory_type` specifies where the texture will be allocated.
 * - `memory_flags` describes the flags of the memory to allocate for the texture.
 * - `cast_formats` points to an array of formats that can be used to cast the texture to another format. Used for
 * format casting in shaders.
 * - `cast_format_count` defines the number of the number of cast formats in the `WisTextureDesc::cast_formats` array.
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
 * @see Functions:
 * wisResourceAllocatorCreateTexture
 * \endcond
 */
