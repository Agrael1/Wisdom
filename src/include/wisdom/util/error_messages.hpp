#ifndef WISDOM_UTIL_ERROR_MESSAGES_HPP
#define WISDOM_UTIL_ERROR_MESSAGES_HPP
#include <wisdom/bridge/source_location.hpp>
#include <wisdom/util/string_literal.hpp>

namespace wis {
namespace detail {
using func_pair = std::pair<std::size_t, std::size_t>;

constexpr func_pair Func(wis::source_location sl = wis::source_location::current()) noexcept
{
    auto func_name = std::string_view{sl.function_name()};
    std::size_t second = func_name.find('(');
    std::size_t first = func_name.rfind(' ', second) + 1;
    return {first, second};
}

template <wis::fixed_string message, func_pair func_pair>
constexpr auto make_error_string(std::source_location sl = std::source_location::current()) noexcept
{
    // SAFETY: We verified bounds at compile time
    // This suppression is required because we are implementing the safe primitive
    // that everyone else should use.
#if defined(__clang__)
#    pragma clang diagnostic push
#    pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
    return wis::fixed_string{"Error in "}
         + wis::fixed_string<func_pair.second - func_pair.first + 1>{std::string_view{
             sl.function_name() + func_pair.first
         }}
         + wis::fixed_string{": "} + message;
#if defined(__clang__)
#    pragma clang diagnostic pop
#endif
}
} // namespace detail
} // namespace wis
#endif // WISDOM_UTIL_ERROR_MESSAGES_HPP
