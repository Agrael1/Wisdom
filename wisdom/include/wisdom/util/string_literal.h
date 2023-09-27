#pragma once
#include <string>
#include <stdexcept>

namespace wis {
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
    constexpr basic_string_literal() = default;
    constexpr basic_string_literal(const value_type (&str)[N])
    {
        traits_type::copy(value, str, N);
    }
    constexpr operator std::basic_string_view<_CharTy>() const noexcept
    {
        return std::basic_string_view<_CharTy>(value, N);
    }

public:
    constexpr auto size_bytes() const noexcept
    {
        return N * sizeof(_CharTy);
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
using string_literal = basic_string_literal<char, N>;

template<size_t size>
inline constexpr string_literal<size> from_string(const char* ptr)
{
    string_literal<size> value;
    basic_string_literal<char, size>::traits_type::copy(value.value, ptr, size);
    return value;
}

template<size_t size1, size_t size2>
inline constexpr string_literal<size1 + size2 - 1> operator+(const string_literal<size1>& lhs,
                                                             const string_literal<size2>& rhs)
{
    string_literal<size1 + size2 - 1> value;
    basic_string_literal<char, size1>::traits_type::copy(value.value, lhs.value, size1 - 1);
    basic_string_literal<char, size2>::traits_type::copy(value.value + size1 - 1, rhs.value, size2);
    return value;
}

inline namespace literals {
template<wis::string_literal lit>
constexpr auto operator""_sl()
{
    return lit;
}
} // namespace literals
} // namespace wis
