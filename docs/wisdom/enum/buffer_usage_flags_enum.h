/**
 * @struct WisBufferUsageFlags WisBufferUsageFlags
 * @ingroup Enumerations
 *
 * @section WisBufferUsageFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisBufferUsageFlags {
 *     WisBufferUsageFlagsNone = 0,
 *     WisBufferUsageFlagsCopySrc = (1 << 0),
 *     WisBufferUsageFlagsCopyDst = (1 << 1),
 *     WisBufferUsageFlagsConstantBuffer = (1 << 2),
 *     WisBufferUsageFlagsIndexBuffer = (1 << 3),
 *     WisBufferUsageFlagsVertexBuffer = (1 << 4),
 *     WisBufferUsageFlagsIndirectBuffer = (1 << 5),
 *     WisBufferUsageFlagsStorageBuffer = (1 << 6),
 *     WisBufferUsageFlagsAccelerationStructureBuffer = (1 << 7),
 *     WisBufferUsageFlagsAccelerationStructureInput = (1 << 8),
 *     WisBufferUsageFlagsShaderBindingTable = (1 << 9),
 * } WisBufferUsageFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class BufferUsageFlags : uint32_t {
 *     None = 0,
 *     CopySrc = (1 << 0),
 *     CopyDst = (1 << 1),
 *     ConstantBuffer = (1 << 2),
 *     IndexBuffer = (1 << 3),
 *     VertexBuffer = (1 << 4),
 *     IndirectBuffer = (1 << 5),
 *     StorageBuffer = (1 << 6),
 *     AccelerationStructureBuffer = (1 << 7),
 *     AccelerationStructureInput = (1 << 8),
 *     ShaderBindingTable = (1 << 9),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBufferUsageFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Buffer usage flags.
 * Determine how the buffer can be used throughout its lifetime.
 * 
 * \note Translates to DirectX 12 as D3D12_RESOURCE_FLAGS, Vulkan as VkBufferUsageFlags.
 * Values:
 * - `WisBufferUsageFlagsNone = 0`: No flags set. Buffer is not used.
 * - `WisBufferUsageFlagsCopySrc = (1 << 0)`: Buffer is used as a source for copy operations.
 * - `WisBufferUsageFlagsCopyDst = (1 << 1)`: Buffer is used as a destination for copy operations.
 * - `WisBufferUsageFlagsConstantBuffer = (1 << 2)`: Buffer is used as a constant buffer.
 * - `WisBufferUsageFlagsIndexBuffer = (1 << 3)`: Buffer is used as an index buffer.
 * - `WisBufferUsageFlagsVertexBuffer = (1 << 4)`: Buffer is used as a vertex buffer or an instance buffer.
 * - `WisBufferUsageFlagsIndirectBuffer = (1 << 5)`: Buffer is used as an indirect buffer.
 * - `WisBufferUsageFlagsStorageBuffer = (1 << 6)`: Buffer is used as a storage unordered access buffer.
 * - `WisBufferUsageFlagsAccelerationStructureBuffer = (1 << 7)`: Buffer is used as an acceleration structure buffer.
 * - `WisBufferUsageFlagsAccelerationStructureInput = (1 << 8)`: Buffer is used as a read only acceleration instance input buffer.
 * - `WisBufferUsageFlagsShaderBindingTable = (1 << 9)`: Buffer is used as a shader binding table buffer.
 * \endcond
 *
 *
 * @section WisBufferUsageFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisBufferDesc
 * \endcond
 */