#pragma once
#ifndef WISDOM_MODULE_DECL
#include <wisdom/util/string_literal.h>
#include <wisdom/bridge/source_location.h>
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis {
constexpr size_t FuncD(std::source_location sl = std::source_location::current()) noexcept
{
    return std::string_view{ sl.function_name() }.size();
}

template<size_t a>
constexpr wis::fixed_string<a> Func(std::source_location sl = std::source_location::current()) noexcept
{
    return wis::fixed_string<a> { sl.function_name() };
}

template<wis::fixed_string func, wis::fixed_string message>
consteval inline auto make_error_string()
{
    using namespace wis::literals;
    return wis::fixed_string{ "Error in " } + func + wis::fixed_string{ ": " } + message;
}
} // namespace wis
