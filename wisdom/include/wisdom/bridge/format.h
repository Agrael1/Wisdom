#pragma once
#if defined(WISDOM_USE_FMT)
#include <fmt/xchar.h>
WIS_EXPORT namespace wis
{
    using fmt::format; // NOLINT
    using fmt::format_to; // NOLINT
}
#elif __has_include(<format>)
#include <format>
WIS_EXPORT namespace wis
{
    using std::format;
    using std::format_to;
}
#else
#error "wisdom requires fmt or std::format"
#endif
