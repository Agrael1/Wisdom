#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/api/api_internal.h>
#include <wisdom/dx12/dx12_views.h>
#include <winrt/base.h>
#include <d3d12.h>
#endif

WIS_EXPORT namespace wis
{
class DX12RootSignature;

template<>
class Internal<DX12RootSignature>
{
public:
    winrt::com_ptr<ID3D12RootSignature> root;
};

class DX12RootSignature : public QueryInternal<DX12RootSignature>
{
public:
    DX12RootSignature() = default;
    explicit DX12RootSignature(winrt::com_ptr<ID3D12RootSignature> xroot) noexcept
        : QueryInternal(std::move(xroot))
    {
    }
    operator DX12RootSignatureView() const noexcept
    {
        return root.get();
    }
};
}
