#if !defined(WIS_INTERNAL_HPP) && defined(__cplusplus)
#define WIS_INTERNAL_HPP

#include <utility>

namespace wis::impl {

/// @brief Implements class for querying the internal implementation
/// @tparam Impl Implementation class type, passed to Internal
/// @tparam Storage Storage type for the internal implementation
template<typename Impl, typename Storage>
struct Implements {
    static_assert(sizeof(Storage) >= sizeof(Impl), "Storage size must be at least the size of Impl");

public:
    Implements() = default;

    template<class... Args>
    explicit Implements(Args&&... args) noexcept
    {
        std::construct_at<Impl>(std::addressof(_impl_storage), std::forward<Args>(args)...);
    }

public:
    /// @brief Get the immutable internal implementation
    /// @return Const reference to the internal implementation
    [[nodiscard]] const Impl& GetInternal() const noexcept
    {
        // launder the storage to Impl
        return *std::launder(reinterpret_cast<const Impl*>(std::addressof(_impl_storage)));
    }

    /// @brief Get the mutable internal implementation
    /// @return Reference to the internal implementation
    [[nodiscard]] Impl& GetMutableInternal() noexcept
    {
        return *std::launder(reinterpret_cast<Impl*>(std::addressof(_impl_storage)));
    }

public:
    Storage _impl_storage;
};
} // namespace wis::impl
#endif // WIS_INTERNAL_HPP
