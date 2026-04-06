/**
 * @struct wisPipelineCacheGetSerializedSize
 * @ingroup Functions Core
 *
 *
 * @section wisPipelineCacheGetSerializedSize_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * size_t wisPipelineCacheGetSerializedSize(const WisPipelineCache* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * size_t wisVKPipelineCacheGetSerializedSize(const WisVKPipelineCache* self);
 *
 * // Provided by Wisdom 0.7.0.
 * size_t wisDX12PipelineCacheGetSerializedSize(const WisDX12PipelineCache* self);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD std::size_t PipelineCache::GetSerializedSize() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD std::size_t VKPipelineCache::GetSerializedSize() const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * WIS_NODISCARD std::size_t DX12PipelineCache::GetSerializedSize() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisPipelineCacheGetSerializedSize_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisPipelineCache instance.
 *
 * - **return** Size of the data in bytes.
 * \endcond
 *
 * @section wisPipelineCacheGetSerializedSize_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisPipelineCacheGetSerializedSize_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */