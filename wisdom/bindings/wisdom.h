#pragma once

#include <stdint.h>

typedef struct WisAdapterDesc WisAdapterDesc;
typedef enum WisAdapterPreference WisAdapterPreference;
typedef enum WisAdapterFlags WisAdapterFlags;

struct WisAdapterDesc{
    const char8_t description[256];
    uint32_t vendor_id;
    uint32_t device_id;
    uint32_t subsys_id;
    uint32_t revision;
    uint64_t dedicated_video_memory;
    uint64_t dedicated_system_memory;
    uint64_t shared_system_memory;
    uint64_t adapter_id;
    WisAdapterFlags flags;
};

enum WisAdapterPreference : int32_t {
    WisAdapterPreferenceNone = 0,
    WisAdapterPreferenceMinConsumption = 1,
    WisAdapterPreferencePerformance = 2,
};

enum WisAdapterFlags : uint32_t {
    WisAdapterFlagsNone = 0x0,
    WisAdapterFlagsRemote = 1 << 0,
    WisAdapterFlagsSoftware = 1 << 1,
    WisAdapterFlagsACGCompatible = 1 << 2,
    WisAdapterFlagsSupportsMonitoredFences = 1 << 3,
    WisAdapterFlagsSupportsNonMonitoredFences = 1 << 4,
    WisAdapterFlagsKeyedMutexConformance = 1 << 5,
    WisAdapterFlagsMax = 0xFFFFFFFF;
};

