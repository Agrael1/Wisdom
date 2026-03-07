/**
 * @struct WisBufferQueueOwnershipBarrier2
 * @ingroup Structures
 *
 *
 * @section WisBufferQueueOwnershipBarrier2_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisBufferQueueOwnershipBarrier2 {
 *     WisBufferQueueOwnershipBarrier barrier;
 *     WisBufferView                  buffer;
 * } WisBufferQueueOwnershipBarrier2;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKBufferQueueOwnershipBarrier2 {
 *     WisBufferQueueOwnershipBarrier barrier;
 *     WisVKBufferView                buffer;
 * } WisVKBufferQueueOwnershipBarrier2;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12BufferQueueOwnershipBarrier2 {
 *     WisBufferQueueOwnershipBarrier barrier;
 *     WisDX12BufferView              buffer;
 * } WisDX12BufferQueueOwnershipBarrier2;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct BufferQueueOwnershipBarrier2 
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKBufferQueueOwnershipBarrier2 ;
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12BufferQueueOwnershipBarrier2 
 * ```
 * </details>
 * 
 * \endcond
 *
 * @section WisBufferQueueOwnershipBarrier2_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - `barrier` Queue ownership transfer barrier description.
 * - `buffer` Buffer view.
 * 
 * \endcond
 *
 * @section WisBufferQueueOwnershipBarrier2_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBufferQueueOwnershipBarrier2_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */