#ifndef WIS_DX12_IMPL_CPP
#define WIS_DX12_IMPL_CPP

#include <wisdom/generated/dx12_cpp_api.hpp>
#include <wisdom/generated/dx12_api.h>
#include <wisdom/generated/dx12_convert.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>

using namespace wis;
using namespace wis::impl;
using namespace wis::detail;

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyRootSignature(WisDX12RootSignature* self)
{
    auto& [root_signature] = *reinterpret_cast<DX12RootSignatureImpl*>(self);
    if (!root_signature) {
        return;
    }

    root_signature->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyBuffer(WisDX12Buffer* self)
{
    auto& [resource, allocation, allocator] = *reinterpret_cast<DX12BufferImpl*>(self);
    if (!resource) {
        return;
    }

    resource->Release();
    allocation->Release();
    allocator->Release();
    resource = nullptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void* wisDX12BufferMap(const WisDX12Buffer* self)
{
    auto& [resource, allocation, allocator] = *reinterpret_cast<const DX12BufferImpl*>(self);
    void* mapped_ptr                        = nullptr;
    auto  hr                                = resource->Map(0, nullptr, &mapped_ptr);
    (void)hr; // Ignore mapping failure, return nullptr in that case
    return mapped_ptr;
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API uint64_t wisDX12BufferGetGPUAddress(const WisDX12Buffer* self)
{
    auto& [resource, allocation, allocator] = *reinterpret_cast<const DX12BufferImpl*>(self);
    return resource->GetGPUVirtualAddress();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyTexture(WisDX12Texture* self)
{
    auto& [resource, allocation, allocator] = *reinterpret_cast<DX12BufferImpl*>(self);
    if (!resource) {
        return;
    }

    resource->Release();
    allocation->Release();
    allocator->Release();
    resource = nullptr;
}

#endif // WIS_DX12_IMPL_CPP
