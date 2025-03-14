#pragma once
#include <string>
#include <memory>
#include <span>

namespace wis {
struct string_hash : public std::hash<std::string_view> {
    using is_transparent = void;
};
} // namespace wis

namespace wis::detail {
template<class Type, std::enable_if_t<std::is_unbounded_array_v<Type>, int> = 0>
[[nodiscard]] constexpr std::unique_ptr<Type> make_unique_for_overwrite(size_t size) noexcept
{
    // make a unique_ptr with default initialization
    using Elem = std::remove_extent_t<Type>;
    return std::unique_ptr<Type>(new (std::nothrow) Elem[size]);
}
template<class Type, class... Types, std::enable_if_t<!std::is_array_v<Type>, int> = 0>
[[nodiscard]] constexpr std::unique_ptr<Type> make_unique(Types&&... Args) noexcept
{ // make a unique_ptr
    return std::unique_ptr<Type>(new (std::nothrow) Type(std::forward<Types>(Args)...));
}

template<std::integral I, std::integral A>
constexpr inline I aligned_size(I size, A alignment) noexcept
{
    using U = std::make_unsigned_t<I>;
    return I(size + U(alignment) - 1) & ~U(alignment - 1);
}

template<typename T>
struct fixed_allocation {
    std::unique_ptr<T[]> data;
    size_t size = 0;

    constexpr operator std::span<const T>() const noexcept
    {
        return { data.get(), size };
    }
    constexpr operator bool() const noexcept
    {
        return data != nullptr;
    }
    constexpr auto* get() noexcept
    {
        return data.get();
    }
    constexpr const auto* get() const noexcept
    {
        return data.get();
    }
    constexpr auto& operator[](size_t index) noexcept
    {
        return data[index];
    }
    constexpr const auto& operator[](size_t index) const noexcept
    {
        return data[index];
    }
    constexpr auto begin() noexcept
    {
        return data.get();
    }
    constexpr auto end() noexcept
    {
        return data.get() + size;
    }
    constexpr auto begin() const noexcept
    {
        return data.get();
    }
    constexpr auto end() const noexcept
    {
        return data.get() + size;
    }

    constexpr auto* get_data() noexcept
    {
        return data.get();
    }
    constexpr const auto* get_data() const noexcept
    {
        return data.get();
    }
};

template<typename T>
[[nodiscard]] constexpr fixed_allocation<T> make_fixed_allocation(size_t size) noexcept
{
    return { make_unique_for_overwrite<T[]>(size), size };
}
} // namespace wis::detail

namespace wis {
using detail::aligned_size;
} // namespace wis
