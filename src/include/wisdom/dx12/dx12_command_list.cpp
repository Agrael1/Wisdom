#ifndef WIS_DX12_COMMAND_LIST_CPP
#define WIS_DX12_COMMAND_LIST_CPP

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

#endif // WIS_DX12_COMMAND_LIST_CPP