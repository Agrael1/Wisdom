#pragma once
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/func.h>
#include <wisdom/util/string_literal.h>
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis {
template<wis::fixed_string func, wis::fixed_string message>
consteval inline auto make_error_string()
{
    using namespace wis::literals;
    return "Error in "_fs + func + ": "_fs + message;
}
} // namespace wis
