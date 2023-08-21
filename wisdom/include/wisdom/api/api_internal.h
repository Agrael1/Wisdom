#pragma once
#include <utility>

namespace wis {
/// @brief Template class for internal implementation
/// @tparam Impl Implementation class type
template<class Impl>
WIS_EXPORT class Internal
{
};

/// @brief QueryInternal class for querying the internal implementation
/// @tparam Impl Implementation class type, passed to Internal
template<class Impl>
WIS_EXPORT struct QueryInternal : protected Internal<Impl> {
    QueryInternal() = default;

    template<class... Args>
    explicit QueryInternal(Args&&... args) noexcept
        : Internal<Impl>(std::forward<Args>(args)...)
    {
    }

    /// @brief Get the immutable internal implementation
    /// @return Const reference to the internal implementation
    [[nodiscard]] const Internal<Impl>& GetInternal() const noexcept
    {
        return *this;
    }
};
} // namespace wis
