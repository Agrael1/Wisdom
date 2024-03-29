#pragma once
#define noxnd noexcept(!DEBUG_MODE)

WIS_EXPORT namespace wis
{
    inline constexpr const bool debug_mode = DEBUG_MODE;
    inline constexpr const bool runtime_asserts = RUNTIME_ASSERTS;
    inline constexpr const unsigned max_render_targets = 8u;
    inline constexpr const unsigned max_vertex_bindings = 16u;
    inline constexpr const unsigned max_shader_stages = 5;
}
