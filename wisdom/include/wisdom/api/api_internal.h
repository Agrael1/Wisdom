#pragma once
#include <utility>

WIS_EXPORT namespace wis
{
    /// @brief Template class for internal implementation
    /// @tparam Impl Implementation class type
    template<class Impl>
    struct Internal {
    };

    /// @brief QueryInternal class for querying the internal implementation
    /// @tparam Impl Implementation class type, passed to Internal
    template<class Impl>
    struct QueryInternal : protected Internal<Impl> {
        QueryInternal() = default;

        template<class... Args>
        explicit QueryInternal(Args&&... args)
            : Internal<Impl>(std::forward<Args>(args)...)
        {
        }

        /// @brief Get the immutable internal implementation
        /// @return Const reference to the internal implementation
        [[nodiscard]] const Internal<Impl>& GetInternal() const
        {
            return *this;
        }
    };

    template<class InternalType>
    struct QueryInternalX : protected InternalType {

        QueryInternalX() = default;
        template<class... Args>
        QueryInternalX(Args&&... args)
            : InternalType(std::forward<Args>(args)...)
        {
        }
        /// @brief Get the immutable internal implementation
        /// @return Const reference to the internal implementation
        [[nodiscard]] const InternalType& GetInternal() const
        {
            return *this;
        }
    };
}
