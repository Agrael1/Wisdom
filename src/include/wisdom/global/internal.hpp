#ifndef WIS_INTERNAL_HPP
#define WIS_INTERNAL_HPP
#ifdef __cplusplus
#include <type_traits>
#include <cstdio>

namespace wis {
namespace impl {

/// @brief Implements class for querying the internal implementation
/// @tparam Impl Implementation class type, passed to Internal
/// @tparam Storage Storage type for the internal implementation
template<typename Impl, typename Storage, typename Deleter>
struct Implements {
    static_assert(sizeof(Storage) >= sizeof(Impl), "Storage size must be at least the size of Impl");
    static_assert(alignof(Storage) >= alignof(Impl), "Storage alignment must be at least the alignment of Impl");
    static_assert(std::is_trivially_destructible<Storage>::value, "Storage must be trivially destructible");
    using ImplType = Implements<Impl, Storage, Deleter>;

public:
    /// @brief Default constructor, zeros the storage
    Implements() noexcept
        : _impl_storage()
    {
        //printf("default constructed\n");
    }

    // Disable copy
    Implements(const Implements&) = delete;
    Implements& operator=(const Implements&) = delete;

    /// @brief Move constructor
    Implements(Implements&& other) noexcept
    {
        // simple memcpy of storage
        std::memcpy(std::addressof(_impl_storage), std::addressof(other._impl_storage), sizeof(Storage));

        // zero out other storage
        std::memset(std::addressof(other._impl_storage), 0, sizeof(Storage));
    }

    /// @brief Move assignment
    Implements& operator=(Implements&& other) noexcept
    {
        if (this != &other) {
            //printf("move assigned\n");
            // call deleter on current storage
            Deleter{}(GetStorage());
            // simple memcpy of storage
            std::memcpy(std::addressof(_impl_storage), std::addressof(other._impl_storage), sizeof(Storage));
            // zero out other storage
            std::memset(std::addressof(other._impl_storage), 0, sizeof(Storage));
        }
        return *this;
    }

    /// @brief Destructor, calls the Deleter on the internal implementation
    ~Implements() noexcept
    {
        Deleter{}(GetStorage());
    }

public:
    /// @brief Get the immutable internal implementation
    /// @return Const reference to the internal implementation
    [[nodiscard]] const Impl& GetInternal() const noexcept
    {
        // launder the storage to Impl
        return *reinterpret_cast<const Impl*>(std::addressof(_impl_storage));
    }

    /// @brief Get the mutable internal implementation
    /// @return Reference to the internal implementation
    [[nodiscard]] Impl& GetMutableInternal() noexcept
    {
        return *reinterpret_cast<Impl*>(std::addressof(_impl_storage));
    }

    /// @brief Get the storage pointer
    /// @return Pointer to the storage
    [[nodiscard]] Storage* GetStorage() noexcept
    {
        return &_impl_storage;
    }

public:
    Storage _impl_storage;
};
} // namespace impl
} // namespace wis

#endif // __cplusplus
#endif // WIS_INTERNAL_HPP
