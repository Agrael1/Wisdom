/**
 * @struct WisBufferBarrier
 * @ingroup Structures
 *
 *
 * @section WisBufferBarrier_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisBufferBarrier {
 *     WisBarrierSync      sync_before;
 *     WisBarrierSync      sync_after;
 *     WisResourceAccess   access_before;
 *     WisResourceAccess   access_after;
 *     WisBufferView       buffer;
 *     uint64_t            offset;
 *     uint64_t            size;
 *     WisCommandQueueType queue_type_before;
 *     WisCommandQueueType queue_type_after;
 * } WisBufferBarrier;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKBufferBarrier {
 *     WisBarrierSync      sync_before;
 *     WisBarrierSync      sync_after;
 *     WisResourceAccess   access_before;
 *     WisResourceAccess   access_after;
 *     WisVKBufferView     buffer;
 *     uint64_t            offset;
 *     uint64_t            size;
 *     WisCommandQueueType queue_type_before;
 *     WisCommandQueueType queue_type_after;
 * } WisVKBufferBarrier;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12BufferBarrier {
 *     WisBarrierSync      sync_before;
 *     WisBarrierSync      sync_after;
 *     WisResourceAccess   access_before;
 *     WisResourceAccess   access_after;
 *     WisDX12BufferView   buffer;
 *     uint64_t            offset;
 *     uint64_t            size;
 *     WisCommandQueueType queue_type_before;
 *     WisCommandQueueType queue_type_after;
 * } WisDX12BufferBarrier;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct BufferBarrier {
 *     wis::BarrierSync      sync_before;
 *     wis::BarrierSync      sync_after;
 *     wis::ResourceAccess   access_before;
 *     wis::ResourceAccess   access_after;
 *     wis::BufferView       buffer;
 *     std::uint64_t         offset;
 *     std::uint64_t         size;
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
 * struct VKBufferBarrier {
 *     wis::BarrierSync      sync_before;
 *     wis::BarrierSync      sync_after;
 *     wis::ResourceAccess   access_before;
 *     wis::ResourceAccess   access_after;
 *     wis::VKBufferView     buffer;
 *     std::uint64_t         offset;
 *     std::uint64_t         size;
 *     wis::CommandQueueType queue_type_before;
 *     wis::CommandQueueType queue_type_after;
 * };
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12BufferBarrier {
 *     wis::BarrierSync      sync_before;
 *     wis::BarrierSync      sync_after;
 *     wis::ResourceAccess   access_before;
 *     wis::ResourceAccess   access_after;
 *     wis::DX12BufferView   buffer;
 *     std::uint64_t         offset;
 *     std::uint64_t         size;
 *     wis::CommandQueueType queue_type_before;
 *     wis::CommandQueueType queue_type_after;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisBufferBarrier_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `sync_before` Synchronization scope before the barrier.
 * - `sync_after` Synchronization scope after the barrier.
 * - `access_before` Access scope before the barrier.
 * - `access_after` Access scope after the barrier.
 * - `buffer` Buffer view.
 * - `offset` Offset in bytes from the start of the buffer. Default is 0.
 * - `size` Barrier size in bytes. Default is `UINT64_MAX`, which means the whole buffer range.
 * - `queue_type_before` Type of the queue the barrier is executed on before the synchronization point. Used for cross-queue barriers.
 * - `queue_type_after` Type of the queue the barrier is executed on after the synchronization point. Used for cross-queue barriers.
 * \endcond
 *
 * @section WisBufferBarrier_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBufferBarrier_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBarrierGroup
 * \endcond
 */