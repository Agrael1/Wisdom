/**
 * @struct WisDescriptorType WisDescriptorType
 * @ingroup Enumerations Core
 *
 * @section WisDescriptorType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisDescriptorType {
 *     WisDescriptorTypeSampler = 0,
 *     WisDescriptorTypeConstantBuffer = 1,
 *     WisDescriptorTypeTexture = 2,
 *     WisDescriptorTypeRWTexture = 3,
 *     WisDescriptorTypeRWBuffer = 4,
 *     WisDescriptorTypeBuffer = 5,
 *     WisDescriptorTypeAccelerationStructure = 6,
 *     WisDescriptorTypeCount = 7,
 * } WisDescriptorType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class DescriptorType {
 *     Sampler = 0,
 *     ConstantBuffer = 1,
 *     Texture = 2,
 *     RWTexture = 3,
 *     RWBuffer = 4,
 *     Buffer = 5,
 *     AccelerationStructure = 6,
 *     Count = 7,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Type of the descriptor in the descriptor table.
 *
 * \note Translates to  `D3D12_DESCRIPTOR_RANGE_TYPE` for DirectX 12 implementation, and `VkDescriptorType` for Vulkan implementation.
 *
 * Values:
 * - `WisDescriptorTypeSampler = 0`: Descriptor is a sampler.
 * - `WisDescriptorTypeConstantBuffer = 1`: Descriptor is a constant buffer.
 * - `WisDescriptorTypeTexture = 2`: Descriptor is a texture.
 * - `WisDescriptorTypeRWTexture = 3`: Descriptor is an unordered access read-write texture.
 * - `WisDescriptorTypeRWBuffer = 4`: Descriptor is an unordered access read-write buffer.
 * - `WisDescriptorTypeBuffer = 5`: Descriptor is a shader resource buffer.
 * - `WisDescriptorTypeAccelerationStructure = 6`: Descriptor is an acceleration structure.
 * - `WisDescriptorTypeCount = 7`: Descriptor is a mutable type.
 * \endcond
 *
 *
 * @section WisDescriptorType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisPushDescriptor, WisDescriptorTableEntry, WisPushDescriptorDataDesc
 * \endcond
 */