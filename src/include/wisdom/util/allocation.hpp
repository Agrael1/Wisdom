#ifndef WIS_UTIL_ALLOCATION_HPP
#define WIS_UTIL_ALLOCATION_HPP
#include <wisdom/global/definitions.h>
#include <memory>
#include <cstdlib>

namespace wis {

// C++11 compatible noexcept make_unique
template<typename T, typename... Args>
WIS_NODISCARD inline typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...)))
{
    return std::unique_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
}

template<typename T>
WIS_NODISCARD inline typename std::enable_if<std::is_array<T>::value &&
                                       std::extent<T>::value == 0,
                               std::unique_ptr<T>>::type
make_unique(std::size_t size) noexcept
{
    using U = typename std::remove_extent<T>::type;
    return std::unique_ptr<T>(new (std::nothrow) U[size]);
}

template<typename I, typename A>
constexpr inline I aligned_size(I size, A alignment) noexcept
{
    static_assert(std::is_integral<I>::value && std::is_unsigned<I>::value, "I must be an unsigned integral type");
    static_assert(std::is_integral<A>::value && std::is_unsigned<A>::value, "A must be an unsigned integral type");
    return I(size + I(alignment) - 1) & ~I(alignment - 1);
}
template<typename T, typename A>
constexpr inline T* aligned_address(T* address, A alignment) noexcept
{
    static_assert(std::is_integral<A>::value && std::is_unsigned<A>::value, "A must be an unsigned integral type");
    using U = std::uintptr_t;
    U addr  = reinterpret_cast<U>(address);
    return reinterpret_cast<T*>((addr + U(alignment) - 1) & ~U(alignment - 1));
}


} // namespace wis

#endif // WIS_UTIL_ALLOCATION_HPP
