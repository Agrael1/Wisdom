#pragma once

#include "types.h"

typedef struct wisDX12Adapter_t*	wisDX12Adapter;
typedef struct wisDX12Factory_t*	wisDX12Factory;
typedef struct wisDX12Device_t*		wisDX12Device;
typedef struct wisDX12Fence_t*		wisDX12Fence;
typedef struct wisDX12Sampler_t*	wisDX12Sampler;
typedef struct wisDX12Shader_t*		wisDX12Shader;

#ifdef __cplusplus
extern "C" {
#endif

//wisVKFactory wisVKFactoryCreate(const wisApplicationInfo* app_info, wisAllocator* allocator);
//void wisVKFactoryDestroy(wisVKFactory factory, wisAllocator* allocator);
//void wisVKFactoryGetAdapterStorage(const wisVKFactory self, wisAdapterPreference preference, void* pstorage);
//void wisVKFactoryFreeAdapterStorage(void* storage);
//wisVKAdapter wisVKFactoryGetNextAdapter(void* storage);
//
//void wisVKAdapterGetDesc(const wisVKAdapter self, struct wisAdapterDesc* pdesc);
//uint32_t wisVKAdapterGeneratorStorageSize();
//
//wisVKDevice wisVKDeviceCreate(const wisVKAdapter adapter, wisAllocator* allocator);
//void wisVKDeviceDestroy(wisVKDevice device, wisAllocator* allocator);
//
//uint64_t wisVKFenceGetCompletedValue(const wisVKFence fence);
//int wisVKFenceWait(const wisVKFence fence, uint64_t value);
//void wisVKFenceSignal(const wisVKFence fence, uint64_t value);

#ifdef __cplusplus
}
#endif