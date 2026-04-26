/**
 * @struct wisRaytracingExtensionGetBottomLevelStructureInfo
 * @ingroup Functions Raytracing
 *
 *
 * @section wisRaytracingExtensionGetBottomLevelStructureInfo_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisRaytracingExtensionGetBottomLevelStructureInfo(WisRaytracingExtension* self,
 *                                                             const WisBottomLevelStructureBuildDesc* build_desc,
 *                                                             WisStructureAllocationInfo*             info);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVKRaytracingExtensionGetBottomLevelStructureInfo(WisVKRaytracingExtension* self,
 *                                                               const WisBottomLevelStructureBuildDesc* build_desc,
 *                                                               WisStructureAllocationInfo*             info);
 *
 * // Provided by Wisdom 0.7.1.
 * WisResult wisDX12RaytracingExtensionGetBottomLevelStructureInfo(WisDX12RaytracingExtension* self,
 *                                                                 const WisBottomLevelStructureBuildDesc* build_desc,
 *                                                                 WisStructureAllocationInfo*             info);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::StructureAllocationInfo RaytracingExtension::GetBottomLevelStructureInfo(const
 * wis::BottomLevelStructureBuildDesc& build_desc, wis::Result&                              out_result) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::StructureAllocationInfo VKRaytracingExtension::GetBottomLevelStructureInfo(const
 * wis::BottomLevelStructureBuildDesc& build_desc, wis::Result&                              out_result) noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::StructureAllocationInfo DX12RaytracingExtension::GetBottomLevelStructureInfo(const
 * wis::BottomLevelStructureBuildDesc& build_desc, wis::Result&                              out_result) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisRaytracingExtensionGetBottomLevelStructureInfo_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisRaytracingExtension instance.
 * - `build_desc` The build description for the bottom-level acceleration structure.
 * - `info` The allocation information for the bottom-level acceleration structure.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisRaytracingExtensionGetBottomLevelStructureInfo_descr Description
 * <hr>
 *
 * The function does not dereference the GPU addresses. It @wis_may only do null checks on the GPU addresses, while all
 * the CPU parameters in the build description are used to determine the allocation sizes.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisRaytracingExtensionGetBottomLevelStructureInfo_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
