/**
 * @struct WisBarrierSync WisBarrierSync
 * @ingroup Enumerations Core
 *
 * @section WisBarrierSync_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisBarrierSync {
 *     WisBarrierSyncNone = 0,
 *     WisBarrierSyncAll = (1u << 0),
 *     WisBarrierSyncDraw = (1u << 1),
 *     WisBarrierSyncIndexInput = (1u << 2),
 *     WisBarrierSyncVertexShading = (1u << 3),
 *     WisBarrierSyncPixelShading = (1u << 4),
 *     WisBarrierSyncDepthStencil = (1u << 5),
 *     WisBarrierSyncRenderTarget = (1u << 6),
 *     WisBarrierSyncCompute = (1u << 7),
 *     WisBarrierSyncRaytracing = (1u << 8),
 *     WisBarrierSyncCopy = (1u << 9),
 *     WisBarrierSyncResolve = (1u << 10),
 *     WisBarrierSyncExecuteIndirect = (1u << 11),
 *     WisBarrierSyncAllShading = (1u << 12),
 *     WisBarrierSyncNonPixelShading = (1u << 13),
 *     WisBarrierSyncClearUAV = (1u << 14),
 *     WisBarrierSyncVideoDecode = (1u << 15),
 *     WisBarrierSyncVideoEncode = (1u << 16),
 *     WisBarrierSyncBuildRTAS = (1u << 17),
 *     WisBarrierSyncCopyRTAS = (1u << 18),
 * } WisBarrierSync;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class BarrierSync : uint32_t {
 *     None = 0,
 *     All = (1u << 0),
 *     Draw = (1u << 1),
 *     IndexInput = (1u << 2),
 *     VertexShading = (1u << 3),
 *     PixelShading = (1u << 4),
 *     DepthStencil = (1u << 5),
 *     RenderTarget = (1u << 6),
 *     Compute = (1u << 7),
 *     Raytracing = (1u << 8),
 *     Copy = (1u << 9),
 *     Resolve = (1u << 10),
 *     ExecuteIndirect = (1u << 11),
 *     AllShading = (1u << 12),
 *     NonPixelShading = (1u << 13),
 *     ClearUAV = (1u << 14),
 *     VideoDecode = (1u << 15),
 *     VideoEncode = (1u << 16),
 *     BuildRTAS = (1u << 17),
 *     CopyRTAS = (1u << 18),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisBarrierSync_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Barrier synchronization flags for resource barriers.
 *
 * \note Translates to DirectX 12 as D3D12_BARRIER_SYNC, Vulkan as VkPipelineStageFlags2.
 *
 * Values:
 * - `WisBarrierSyncNone = 0`: No synchronization is performed.
 * - `WisBarrierSyncAll = (1 << 0)`: Synchronize all commands.
 * - `WisBarrierSyncDraw = (1 << 1)`: Synchronize draw commands.
 * - `WisBarrierSyncIndexInput = (1 << 2)`: Synchronize index input commands.
 * - `WisBarrierSyncVertexShading = (1 << 3)`: Synchronize vertex shading commands.
 * - `WisBarrierSyncPixelShading = (1 << 4)`: Synchronize pixel shading commands.
 * - `WisBarrierSyncDepthStencil = (1 << 5)`: Synchronize depth-stencil commands.
 * - `WisBarrierSyncRenderTarget = (1 << 6)`: Synchronize render target commands.
 * - `WisBarrierSyncCompute = (1 << 7)`: Synchronize compute commands.
 * - `WisBarrierSyncRaytracing = (1 << 8)`: Synchronize ray tracing commands.
 * - `WisBarrierSyncCopy = (1 << 9)`: Synchronize copy commands.
 * - `WisBarrierSyncResolve = (1 << 10)`: Synchronize resolve commands.
 * - `WisBarrierSyncExecuteIndirect = (1 << 11)`: Synchronize execute indirect commands.
 * - `WisBarrierSyncAllShading = (1 << 12)`: Synchronize all shading commands.
 * - `WisBarrierSyncNonPixelShading = (1 << 13)`: Synchronize non-pixel shading commands.
 * - `WisBarrierSyncClearUAV = (1 << 14)`: Synchronize UAV clear commands.
 * - `WisBarrierSyncVideoDecode = (1 << 15)`: Synchronize video decode commands.
 * - `WisBarrierSyncVideoEncode = (1 << 16)`: Synchronize video encode commands.
 * - `WisBarrierSyncBuildRTAS = (1 << 17)`: Synchronize ray tracing acceleration structure build commands.
 * - `WisBarrierSyncCopyRTAS = (1 << 18)`: Synchronize ray tracing acceleration structure copy commands.
 * \endcond
 *
 *
 * @section WisBarrierSync_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisBufferBarrier, WisGlobalBarrier, WisTextureBarrier
 * \endcond
 */
