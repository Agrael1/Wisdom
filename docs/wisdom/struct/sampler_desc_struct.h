/**
 * @struct WisSamplerDesc
 * @ingroup Structures Core
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
 *     WisFilter        min_filter;
 *     WisFilter        mag_filter;
 *     WisFilter        mip_filter;
 *     WisReductionMode reduction_mode;
 *     bool             is_anisotropic;
 *     uint32_t         max_anisotropy;
 *     WisAddressMode   address_u;
 *     WisAddressMode   address_v;
 *     WisAddressMode   address_w;
 *     float            min_lod;
 *     float            max_lod;
 *     float            mip_lod_bias;
 *     WisCompareOp     comparison_op;
 *     WisStaticBorder  static_border_color;
 *     WisSamplerFlags  flags;
 * } WisSamplerDesc;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct  SamplerDesc {
 *     wis::Filter        min_filter;
 *     wis::Filter        mag_filter;
 *     wis::Filter        mip_filter;
 *     wis::ReductionMode reduction_mode;
 *     bool               is_anisotropic;
 *     std::uint32_t      max_anisotropy;
 *     wis::AddressMode   address_u;
 *     wis::AddressMode   address_v;
 *     wis::AddressMode   address_w;
 *     float              min_lod;
 *     float              max_lod;
 *     float              mip_lod_bias;
 *     wis::CompareOp     comparison_op;
 *     wis::StaticBorder  static_border_color;
 *     wis::SamplerFlags  flags;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSamplerDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `min_filter` describes minification filter.
 * - `mag_filter` specifies magnification filter.
 * - `mip_filter` indicates mip level filter.
 * - `reduction_mode` describes reduction mode for min/mag/mip filters.
 * - `is_anisotropic` specifies anisotropic filtering enable.
 * - `max_anisotropy` describes max anisotropy level. Min is 1, Max is 16.
 * - `address_u` specifies address mode for U coordinate.
 * - `address_v` indicates address mode for V coordinate.
 * - `address_w` defines address mode for W coordinate.
 * - `min_lod` specifies min LOD value.
 * - `max_lod` defines max LOD value.
 * - `mip_lod_bias` specifies mip LOD bias value.
 * - `comparison_op` describes comparison operation for comparison samplers.
 * - `static_border_color` indicates static border color. Used if any address mode is set to WisAddressMode.
 * - `flags` describes sampler flags. Used to set additional sampler options.
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
 * @see Structs:
 * WisStaticSamplerDesc
 * Functions:
 * wisDescriptorHeapWriteSampler
 * \endcond
 */