#pragma once

#include "types.h"

typedef struct wisVKAdapter_t* wisVKAdapter;
typedef struct wisVKFactory_t* wisVKFactory;
typedef struct wisVKDevice_t* wisVKDevice;
typedef struct wisVKFence_t* wisVKFence;
typedef struct wisVKSampler_t* wisVKSampler;
typedef struct wisVKShader_t* wisVKShader;

#ifdef __cplusplus
extern "C" {
#endif

wisVKFactory wisVKFactoryCreate(const wisApplicationInfo* app_info, wisAllocator* allocator);
void wisVKFactoryDestroy(wisVKFactory factory, wisAllocator* allocator);
void wisVKFactoryGetAdapterStorage(const wisVKFactory self, wisAdapterPreference preference, void* pstorage);
void wisVKFactoryFreeAdapterStorage(void* storage);
wisVKAdapter wisVKFactoryGetNextAdapter(void* storage);

void wisVKAdapterGetDesc(const wisVKAdapter self, struct wisAdapterDesc* pdesc);
uint32_t wisVKAdapterGeneratorStorageSize();

wisVKDevice wisVKDeviceCreate(const wisVKAdapter adapter, wisAllocator* allocator);
void wisVKDeviceDestroy(wisVKDevice device, wisAllocator* allocator);

uint64_t wisVKFenceGetCompletedValue(const wisVKFence fence);
int wisVKFenceWait(const wisVKFence fence, uint64_t value);
void wisVKFenceSignal(const wisVKFence fence, uint64_t value);



#ifdef __cplusplus
}
#endif
