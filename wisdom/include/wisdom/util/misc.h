#pragma once
#include <string>
#include <memory>

namespace wis {
struct string_hash {
    using is_transparent = void;
    [[nodiscard]] size_t operator()(const char* txt) const
    {
        return std::hash<std::string_view>{}(txt);
    }
    [[nodiscard]] size_t operator()(std::string_view txt) const
    {
        return std::hash<std::string_view>{}(txt);
    }
    [[nodiscard]] size_t operator()(const std::string& txt) const
    {
        return std::hash<std::string>{}(txt);
    }
};
} // namespace wis

namespace wis::detail {
template<class Type, std::enable_if_t<std::is_unbounded_array_v<Type>, int> = 0>
[[nodiscard]] constexpr std::unique_ptr<Type> make_unique_for_overwrite(size_t size)
{
    // make a unique_ptr with default initialization
    using Elem = std::remove_extent_t<Type>;
    return std::unique_ptr<Type>(new (std::nothrow) Elem[size]);
}
} // namespace wis::detail
