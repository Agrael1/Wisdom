/*
MIT License
Copyright (c) 2020 Uy Ha
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#ifndef WISDOM_MODULE_DECL
#include <type_traits>
#include <wisdom/global/definitions.h>
#endif

// namespace adapted for wisdom from river::flags to enable ADL
WISDOM_EXPORT
namespace wis {
template<typename T>
struct is_flag_enum;

template<typename T>
concept unsigned_enum = std::conjunction_v<std::is_enum<T>, std::negation<std::is_convertible<T, int>>> && std::is_unsigned_v<std::underlying_type_t<T>>;

template<typename T>
concept enum_type = std::is_enum_v<T>;

namespace detail {
template<unsigned_enum T>
constexpr bool is_flag()
{
    return true;
}
template<typename T>
consteval bool is_flag()
{
    return wis::is_flag_enum<T>::value;
}
} // namespace detail

template<typename T>
concept Flag = detail::is_flag<T>();

template<enum_type T>
constexpr auto underlying_value(T enum_value)
{
    return static_cast<std::underlying_type_t<T>>(enum_value);
}

template<enum_type T>
constexpr auto operator+(T enum_value)
{
    return underlying_value(enum_value);
}

template<Flag T>
constexpr auto operator~(T value)
{
    return static_cast<std::underlying_type_t<T>>(~underlying_value(value));
}

template<Flag T>
constexpr T operator|(T lhs, T rhs)
{
    return static_cast<T>(underlying_value(lhs) | underlying_value(rhs));
}
template<Flag T>
constexpr auto operator|(std::underlying_type_t<T> lhs, T rhs)
{
    return static_cast<std::underlying_type_t<T>>(lhs | underlying_value(rhs));
}
template<Flag T>
constexpr auto operator|(T lhs, std::underlying_type_t<T> rhs)
{
    return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) | rhs);
}
template<Flag T>
constexpr auto operator|=(std::underlying_type_t<T>& value, T const flag)
{
    return value = value | flag;
}

// Compatibility
template<enum_type T>
constexpr auto operator|=(T& value, T flag)
{
    return value = T(underlying_value(value) | flag);
}

template<Flag T>
constexpr auto operator&(T lhs, T rhs)
{
    return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) & underlying_value(rhs));
}
template<Flag T>
constexpr auto operator&(std::underlying_type_t<T> lhs, T rhs)
{
    return static_cast<std::underlying_type_t<T>>(lhs & underlying_value(rhs));
}
template<Flag T>
constexpr auto operator&(T lhs, std::underlying_type_t<T> rhs)
{
    return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) & rhs);
}
template<Flag T>
constexpr auto operator&=(std::underlying_type_t<T>& value, T const flag)
{
    return value = value & flag;
}

template<Flag T>
constexpr auto operator^(T lhs, T rhs)
{
    return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) ^ underlying_value(rhs));
}
template<Flag T>
constexpr auto operator^(std::underlying_type_t<T> lhs, T rhs)
{
    return static_cast<std::underlying_type_t<T>>(lhs ^ underlying_value(rhs));
}
template<Flag T>
constexpr auto operator^(T lhs, std::underlying_type_t<T> rhs)
{
    return static_cast<std::underlying_type_t<T>>(underlying_value(lhs) ^ rhs);
}
template<Flag T>
constexpr auto operator^=(std::underlying_type_t<T>& value, T const flag)
{
    return value = value ^ flag;
}

template<auto mask>
constexpr bool has(std::underlying_type_t<decltype(mask)> value)
{
    return (value & mask) == underlying_value(mask);
}

template<Flag T>
constexpr bool has(T lhs, T flag)
{
    return (lhs & flag) == flag;
}
} // namespace wis
