#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/util/com_ptr.h>

namespace wis {
class DX12RootSignature;

template<>
struct Internal<DX12RootSignature> {
    wis::com_ptr<ID3D12RootSignature> root;
    std::array<int8_t, size_t(wis::ShaderStages::Count)> stage_map;
    uint32_t push_constant_count = 0;
    uint32_t push_descriptor_count = 0;
};

class DX12RootSignature : public QueryInternal<DX12RootSignature>
{
public:
    static constexpr inline auto root_const_register = 16;

public:
    DX12RootSignature() = default;
    explicit DX12RootSignature(wis::com_ptr<ID3D12RootSignature> xroot, std::array<int8_t, size_t(wis::ShaderStages::Count)> stage_map, uint32_t push_constant_count, uint32_t push_descriptor_count) noexcept
        : QueryInternal(std::move(xroot), stage_map, push_constant_count, push_descriptor_count)
    {
    }
    operator DX12RootSignatureView() const noexcept
    {
        return { root.get(), stage_map, push_constant_count, push_descriptor_count };
    }
    operator DX12RootSignatureView2() const noexcept
    {
        return {};
    }
    operator bool() const noexcept
    {
        return bool(root);
    }
};
} // namespace wis
