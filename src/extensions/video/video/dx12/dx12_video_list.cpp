#ifndef WIS_DX12_VIDEO_COMMAND_LIST_CPP
#define WIS_DX12_VIDEO_COMMAND_LIST_CPP

#include <wisdom/dx12/detail/dx12_utils.hpp>
#include <wisdom/generated/dx12_convert.hpp>
#include <video/generated/cpp_api.hpp>

#ifndef DX12SDKVER
#    include <directx/d3d12video.h>
#else
#    include <d3d12video.h>
#endif // DX12SDKVER


//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12VideoDecodeCommandListBegin(const WisDX12VideoDecodeCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::DX12VideoDecodeCommandListImpl>(self);
    auto hr = impl.command_list->Reset(impl.allocator);
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to reset command list for recording">(hr);
    }

    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_API WisResult wisDX12VideoDecodeCommandListEnd(const WisDX12VideoDecodeCommandList* self)
{
    auto& impl = wis::from_handle_ref<const wis::impl::DX12VideoDecodeCommandListImpl>(self);
    auto hr = impl.command_list->Close();
    if (!wis::detail::succeeded(hr)) {
        return wis::detail::make_result<wis::detail::Func(), "Failed to reset command list for recording">(hr);
    }

    return wis::detail::dx_success;
}

//----------------------------------------------------------------------------------------------------------------------
WIS_EXTERN_C WISDOM_VIDEO_API void wisDX12DestroyVideoDecodeCommandList(WisDX12VideoDecodeCommandList* self) {
    auto& impl = wis::from_handle_ref<wis::impl::DX12VideoDecodeCommandListImpl>(self);
    if (impl.command_list) {
        impl.command_list->Release();
        impl.allocator->Release();
        impl.command_list = nullptr;
    }
}


#endif // WIS_DX12_VIDEO_COMMAND_LIST_CPP
