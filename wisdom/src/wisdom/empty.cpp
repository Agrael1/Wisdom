#include <wisdom/vulkan/vk_factory.h>
#include <wisdom/bindings/vulkan.h>


template<class Type, class ...Args>
static Type* Allocate(wisAllocator* allocator, Args&&... args)
{
    auto* ptr = allocator?
        allocator->allocate(allocator->user_data, sizeof(Type), alignof(Type))
                          : _aligned_malloc(sizeof(Type()), alignof(Type));
    return reinterpret_cast<Type*>(new(ptr) Type(std::forward<Args>(args)...));
}

template<class Type>
static void Deallocate(wisAllocator* allocator, Type* ptr)
{
    if (allocator) {
		allocator->free(allocator->user_data, ptr);
    }
    else {
		ptr->~Type();
		_aligned_free(ptr);
	}
}

template <class CType, class Type>
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
    return As<wisVKFactory>(Allocate<wis::VKFactory>(allocator, *app_info));
}
void wisVKFactoryDestroy(wisVKFactory factory, wisAllocator* allocator)
{
    Deallocate(allocator, As<wis::VKFactory*>(factory));
}

void wisVKAdapterGetDesc(wisVKAdapter self, wisAdapterDesc* pdesc)
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
    return a == gen.end()? nullptr : reinterpret_cast<wisVKAdapter>(&*a);
}


//wisVKDevice wisVKDeviceCreate(const wisVKAdapter adapter, wisAllocator* allocator)
//{
//    return As<wisVKDevice>(Allocate<wis::VKDevice>(allocator, As<wis::VKAdapter*>(adapter)));
//}
//
//void wisVKDeviceDestroy(wisVKDevice device, wisAllocator* allocator)
//{
//    Deallocate(allocator, As<wis::VKDevice*>(device));
//}

}