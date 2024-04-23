#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/dx12/dx12_views.h>
#include <wisdom/util/com_ptr.h>
#include <D3D12MemAlloc.h>
#include <optional>

namespace wis {
class DX12Buffer;

template<>
struct Internal<DX12Buffer> {
    wis::com_ptr<D3D12MA::Allocator> allocator;
    wis::com_ptr<D3D12MA::Allocation> allocation;
    wis::com_ptr<ID3D12Resource> resource;
};

class DX12Buffer : public QueryInternal<DX12Buffer>
{
public:
    DX12Buffer() noexcept = default;
    explicit DX12Buffer(wis::com_ptr<ID3D12Resource> rc, wis::com_ptr<D3D12MA::Allocation> al, wis::com_ptr<D3D12MA::Allocator> allocator) noexcept
        : QueryInternal(std::move(allocator), std::move(al), std::move(rc))
    {
    }

    operator DX12BufferView() const noexcept
    {
        return resource.get();
    }
    operator bool() const noexcept
    {
        return bool(resource);
    }

public:
    void* Map() const noexcept
    {
        void* data;
        resource->Map(0, nullptr, &data);
        return data;
    }
    void Unmap() const noexcept
    {
        resource->Unmap(0, nullptr);
    }

public:
    template<typename T>
    T* Map() const noexcept
    {
        return static_cast<T*>(Map());
    }
};

using DX12Texture = DX12Buffer;

// =================================================================================================
class DX12RenderTarget;

template<>
struct Internal<DX12RenderTarget> {
    wis::com_ptr<ID3D12DescriptorHeap> heap;
    D3D12_CPU_DESCRIPTOR_HANDLE handle{};
};

class DX12RenderTarget : public QueryInternal<DX12RenderTarget>
{
public:
    DX12RenderTarget() = default;
    explicit DX12RenderTarget(wis::com_ptr<ID3D12DescriptorHeap> desc, D3D12_CPU_DESCRIPTOR_HANDLE handle) noexcept
        : QueryInternal(std::move(desc), handle) { }

    operator bool() const noexcept
    {
        return bool(heap);
    }
    operator DX12RenderTargetView() const noexcept
    {
        return handle;
    }
};

// =================================================================================================

class DX12Sampler;

template<>
struct Internal<DX12Sampler> {
    wis::com_ptr<ID3D12DescriptorHeap> heap;
};

class DX12Sampler : public QueryInternal<DX12Sampler>
{
public:
    DX12Sampler() noexcept = default;
    explicit DX12Sampler(wis::com_ptr<ID3D12DescriptorHeap> heap) noexcept
        : QueryInternal(std::move(heap)) { }

    operator bool() const noexcept
    {
        return bool(heap);
    }
    operator DX12SamplerView() const noexcept
    {
        return heap->GetCPUDescriptorHandleForHeapStart();
    }
};

// =================================================================================================

class DX12ShaderResource;

template<>
struct Internal<DX12ShaderResource> {
    wis::com_ptr<ID3D12DescriptorHeap> heap;
};

class DX12ShaderResource : public QueryInternal<DX12ShaderResource>
{
public:
    DX12ShaderResource() noexcept = default;
    explicit DX12ShaderResource(wis::com_ptr<ID3D12DescriptorHeap> heap) noexcept
        : QueryInternal(std::move(heap)) { }

    operator bool() const noexcept
    {
        return bool(heap);
    }
    operator DX12ShaderResourceView() const noexcept
    {
        return heap->GetCPUDescriptorHandleForHeapStart();
    }
};

} // namespace wis