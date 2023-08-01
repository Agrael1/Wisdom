#pragma once

WIS_EXPORT namespace wis
{
/// @brief Template class for internal implementation
/// @tparam Impl Implementation class type
template<class Impl>
class Internal
{
};

/// @brief QueryInternal class for querying the internal implementation
/// @tparam Impl Implementation class type, passed to Internal
template<class Impl>
class QueryInternal : protected Internal<Impl>
{
public:
    using base = QueryInternal<Impl>;
    using Internal<Impl>::Internal;

    /// @brief Get the immutable internal implementation
    /// @return Const reference to the internal implementation
    [[nodiscard]] const Internal<Impl>& GetInternal() const
    {
        return *this;
    }
};
}
