/**
 * @struct WisStdVideoAV1TransferCharacteristics WisStdVideoAV1TransferCharacteristics
 * @ingroup Enumerations Video
 *
 * @section WisStdVideoAV1TransferCharacteristics_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef enum WisStdVideoAV1TransferCharacteristics {
 *     WisStdVideoAV1TransferCharacteristicsReserved0 = 0,
 *     WisStdVideoAV1TransferCharacteristicsBt709 = 1,
 *     WisStdVideoAV1TransferCharacteristicsUnspecified = 2,
 *     WisStdVideoAV1TransferCharacteristicsReserved3 = 3,
 *     WisStdVideoAV1TransferCharacteristicsBt470M = 4,
 *     WisStdVideoAV1TransferCharacteristicsBt470BG = 5,
 *     WisStdVideoAV1TransferCharacteristicsBt601 = 6,
 *     WisStdVideoAV1TransferCharacteristicsSmpte240 = 7,
 *     WisStdVideoAV1TransferCharacteristicsLinear = 8,
 *     WisStdVideoAV1TransferCharacteristicsLog100 = 9,
 *     WisStdVideoAV1TransferCharacteristicsLog100Sqrt10 = 10,
 *     WisStdVideoAV1TransferCharacteristicsIec61966 = 11,
 *     WisStdVideoAV1TransferCharacteristicsBt1361 = 12,
 *     WisStdVideoAV1TransferCharacteristicsSrgb = 13,
 *     WisStdVideoAV1TransferCharacteristicsBt2020_10Bit = 14,
 *     WisStdVideoAV1TransferCharacteristicsBt2020_12Bit = 15,
 *     WisStdVideoAV1TransferCharacteristicsSmpte2084 = 16,
 *     WisStdVideoAV1TransferCharacteristicsSmpte428 = 17,
 *     WisStdVideoAV1TransferCharacteristicsHlg = 18,
 *     WisStdVideoAV1TransferCharacteristicsInvalid = 0x7FFFFFFF,
 * } WisStdVideoAV1TransferCharacteristics;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * enum class StdVideoAV1TransferCharacteristics {
 *     Reserved0 = 0,
 *     Bt709 = 1,
 *     Unspecified = 2,
 *     Reserved3 = 3,
 *     Bt470M = 4,
 *     Bt470BG = 5,
 *     Bt601 = 6,
 *     Smpte240 = 7,
 *     Linear = 8,
 *     Log100 = 9,
 *     Log100Sqrt10 = 10,
 *     Iec61966 = 11,
 *     Bt1361 = 12,
 *     Srgb = 13,
 *     Bt2020_10Bit = 14,
 *     Bt2020_12Bit = 15,
 *     Smpte2084 = 16,
 *     Smpte428 = 17,
 *     Hlg = 18,
 *     Invalid = 0x7FFFFFFF,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1TransferCharacteristics_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Transfer characteristics for AV1.
 *
 * Values:
 * - `WisStdVideoAV1TransferCharacteristicsReserved0 = 0`:
 * - `WisStdVideoAV1TransferCharacteristicsBt709 = 1`: Rec. ITU-R BT.709-6.
 * - `WisStdVideoAV1TransferCharacteristicsUnspecified = 2`: Unspecified.
 * - `WisStdVideoAV1TransferCharacteristicsReserved3 = 3`:
 * - `WisStdVideoAV1TransferCharacteristicsBt470M = 4`: Rec. ITU-R BT.470-6 System M (historical).
 * - `WisStdVideoAV1TransferCharacteristicsBt470BG = 5`: Rec. ITU-R BT.470-6 System B, G (historical).
 * - `WisStdVideoAV1TransferCharacteristicsBt601 = 6`: Rec. ITU-R BT.601-7.
 * - `WisStdVideoAV1TransferCharacteristicsSmpte240 = 7`: SMPTE 240M.
 * - `WisStdVideoAV1TransferCharacteristicsLinear = 8`: Linear transfer characteristics.
 * - `WisStdVideoAV1TransferCharacteristicsLog100 = 9`: Logarithmic transfer characteristic (100:1 range).
 * - `WisStdVideoAV1TransferCharacteristicsLog100Sqrt10 = 10`: Logarithmic transfer characteristic (100 * Sqrt(10) : 1
 * range).
 * - `WisStdVideoAV1TransferCharacteristicsIec61966 = 11`: IEC 61966-2-4.
 * - `WisStdVideoAV1TransferCharacteristicsBt1361 = 12`: Rec. ITU-R BT.1361-0 extended colour gamut system (historical).
 * - `WisStdVideoAV1TransferCharacteristicsSrgb = 13`: IEC 61966-2-1 sRGB.
 * - `WisStdVideoAV1TransferCharacteristicsBt2020_10Bit = 14`: Rec. ITU-R BT.2020-2 (10-bit system).
 * - `WisStdVideoAV1TransferCharacteristicsBt2020_12Bit = 15`: Rec. ITU-R BT.2020-2 (12-bit system).
 * - `WisStdVideoAV1TransferCharacteristicsSmpte2084 = 16`: SMPTE ST 2084 (PQ).
 * - `WisStdVideoAV1TransferCharacteristicsSmpte428 = 17`: SMPTE ST 428-1.
 * - `WisStdVideoAV1TransferCharacteristicsHlg = 18`: ARIB STD-B67 (HLG).
 * - `WisStdVideoAV1TransferCharacteristicsInvalid = 0x7FFFFFFF`:
 * \endcond
 *
 *
 * @section WisStdVideoAV1TransferCharacteristics_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoAV1ColorConfig
 * \endcond
 */
