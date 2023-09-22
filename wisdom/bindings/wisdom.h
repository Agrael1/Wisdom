#pragma once
#include <stdint.h>

typedef struct WisAdapterDesc WisAdapterDesc;
typedef enum WisAdapterPreference WisAdapterPreference;
typedef enum WisAdapterFlags WisAdapterFlags;

enum WisAdapterPreference : int32_t {
    WisAdapterPreferenceNone = 0,
    WisAdapterPreferenceMinConsumption = 1,
    WisAdapterPreferencePerformance = 2,
};

enum WisAdapterFlags : uint32_t {
    WisAdapterFlagsNone = 0x0,
    WisAdapterFlagsRemote = 1 << 0,
    WisAdapterFlagsSoftware = 1 << 1,
    WisAdapterFlagsDXACGCompatible = 1 << 2,
    WisAdapterFlagsDXSupportsMonitoredFences = 1 << 3,
    WisAdapterFlagsDXSupportsNonMonitoredFences = 1 << 4,
    WisAdapterFlagsDXKeyedMutexConformance = 1 << 5,
    WisAdapterFlagsMax = 0xFFFFFFFF;
};

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

//==================================HANDLES==================================

typedef struct DX12Factory_t* DX12Factory;
typedef struct VKFactory_t* VKFactory;

typedef struct DX12Adapter_t* DX12Adapter;
typedef struct VKAdapter_t* VKAdapter;

//=================================FUNCTIONS=================================

DX12Factory DX12FactoryCreate();
VKFactory VKFactoryCreate();
void DX12FactoryDestroy(DX12Factory self);
void VKFactoryDestroy(VKFactory self);
