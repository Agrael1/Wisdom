#pragma once

#include "types.h"

typedef struct wisVKAdapter_t* wisVKAdapter;
typedef struct wisVKFactory_t* wisVKFactory;
typedef struct wisVKDevice_t* wisVKDevice;

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

#ifdef __cplusplus
}
#endif
