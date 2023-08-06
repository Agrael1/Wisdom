#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/vulkan/vk_fence.h>
#include <wisdom/bindings/vulkan.h>

template<class Type>
static void Deallocate(wisAllocator* allocator, Type* ptr)
{
    ptr->~Type();
    if (allocator)
        allocator->free(allocator->user_data, ptr);
    else
        _aligned_free(ptr);
}

template<class Type, class... Args>
static Type* Allocate(wisAllocator* allocator, Args&&... args)
{
    auto* ptr = allocator ? allocator->allocate(allocator->user_data, sizeof(Type), alignof(Type))
                          : _aligned_malloc(sizeof(Type), alignof(Type));
    auto* type = reinterpret_cast<Type*>(new (ptr) Type(std::forward<Args>(args)...));
    if (!bool(*type)) {
        Deallocate(allocator, type);
        return nullptr;
    } else {
        return type;
    }
}

template<class CType, class Type>
CType As(Type* t)
{
    return reinterpret_cast<CType>(t);
}

extern "C" {
void wisVKFactoryGetAdapterStorage(const wisVKFactory self, wisAdapterPreference preference, void* pstorage)
{
    auto gen = reinterpret_cast<const wis::VKFactory*>(self)->EnumerateAdapters(wis::AdapterPreference(preference));
    new (pstorage) wis::generator<wis::VKAdapter>(std::move(gen));
}
void wisVKFactoryFreeAdapterStorage(void* storage)
{
    reinterpret_cast<wis::generator<wis::VKAdapter>*>(storage)->~generator();
}

wisVKFactory wisVKFactoryCreate(const wisApplicationInfo* app_info, wisAllocator* allocator)
{
    return As<wisVKFactory>(Allocate<wis::VKFactory>(allocator, static_cast<const wis::ApplicationInfo&>(*app_info)));
}
void wisVKFactoryDestroy(wisVKFactory factory, wisAllocator* allocator)
{
    Deallocate(allocator, As<wis::VKFactory*>(factory));
}

void wisVKAdapterGetDesc(const wisVKAdapter self, wisAdapterDesc* pdesc)
{
    reinterpret_cast<const wis::VKAdapter*>(self)->GetDesc(reinterpret_cast<wis::AdapterDesc&>(*pdesc));
}

uint32_t wisVKAdapterGeneratorStorageSize()
{
    return sizeof(wis::generator<wis::VKAdapter>);
}

wisVKAdapter wisVKFactoryGetNextAdapter(void* storage)
{
    auto& gen = *reinterpret_cast<wis::generator<wis::VKAdapter>*>(storage);
    auto a = gen.begin();
    return a == gen.end() ? nullptr : reinterpret_cast<wisVKAdapter>(&*a);
}

uint64_t wisVKFenceGetCompletedValue(const wisVKFence fence)
{
    return reinterpret_cast<const wis::VKFence*>(fence)->GetCompletedValue();
}
int wisVKFenceWait(const wisVKFence fence, uint64_t value)
{
    return reinterpret_cast<const wis::VKFence*>(fence)->Wait(value);
}
void wisVKFenceSignal(const wisVKFence fence, uint64_t value)
{
    reinterpret_cast<wis::VKFence*>(fence)->Signal(value);
}

// wisVKDevice wisVKDeviceCreate(const wisVKAdapter adapter, wisAllocator* allocator)
//{
//     return As<wisVKDevice>(Allocate<wis::VKDevice>(allocator, As<wis::VKAdapter*>(adapter)));
// }
//
// void wisVKDeviceDestroy(wisVKDevice device, wisAllocator* allocator)
//{
//     Deallocate(allocator, As<wis::VKDevice*>(device));
// }
}