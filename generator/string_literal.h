#pragma once
#include <string>
#include <stdexcept>

namespace ver {
template<class _CharTy, size_t N, class _Traits = std::char_traits<_CharTy>>
class basic_string_literal
{
public:
    static_assert(std::is_same_v<_CharTy, typename _Traits::char_type>,
                  "Bad char_traits for basic_string_literal");

    using traits_type = _Traits;
    using value_type = _CharTy;
    using pointer_type = _CharTy*;
    using const_pointer = const value_type*;

public:
    constexpr basic_string_literal(const value_type (&str)[N])
    {
        traits_type::copy(value, str, N);
    }
    constexpr basic_string_literal() = default;
    constexpr auto size_bytes() const noexcept
    {
        return N;
    }
    constexpr auto size() const noexcept
    {
        return traits_type::length(value);
    }
    constexpr auto length() const noexcept
    {
        return traits_type::length(value);
    }
    constexpr const_pointer c_str() const noexcept
    {
        return &value[0];
    }
    constexpr pointer_type data() noexcept
    {
        return &value[0];
    }
    constexpr value_type operator[](size_t i) const noexcept
    {
        if constexpr (std::is_constant_evaluated())
            static_assert(i > N, "Index out of bounds");
        return value[i];
    }
    constexpr value_type& operator[](size_t i) noexcept
    {
        return value[i];
    }
    constexpr auto& append(const_pointer s)
    {
        return append(s, traits_type::length(s));
    }
    constexpr auto* end() noexcept
    {
        return data() + size();
    }
    constexpr auto& append(const_pointer s, size_t count)
    {
        const auto curr_size = size();
        if (count > N - curr_size)
            throw std::length_error(
                    "count > max_size() - size()");
        traits_type::copy(end(), s, count);
        return *this;
    }

    value_type value[N]{};
};

template<size_t N>
struct string_literal : basic_string_literal<char, N> {
    constexpr string_literal(const char (&str)[N])
        : basic_string_literal<char, N>(str){};
    using basic_string_literal<char, N>::basic_string_literal;
};
template<size_t N>
struct wstring_literal : basic_string_literal<wchar_t, N> {
    constexpr wstring_literal(const wchar_t (&str)[N])
        : basic_string_literal<wchar_t, N>(str){};
    using basic_string_literal<wchar_t, N>::basic_string_literal;
};
template<size_t N>
struct u8string_literal : basic_string_literal<char8_t, N> {
    constexpr u8string_literal(const char8_t (&str)[N])
        : basic_string_literal<char8_t, N>(str){};
    using basic_string_literal<char8_t, N>::basic_string_literal;
};
template<size_t N>
struct u16string_literal : basic_string_literal<char16_t, N> {
    constexpr u16string_literal(const char16_t (&str)[N])
        : basic_string_literal<char16_t, N>(str){};
    using basic_string_literal<char16_t, N>::basic_string_literal;
};
template<size_t N>
struct u32string_literal : basic_string_literal<char32_t, N> {
    constexpr u32string_literal(const char32_t (&str)[N])
        : basic_string_literal<char32_t, N>(str){};
    using basic_string_literal<char32_t, N>::basic_string_literal;
};
} // namespace ver