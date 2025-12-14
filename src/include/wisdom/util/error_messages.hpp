#ifndef WISDOM_UTIL_ERROR_MESSAGES_HPP
#define WISDOM_UTIL_ERROR_MESSAGES_HPP
#include <wisdom/util/string_literal.hpp>
#include <wisdom/bridge/source_location.hpp>

namespace wis::detail {
using func_pair = std::pair<std::size_t, std::size_t>;

constexpr func_pair
Func(std::source_location sl = std::source_location::current()) noexcept
{
    auto        func_name = std::string_view{ sl.function_name() };
    std::size_t second    = func_name.find('(');
    std::size_t first     = func_name.rfind(' ', second) + 1;
    return { first, second };
}

template<wis::fixed_string message, func_pair func_pair>
constexpr auto make_error_string(std::source_location sl = std::source_location::current()) noexcept
{
    return wis::fixed_string{ "Error in " } + wis::fixed_string<func_pair.second - func_pair.first + 1>{ std::string_view{ sl.function_name() + func_pair.first } } + wis::fixed_string{ ": " } + message;
}
} // namespace wis::detail
#endif // WISDOM_UTIL_ERROR_MESSAGES_HPP