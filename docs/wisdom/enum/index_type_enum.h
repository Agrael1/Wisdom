/**
 * @struct WisIndexType WisIndexType
 * @ingroup Enumerations Core
 *
 * @section WisIndexType_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisIndexType {
 *     WisIndexTypeUInt16 = 2,
 *     WisIndexTypeUInt32 = 4,
 * } WisIndexType;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class IndexType {
 *     UInt16 = 2,
 *     UInt32 = 4,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisIndexType_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Index type for index buffer.
 * Enum values resemble the byte stride of the format.
 *
 * \note Translates to  `DXGI_FORMAT` for DirectX 12 implementation, and `VkIndexType` for Vulkan implementation.
 *
 * Values:
 * - `WisIndexTypeUInt16 = 2`: 16-bit unsigned integer index type.
 * - `WisIndexTypeUInt32 = 4`: 32-bit unsigned integer index type.
 * \endcond
 *
 *
 * @section WisIndexType_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisCommandListSetIndexBuffer, wisCommandListSetIndexBuffer2
 * \endcond
 */
