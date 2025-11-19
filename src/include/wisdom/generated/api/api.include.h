#pragma once
#include <array>
#include <string_view>
#include <string>
#include <span>
#include <wisdom/bridge/source_location.h>
#include <wisdom/bridge/format.h>
#include <wisdom/global/definitions.h>
#include <memory>
#include <utility>
#include <functional>

#ifdef __clang__
struct _GUID {
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint8_t Data4[8];
};
#endif
