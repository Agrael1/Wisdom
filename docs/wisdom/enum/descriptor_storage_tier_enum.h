/**
 * @struct WisDescriptorStorageTier WisDescriptorStorageTier
 * @ingroup Enumerations Core
 *
 * @section WisDescriptorStorageTier_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * typedef enum WisDescriptorStorageTier {
 *     WisDescriptorStorageTierTier1 = 0,
 *     WisDescriptorStorageTierTier2 = 1,
 *     WisDescriptorStorageTierTier3 = 2,
 *     WisDescriptorStorageTierTier4 = 3,
 * } WisDescriptorStorageTier;
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * enum class DescriptorStorageTier {
 *     Tier1 = 0,
 *     Tier2 = 1,
 *     Tier3 = 2,
 *     Tier4 = 3,
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisDescriptorStorageTier_descr Description
 * <hr>
 * \cond WIS_GEN_DESC
 * Descriptor storage tier. Decides how many descriptors can be allocated in a single heap.
 *
 * Values:
 * - `WisDescriptorStorageTierTier1 = 0`: Tier 1: VkDescriptorSets and VkDescriptorPools.
 * - `WisDescriptorStorageTierTier2 = 1`: Tier 2: Descriptor Buffer with no mutable descriptor type.
 * - `WisDescriptorStorageTierTier3 = 2`: Tier 3: Descriptor Buffer with mutable descriptor type.
 * - `WisDescriptorStorageTierTier4 = 3`: Tier 4: Descriptor Heap.
 * \endcond
 *
 *
 * @section WisDescriptorStorageTier_see_also See Also
 * <hr>
 *
 * \cond WIS_GEN_REFS
 * \endcond
 */
