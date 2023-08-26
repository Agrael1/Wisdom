#pragma once
#ifndef WISDOM_MODULES
#include <wisdom/dx12/dx12_checks.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/api/api_internal.h>
#include <D3D12MemAlloc.h>

#include <winrt/base.h>
#include <span>
#endif

namespace wis {
class DX12Buffer;

template<>
class Internal<DX12Buffer>
{
public:
    winrt::com_ptr<D3D12MA::Allocation> allocation;
    winrt::com_ptr<ID3D12Resource> resource;
};

WIS_EXPORT class DX12Buffer : public QueryInternal<DX12Buffer>
{
public:
    DX12Buffer() = default;
    explicit DX12Buffer(winrt::com_ptr<ID3D12Resource> rc, winrt::com_ptr<D3D12MA::Allocation> al) noexcept
        : QueryInternal(std::move(al), std::move(rc)) { }
    operator DX12BufferView() const noexcept
    {
        return resource.get();
    }
    operator bool() const noexcept
    {
        return resource != nullptr;
    }

public:
    bool UpdateSubresource(std::span<const std::byte> data) noexcept
    {
        void* bytes = nullptr;
        if (!wis::succeeded(resource->Map(0, nullptr, &bytes)))
            return false;

        std::copy(data.data(), data.data() + data.size(), (std::byte*)bytes);
        resource->Unmap(0, nullptr);
        return true;
    }

    [[nodiscard]] std::span<std::byte>
    MapMemory() noexcept
    {
        void* bytes = nullptr;
        if (!wis::succeeded(resource->Map(0, nullptr, &bytes)))
            return {};
        return { (std::byte*)bytes, resource->GetDesc().Width };
    }

    void UnmapMemory() noexcept
    {
        resource->Unmap(0, nullptr);
    }

    [[nodiscard]] DX12VertexBufferView
    GetVertexBufferView(uint32_t byte_stride)
    {
        return DX12VertexBufferView{ D3D12_VERTEX_BUFFER_VIEW{ resource->GetGPUVirtualAddress(), uint32_t(resource->GetDesc().Width), byte_stride } };
    }
};

WIS_EXPORT class DX12Texture : public QueryInternal<DX12Buffer>
{
public:
    DX12Texture() = default;
    explicit DX12Texture(winrt::com_ptr<ID3D12Resource> rc, winrt::com_ptr<D3D12MA::Allocation> al)
        : QueryInternal(std::move(al), std::move(rc)) { }
    operator DX12TextureView() const noexcept
    {
        return resource.get();
    }
    operator bool() const noexcept
    {
        return resource != nullptr;
    }
};
} // namespace wis
