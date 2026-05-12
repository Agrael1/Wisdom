/**
 * @struct WisStdVideoAV1TileInfo
 * @ingroup Structures Video
 *
 *
 * @section WisStdVideoAV1TileInfo_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * typedef struct  WisStdVideoAV1TileInfo {
 *     WisStdVideoAV1TileInfoFlags flags;
 *     uint8_t                     TileCols;
 *     uint8_t                     TileRows;
 *     uint16_t                    context_update_tile_id;
 *     uint8_t                     tile_size_bytes_minus_1;
 *     uint8_t                     reserved1[7];
 *     const uint16_t*             pMiColStarts;
 *     const uint16_t*             pMiRowStarts;
 *     const uint16_t*             pWidthInSbsMinus1;
 *     const uint16_t*             pHeightInSbsMinus1;
 * } WisStdVideoAV1TileInfo;
 *
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * struct  StdVideoAV1TileInfo {
 *     wis::StdVideoAV1TileInfoFlags flags;
 *     std::uint8_t                  TileCols;
 *     std::uint8_t                  TileRows;
 *     std::uint16_t                 context_update_tile_id;
 *     std::uint8_t                  tile_size_bytes_minus_1;
 *     std::array<std::uint8_t, 7>   reserved1;
 *     const std::uint16_t*          pMiColStarts;
 *     const std::uint16_t*          pMiRowStarts;
 *     const std::uint16_t*          pWidthInSbsMinus1;
 *     const std::uint16_t*          pHeightInSbsMinus1;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisStdVideoAV1TileInfo_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `flags` Tile info flags.
 * - `TileCols` Number of tiles across the picture.
 * - `TileRows` Number of tiles down the picture.
 * - `context_update_tile_id` Specifies which tile to use for the CDF update.
 * - `tile_size_bytes_minus_1` Specifies the number of bytes needed to code each tile size.
 * - `reserved1` No description.
 * - `pMiColStarts` Pointer to an array specifying the start column (in MI units) for each tile column.
 * - `pMiRowStarts` Pointer to an array specifying the start row (in MI units) for each tile row.
 * - `pWidthInSbsMinus1` Pointer to an array of tile widths in superblocks minus 1.
 * - `pHeightInSbsMinus1` Pointer to an array of tile heights in superblocks minus 1.
 * \endcond
 *
 * @section WisStdVideoAV1TileInfo_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisStdVideoAV1TileInfo_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Structs:
 * WisStdVideoDecodeAV1PictureInfo
 * \endcond
 */
