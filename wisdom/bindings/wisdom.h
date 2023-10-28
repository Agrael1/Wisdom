#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct WisResult WisResult;
typedef struct WisAdapterDesc WisAdapterDesc;
typedef struct WisRootConstant WisRootConstant;
typedef struct WisPushDescriptor WisPushDescriptor;
typedef enum WisShaderStages WisShaderStages;
typedef enum WisStatus WisStatus;
typedef enum WisDescriptorType WisDescriptorType;
typedef enum WisQueueType WisQueueType;
typedef enum WisMutiWaitFlags WisMutiWaitFlags;
typedef enum WisAdapterPreference WisAdapterPreference;
typedef enum WisSeverity WisSeverity;
typedef enum WisAdapterFlagsBits WisAdapterFlagsBits;
typedef uint32_t WisAdapterFlags;
typedef enum WisDeviceFeaturesBits WisDeviceFeaturesBits;
typedef uint32_t WisDeviceFeatures;

enum WisShaderStages {
    ShaderStagesAll = 0,
    ShaderStagesVertex = 1,
    ShaderStagesHull = 2,
    ShaderStagesDomain = 3,
    ShaderStagesGeometry = 4,
    ShaderStagesPixel = 5,
    ShaderStagesAmplification = 6,
    ShaderStagesMesh = 7,
};

enum WisStatus {
    StatusOk = 0,
    StatusTimeout = 1,
    StatusError = -1,
    StatusInvalidArgument = -2,
    StatusOutOfMemory = -3,
    StatusDeviceLost = -4,
};

enum WisDescriptorType {
    DescriptorTypeNone = 0,
    DescriptorTypeConstantBuffer = 2,
    DescriptorTypeShaderResource = 3,
    DescriptorTypeUnorderedAccess = 4,
};

enum WisQueueType {
    QueueTypeGraphics = 0,
    QueueTypeDX12Bundle = 1,
    QueueTypeCompute = 2,
    QueueTypeCopy = 3,
    QueueTypeVideoDecode = 4,
    QueueTypeDX12VideoProcess = 5,
    QueueTypeDX12VideoEncode = 6,
};

enum WisMutiWaitFlags {
    MutiWaitFlagsAll = 0,
    MutiWaitFlagsAny = 1,
};

enum WisAdapterPreference {
    AdapterPreferenceNone = 0,
    AdapterPreferenceMinConsumption = 1,
    AdapterPreferencePerformance = 2,
};

enum WisSeverity {
    SeverityDebug = 0,
    SeverityTrace = 1,
    SeverityInfo = 2,
    SeverityWarning = 3,
    SeverityError = 4,
    SeverityCritical = 5,
};

enum WisAdapterFlagsBits {
    AdapterFlagsNone = 0x0,
    AdapterFlagsRemote = 1 << 0,
    AdapterFlagsSoftware = 1 << 1,
    AdapterFlagsDX12ACGCompatible = 1 << 2,
    AdapterFlagsDX12SupportsMonitoredFences = 1 << 3,
    AdapterFlagsDX12SupportsNonMonitoredFences = 1 << 4,
    AdapterFlagsDX12KeyedMutexConformance = 1 << 5,
};

