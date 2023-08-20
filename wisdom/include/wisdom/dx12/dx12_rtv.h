#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/api/api_internal.h>
#include <d3dx12/d3dx12.h>
#include <winrt/base.h>
#endif // !WISDOM_MODULES

namespace wis {
class DX12RenderTarget;

template<>
class Internal<DX12RenderTarget> {
public:
    winrt::com_ptr<ID3D12DescriptorHeap> desc;
    CD3DX12_CPU_DESCRIPTOR_HANDLE handle{};
};

WIS_EXPORT class DX12RenderTarget : public QueryInternal<DX12RenderTarget>
{
public:
    DX12RenderTarget() = default;
    explicit DX12RenderTarget(winrt::com_ptr<ID3D12DescriptorHeap> desc) noexcept
        : QueryInternal(std::move(desc), CD3DX12_CPU_DESCRIPTOR_HANDLE(desc->GetCPUDescriptorHandleForHeapStart())) { }

    operator bool() const noexcept {
        return bool(desc);
    }

    operator DX12RenderTargetHandle() const noexcept {
        return handle;
    }
};

using DX12DepthStencil = DX12RenderTarget;
using DX12DepthStencilHandle = DX12RenderTargetHandle;
} // namespace wis