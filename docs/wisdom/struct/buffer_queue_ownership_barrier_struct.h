/**
 * @struct WisBufferQueueOwnershipBarrier
 * @ingroup Structures Core
 *
 *
 * @section WisBufferQueueOwnershipBarrier_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisBufferQueueOwnershipBarrier {
 *     WisCommandQueueType source_queue;
 *     WisCommandQueueType destination_queue;
 *     WisBarrierSync      sync_before;
 *     WisBarrierSync      sync_after;
 *     WisResourceAccess   access_before;
 *     WisResourceAccess   access_after;
 *     uint64_t            offset;
 *     uint64_t            size;
 * } WisBufferQueueOwnershipBarrier;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  BufferQueueOwnershipBarrier 
 * ```
 * 
 * \endcond
 *
 * @section WisBufferQueueOwnershipBarrier_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - `source_queue` Source queue type that currently owns the buffer.
 * - `destination_queue` Destination queue type that will acquire the buffer.
 * - `sync_before` Synchronization scope before the transfer.
 * - `sync_after` Synchronization scope after the transfer.
 * - `access_before` Access scope before the transfer.
 * - `access_after` Access scope after the transfer.
 * - `offset` Offset in bytes from the start of the buffer. Default is 0.
 * - `size` Barrier size in bytes. Default is `UINT64_MAX`, which means the whole buffer range.
 * 
 * \endcond
 *
 * @section WisBufferQueueOwnershipBarrier_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBufferQueueOwnershipBarrier_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
