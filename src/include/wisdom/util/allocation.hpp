#ifndef WIS_UTIL_ALLOCATION_HPP
#define WIS_UTIL_ALLOCATION_HPP
#include <wisdom/global/definitions.h>

#include <cstdint>
#include <cstdlib>
#include <memory>
#include <new>
#include <type_traits>

namespace wis
{

namespace detail
{
template <typename T>
WIS_NODISCARD inline T* launder_ptr(T* ptr) noexcept
{
#if defined(__cpp_lib_launder) && __cpp_lib_launder >= 201606L
    return std::launder(ptr);
#elif defined(__has_builtin)
#    if __has_builtin(__builtin_launder)
    return __builtin_launder(ptr);
#    else
    return ptr;
#    endif
#elif defined(__GNUC__) && (__GNUC__ >= 7)
    return __builtin_launder(ptr);
#else
    return ptr;
#endif
}
} // namespace detail

template <typename T, typename U>
WIS_NODISCARD inline T* from_handle(U* handle) noexcept
{
    return detail::launder_ptr(reinterpret_cast<T*>(handle));
}

template <typename T, typename U>
WIS_NODISCARD inline T& from_handle_ref(U* handle) noexcept
{
    return *from_handle<T>(handle);
}

// C++11 compatible noexcept make_unique
template <typename T, typename... Args>
WIS_NODISCARD inline typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type make_unique(
    Args&&... args
) noexcept(noexcept(T(std::forward<Args>(args)...)))
{
    return std::unique_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
}

template <typename T>
WIS_NODISCARD inline
    typename std::enable_if<std::is_array<T>::value && std::extent<T>::value == 0, std::unique_ptr<T>>::type
    make_unique(std::size_t size) noexcept
{
    using U = typename std::remove_extent<T>::type;
    return std::unique_ptr<T>(new (std::nothrow) U[size]);
}

template <typename I, typename A>
constexpr inline I aligned_size(I size, A alignment) noexcept
{
    static_assert(std::is_integral<I>::value && std::is_unsigned<I>::value, "I must be an unsigned integral type");
    static_assert(std::is_integral<A>::value && std::is_unsigned<A>::value, "A must be an unsigned integral type");
    return I(size + I(alignment) - 1) & ~I(alignment - 1);
}
template <typename T, typename A>
constexpr inline T* aligned_address(T* address, A alignment) noexcept
{
    static_assert(std::is_integral<A>::value && std::is_unsigned<A>::value, "A must be an unsigned integral type");
    using U = std::uintptr_t;
    U addr = reinterpret_cast<U>(address);
    return reinterpret_cast<T*>((addr + U(alignment) - 1) & ~U(alignment - 1));
}

} // namespace wis

#endif // WIS_UTIL_ALLOCATION_HPP
