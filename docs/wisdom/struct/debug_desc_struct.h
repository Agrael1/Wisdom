/**
 * @struct WisDebugDesc
 * @ingroup Structures
 *
 *
 * @section WisDebugDesc_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisDebugDesc {
 *     bool             enable_debug_layer;
 *     WisDebugCallback callback;
 *     void*            user_data;
 * } WisDebugDesc;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  DebugDesc {
 *     bool               enable_debug_layer;
 *     wis::DebugCallback callback;
 *     void*              user_data;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDebugDesc_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `enable_debug_layer` enables or disables debug layer on both DX12 and VK backends.
 * - `callback` defines the debug callback function.
 * - `user_data` user defined data pointer passed to the callback.
 * \endcond
 *
 * @section WisDebugDesc_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDebugDesc_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCreateInstance
 * \endcond
 */