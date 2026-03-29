/**
 * @struct wisCommandListDraw
 * @ingroup Functions
 *
 *
 * @section wisCommandListDraw_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisCommandListDraw(const WisCommandList* self,
 *                         uint32_t              vertex_count,
 *                         uint32_t              instance_count,
 *                         uint32_t              start_vertex,
 *                         uint32_t              start_instance);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void wisVKCommandListDraw(const WisVKCommandList* self,
 *                           uint32_t                vertex_count,
 *                           uint32_t                instance_count,
 *                           uint32_t                start_vertex,
 *                           uint32_t                start_instance);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void wisDX12CommandListDraw(const WisDX12CommandList* self,
 *                             uint32_t                  vertex_count,
 *                             uint32_t                  instance_count,
 *                             uint32_t                  start_vertex,
 *                             uint32_t                  start_instance);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void CommandList::Draw(std::uint32_t vertex_count,
 *                        std::uint32_t instance_count,
 *                        std::uint32_t start_vertex,
 *                        std::uint32_t start_instance) const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * void VKCommandList::Draw(std::uint32_t vertex_count,
 *                          std::uint32_t instance_count,
 *                          std::uint32_t start_vertex,
 *                          std::uint32_t start_instance) const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * void DX12CommandList::Draw(std::uint32_t vertex_count,
 *                            std::uint32_t instance_count,
 *                            std::uint32_t start_vertex,
 *                            std::uint32_t start_instance) const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisCommandListDraw_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisCommandList instance.
 * - `vertex_count` The number of vertices to draw.
 * - `instance_count` The number of instances to draw. Default is 1.
 * - `start_vertex` The index of the first vertex to draw. Default is 0.
 * - `start_instance` The index of the first instance to draw. Default is 0.
 * \endcond
 *
 * @section wisCommandListDraw_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisCommandListDraw_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */