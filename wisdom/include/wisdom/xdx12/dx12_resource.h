#pragma once
#include <wisdom/global/internal.h>
#include <wisdom/xdx12/dx12_views.h>
#include <wisdom/util/com_ptr.h>
#include <D3D12MemAlloc.h>

namespace wis {
class DX12Buffer;

template<>
struct Internal<DX12Buffer> {
    wis::com_ptr<ID3D12Resource> resource;
    wis::com_ptr<D3D12MA::Allocation> allocation;
};

class DX12Buffer : public QueryInternal<DX12Buffer>
{
public:
    DX12Buffer() noexcept = default;
    explicit DX12Buffer(wis::com_ptr<ID3D12Resource> rc, wis::com_ptr<D3D12MA::Allocation> al) noexcept
        : QueryInternal(std::move(rc), std::move(al)) { }

    operator DX12BufferView() const noexcept { return resource.get(); }
    operator bool() const noexcept { return bool(resource); }

public:
};

class DX12UploadBuffer : public DX12Buffer
{
public:
    DX12UploadBuffer() noexcept = default;
    explicit DX12UploadBuffer(wis::com_ptr<ID3D12Resource> rc, wis::com_ptr<D3D12MA::Allocation> al) noexcept
        : DX12Buffer(std::move(rc), std::move(al)) { }
    explicit DX12UploadBuffer(DX12Buffer&& buffer) noexcept
        : DX12Buffer(std::move(buffer)) { }
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

} // namespace wis