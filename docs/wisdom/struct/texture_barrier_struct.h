/**
 * @struct WisTextureBarrier
 * @ingroup Structures
 *
 *
 * @section WisTextureBarrier_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisTextureBarrier {
 *     WisBarrierSync      sync_before;
 *     WisBarrierSync      sync_after;
 *     WisResourceAccess   access_before;
 *     WisResourceAccess   access_after;
 *     WisTextureState     state_before;
 *     WisTextureState     state_after;
 *     WisBarrierFlags     flags;
 *     WisTextureView      texture;
 *     WisSubresourceRange subresource_range;
 *     WisCommandQueueType queue_type_before;
 *     WisCommandQueueType queue_type_after;
 * } WisTextureBarrier;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKTextureBarrier {
 *     WisBarrierSync      sync_before;
 *     WisBarrierSync      sync_after;
 *     WisResourceAccess   access_before;
 *     WisResourceAccess   access_after;
 *     WisTextureState     state_before;
 *     WisTextureState     state_after;
 *     WisBarrierFlags     flags;
 *     WisVKTextureView    texture;
 *     WisSubresourceRange subresource_range;
 *     WisCommandQueueType queue_type_before;
 *     WisCommandQueueType queue_type_after;
 * } WisVKTextureBarrier;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12TextureBarrier {
 *     WisBarrierSync      sync_before;
 *     WisBarrierSync      sync_after;
 *     WisResourceAccess   access_before;
 *     WisResourceAccess   access_after;
 *     WisTextureState     state_before;
 *     WisTextureState     state_after;
 *     WisBarrierFlags     flags;
 *     WisDX12TextureView  texture;
 *     WisSubresourceRange subresource_range;
 *     WisCommandQueueType queue_type_before;
 *     WisCommandQueueType queue_type_after;
 * } WisDX12TextureBarrier;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct TextureBarrier {
 *     wis::BarrierSync      sync_before;
 *     wis::BarrierSync      sync_after;
 *     wis::ResourceAccess   access_before;
 *     wis::ResourceAccess   access_after;
 *     wis::TextureState     state_before;
 *     wis::TextureState     state_after;
 *     wis::BarrierFlags     flags;
 *     wis::TextureView      texture;
 *     wis::SubresourceRange subresource_range;
 *     wis::CommandQueueType queue_type_before;
 *     wis::CommandQueueType queue_type_after;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKTextureBarrier {
 *     wis::BarrierSync      sync_before;
 *     wis::BarrierSync      sync_after;
 *     wis::ResourceAccess   access_before;
 *     wis::ResourceAccess   access_after;
 *     wis::TextureState     state_before;
 *     wis::TextureState     state_after;
 *     wis::BarrierFlags     flags;
 *     wis::VKTextureView    texture;
 *     wis::SubresourceRange subresource_range;
 *     wis::CommandQueueType queue_type_before;
 *     wis::CommandQueueType queue_type_after;
 * };
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12TextureBarrier {
 *     wis::BarrierSync      sync_before;
 *     wis::BarrierSync      sync_after;
 *     wis::ResourceAccess   access_before;
 *     wis::ResourceAccess   access_after;
 *     wis::TextureState     state_before;
 *     wis::TextureState     state_after;
 *     wis::BarrierFlags     flags;
 *     wis::DX12TextureView  texture;
 *     wis::SubresourceRange subresource_range;
 *     wis::CommandQueueType queue_type_before;
 *     wis::CommandQueueType queue_type_after;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisTextureBarrier_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `sync_before` Synchronization scope before the barrier.
 * - `sync_after` Synchronization scope after the barrier.
 * - `access_before` Access scope before the barrier.
 * - `access_after` Access scope after the barrier.
 * - `state_before` Texture state before the barrier.
 * - `state_after` Texture state after the barrier.
 * - `flags` Barrier flags. Describe additional options for the barrier.
 * - `texture` Texture view.
 * - `subresource_range` Subresource range for the barrier.
 * - `queue_type_before` Type of the queue the barrier is executed on before the synchronization point. Used for cross-queue barriers.
 * - `queue_type_after` Type of the queue the barrier is executed on after the synchronization point. Used for cross-queue barriers.
 * \endcond
 *
 * @section WisTextureBarrier_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisTextureBarrier_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see WisBarrierGroup
 * \endcond
 */