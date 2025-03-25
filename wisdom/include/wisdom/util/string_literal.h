#pragma once
#ifndef WISDOM_MODULE_DECL
#include <string>
#include <stdexcept>
#include <wisdom/global/definitions.h>
#endif // !WISDOM_MODULE_DECL

WISDOM_EXPORT
namespace wis {
template<typename Char, std::size_t N>
struct basic_fixed_string {
public:
    using value_type = std::remove_cv_t<Char>;
    using char_traits = std::char_traits<value_type>;

public:
    constexpr basic_fixed_string() noexcept = default;
    constexpr basic_fixed_string(const value_type (&str)[N]) noexcept
    {
        char_traits::copy(_data, str, N - 1);
    }
    constexpr explicit basic_fixed_string(std::string_view str) noexcept
    {
        char_traits::copy(_data, str.data(), N - 1);
    }

public:
    constexpr operator const value_type*() const noexcept
    {
        return _data;
    }

    constexpr std::size_t size() const noexcept
    {
        return N - 1;
    }

    constexpr const value_type* c_str() const noexcept
    {
        return _data;
    }

    constexpr value_type* data() noexcept
    {
        return _data;
    }

    constexpr operator std::basic_string_view<value_type>() const noexcept
    {
        return { _data };
    }

public:
    value_type _data[N]{}; // +1 for null terminator
};

// Define some aliases for common fixed string types
template<std::size_t N>
struct fixed_string : public basic_fixed_string<char, N> {
    using basic_fixed_string<char, N>::basic_fixed_string;
};

// deduction guide
template<std::size_t N>
fixed_string(const char (&)[N]) -> fixed_string<N>;

template<std::size_t N>
using fixed_wstring = basic_fixed_string<wchar_t, N>;

template<std::size_t N>
using fixed_u16string = basic_fixed_string<char16_t, N>;

template<std::size_t N>
using fixed_u32string = basic_fixed_string<char32_t, N>;

// Addition operator
template<typename Char, std::size_t N1, std::size_t N2>
constexpr auto operator+(const basic_fixed_string<Char, N1>& lhs, const basic_fixed_string<Char, N2>& rhs)
{
    basic_fixed_string<Char, N1 + N2 - 1> result;
    std::char_traits<Char>::copy(result.data(), lhs.c_str(), N1 - 1);
    std::char_traits<Char>::copy(result.data() + N1 - 1, rhs.c_str(), N2);
    return result;
}

inline namespace literals {
template<wis::fixed_string lit>
constexpr auto operator""_fs()
{
    return lit;
}
} // namespace literals
} // namespace wis
