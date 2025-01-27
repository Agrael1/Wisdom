#pragma once
#ifdef WISDOM_BUILD_BINARIES // If we are building the binaries
#define WIS_INLINE
#else
#define WIS_INLINE inline
#endif // WISDOM_BUILD_BINARIES

namespace wis {
inline constexpr const bool debug_mode = DEBUG_MODE;
inline constexpr const bool debug_layer = debug_mode;

inline constexpr const unsigned max_render_targets = 8u;
inline constexpr const unsigned max_vertex_bindings = 16u;
inline constexpr const unsigned max_shader_stages = 5u;
inline constexpr const unsigned max_push_constants = 5u;
inline constexpr const unsigned max_push_descriptors = 8u;
inline constexpr const unsigned max_descriptor_storage_resource_count = 4096u;
inline constexpr const unsigned max_descriptor_storage_sampler_count = 2048u;
} // namespace wis
