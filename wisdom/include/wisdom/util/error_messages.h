#pragma once
#include <wisdom/util/string_literal.h>

#if defined(__clang__) || defined(__GNUC__)
#define FUNC __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
#define FUNC __FUNCSIG__
#else
#define FUNC __func__
#endif

namespace wis {

template<wis::string_literal func, wis::string_literal message>
consteval inline auto MakeErrorString()
{
    using namespace wis::literals;
    return "Error in "_sl + func + ": "_sl + message;
}
} // namespace wis