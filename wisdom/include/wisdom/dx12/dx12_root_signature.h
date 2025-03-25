#ifndef WIS_DX12_ROOT_SIGNATURE_H
#define WIS_DX12_ROOT_SIGNATURE_H
#ifndef WISDOM_MODULE_DECL
#include <wisdom/global/internal.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/util/com_ptr.h>
#endif

namespace wis {
WISDOM_EXPORT class DX12RootSignature;

WISDOM_EXPORT
template<>
struct Internal<DX12RootSignature> {
    wis::com_ptr<ID3D12RootSignature> root;
    std::array<int8_t, size_t(wis::ShaderStages::Count)> stage_map;
    uint32_t push_constant_count = 0;
    uint32_t push_descriptor_count = 0;
};

WISDOM_EXPORT
class DX12RootSignature : public QueryInternal<DX12RootSignature>
{
public:
    DX12RootSignature() = default;
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
#endif // WIS_DX12_ROOT_SIGNATURE_H
