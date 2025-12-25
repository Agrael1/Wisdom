/**
 * @struct WisSeverity WisSeverity
 * @ingroup Enumerations
 *
 * @section WisSeverity_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef enum WisSeverity {
 *     WisSeverityVerbose = 0,
 *     WisSeverityInfo = 1,
 *     WisSeverityWarning = 2,
 *     WisSeverityError = 3,
 *     WisSeverityFatal = 4,
 * } WisSeverity;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * enum class Severity {
 *     Verbose = 0,
 *     Info = 1,
 *     Warning = 2,
 *     Error = 3,
 *     Fatal = 4,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSeverity_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Defines severity levels for logging and debugging messages.
 * 
 * Values:
 * - `WisSeverityVerbose = 0`: Verbose level messages, typically used for detailed debugging information.
 * - `WisSeverityInfo = 1`: Informational messages that highlight the progress of the application.
 * - `WisSeverityWarning = 2`: Potentially harmful situations that warrant attention but do not prevent normal operation.
 * - `WisSeverityError = 3`: Error events that might still allow the application to continue running.
 * - `WisSeverityFatal = 4`: Severe error events that will presumably lead the application to abort.
 * \endcond
 *
 *
 * @section WisSeverity_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see WisDebugCallback
 * \endcond
 */