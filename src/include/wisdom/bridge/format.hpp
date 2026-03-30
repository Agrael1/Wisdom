#ifndef WIS_BRIDGE_FORMAT_H
#define WIS_BRIDGE_FORMAT_H
#if defined(WISDOM_USE_FMT)
#    include <fmt/xchar.h>
namespace wis {
using fmt::format;           // NOLINT
using fmt::format_to;        // NOLINT
using fmt::make_format_args; // NOLINT
using fmt::vformat;          // NOLINT
} // namespace wis
#elif __has_include(<format>)
#    include <format>
namespace wis {
using std::format;
using std::format_to;
using std::make_format_args;
using std::vformat;
} // namespace wis
#else
#    error "wisdom requires fmt or std::format"
#endif
#endif // WISDOM_BRIDGE_FORMAT_H
