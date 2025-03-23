#pragma once
#if defined(__clang__) || defined(__GNUC__)
#define FUNC __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNC __FUNCSIG__
#else
#define FUNC __func__
#endif

#include <wisdom/util/string_literal.h>

WISDOM_EXPORT
namespace wis {
template<wis::fixed_string func, wis::fixed_string message>
consteval inline auto make_error_string()
{
    using namespace wis::literals;
    return "Error in "_fs + func + ": "_fs + message;
}
} // namespace wis
