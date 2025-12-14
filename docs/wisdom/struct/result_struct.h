/**
 * @ingroup Structures
 * @struct WisResult
 *
 *
 * @section Result_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef struct WIS_NODISCARD WisResult {
 *     WisStatus   status;
 *     int32_t     platform_code;
 *     const char* error;
 * } WisResult;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * struct WIS_NODISCARD Result {
 *     wis::Status  status;
 *     std::int32_t platform_code;
 *     const char*  error;
 * };
 * }
 * ```
 * \endcond
 *
 * @section Result_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `status` defines operation status. Compare with `WisStatusOk`.
 * - `platform_code` defines platfrom code from underlying implementation. Is an `HRESULT` for DX12 and a `VkResult` for Vulkan.
 * - `error` contains a human readable error message.
 * \endcond
 *
 * @section Result_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section Result_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
