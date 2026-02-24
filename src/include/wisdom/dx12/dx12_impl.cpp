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
WIS_EXTERN_C WISDOM_API void wisDX12DestroyCommandList(WisDX12CommandList* self)
{
    auto& [list] = *reinterpret_cast<DX12CommandListImpl*>(self);
    if (!list) {
        return;
    }

    list->Release();
}

//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyPipelineLayout(WisDX12PipelineLayout* self)
{
    auto& [root_signature] = *reinterpret_cast<DX12PipelineLayoutImpl*>(self);
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
