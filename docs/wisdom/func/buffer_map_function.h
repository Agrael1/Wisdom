/**
 * @struct wisBufferMap
 * @ingroup Functions Core
 *
 *
 * @section wisBufferMap_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void* wisBufferMap(const WisBuffer* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * void* wisVKBufferMap(const WisVKBuffer* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * void* wisDX12BufferMap(const WisDX12Buffer* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD void* Buffer::Map() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD void* VKBuffer::Map() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD void* DX12Buffer::Map() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisBufferMap_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisBuffer instance.
 * 
 * - **return** points to the pointer, which is filled with the address of the mapped memory on success.
 * \endcond
 *
 * @section wisBufferMap_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisBufferMap_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */