#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct WisResult WisResult;
typedef struct WisAdapterDesc WisAdapterDesc;
typedef enum WisStatus WisStatus;
typedef enum WisAdapterPreference WisAdapterPreference;
typedef enum WisSeverity WisSeverity;
typedef enum WisQueueType WisQueueType;
typedef enum WisAdapterFlags WisAdapterFlags;

enum WisStatus {
    WisStatusOk = 0,
    WisStatusError = 1,
    WisStatusInvalidArgument = 2,
    WisStatusOutOfMemory = 3,
    WisStatusDeviceLost = 4,
};

enum WisAdapterPreference {
    WisAdapterPreferenceNone = 0,
    WisAdapterPreferenceMinConsumption = 1,
    WisAdapterPreferencePerformance = 2,
};

enum WisSeverity {
    WisSeverityDebug = 0,
    WisSeverityTrace = 1,
    WisSeverityInfo = 2,
    WisSeverityWarning = 3,
    WisSeverityError = 4,
    WisSeverityCritical = 5,
};

enum WisQueueType {
    WisQueueTypeGraphics = 0,
    WisQueueTypeDXBundle = 1,
    WisQueueTypeCompute = 2,
    WisQueueTypeCopy = 3,
    WisQueueTypeVideoDecode = 4,
    WisQueueTypeDXVideoProcess = 5,
    WisQueueTypeDXVideoEncode = 6,
};

enum WisAdapterFlags {
    WisAdapterFlagsNone = 0x0,
    WisAdapterFlagsRemote = 1 << 0,
    WisAdapterFlagsSoftware = 1 << 1,
    WisAdapterFlagsDXACGCompatible = 1 << 2,
    WisAdapterFlagsDXSupportsMonitoredFences = 1 << 3,
    WisAdapterFlagsDXSupportsNonMonitoredFences = 1 << 4,
    WisAdapterFlagsDXKeyedMutexConformance = 1 << 5,
    WisAdapterFlagsMax = 0xFFFFFFFF,
};

struct WisResult{
    WisStatus status;
    const char* error;
};

struct WisAdapterDesc{
    const char description[256];
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

//=================================DELEGATES=================================

typedef void (*WisDebugCallback)(WisSeverity severity, const char *message, void *user_data);
//==================================HANDLES==================================

typedef struct DX12Factory_t* DX12Factory;
typedef struct VKFactory_t* VKFactory;

typedef struct DX12Adapter_t* DX12Adapter;
typedef struct VKAdapter_t* VKAdapter;

typedef struct DX12Device_t* DX12Device;
typedef struct VKDevice_t* VKDevice;

//=================================FUNCTIONS=================================

 WisResult  DX12CreateFactory( bool debug_layer,  WisDebugCallback callback,  void *user_data,  DX12Factory * out_factory);
 WisResult  VKCreateFactory( bool debug_layer,  WisDebugCallback callback,  void *user_data,  VKFactory * out_factory);
 WisResult  DX12FactoryGetAdapter( DX12Factory self,  uint32_t index,  WisAdapterPreference preference,  DX12Adapter * out_adapter);
 WisResult  VKFactoryGetAdapter( VKFactory self,  uint32_t index,  WisAdapterPreference preference,  VKAdapter * out_adapter);
 void  DX12FactoryDestroy( DX12Factory self);
 void  VKFactoryDestroy( VKFactory self);
 WisResult  DX12AdapterGetDesc( DX12Adapter self,  WisAdapterDesc *desc);
 WisResult  VKAdapterGetDesc( VKAdapter self,  WisAdapterDesc *desc);
 WisResult  DX12CreateDevice( DX12Factory factory,  DX12Adapter adapter,  DX12Device * out_device);
 WisResult  VKCreateDevice( VKFactory factory,  VKAdapter adapter,  VKDevice * out_device);
