/**
 * @struct WisQueryStructHeader
 * @ingroup Structures Core
 *
 *
 * @section WisQueryStructHeader_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisQueryStructHeader {
 *     WisQueryPropertyType property_type;
 *     void*                next_in_chain;
 * } WisQueryStructHeader;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  QueryStructHeader {
 *     wis::QueryPropertyType property_type;
 *     void*                  next_in_chain;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisQueryStructHeader_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `property_type` specifies the type of the queried property. Used to determine what struct is passed.
 * - `next_in_chain` indicates pointer to the next queried data struct.
 * \endcond
 *
 * @section WisQueryStructHeader_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisQueryStructHeader_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */