/**
 * @struct wisPipelineCacheSerialize
 * @ingroup Functions
 *
 *
 * @section wisPipelineCacheSerialize_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisPipelineCacheSerialize(const WisPipelineCache* self,
 *                                     uint8_t*                data,
 *                                     size_t                  data_size);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisVKPipelineCacheSerialize(const WisVKPipelineCache* self,
 *                                       uint8_t*                  data,
 *                                       size_t                    data_size);
 * 
 * // Provided by Wisdom 0.7.0. 
 * WisResult wisDX12PipelineCacheSerialize(const WisDX12PipelineCache* self,
 *                                         uint8_t*                    data,
 *                                         size_t                      data_size);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result PipelineCache::Serialize(wis::span<std::uint8_t> data) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * wis::Result VKPipelineCache::Serialize(wis::span<std::uint8_t> data) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * wis::Result DX12PipelineCache::Serialize(wis::span<std::uint8_t> data) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisPipelineCacheSerialize_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisPipelineCache instance.
 * - `data` points to an array that is filled with serialized cache data on success.
 * - `data_size` defines the size of the data chunk in bytes. It @wis_must be greater or equal to the value returned by wisPipelineCacheGetSerializedSize.
 * 
 * - **return** denoting the outcome of operation.
 * \endcond
 *
 * @section wisPipelineCacheSerialize_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisPipelineCacheSerialize_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */