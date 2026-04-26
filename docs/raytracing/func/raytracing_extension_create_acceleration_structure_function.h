/**
 * @struct wisRaytracingExtensionCreateAccelerationStructure
 * @ingroup Functions Raytracing
 *
 *
 * @section wisRaytracingExtensionCreateAccelerationStructure_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisRaytracingExtensionCreateAccelerationStructure(WisRaytracingExtension* self,
 *                                                             WisBuffer*                          buffer,
 *                                                             const WisAccelerationStructureDesc* desc,
 *                                                             WisAccelerationStructure* acceleration_structure);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.1.
 * WisResult wisVKRaytracingExtensionCreateAccelerationStructure(WisVKRaytracingExtension* self,
 *                                                               WisVKBuffer*                          buffer,
 *                                                               const WisVKAccelerationStructureDesc* desc,
 *                                                               WisVKAccelerationStructure* acceleration_structure);
 *
 * // Provided by Wisdom 0.7.1.
 * WisResult wisDX12RaytracingExtensionCreateAccelerationStructure(WisDX12RaytracingExtension* self,
 *                                                                 WisDX12Buffer*                          buffer,
 *                                                                 const WisDX12AccelerationStructureDesc* desc,
 *                                                                 WisDX12AccelerationStructure*
 * acceleration_structure);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::AccelerationStructure RaytracingExtension::CreateAccelerationStructure(wis::Buffer& buffer, const
 * wis::AccelerationStructureDesc& desc, wis::Result&                          out_result) noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::VKAccelerationStructure VKRaytracingExtension::CreateAccelerationStructure(wis::VKBuffer& buffer,
 *                                                                                               const
 * wis::VKAccelerationStructureDesc& desc, wis::Result&                            out_result) noexcept;
 *
 * // Provided by Wisdom 0.7.1.
 * WIS_NODISCARD wis::DX12AccelerationStructure DX12RaytracingExtension::CreateAccelerationStructure(wis::DX12Buffer&
 * buffer, const wis::DX12AccelerationStructureDesc& desc, wis::Result&                              out_result)
 * noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisRaytracingExtensionCreateAccelerationStructure_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisRaytracingExtension instance.
 * - `buffer` The buffer to write the acceleration structure data to.
 * - `desc` The description of the acceleration structure to create.
 * - `acceleration_structure` The created acceleration structure handle.
 *
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisRaytracingExtensionCreateAccelerationStructure_descr Description
 * <hr>
 *
 * The resulting acceleration structure does not reference the provided buffer. The buffer @wis_must be created with
 * `WisBufferUsageFlagsAccelerationStructureBuffer` usage flag.
 * 
 * @warning The resulting acceleration structure does not hold a reference to a device or buffer. The caller is
 * responsible for ensuring that the buffer and device remain valid until the acceleration structure is destroyed.
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisRaytracingExtensionCreateAccelerationStructure_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */
