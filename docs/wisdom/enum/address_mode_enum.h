/**
 * @struct WisAddressMode WisAddressMode
 * @ingroup Enumerations Core
 *
 * @section WisAddressMode_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisAddressMode {
 *     WisAddressModeRepeat = 0,
 *     WisAddressModeMirroredRepeat = 1,
 *     WisAddressModeClampToEdge = 2,
 *     WisAddressModeClampToBorder = 3,
 *     WisAddressModeMirrorClampToEdge = 4,
 * } WisAddressMode;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class AddressMode {
 *     Repeat = 0,
 *     MirroredRepeat = 1,
 *     ClampToEdge = 2,
 *     ClampToBorder = 3,
 *     MirrorClampToEdge = 4,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisAddressMode_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Address mode for texture sampling.
 * 
 * \note Translates to  `D3D12_TEXTURE_ADDRESS_MODE` for DirectX 12 implementation, and `VkSamplerAddressMode` for Vulkan implementation.
 * 
 * Values:
 * - `WisAddressModeRepeat = 0`: Repeat the texture.
 * - `WisAddressModeMirroredRepeat = 1`: Repeat the texture with mirroring.
 * - `WisAddressModeClampToEdge = 2`: Clamp the texture to the edge.
 * - `WisAddressModeClampToBorder = 3`: Clamp the texture to the border.
 * - `WisAddressModeMirrorClampToEdge = 4`: Mirror and clamp the texture to the edge.
 * \endcond
 *
 *
 * @section WisAddressMode_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisSamplerDesc
 * \endcond
 */
