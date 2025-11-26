/**
 * @ingroup Structures
 * @struct WisResult
 *
 *
 * @section Result_spec C Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct WIS_NODISCARD WisResult {
 *     WisStatus   status;
 *     int32_t     platform_code;
 *     const char* error;
 * } WisResult;
 * 
 * ```
 * \endcond
 *
 * @section Result_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `status` defines operation status. Compare with WisStatusOk.
 * - `platform_code` defines platfrom code from underlying implementation. Can be compared with `HRESULT` for DX12 and `VkResult` for Vulkan.
 * - `error` contains a human readable error message.
 * \endcond
 *
 * @section Result_descr Description
 * <hr>
 *
 *
 * @section Result_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */