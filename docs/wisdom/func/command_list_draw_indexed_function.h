/**
 * @struct wisCommandListDrawIndexed
 * @ingroup Functions Core
 *
 *
 * @section wisCommandListDrawIndexed_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisCommandListDrawIndexed(const WisCommandList* self,
 *                                uint32_t              index_count,
 *                                uint32_t              instance_count,
 *                                uint32_t              start_index,
 *                                int32_t               base_vertex,
 *                                uint32_t              start_instance);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0.
 * void wisVKCommandListDrawIndexed(const WisVKCommandList* self,
 *                                  uint32_t                index_count,
 *                                  uint32_t                instance_count,
 *                                  uint32_t                start_index,
 *                                  int32_t                 base_vertex,
 *                                  uint32_t                start_instance);
 *
 * // Provided by Wisdom 0.7.0.
 * void wisDX12CommandListDrawIndexed(const WisDX12CommandList* self,
 *                                    uint32_t                  index_count,
 *                                    uint32_t                  instance_count,
 *                                    uint32_t                  start_index,
 *                                    int32_t                   base_vertex,
 *                                    uint32_t                  start_instance);
 * ```
 * </details>
 *
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void CommandList::DrawIndexed(std::uint32_t index_count,
 *                               std::uint32_t instance_count,
 *                               std::uint32_t start_index,
 *                               std::int32_t  base_vertex,
 *                               std::uint32_t start_instance) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0.
 * void VKCommandList::DrawIndexed(std::uint32_t index_count,
 *                                 std::uint32_t instance_count,
 *                                 std::uint32_t start_index,
 *                                 std::int32_t  base_vertex,
 *                                 std::uint32_t start_instance) const noexcept;
 *
 * // Provided by Wisdom 0.7.0.
 * void DX12CommandList::DrawIndexed(std::uint32_t index_count,
 *                                   std::uint32_t instance_count,
 *                                   std::uint32_t start_index,
 *                                   std::int32_t  base_vertex,
 *                                   std::uint32_t start_instance) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListDrawIndexed_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `index_count` defines number of indices to draw.
 * - `instance_count` describes number of instances to draw; default is 1.
 * - `start_index` describes index of the first index to draw; default is 0.
 * - `base_vertex` defines value added to each index before reading a vertex from the vertex buffer; default is 0.
 * - `start_instance` specifies index of the first instance to draw; default is 0.
 * \endcond
 *
 * @section wisCommandListDrawIndexed_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListDrawIndexed_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */