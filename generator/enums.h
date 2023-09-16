#pragma once
#include <optional>
#include <array>

struct EnumValue {
    const char* name = "";
    std::optional<int64_t> value;
    const char* comment = "";
};
struct BitmaskValue {
    const char* name = "";
    std::optional<int64_t> bit;
    std::optional<int64_t> value;
    const char* comment = "";
};

enum EnumSize {
    s8 = 8,
    s16 = 16,
    s32 = 32,
    s64 = 64,
};

template<size_t esize>
struct Enum {
    const char* name = "";
    const char* comment = "";
    EnumSize size = s32;
    std::array<EnumValue, esize> values;
};

template<size_t esize>
struct Bitmask {
    const char* name = "";
    const char* comment = "";
    EnumSize size = s32;
    std::array<BitmaskValue, esize> values;
};

template<size_t esize>
constexpr Enum<esize> XMakeEnum(std::array<EnumValue, esize> values, const char* name = "",
                                const char* comment = "",
                                EnumSize size = s32)
{
    return Enum<esize>{ name, comment, size, values };
}

template<size_t esize>
constexpr Bitmask<esize> XMakeBitmask(std::array<BitmaskValue, esize> values, const char* name = "",
                                      const char* comment = "",
                                      EnumSize size = s32)
{
    return Bitmask<esize>{ name, comment, size, values };
}

template<size_t size>
constexpr Enum<size> StabilizeEnum(Enum<size> in)
{
    size_t i = 0;
    for (auto& v : in.values) {
        if (!v.value)
            v.value = i++;
        else
            i = v.value.value() + 1;
    }
    return in;
}

template<size_t size>
constexpr Bitmask<size> StabilizeEnum(Bitmask<size> in)
{
    size_t i = 0;
    for (auto& v : in.values) {
        if (v.value)
            continue;
        if (!v.bit)
            v.bit = i++;
        else
            i = v.bit.value() + 1;
    }
    return in;
}

template<size_t asize>
constexpr auto ComposeEnum(std::array<EnumValue, asize> values, const char* name = "",
                           const char* comment = "",
                           EnumSize size = s32)
{
    return StabilizeEnum(XMakeEnum(values, name, comment, size));
}

template<size_t asize>
constexpr auto ComposeBitmask(std::array<BitmaskValue, asize> values, const char* name = "",
                              const char* comment = "",
                              EnumSize size = s32)
{
    return StabilizeEnum(XMakeBitmask(values, name, comment, size));
}



constexpr auto a = ComposeBitmask(
        std::array{
                BitmaskValue{ "a", 0 },
        });
