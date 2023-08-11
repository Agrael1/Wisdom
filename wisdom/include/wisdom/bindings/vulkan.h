#pragma once

#include "types.h"

typedef struct wisVKAdapter_t* wisVKAdapter;
typedef struct wisVKFactory_t* wisVKFactory;
typedef struct wisVKDevice_t* wisVKDevice;
typedef struct wisVKFence_t* wisVKFence;
typedef struct wisVKBuffer_t* wisVKBuffer;
typedef struct wisVKSampler_t* wisVKSampler;
typedef struct wisVKShader_t* wisVKShader;
typedef struct wisVKResourceAllocator_t* wisVKResourceAllocator;

#ifdef __cplusplus
extern "C" {
#endif

// Factory
wisVKFactory wisVKFactoryCreate(const wisApplicationInfo* app_info, wisAllocator* allocator);
void wisVKFactoryDestroy(wisVKFactory factory, wisAllocator* allocator);
void wisVKFactoryGetAdapterStorage(const wisVKFactory self, wisAdapterPreference preference, void* pstorage);
void wisVKFactoryFreeAdapterStorage(void* storage);
wisVKAdapter wisVKFactoryGetNextAdapter(void* storage);

// Adapter
void wisVKAdapterGetDesc(const wisVKAdapter self, struct wisAdapterDesc* pdesc);
uint32_t wisVKAdapterGeneratorStorageSize();

// Device
wisVKDevice wisVKDeviceCreate(wisVKFactory factory, const wisVKAdapter adapter, wisAllocator* allocator);
void wisVKDeviceDestroy(wisVKDevice device, wisAllocator* allocator);

// Fence
uint64_t wisVKFenceGetCompletedValue(const wisVKFence fence);
int wisVKFenceWait(const wisVKFence fence, uint64_t value);
void wisVKFenceSignal(const wisVKFence fence, uint64_t value);

// Allocator
wisVKResourceAllocator wisVKResourceAllocatorCreate(const wisVKDevice device, wisAllocator* allocator);
void wisVKResourceAllocatorDestroy(wisVKResourceAllocator res_allocator, wisAllocator* allocator);

wisVKBuffer wisVKResourceAllocatorCreatePersistentBuffer(const wisVKResourceAllocator alloc, size_t size, wisBufferFlags flags, wisAllocator* allocator);
wisVKBuffer wisVKResourceAllocatorCreateUploadBuffer(const wisVKResourceAllocator alloc, size_t size, wisAllocator* allocator);
wisVKBuffer wisVKResourceAllocatorCreateConstantBuffer(const wisVKResourceAllocator alloc, size_t size, wisAllocator* allocator);
wisVKBuffer wisVKResourceAllocatorCreateTexture(const wisVKResourceAllocator alloc, const struct wisTextureDescriptor* desc, wisTextureFlags flags, wisAllocator* allocator);
wisVKBuffer wisVKResourceAllocatorCreateDepthStencilTexture(const wisVKResourceAllocator alloc, const struct wisDepthDescriptor* desc, wisTextureFlags flags, wisAllocator* allocator);

#ifdef __cplusplus
}
#endif
