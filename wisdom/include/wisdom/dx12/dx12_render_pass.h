#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/global/definitions.h>
#include <wisdom/api/api_internal.h>
#include <wisdom/util/small_allocator.h>
#include <wisdom/dx12/dx12_views.h>
#include <d3d12.h>
#include <memory>
#endif

namespace wis {
class DX12RenderPass;

// Render pass descriptor is way too big to be allocated on the stack
struct DX12RenderPassInternal {
    wis::internals::uniform_allocator<DXGI_FORMAT, max_render_targets> rt_formats;
    mutable std::array<D3D12_RENDER_PASS_RENDER_TARGET_DESC, max_render_targets> rt_descs;
    mutable D3D12_RENDER_PASS_DEPTH_STENCIL_DESC ds_desc;
    DXGI_FORMAT ds_format = DXGI_FORMAT_UNKNOWN;
};

template<>
class Internal<DX12RenderPass>
{
public:
    std::shared_ptr<DX12RenderPassInternal> desc;
};

WIS_EXPORT class DX12RenderPass : public QueryInternal<DX12RenderPass>
{
public:
    DX12RenderPass() = default;
    explicit DX12RenderPass(std::shared_ptr<DX12RenderPassInternal> desc) noexcept
        : QueryInternal(std::move(desc)) { }

    operator DX12RenderPassView() const noexcept
    {
        return desc.get();
    }

    operator bool() const noexcept
    {
        return bool(desc);
    }
};
} // namespace wis
