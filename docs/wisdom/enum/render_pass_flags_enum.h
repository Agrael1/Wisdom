/**
 * @struct WisRenderPassFlags WisRenderPassFlags
 * @ingroup Enumerations Core
 *
 * @section WisRenderPassFlags_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisRenderPassFlags {
 *     WisRenderPassFlagsNone = 0,
 *     WisRenderPassFlagsSuspending = (1u << 1),
 *     WisRenderPassFlagsResuming = (1u << 2),
 *     WisRenderPassFlagsAllowUAVWrites = (1u << 3),
 * } WisRenderPassFlags;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class RenderPassFlags : uint32_t {
 *     None = 0,
 *     Suspending = (1u << 1),
 *     Resuming = (1u << 2),
 *     AllowUAVWrites = (1u << 3),
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisRenderPassFlags_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Set of flags that affect render pass behavior.
 * More on render pass flags [here](https://learn.microsoft.com/en-us/windows/win32/direct3d12/direct3d-12-render-passes).
 *
 * \note Translates to DirectX 12 as D3D12_RENDER_PASS_FLAGS, Vulkan as VkRenderingFlags.
 *
 * Values:
 * - `WisRenderPassFlagsNone = 0`: No flags set. Render pass is regular.
 * - `WisRenderPassFlagsSuspending = (1 << 1)`: Render pass is suspending.
 * - `WisRenderPassFlagsResuming = (1 << 2)`: Render pass is resuming.
 * - `WisRenderPassFlagsAllowUAVWrites = (1 << 3)`: Allow UAV writes. If set, unordered access view (UAV) writes are allowed during the render pass. If not set, UAV writes are not allowed and @wis_may result in undefined behavior if attempted.
 * \endcond
 *
 *
 * @section WisRenderPassFlags_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisRenderPassDesc
 * \endcond
 */
