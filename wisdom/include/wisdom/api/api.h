#pragma once
#include <array>

namespace wis
{
struct AdapterDesc;
enum class AdapterPreference;
enum class AdapterFlags;

enum class AdapterPreference : int32_t {
    None = 0,
    MinConsumption = 1,
    Performance = 2,
};

enum class AdapterFlags : uint32_t {
    None = 0x0,
    Remote = 1 << 0,
    Software = 1 << 1,
    DXACGCompatible = 1 << 2,
    DXSupportsMonitoredFences = 1 << 3,
    DXSupportsNonMonitoredFences = 1 << 4,
    DXKeyedMutexConformance = 1 << 5,
    Max = 0xFFFFFFFF;
};

struct AdapterDesc{
    std::array<const char8_t, 256> description;
    uint32_t vendor_id;
    uint32_t device_id;
    uint32_t subsys_id;
    uint32_t revision;
    uint64_t dedicated_video_memory;
    uint64_t dedicated_system_memory;
    uint64_t shared_system_memory;
    uint64_t adapter_id;
    AdapterFlags flags;
};

//==============================TYPE TRAITS==============================

template <typename T> struct is_flag_enum : public std::false_type {};
template <> struct is_flag_enum<wis::AdapterFlags>:public std::true_type {};
}
