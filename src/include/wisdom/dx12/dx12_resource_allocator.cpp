#ifndef WIS_DX12_RESOURCE_ALLOCATOR_CPP
#define WIS_DX12_RESOURCE_ALLOCATOR_CPP

#include <wisdom/generated/dx12_api.h>
#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyResourceAllocator(WisDX12ResourceAllocator* self)
{
    auto& [allocator] = *reinterpret_cast<wis::impl::DX12ResourceAllocatorImpl*>(self);
    if (!allocator) {
        return;
    }

    allocator->Release();
}

#endif // WIS_DX12_RESOURCE_ALLOCATOR_CPP