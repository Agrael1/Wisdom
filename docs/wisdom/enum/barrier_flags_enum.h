/**
 * @struct WisBarrierFlags WisBarrierFlags
 * @ingroup Enumerations
 *
 * @section WisBarrierFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisBarrierFlags {
 *     WisBarrierFlagsNone = 0,
 *     WisBarrierFlagsDiscardContent = (1u << 0),
 *     WisBarrierFlagsDepthResource = (1u << 1),
 *     WisBarrierFlagsStencilResource = (1u << 2),
 *     WisBarrierFlagsWholeRange = (1u << 3),
 *     WisBarrierFlagsPlanarImage = (1u << 4),
 * } WisBarrierFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class BarrierFlags : uint32_t {
 *     None = 0,
 *     DiscardContent = (1u << 0),
 *     DepthResource = (1u << 1),
 *     StencilResource = (1u << 2),
 *     WholeRange = (1u << 3),
 *     PlanarImage = (1u << 4),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBarrierFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Barrier flags for resource barriers.
 * 
 * Values:
 * - `WisBarrierFlagsNone = 0`: No flags set. Barrier is regular.
 * - `WisBarrierFlagsDiscardContent = (1 << 0)`: Discard resource content. The content of the resource before the barrier is treated as if resource was not initialized.
 * - `WisBarrierFlagsDepthResource = (1 << 1)`: Resource is a depth resource. This flag @wis_must be set for all depth resources to make transitions on them.
 * - `WisBarrierFlagsStencilResource = (1 << 2)`: Resource is a stencil resource. This flag @wis_must be set for all stencil resources to make transitions on them. If resource has format `WisDataFormatD24UnormS8Uint` both `WisBarrierFlags::DepthResource` and `WisBarrierFlags::StencilResource` @wis_must be set.
 * - `WisBarrierFlagsWholeRange = (1 << 3)`: Transition whole resource. If not set, the transition is applied only to the specified subresource range. If set, the subresource range is ignored and the transition is applied to all subresources of the resource.
 * - `WisBarrierFlagsPlanarImage = (1 << 4)`: Resource is a planar image. If the flag is not set, plane slices in WisSubresourceRange are ignored.
 * \endcond
 *
 *
 * @section WisBarrierFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisTextureBarrier
 * \endcond
 */