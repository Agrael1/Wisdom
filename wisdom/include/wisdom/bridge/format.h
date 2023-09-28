#pragma once
#if defined(WISDOM_USE_FMT)
#include <fmt/xchar.h>
namespace wis
{
    using fmt::format; // NOLINT
    using fmt::format_to; // NOLINT
}
#elif __has_include(<format>)
#include <format>
namespace wis
{
    using std::format;
    using std::format_to;
}
#else
#error "wisdom requires fmt or std::format"
#endif
