/**
 * @struct WisBarrierGroup
 * @ingroup Structures
 *
 *
 * @section WisBarrierGroup_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisBarrierGroup {
 *     const WisBufferBarrier*  buffer_barriers;
 *     size_t                   buffer_barrier_count;
 *     const WisTextureBarrier* texture_barriers;
 *     size_t                   texture_barrier_count;
 *     const WisGlobalBarrier*  global_barriers;
 *     size_t                   global_barrier_count;
 * } WisBarrierGroup;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKBarrierGroup {
 *     const WisVKBufferBarrier*  buffer_barriers;
 *     size_t                     buffer_barrier_count;
 *     const WisVKTextureBarrier* texture_barriers;
 *     size_t                     texture_barrier_count;
 *     const WisVKGlobalBarrier*  global_barriers;
 *     size_t                     global_barrier_count;
 * } WisVKBarrierGroup;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12BarrierGroup {
 *     const WisDX12BufferBarrier*  buffer_barriers;
 *     size_t                       buffer_barrier_count;
 *     const WisDX12TextureBarrier* texture_barriers;
 *     size_t                       texture_barrier_count;
 *     const WisDX12GlobalBarrier*  global_barriers;
 *     size_t                       global_barrier_count;
 * } WisDX12BarrierGroup;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct BarrierGroup {
 *     wis::span<const wis::BufferBarrier>  buffer_barriers;
 *     wis::span<const wis::TextureBarrier> texture_barriers;
 *     wis::span<const wis::GlobalBarrier>  global_barriers;
 * };
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKBarrierGroup {
 *     wis::span<const wis::VKBufferBarrier>  buffer_barriers;
 *     wis::span<const wis::VKTextureBarrier> texture_barriers;
 *     wis::span<const wis::VKGlobalBarrier>  global_barriers;
 * };
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12BarrierGroup {
 *     wis::span<const wis::DX12BufferBarrier>  buffer_barriers;
 *     wis::span<const wis::DX12TextureBarrier> texture_barriers;
 *     wis::span<const wis::DX12GlobalBarrier>  global_barriers;
 * };
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section WisBarrierGroup_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `buffer_barriers` Array of buffer barriers.
 * - `buffer_barrier_count` Number of buffer barriers in the `WisBarrierGroup::buffer_barriers` array.
 * - `texture_barriers` Array of texture barriers.
 * - `texture_barrier_count` Number of texture barriers in the `WisBarrierGroup::texture_barriers` array.
 * - `global_barriers` Array of global barriers.
 * - `global_barrier_count` Number of global barriers in the `WisBarrierGroup::global_barriers` array.
 * \endcond
 *
 * @section WisBarrierGroup_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBarrierGroup_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see wisCommandListInsertBarriers
 * \endcond
 */