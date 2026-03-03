/**
 * @struct wisBufferGetGPUAddress
 * @ingroup Functions
 *
 *
 * @section wisBufferGetGPUAddress_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 *  C Version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisBufferGetGPUAddress(const WisBuffer* self);
 * ```
 * <details>
 * <summary>C Implementation Specific Version:</summary>
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisVKBufferGetGPUAddress(const WisVKBuffer* self);
 * 
 * // Provided by Wisdom 0.7.0. 
 * uint64_t wisDX12BufferGetGPUAddress(const WisDX12Buffer* self);
 * ```
 * </details>
 * 
 * C++ Version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t Buffer::GetGPUAddress() const noexcept;
 * }
 * ```
 * <details>
 * <summary>C++ Implementation Specific Version:</summary>
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t VKBuffer::GetGPUAddress() const noexcept;
 * 
 * // Provided by Wisdom 0.7.0. 
 * WIS_NODISCARD std::uint64_t DX12Buffer::GetGPUAddress() const noexcept;
 * }
 * ```
 * </details>
 * \endcond
 *
 * @section wisBufferGetGPUAddress_memb Parameters
 * <hr>
 * \cond WIS_GEN_DESC
 * - **this** `self` self is a pointer to the valid WisBuffer instance.
 * 
 * - **return** Address of the buffer on GPU.
 * \endcond
 *
 * @section wisBufferGetGPUAddress_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section wisBufferGetGPUAddress_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * \endcond
 */