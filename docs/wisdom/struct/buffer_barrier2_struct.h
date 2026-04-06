/**
 * @struct WisBufferBarrier2
 * @ingroup Structures Core
 *
 *
 * @section WisBufferBarrier2_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisBufferBarrier2 {
 *     WisBufferBarrier barrier;
 *     WisBufferView    buffer;
 * } WisBufferBarrier2;
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisVKBufferBarrier2 {
 *     WisBufferBarrier barrier;
 *     WisVKBufferView  buffer;
 * } WisVKBufferBarrier2;
 * 
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WisDX12BufferBarrier2 {
 *     WisBufferBarrier  barrier;
 *     WisDX12BufferView buffer;
 * } WisDX12BufferBarrier2;
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct BufferBarrier2 
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct VKBufferBarrier2 ;
 * 
 * // Provided by Wisdom 0.7.0. 
 * struct DX12BufferBarrier2 
 * ```
 * </details>
 * 
 * \endcond
 *
 * @section WisBufferBarrier2_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 *  * - `barrier` Buffer barrier description.
 * - `buffer` Buffer view.
 * 
 * \endcond
 *
 * @section WisBufferBarrier2_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisBufferBarrier2_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
