#pragma once
#include <wisdom/util/string_literal.h>
#include <wisdom/bridge/source_location.h>

namespace wis {
struct sw {
const char* str;
size_t size;
};
consteval auto sl_to_sl(wis::source_location sl = source_location::current())
{
return sw{ sl.function_name() };
}

template<sw sw>
constexpr inline auto MakeErrorString()
{
    using namespace wis::literals;
    static auto estr = "Error in "_sl + sw.str;
    return estr.c_str();
}

template<wis::string_literal func, size_t size>
constexpr inline auto MakeErrorString(uint32_t error_code, wis::string_literal<size> message)
{
    using namespace wis::literals;
    static auto estr = "Error in "_sl + func + ": "_sl + message;
    return estr.c_str();
}
} // namespace wis