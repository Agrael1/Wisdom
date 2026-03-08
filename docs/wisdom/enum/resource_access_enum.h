/**
 * @struct WisResourceAccess WisResourceAccess
 * @ingroup Enumerations
 *
 * @section WisResourceAccess_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisResourceAccess {
 *     WisResourceAccessCommon = 0,
 *     WisResourceAccessVertexBuffer = (1u << 0),
 *     WisResourceAccessConstantBuffer = (1u << 1),
 *     WisResourceAccessIndexBuffer = (1u << 2),
 *     WisResourceAccessRenderTarget = (1u << 3),
 *     WisResourceAccessUnorderedAccess = (1u << 4),
 *     WisResourceAccessDepthWrite = (1u << 5),
 *     WisResourceAccessDepthRead = (1u << 6),
 *     WisResourceAccessShaderResource = (1u << 7),
 *     WisResourceAccessStreamOutput = (1u << 8),
 *     WisResourceAccessIndirectArgument = (1u << 9),
 *     WisResourceAccessCopyDst = (1u << 10),
 *     WisResourceAccessCopySrc = (1u << 11),
 *     WisResourceAccessConditionalRendering = (1u << 12),
 *     WisResourceAccessAccelerationStructureRead = (1u << 13),
 *     WisResourceAccessAccelerationStructureWrite = (1u << 14),
 *     WisResourceAccessShadingRate = (1u << 15),
 *     WisResourceAccessVideoDecodeRead = (1u << 16),
 *     WisResourceAccessVideoDecodeWrite = (1u << 17),
 *     WisResourceAccessResolveDst = (1u << 18),
 *     WisResourceAccessResolveSrc = (1u << 19),
 *     WisResourceAccessNone = (1u << 31),
 * } WisResourceAccess;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class ResourceAccess : uint32_t {
 *     Common = 0,
 *     VertexBuffer = (1u << 0),
 *     ConstantBuffer = (1u << 1),
 *     IndexBuffer = (1u << 2),
 *     RenderTarget = (1u << 3),
 *     UnorderedAccess = (1u << 4),
 *     DepthWrite = (1u << 5),
 *     DepthRead = (1u << 6),
 *     ShaderResource = (1u << 7),
 *     StreamOutput = (1u << 8),
 *     IndirectArgument = (1u << 9),
 *     CopyDst = (1u << 10),
 *     CopySrc = (1u << 11),
 *     ConditionalRendering = (1u << 12),
 *     AccelerationStructureRead = (1u << 13),
 *     AccelerationStructureWrite = (1u << 14),
 *     ShadingRate = (1u << 15),
 *     VideoDecodeRead = (1u << 16),
 *     VideoDecodeWrite = (1u << 17),
 *     ResolveDst = (1u << 18),
 *     ResolveSrc = (1u << 19),
 *     None = (1u << 31),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisResourceAccess_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Resource access flags for resource barriers.
 * 
 * \note Translates to DirectX 12 as D3D12_BARRIER_ACCESS, Vulkan as VkAccessFlags2.
 * Values:
 * - `WisResourceAccessCommon = 0`: Common access.
 * - `WisResourceAccessVertexBuffer = (1 << 0)`: Vertex buffer access. Applies only to buffers.
 * - `WisResourceAccessConstantBuffer = (1 << 1)`: Constant buffer access. Applies only to buffers.
 * - `WisResourceAccessIndexBuffer = (1 << 2)`: Index buffer access. Applies only to buffers.
 * - `WisResourceAccessRenderTarget = (1 << 3)`: Render target access. Applies only to textures.
 * - `WisResourceAccessUnorderedAccess = (1 << 4)`: Unordered access.
 * - `WisResourceAccessDepthWrite = (1 << 5)`: Depth write access. Applies only to depth-stencil textures.
 * - `WisResourceAccessDepthRead = (1 << 6)`: Depth read access. Applies only to depth-stencil textures.
 * - `WisResourceAccessShaderResource = (1 << 7)`: Shader resource access.
 * - `WisResourceAccessStreamOutput = (1 << 8)`: Stream output access. Applies only to buffers.
 * - `WisResourceAccessIndirectArgument = (1 << 9)`: Indirect argument access.
 * - `WisResourceAccessCopyDst = (1 << 10)`: Copy destination access.
 * - `WisResourceAccessCopySrc = (1 << 11)`: Copy source access.
 * - `WisResourceAccessConditionalRendering = (1 << 12)`: Conditional rendering access.
 * - `WisResourceAccessAccelerationStructureRead = (1 << 13)`: Acceleration structure read access.
 * - `WisResourceAccessAccelerationStructureWrite = (1 << 14)`: Acceleration structure write access.
 * - `WisResourceAccessShadingRate = (1 << 15)`: Shading rate access.
 * - `WisResourceAccessVideoDecodeRead = (1 << 16)`: Video decode read access.
 * - `WisResourceAccessVideoDecodeWrite = (1 << 17)`: Video decode write access.
 * - `WisResourceAccessResolveDst = (1 << 18)`: Resolve destination access.
 * - `WisResourceAccessResolveSrc = (1 << 19)`: Resolve source access.
 * - `WisResourceAccessNone = (1 << 31)`: No access. Used to indicate no access throughout the pipeline.
 * \endcond
 *
 *
 * @section WisResourceAccess_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisBufferBarrier, WisBufferBarrier, WisGlobalBarrier, WisGlobalBarrier, WisTextureBarrier, WisTextureBarrier
 * \endcond
 */