#pragma once
#include <utility>
#include <wisdom/global/definitions.h>

#include <source_location>
#include <cstdio>

namespace wis {
template<typename T, bool enabled = false>
class atype
{
    static inline void xprint(std::source_location sl = std::source_location::current())
    {
        if constexpr (enabled)
            std::printf("%s\n", sl.function_name());
    }

public:
    atype() {
        xprint();
    }
    ~atype() {
        xprint();
    }
    atype(const atype&) {
        xprint();
    }
    atype(atype&&) noexcept {
        xprint();
    }
    atype& operator=(atype&&) noexcept
    {
        xprint();
        return *this;
    }
    atype& operator=(const atype&) noexcept
    {
        xprint();
        return *this;
    }
};

/// @brief Template class for internal implementation
/// @tparam Impl Implementation class type
template<class Impl>
struct Internal {
};

/// @brief QueryInternal class for querying the internal implementation
/// @tparam Impl Implementation class type, passed to Internal
template<class Impl>
struct QueryInternal : protected Internal<Impl> { /*, public atype<Impl, enable>*/
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

    /// @brief Get the mutable internal implementation
    /// @return Reference to the internal implementation
    [[nodiscard]] Internal<Impl>& GetMutableInternal() noexcept
    {
        return *this;
    }
};

template<class Impl, class Extension>
struct QueryInternalExtension : protected Internal<Impl>, public Extension {
    QueryInternalExtension() = default;

    template<class... Args>
    explicit QueryInternalExtension(Args&&... args) noexcept
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
