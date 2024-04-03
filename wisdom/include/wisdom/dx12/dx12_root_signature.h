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
    uint32_t root_table_offset = 0;
};

class DX12RootSignature : public QueryInternal<DX12RootSignature>
{
public:
    static constexpr inline auto root_const_register = 16;

public:
    DX12RootSignature() = default;
    explicit DX12RootSignature(wis::com_ptr<ID3D12RootSignature> xroot, std::array<int8_t, size_t(wis::ShaderStages::Count)> stage_map, uint32_t root_table_offset) noexcept
        : QueryInternal(std::move(xroot), stage_map, root_table_offset)
    {
    }
    operator DX12RootSignatureView() const noexcept
    {
        return { root.get(), stage_map, root_table_offset };
    }
    operator bool() const noexcept
    {
        return bool(root);
    }
};
} // namespace wis
