#pragma once
#if defined(WISDOM_USE_FMT)
#include <fmt/xchar.h>
namespace wis {
using fmt::format; // NOLINT
using fmt::vformat; // NOLINT
using fmt::make_format_args; // NOLINT
using fmt::format_to; // NOLINT
} // namespace wis
#elif __has_include(<format>)
#include <format>
namespace wis {
using std::format;
using std::vformat;
using std::make_format_args;
using std::format_to;
} // namespace wis
#else
#error "wisdom requires fmt or std::format"
#endif