enum WisDeviceFeaturesBits {
    DeviceFeaturesNone = 0x0,
    DeviceFeaturesPushDescriptors = 1 << 0,
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

struct WisRootConstant{
    WisShaderStages stage;
    uint32_t size_bytes;
};

struct WisPushDescriptor{
    WisShaderStages stage;
    uint32_t bind_register;
    WisDescriptorType type;
    uint32_t reserved;
};

struct DX12FenceView{
    void* value;
};

struct VKFenceView{
    void* value;
};

//=================================DELEGATES=================================

typedef void (*DebugCallback)( WisSeverity severity,  const char* message,  void* user_data);
//==================================HANDLES==================================

typedef struct DX12RootSignature_t* DX12RootSignature;
typedef struct VKRootSignature_t* VKRootSignature;

typedef struct DX12Factory_t* DX12Factory;
typedef struct VKFactory_t* VKFactory;

typedef struct DX12Adapter_t* DX12Adapter;
typedef struct VKAdapter_t* VKAdapter;

typedef struct DX12Device_t* DX12Device;
typedef struct VKDevice_t* VKDevice;

typedef struct DX12DescriptorLayout_t* DX12DescriptorLayout;
typedef struct VKDescriptorLayout_t* VKDescriptorLayout;

typedef struct DX12Fence_t* DX12Fence;
typedef struct VKFence_t* VKFence;

typedef struct DX12ResourceAllocator_t* DX12ResourceAllocator;
typedef struct VKResourceAllocator_t* VKResourceAllocator;

//=================================FUNCTIONS=================================

WisResult DX12CreateFactory( bool debug_layer,  DebugCallback callback,  void* user_data, DX12Factory* out_factory);
WisResult VKCreateFactory( bool debug_layer,  DebugCallback callback,  void* user_data, VKFactory* out_factory);
WisResult DX12CreateDevice( DX12Factory factory,  DX12Adapter adapter, DX12Device* out_device);
WisResult VKCreateDevice( VKFactory factory,  VKAdapter adapter, VKDevice* out_device);
WisResult DX12GetAdapter(DX12Factory self,  uint32_t index,  WisAdapterPreference preference, DX12Adapter* out_adapter);
WisResult VKGetAdapter(VKFactory self,  uint32_t index,  WisAdapterPreference preference, VKAdapter* out_adapter);
void DX12FactoryDestroy(DX12Factory self);
void VKFactoryDestroy(VKFactory self);
WisResult DX12GetDesc(DX12Adapter self,  WisAdapterDesc* desc);
WisResult VKGetDesc(VKAdapter self,  WisAdapterDesc* desc);
void DX12DeviceDestroy(DX12Device self);
void VKDeviceDestroy(VKDevice self);
WisResult DX12CreateFence(DX12Device self,  uint64_t initial_value, DX12Fence* out_fence);
WisResult VKCreateFence(VKDevice self,  uint64_t initial_value, VKFence* out_fence);
WisResult DX12CreateRootSignature(DX12Device self,  WisRootConstant* constants,  uint32_t constants_size, DX12RootSignature* out_root_signature);
WisResult VKCreateRootSignature(VKDevice self,  WisRootConstant* constants,  uint32_t constants_size, VKRootSignature* out_root_signature);
WisResult DX12CreateAllocator(DX12Device self, DX12ResourceAllocator* out_allocator);
WisResult VKCreateAllocator(VKDevice self, VKResourceAllocator* out_allocator);
WisResult DX12WaitForMultipleFences(DX12Device self,  DX12FenceView* fences,  uint64_t* values,  uint32_t count,  WisMutiWaitFlags wait_all,  uint64_t timeout);
WisResult VKWaitForMultipleFences(VKDevice self,  VKFenceView* fences,  uint64_t* values,  uint32_t count,  WisMutiWaitFlags wait_all,  uint64_t timeout);
void DX12RootSignatureDestroy(DX12RootSignature self);
void VKRootSignatureDestroy(VKRootSignature self);
void DX12FenceDestroy(DX12Fence self);
void VKFenceDestroy(VKFence self);
uint64_t DX12GetCompletedValue(DX12Fence self);
uint64_t VKGetCompletedValue(VKFence self);
WisResult DX12Wait(DX12Fence self,  uint64_t value,  uint64_t timeout_ns);
WisResult VKWait(VKFence self,  uint64_t value,  uint64_t timeout_ns);
WisResult DX12Signal(DX12Fence self,  uint64_t value);
WisResult VKSignal(VKFence self,  uint64_t value);
void DX12ResourceAllocatorDestroy(DX12ResourceAllocator self);
void VKResourceAllocatorDestroy(VKResourceAllocator self);
DX12FenceView AsDX12FenceView(DX12Fence self);
VKFenceView AsVKFenceView(VKFence self);
