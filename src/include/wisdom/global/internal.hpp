#ifndef WIS_INTERNAL_HPP
#define WIS_INTERNAL_HPP
#ifdef __cplusplus
#    include <wisdom/util/allocation.hpp>

#    include <cstdio>
#    include <cstring>

namespace wis
{
namespace impl
{

/// @brief Implements class for querying the internal implementation
/// @tparam Impl Implementation class type, passed to Internal
/// @tparam Storage Storage type for the internal implementation
template <typename Impl, typename Storage, typename Deleter>
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
    }

    /// @brief Default constructor, zeros the storage
    template <typename... Args>
    Implements(std::in_place_t in_place, Args&&... args) noexcept
    {
        (void)in_place;
        // explicitly start life of Impl in our storage
        new (std::addressof(_impl_storage)) Impl(std::forward<Args>(args)...);
    }

    // Disable copy
    Implements(const Implements&) = delete;
    Implements& operator=(const Implements&) = delete;

    /// @brief Move constructor
    Implements(Implements&& other) noexcept
    {
        // explicitly start life of Impl in our storage
        auto* impl = new (std::addressof(_impl_storage)) Impl();

        // simple copy of implementation
        std::memcpy(impl, std::addressof(other._impl_storage), sizeof(Impl));

        // zero out other storage
        other._impl_storage = {};
    }

    /// @brief Move assignment
    Implements& operator=(Implements&& other) noexcept
    {
        if (this != &other) {
            // printf("move assigned\n");
            //  call deleter on current storage
            Deleter{}(GetStorage());

            // explicitly start life of Impl in our storage
            auto* impl = new (std::addressof(_impl_storage)) Impl();

            // simple copy of implementation
            std::memcpy(impl, std::addressof(other._impl_storage), sizeof(Impl));

            // zero out other storage
            other._impl_storage = {};
        }
        return *this;
    }

    /// @brief Destructor, calls the Deleter on the internal implementation
    ~Implements() noexcept { Deleter{}(GetStorage()); }

public:
    /// @brief Get the immutable internal implementation
    /// @return Const reference to the internal implementation
    [[nodiscard]] const Impl& GetInternal() const noexcept
    {
        return wis::from_handle_ref<const Impl>(std::addressof(_impl_storage));
    }

    /// @brief Get the mutable internal implementation
    /// @return Reference to the internal implementation
    [[nodiscard]] Impl& GetMutableInternal() noexcept
    {
        return wis::from_handle_ref<Impl>(std::addressof(_impl_storage));
    }

    /// @brief Get the storage pointer
    /// @return Pointer to the storage
    [[nodiscard]] Storage* GetStorage() noexcept { return &_impl_storage; }

    /// @brief Check if the handle holds a valid object
    /// @return true if the first 8 bytes of storage are non-zero
    [[nodiscard]] bool IsValid() const noexcept
    {
        static constexpr std::uint64_t zero = 0;
        return std::memcmp(&_impl_storage, &zero, sizeof(std::uint64_t)) != 0;
    }

    /// @brief Bool conversion, checks handle validity
    explicit operator bool() const noexcept { return IsValid(); }

public:
    Storage _impl_storage;
};
} // namespace impl

template <typename WisClass>
auto view_of(const WisClass& obj) -> decltype(obj.GetView())
{
    return obj.GetView();
}
} // namespace wis

#endif // __cplusplus
#endif // WIS_INTERNAL_HPP
