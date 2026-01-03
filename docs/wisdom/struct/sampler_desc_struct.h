/**
 * @struct WisSamplerDesc
 * @ingroup Structures
 *
 *
 * @section WisSamplerDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisSamplerDesc {
 *     WisFilter           min_filter;
 *     WisFilter           mag_filter;
 *     WisFilter           mip_filter;
 *     bool                is_anisotropic;
 *     uint32_t            max_anisotropy;
 *     WisAddressMode      address_u;
 *     WisAddressMode      address_v;
 *     WisAddressMode      address_w;
 *     float               min_lod;
 *     float               max_lod;
 *     float               mip_lod_bias;
 *     WisCompareOperation comparison_op;
 *     WisStaticBorder     static_border_color;
 *     float               border_color[4];
 *     WisSamplerFlags     flags;
 * } WisSamplerDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  SamplerDesc {
 *     wis::Filter           min_filter;
 *     wis::Filter           mag_filter;
 *     wis::Filter           mip_filter;
 *     bool                  is_anisotropic;
 *     std::uint32_t         max_anisotropy;
 *     wis::AddressMode      address_u;
 *     wis::AddressMode      address_v;
 *     wis::AddressMode      address_w;
 *     float                 min_lod;
 *     float                 max_lod;
 *     float                 mip_lod_bias;
 *     wis::CompareOperation comparison_op;
 *     wis::StaticBorder     static_border_color;
 *     std::array<float, 4>  border_color;
 *     wis::SamplerFlags     flags;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSamplerDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `min_filter` Minification filter.
 * - `mag_filter` Magnification filter.
 * - `mip_filter` Mip level filter.
 * - `is_anisotropic` Anisotropic filtering enable.
 * - `max_anisotropy` Max anisotropy level. Max is 16.
 * - `address_u` Address mode for U coordinate.
 * - `address_v` Address mode for V coordinate.
 * - `address_w` Address mode for W coordinate.
 * - `min_lod` Min LOD value.
 * - `max_lod` Max LOD value.
 * - `mip_lod_bias` Mip LOD bias value.
 * - `comparison_op` Comparison operation for comparison samplers.
 * - `static_border_color` Static border color. Used if any address mode is set to wis::AddressMode.
 * - `border_color` Border color. Used if any address mode is set to wis::AddressMode and static_border_color is set to `WisStaticBorderCustom`.
 * - `flags` Sampler flags. Used to set additional sampler options.
 * \endcond
 *
 * @section WisSamplerDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisSamplerDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see WisStaticSamplerDesc, wisDeviceCreateSampler
 * \endcond
 */