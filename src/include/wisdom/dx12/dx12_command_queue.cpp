#ifndef WIS_DX12_COMMAND_QUEUE_CPP
#define WIS_DX12_COMMAND_QUEUE_CPP

#include <wisdom/generated/dx12_api.h>
#include <wisdom/dx12/dx12_types.hpp>
#include <wisdom/dx12/detail/dx12_utils.hpp>


//-----------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API void wisDX12DestroyCommandQueue(WisDX12CommandQueue* self)
{
    auto& [queue] = *reinterpret_cast<wis::impl::DX12CommandQueueImpl*>(self);
    if (!queue) {
        return;
    }
    queue->Release();
}

#endif // WIS_DX12_COMMAND_QUEUE_CPP