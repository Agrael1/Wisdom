#pragma once
#include <wisdom/api/internal.h>
#include <wisdom/dx12/xdx12_views.h>
#include <wisdom/util/com_ptr.h>
#include <d3dx12/d3dx12_root_signature.h>
#include <memory>

namespace wis {
class DX12RootSignature;

template<>
struct Internal<DX12RootSignature> {
    wis::com_ptr<ID3D12RootSignature> root;
};

class DX12RootSignature : public QueryInternal<DX12RootSignature>
{
public:
    static constexpr inline auto root_const_register = 16;
public:
    DX12RootSignature() = default;
    explicit DX12RootSignature(wis::com_ptr<ID3D12RootSignature> xroot) noexcept
        : QueryInternal(std::move(xroot))
    {
    }
    operator DX12RootSignatureView() const noexcept
    {
        return root.get();
    }
    operator bool() const noexcept
    {
        return bool(root);
    }
};
} // namespace wis
