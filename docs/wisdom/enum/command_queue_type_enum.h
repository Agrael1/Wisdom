/**
 * @struct WisCommandQueueType WisCommandQueueType
 * @ingroup Enumerations
 *
 * @section WisCommandQueueType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisCommandQueueType {
 *     WisCommandQueueTypeGraphics = 0,
 *     WisCommandQueueTypeCompute = 1,
 *     WisCommandQueueTypeTransfer = 2,
 *     WisCommandQueueTypeVideoDecode = 3,
 *     WisCommandQueueTypeVideoEncode = 4,
 *     WisCommandQueueTypeCount = 5,
 * } WisCommandQueueType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class CommandQueueType {
 *     Graphics = 0,
 *     Compute = 1,
 *     Transfer = 2,
 *     VideoDecode = 3,
 *     VideoEncode = 4,
 *     Count = 5,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisCommandQueueType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Defines types of command queues that can be created on the device. Each type corresponds to a specific kind of workload that the GPU can handle.
 * 
 * \note Translates to  `D3D12_COMMAND_LIST_TYPE` for DirectX 12 implementation.
 * 
 * Values:
 * - `WisCommandQueueTypeGraphics = 0`: Command queue for graphics operations, including rendering and compute tasks.
 * - `WisCommandQueueTypeCompute = 1`: Command queue dedicated to compute operations, optimized for parallel processing tasks.
 * - `WisCommandQueueTypeTransfer = 2`: Command queue for data transfer operations, such as copying resources between buffers and images.
 * - `WisCommandQueueTypeVideoDecode = 3`: Command queue for video decoding operations.
 * - `WisCommandQueueTypeVideoEncode = 4`: Command queue for video encoding operations.
 * - `WisCommandQueueTypeCount = 5`: Number of command queue types available.
 * \endcond
 *
 *
 * @section WisCommandQueueType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisCommandQueueDesc, WisBufferBarrier, WisTextureBarrier
 * Functions:
 * wisDeviceCreateCommandQueue, wisDeviceCreateCommandAllocator
 * \endcond
 */