/**
 * @struct WisDebugCallback
 * @ingroup Functions
 *
 *
 * @section WisDebugCallback_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef void (*WisDebugCallback)(WisSeverity severity, const char* message, uint64_t device, void* user_data);
 * ```
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * using DebugCallback = void (*)(wis::Severity severity, const char* message, std::uint64_t device, void* user_data);
 * }
 * ```
 * \endcond
 *
 * @section WisDebugCallback_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - `severity` indicates message severity level.
 * - `message` describes contains the debug message string.
 * - `device` defines handle to the device that generated the message. Can be `0` if message is not device specific.
 * - `user_data` defines user defined data pointer passed during callback registration.
 * \endcond
 *
 * @section WisDebugCallback_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisDebugCallback_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisDebugDesc
 * \endcond
 */