/**
 * @struct WisTextureUsageFlags WisTextureUsageFlags
 * @ingroup Enumerations Core
 *
 * @section WisTextureUsageFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisTextureUsageFlags {
 *     WisTextureUsageFlagsNone = 0,
 *     WisTextureUsageFlagsRenderTarget = (1u << 0),
 *     WisTextureUsageFlagsDepthStencil = (1u << 1),
 *     WisTextureUsageFlagsCopySrc = (1u << 2),
 *     WisTextureUsageFlagsCopyDst = (1u << 3),
 *     WisTextureUsageFlagsShaderResource = (1u << 4),
 *     WisTextureUsageFlagsUnorderedAccess = (1u << 5),
 *     WisTextureUsageFlagsHostCopy = (1u << 7),
 * } WisTextureUsageFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class TextureUsageFlags : uint32_t {
 *     None = 0,
 *     RenderTarget = (1u << 0),
 *     DepthStencil = (1u << 1),
 *     CopySrc = (1u << 2),
 *     CopyDst = (1u << 3),
 *     ShaderResource = (1u << 4),
 *     UnorderedAccess = (1u << 5),
 *     HostCopy = (1u << 7),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisTextureUsageFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Texture usage flags.
 * Determine how the texture can be used throughout its lifetime.
 *
 * \note Translates to DirectX 12 as D3D12_RESOURCE_FLAGS, Vulkan as VkImageUsageFlags.
 *
 * Values:
 * - `WisTextureUsageFlagsNone = 0`: No flags set. Texture is not used.
 * - `WisTextureUsageFlagsRenderTarget = (1 << 0)`: Texture is used as a render target.
 * - `WisTextureUsageFlagsDepthStencil = (1 << 1)`: Texture is used as a depth stencil buffer.
 * - `WisTextureUsageFlagsCopySrc = (1 << 2)`: Texture is used as a source for copy operations.
 * - `WisTextureUsageFlagsCopyDst = (1 << 3)`: Texture is used as a destination for copy operations.
 * - `WisTextureUsageFlagsShaderResource = (1 << 4)`: Texture is used as a shader resource.
 * - `WisTextureUsageFlagsUnorderedAccess = (1 << 5)`: Texture is used as an unordered access resource.
 * - `WisTextureUsageFlagsHostCopy = (1 << 7)`: Texture is used for host copy operations. Works with GPUUpload heap.
 * \endcond
 *
 *
 * @section WisTextureUsageFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisTextureDesc, WisSurfaceParameters, WisSwapchainDesc
 * \endcond
 */
