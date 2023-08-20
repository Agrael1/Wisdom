#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <d3dx12/d3dx12.h>
#endif // !WISDOM_MODULES

WIS_EXPORT namespace wis
{
    class DX12RenderTargetView;

    template<>
    class Internal<DX12RenderTargetView>
    {
    public:
        CD3DX12_CPU_DESCRIPTOR_HANDLE handle{};
    };

    class DX12RenderTargetView : public QueryInternal<DX12RenderTargetView>
    {
    public:
        DX12RenderTargetView() = default;
        explicit DX12RenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE xhandle)
            : QueryInternal(xhandle) { }
    };

    using DX12DepthStencilView = DX12RenderTargetView;
}
