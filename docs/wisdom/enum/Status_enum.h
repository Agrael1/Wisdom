/**
 * @ingroup Enumerations Core
 * @struct WisStatus
 * WisStatus - Common return status codes.
 *
 * @section Status_spec Specification
 * <hr>
 *
 * Common return status codes. Used to communicate success or failure of operations from the underlying API.
 * There are 2 categories of status codes: non-negative values indicate success, and negative values indicate failure.
 *
 * - Positive status codes provide additional information about the success of an operation.
 * - Negative status codes indicate various error conditions.
 * - Special value `WisStatusError` is used for general errors. This means that the operation failed, but no common
 * error code is suit to describe the error. Specific details @wis_may be provided by platform code or by other means
 * (e.g., error callbacks, logs, etc.).
 *
 * Common return status codes are:
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisStatus {
 *     WisStatusOk = 0,
 *     WisStatusTimeout = 1,
 *     WisStatusPartial = 2,
 *     WisStatusInvalidArgument = -1,
 *     WisStatusOutOfHostMemory = -2,
 *     WisStatusOutOfDeviceMemory = -3,
 *     WisStatusDeviceLost = -4,
 *     WisStatusOccluded = -5,
 *     WisStatusValidationFailed = -6,
 *     WisStatusFail = -7,
 *     WisStatusError = -10000,
 * } WisStatus;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class Status {
 *     Ok = 0,
 *     Timeout = 1,
 *     Partial = 2,
 *     InvalidArgument = -1,
 *     OutOfHostMemory = -2,
 *     OutOfDeviceMemory = -3,
 *     DeviceLost = -4,
 *     Occluded = -5,
 *     ValidationFailed = -6,
 *     Fail = -7,
 *     Error = -10000,
 * };
 * }
 * ```
 * \endcond
 *
 * @section Status_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Defines common return status codes. Compare against `WisStatusOk` for success.
 * 
 * Values:
 * - `WisStatusOk = 0`: Operation succeeded.
 * - `WisStatusTimeout = 1`: Operation timed out.
 * - `WisStatusPartial = 2`: Operation partially succeeded.
 * - `WisStatusInvalidArgument = -1`: One or more arguments, or parts of arguments passed to the function were incorrect.
 * - `WisStatusOutOfHostMemory = -2`: There is no more host memory available.
 * - `WisStatusOutOfDeviceMemory = -3`: There is no more device memory available.
 * - `WisStatusDeviceLost = -4`: Device driver was forcefully stopped.
 * - `WisStatusOccluded = -5`: Swap chain presentation was not visible to the user. Rendering is too fast.
 * - `WisStatusValidationFailed = -6`: A validation layer found an error.
 * - `WisStatusFail = -7`: Operation failed as expected.
 * - `WisStatusError = -10000`: Operation failed.
 * \endcond
 *
 * Performance critical commands @wis_should avoid returning error codes when possible. Instead they @wis_may report
 * errors on command list close.
 *
 * @section Status_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisResult
 * \endcond
 */
