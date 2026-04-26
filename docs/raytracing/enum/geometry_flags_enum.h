/**
 * @struct WisGeometryFlags WisGeometryFlags
 * @ingroup Enumerations Raytracing
 *
 * @section WisGeometryFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisGeometryFlags {
 *     WisGeometryFlagsNone = 0,
 *     WisGeometryFlagsOpaque = (1u << 0),
 *     WisGeometryFlagsNoDuplicateAnyHitInvocation = (1u << 1),
 * } WisGeometryFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class GeometryFlags : uint32_t {
 *     None = 0,
 *     Opaque = (1u << 0),
 *     NoDuplicateAnyHitInvocation = (1u << 1),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisGeometryFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Bitmask for geometry flags in raytracing.
 *
 * \note Translates to DirectX 12 as D3D12_RAYTRACING_GEOMETRY_FLAGS, Vulkan as VkGeometryFlagsKHR.
 *
 * Values:
 * - `WisGeometryFlagsNone = 0`: No flags set. Geometry is regular.
 * - `WisGeometryFlagsOpaque = (1 << 0)`: Geometry is opaque. Used for opaque geometry.
 * - `WisGeometryFlagsNoDuplicateAnyHitInvocation = (1 << 1)`: Geometry has no duplicate any hit invocation.
 * \endcond
 *
 *
 * @section WisGeometryFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisAcceleratedGeometryDesc
 * \endcond
 */
