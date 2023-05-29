#pragma once
#define noxnd noexcept(!DEBUG_MODE)

#if defined(WISDOM_HEADER_ONLY)
#define WIS_INLINE inline	// if header only, inline everything
#else
#define WIS_INLINE			// if not header only, don't inline anything
#endif


namespace wis
{
	inline constexpr const bool debug_mode = DEBUG_MODE;
	inline constexpr const bool runtime_asserts = RUNTIME_ASSERTS;
	inline constexpr const unsigned max_render_targets = 8u;
	inline constexpr const unsigned max_vertex_bindings = 16u;
	inline constexpr const unsigned max_shader_stages = 5;
}