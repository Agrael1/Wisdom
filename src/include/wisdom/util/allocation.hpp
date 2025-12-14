#ifndef WIS_UTIL_ALLOCATION_HPP
#define WIS_UTIL_ALLOCATION_HPP
#include <memory>

namespace wis {

// C++11 compatible noexcept make_unique
template<typename T, typename... Args>
inline typename std::enable_if<!std::is_array<T>::value, std::unique_ptr<T>>::type
make_unique(Args&&... args) noexcept(noexcept(T(std::forward<Args>(args)...)))
{
    return std::unique_ptr<T>(new (std::nothrow) T(std::forward<Args>(args)...));
}

template<typename T>
inline typename std::enable_if<std::is_array<T>::value &&
std::extent<T>::value == 0,
    std::unique_ptr<T>>::type
    make_unique(std::size_t size) noexcept
{
    using U = typename std::remove_extent<T>::type;
    return std::unique_ptr<T>(new (std::nothrow) U[size]());
}
} // namespace wis

#endif // WIS_UTIL_ALLOCATION_HPP