/**
 * @struct WisSurfaceParameters
 * @ingroup Structures
 *
 *
 * @section WisSurfaceParameters_spec Specification
 * <hr>
 *
 * \cond WIS_GEN_CODE
 * C version:
 * ```c
 * // Provided by Wisdom 0.7.0. 
 * typedef struct  WisSurfaceParameters {
 *     uint32_t             min_swapchain_images;
 *     uint32_t             max_swapchain_images;
 *     uint32_t             alpha_modes_supported;
 *     WisTextureUsageFlags texture_usage_flags_supported;
 *     bool                 stereo_supported;
 * } WisSurfaceParameters;
 * 
 * ```
 * C++ version:
 * ```cpp
 * namespace wis{
 * // Provided by Wisdom 0.7.0. 
 * struct  SurfaceParameters {
 *     std::uint32_t          min_swapchain_images;
 *     std::uint32_t          max_swapchain_images;
 *     std::uint32_t          alpha_modes_supported;
 *     wis::TextureUsageFlags texture_usage_flags_supported;
 *     bool                   stereo_supported;
 * };
 * }
 * ```
 * \endcond
 *
 * @section WisSurfaceParameters_memb Members
 * <hr>
 * \cond WIS_GEN_DESC
 * - `min_swapchain_images` specifies minimum number of images in the swapchain.
 * - `max_swapchain_images` defines maximum number of images in the swapchain.
 * - `alpha_modes_supported` specifies bitmask of supported alpha modes for the swapchain. Each bit represents a different alpha mode. Used to determine the supported alpha modes for the swapchain.
 * - `texture_usage_flags_supported` specifies bitmask of supported texture usage flags for the swapchain images.
 * - `stereo_supported` indicates if stereo rendering is supported. If true, the surface can be used to create a swapchain with stereo support.
 * \endcond
 *
 * @section WisSurfaceParameters_descr Description
 * <hr>
 *
 * \cond WIS_GEN_WIS_IDS
 * \endcond
 *
 * @section WisSurfaceParameters_see_also See Also
 * <hr>
 * \cond WIS_GEN_REFS
 * @see Functions:
 * wisDeviceGetSurfaceParameters
 * \endcond
 */