/**
 * @ingroup Structures Core
 * @struct WisResult
 *
 *
 * @section Result_spec Specification
 * <hr>
 * 
 * When an operation is performed, a `WisResult` structure is returned to indicate the outcome of the operation.
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
 * - `platform_code` indicates platform code from underlying implementation. Is an `HRESULT` for DX12 and a `VkResult` for Vulkan.
 * - `error` specifies contains a human readable error message.
 * \endcond
 *
 * @section Result_descr Description
 * <hr>
 * 
 * To check if an operation succeeded, the return code is greater or equal to 0. Negative values indicate failure.
 * Some operations @wis_may return positive codes other than `WisStatusOk`, indicating partial success, timeout or additional information about the operation.
 * The structure also contains `platform_code`, which provides additional information about the result from the underlying graphics API.
 * 
 * `error` contains a human-readable error message, which @wis_may provide additional context about the failure. The message is static and @wis_must not be freed or modified.
 * 
 * \note In C implementation, the structure is returned by value. In C++ implementation, because of RVO optimizations, most functions return wis::Result by reference.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section Result_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
