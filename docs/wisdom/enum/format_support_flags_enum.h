/**
 * @struct WisFormatSupportFlags WisFormatSupportFlags
 * @ingroup Enumerations Core
 *
 * @section WisFormatSupportFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisFormatSupportFlags {
 *     WisFormatSupportFlagsNone = 0,
 *     WisFormatSupportFlagsBuffer = (1u << 0),
 *     WisFormatSupportFlagsVertexBuffer = (1u << 1),
 *     WisFormatSupportFlagsTexture1D = (1u << 2),
 *     WisFormatSupportFlagsTexture2D = (1u << 3),
 *     WisFormatSupportFlagsTexture3D = (1u << 4),
 *     WisFormatSupportFlagsTextureCube = (1u << 5),
 *     WisFormatSupportFlagsRenderTarget = (1u << 6),
 *     WisFormatSupportFlagsDepthStencil = (1u << 7),
 *     WisFormatSupportFlagsBlendable = (1u << 8),
 *     WisFormatSupportFlagsMultisampleRenderTarget = (1u << 9),
 *     WisFormatSupportFlagsMultisampleResolve = (1u << 10),
 *     WisFormatSupportFlagsShaderResource = (1u << 11),
 *     WisFormatSupportFlagsUnorderedAccess = (1u << 12),
 * } WisFormatSupportFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class FormatSupportFlags : uint32_t {
 *     None = 0,
 *     Buffer = (1u << 0),
 *     VertexBuffer = (1u << 1),
 *     Texture1D = (1u << 2),
 *     Texture2D = (1u << 3),
 *     Texture3D = (1u << 4),
 *     TextureCube = (1u << 5),
 *     RenderTarget = (1u << 6),
 *     DepthStencil = (1u << 7),
 *     Blendable = (1u << 8),
 *     MultisampleRenderTarget = (1u << 9),
 *     MultisampleResolve = (1u << 10),
 *     ShaderResource = (1u << 11),
 *     UnorderedAccess = (1u << 12),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisFormatSupportFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Format support flags.
 * Indicates what features are supported for a specific format.
 *
 * Values:
 * - `WisFormatSupportFlagsNone = 0`: No features supported.
 * - `WisFormatSupportFlagsBuffer = (1 << 0)`: Format can be used for buffers.
 * - `WisFormatSupportFlagsVertexBuffer = (1 << 1)`: Format can be used for vertex buffers.
 * - `WisFormatSupportFlagsTexture1D = (1 << 2)`: Format can be used for 1D textures.
 * - `WisFormatSupportFlagsTexture2D = (1 << 3)`: Format can be used for 2D textures.
 * - `WisFormatSupportFlagsTexture3D = (1 << 4)`: Format can be used for 3D textures.
 * - `WisFormatSupportFlagsTextureCube = (1 << 5)`: Format can be used for cube textures.
 * - `WisFormatSupportFlagsRenderTarget = (1 << 6)`: Format can be used for render targets.
 * - `WisFormatSupportFlagsDepthStencil = (1 << 7)`: Format can be used for depth stencil.
 * - `WisFormatSupportFlagsBlendable = (1 << 8)`: Format supports blending.
 * - `WisFormatSupportFlagsMultisampleRenderTarget = (1 << 9)`: Format supports multisampled render targets.
 * - `WisFormatSupportFlagsMultisampleResolve = (1 << 10)`: Format supports multisample resolve.
 * - `WisFormatSupportFlagsShaderResource = (1 << 11)`: Format can be used for shader resource.
 * - `WisFormatSupportFlagsUnorderedAccess = (1 << 12)`: Format can be used for unordered access.
 * \endcond
 *
 *
 * @section WisFormatSupportFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisFormatProperties
 * \endcond
 */