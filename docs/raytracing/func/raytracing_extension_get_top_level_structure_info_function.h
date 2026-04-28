/**
 * @struct wisRaytracingExtensionGetTopLevelStructureInfo
 * @ingroup Functions Raytracing
 *
 *
 * @section wisRaytracingExtensionGetTopLevelStructureInfo_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisRaytracingExtensionGetTopLevelStructureInfo(WisRaytracingExtension* self,
 *                                                          const WisTopLevelStructureBuildDesc* build_desc,
 *                                                          WisStructureAllocationInfo*          info);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVKRaytracingExtensionGetTopLevelStructureInfo(WisVKRaytracingExtension* self,
 *                                                            const WisTopLevelStructureBuildDesc* build_desc,
 *                                                            WisStructureAllocationInfo*          info);
 *
 * // Provided by Wisdom 0.7.1.
 * WisResult wisDX12RaytracingExtensionGetTopLevelStructureInfo(WisDX12RaytracingExtension* self,
 *                                                              const WisTopLevelStructureBuildDesc* build_desc,
 *                                                              WisStructureAllocationInfo*          info);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::StructureAllocationInfo RaytracingExtension::GetTopLevelStructureInfo(const
 * wis::TopLevelStructureBuildDesc& build_desc, wis::Result&                           out_result) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::StructureAllocationInfo VKRaytracingExtension::GetTopLevelStructureInfo(const
 * wis::TopLevelStructureBuildDesc& build_desc, wis::Result&                           out_result) noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::StructureAllocationInfo DX12RaytracingExtension::GetTopLevelStructureInfo(const
 * wis::TopLevelStructureBuildDesc& build_desc, wis::Result&                           out_result) noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisRaytracingExtensionGetTopLevelStructureInfo_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisRaytracingExtension instance.
 * - `build_desc` The build description for the bottom-level acceleration structure.
 * - `info` The allocation information for the bottom-level acceleration structure.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisRaytracingExtensionGetTopLevelStructureInfo_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisRaytracingExtensionGetTopLevelStructureInfo_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
