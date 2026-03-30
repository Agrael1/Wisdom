/**
 * @struct WisGlobalBarrier
 * @ingroup Structures
 *
 *
 * @section WisGlobalBarrier_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisGlobalBarrier {
 *     WisBarrierSync    sync_before;
 *     WisBarrierSync    sync_after;
 *     WisResourceAccess access_before;
 *     WisResourceAccess access_after;
 * } WisGlobalBarrier;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKGlobalBarrier {
 *     WisBarrierSync    sync_before;
 *     WisBarrierSync    sync_after;
 *     WisResourceAccess access_before;
 *     WisResourceAccess access_after;
 * } WisVKGlobalBarrier;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12GlobalBarrier {
 *     WisBarrierSync    sync_before;
 *     WisBarrierSync    sync_after;
 *     WisResourceAccess access_before;
 *     WisResourceAccess access_after;
 * } WisDX12GlobalBarrier;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct GlobalBarrier {
 *     wis::BarrierSync    sync_before;
 *     wis::BarrierSync    sync_after;
 *     wis::ResourceAccess access_before;
 *     wis::ResourceAccess access_after;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKGlobalBarrier {
 *     wis::BarrierSync    sync_before;
 *     wis::BarrierSync    sync_after;
 *     wis::ResourceAccess access_before;
 *     wis::ResourceAccess access_after;
 * };
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12GlobalBarrier {
 *     wis::BarrierSync    sync_before;
 *     wis::BarrierSync    sync_after;
 *     wis::ResourceAccess access_before;
 *     wis::ResourceAccess access_after;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisGlobalBarrier_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `sync_before` indicates synchronization scope before the barrier.
 * - `sync_after` specifies synchronization scope after the barrier.
 * - `access_before` defines access scope before the barrier.
 * - `access_after` indicates access scope after the barrier.
 * \endcond
 *
 * @section WisGlobalBarrier_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisGlobalBarrier_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBarrierGroup
 * \endcond
 */