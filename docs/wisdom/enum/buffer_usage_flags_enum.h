/**
 * @struct WisBufferUsageFlags WisBufferUsageFlags
 * @ingroup Enumerations Core
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
 *     WisBufferUsageFlagsCopySrc = (1u << 0),
 *     WisBufferUsageFlagsCopyDst = (1u << 1),
 *     WisBufferUsageFlagsConstantBuffer = (1u << 2),
 *     WisBufferUsageFlagsIndexBuffer = (1u << 3),
 *     WisBufferUsageFlagsVertexBuffer = (1u << 4),
 *     WisBufferUsageFlagsIndirectBuffer = (1u << 5),
 *     WisBufferUsageFlagsStorageBuffer = (1u << 6),
 *     WisBufferUsageFlagsAccelerationStructureBuffer = (1u << 7),
 *     WisBufferUsageFlagsAccelerationStructureInput = (1u << 8),
 *     WisBufferUsageFlagsShaderBindingTable = (1u << 9),
 * } WisBufferUsageFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class BufferUsageFlags : uint32_t {
 *     None = 0,
 *     CopySrc = (1u << 0),
 *     CopyDst = (1u << 1),
 *     ConstantBuffer = (1u << 2),
 *     IndexBuffer = (1u << 3),
 *     VertexBuffer = (1u << 4),
 *     IndirectBuffer = (1u << 5),
 *     StorageBuffer = (1u << 6),
 *     AccelerationStructureBuffer = (1u << 7),
 *     AccelerationStructureInput = (1u << 8),
 *     ShaderBindingTable = (1u << 9),
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
 * @see Structs:
 * WisBufferDesc
 * \endcond
 */