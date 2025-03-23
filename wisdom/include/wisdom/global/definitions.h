#pragma once
#if defined(WISDOM_BUILD_BINARIES) || defined(WISDOM_MODULE_DECL) // If we are building the binaries
#define WIS_INLINE
#else
#define WIS_INLINE inline
#endif // WISDOM_BUILD_BINARIES

#ifndef WISDOM_EXPORT
#ifndef WISDOM_MODULE_DECL
#define WISDOM_EXPORT
#else
#define WISDOM_EXPORT export
#endif
#endif // WISDOM_EXPORT

// C++23 and later
#if __cplusplus > 202211L
#define WIS_CONSTEXPR23 constexpr
#else
#define WIS_CONSTEXPR23
#endif

WISDOM_EXPORT
namespace wis {
#if !defined(NDEBUG) || defined(_DEBUG)
inline constexpr const bool debug_mode = true;
#else
inline constexpr const bool debug_mode = false;
#endif

inline constexpr const bool debug_layer = debug_mode;

inline constexpr const unsigned max_render_targets = 8u;
inline constexpr const unsigned max_vertex_bindings = 16u;
inline constexpr const unsigned max_shader_stages = 5u;
inline constexpr const unsigned max_push_constants = 5u;
inline constexpr const unsigned max_push_descriptors = 8u;
inline constexpr const unsigned max_descriptor_storage_resource_count = 4096u;
inline constexpr const unsigned max_descriptor_storage_sampler_count = 2048u;
} // namespace wis
